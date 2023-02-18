#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Producer
{
  public:
    Producer(std::queue<int> &buffer, std::mutex &buffer_mutex, std::condition_variable &buffer_cv, int id, int n)
        : buffer_(buffer), buffer_mutex_(buffer_mutex), buffer_cv_(buffer_cv), id_(id), n_(n)
    {
    }

    void operator()()
    {
        for (int i = 0; i < n_; i++)
        {
            std::unique_lock<std::mutex> lock(buffer_mutex_);
            buffer_cv_.wait(lock, [this] { return buffer_.size() < buffer_size_; }); // wait for space in buffer
            buffer_.push(i + id_ * n_);
            std::cout << "Producer " << id_ << " produced " << i + id_ * n_ << std::endl;
            lock.unlock();
            buffer_cv_.notify_all();
        }
    }

  private:
    std::queue<int> &buffer_;
    std::mutex &buffer_mutex_;
    std::condition_variable &buffer_cv_;
    const int id_;
    const int n_;
    const int buffer_size_ = 10;
};

class Consumer
{
  public:
    Consumer(std::queue<int> &buffer, std::mutex &buffer_mutex, std::condition_variable &buffer_cv, int id, int n)
        : buffer_(buffer), buffer_mutex_(buffer_mutex), buffer_cv_(buffer_cv), id_(id), n_(n)
    {
    }

    void operator()()
    {
        for (int i = 0; i < n_; i++)
        {
            std::unique_lock<std::mutex> lock(buffer_mutex_);
            buffer_cv_.wait(lock, [this] { return !buffer_.empty(); }); // wait for data in buffer
            int value = buffer_.front();
            buffer_.pop();
            std::cout << "Consumer " << id_ << " consumed " << value << std::endl;
            lock.unlock();
            buffer_cv_.notify_all();
        }
    }

  private:
    std::queue<int> &buffer_;
    std::mutex &buffer_mutex_;
    std::condition_variable &buffer_cv_;
    const int id_;
    const int n_;
};

int main()
{
    std::queue<int> buffer;            // shared buffer between producers and consumers
    std::mutex buffer_mutex;           // mutex to protect access to the buffer
    std::condition_variable buffer_cv; // condition variable to notify producers and consumers of changes to the buffer
    int num_producers = 2;             // number of producers
    int num_consumers = 2;             // number of consumers
    int num_items_per_producer = 10;   // number of items to produce per producer
    int num_items_per_consumer = 5;    // number of items to consume per consumer

    // Create producer and consumer threads using RAII
    std::vector<std::unique_ptr<std::thread>> producer_threads;
    std::vector<std::unique_ptr<std::thread>> consumer_threads;
    for (int i = 0; i < num_producers; i++)
    {
        producer_threads.push_back(
            std::make_unique<std::thread>(Producer(buffer, buffer_mutex, buffer_cv, i, num_items_per_producer)));
    }
    for (int i = 0; i < num_consumers; i++)
    {
        consumer_threads.push_back(
            std::make_unique<std::thread>(Consumer(buffer, buffer_mutex, buffer_cv, i, num_items_per_consumer)));
    }
    // Wait for threads to finish
    for (auto &thread : producer_threads)
    {
        thread->join();
    }
    for (auto &thread : consumer_threads)
    {
        thread->join();
    }

    return 0;
}