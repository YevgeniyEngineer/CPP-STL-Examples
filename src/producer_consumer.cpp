#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

class Producer
{
  public:
    Producer(std::queue<int> &buffer, std::mutex &buffer_mutex, std::condition_variable &buffer_cv, int n)
        : buffer_(buffer), buffer_mutex_(buffer_mutex), buffer_cv_(buffer_cv), n_(n)
    {
    }

    void operator()()
    {
        for (int i = 0; i < n_; i++)
        {
            std::unique_lock<std::mutex> lock(buffer_mutex_);
            buffer_cv_.wait(lock, [this] { return buffer_.size() < buffer_size_; }); // wait for space in buffer
            buffer_.push(i);
            std::cout << "Produced " << i << std::endl;
            lock.unlock();
            buffer_cv_.notify_all();
        }
    }

  private:
    std::queue<int> &buffer_;
    std::mutex &buffer_mutex_;
    std::condition_variable &buffer_cv_;
    const int n_;
    const int buffer_size_ = 10;
};

class Consumer
{
  public:
    Consumer(std::queue<int> &buffer, std::mutex &buffer_mutex, std::condition_variable &buffer_cv, int n)
        : buffer_(buffer), buffer_mutex_(buffer_mutex), buffer_cv_(buffer_cv), n_(n)
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
            std::cout << "Consumed " << value << std::endl;
            lock.unlock();
            buffer_cv_.notify_all();
        }
    }

  private:
    std::queue<int> &buffer_;
    std::mutex &buffer_mutex_;
    std::condition_variable &buffer_cv_;
    const int n_;
};

class JoiningThread
{
  public:
    JoiningThread(std::thread &&t) : thread_(std::move(t))
    {
    }
    ~JoiningThread()
    {
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

  private:
    std::thread thread_;
};

int main()
{
    std::queue<int> buffer;            // shared buffer between producer and consumer
    std::mutex buffer_mutex;           // mutex to protect access to the buffer
    std::condition_variable buffer_cv; // condition variable to notify producer and consumer of changes to the buffer
    int n = 20;                        // number of items to produce/consume

    // Create producer and consumer threads using RAII
    JoiningThread producer_thread(std::thread(Producer(buffer, buffer_mutex, buffer_cv, n)));
    JoiningThread consumer_thread(std::thread(Consumer(buffer, buffer_mutex, buffer_cv, n)));

    return 0;
}