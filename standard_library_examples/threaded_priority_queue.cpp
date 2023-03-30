#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>

class ThreadsafePriorityQueue
{
  public:
    void push(int value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(value);
        lock.unlock();
        cond_var_.notify_one();
    }

    bool pop(int &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty())
        {
            cond_var_.wait(lock);
        }
        value = queue_.top();
        queue_.pop();
        return true;
    }

  private:
    std::priority_queue<int> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

void producer(ThreadsafePriorityQueue &pq)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 10; ++i)
    {
        int value = dis(gen);
        std::cout << "Producing: " << value << std::endl;
        pq.push(value);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(ThreadsafePriorityQueue &pq)
{
    for (int i = 0; i < 10; ++i)
    {
        int value;
        pq.pop(value);
        std::cout << "Consuming: " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main()
{
    ThreadsafePriorityQueue pq;

    std::thread producer_thread(producer, std::ref(pq));
    std::thread consumer_thread(consumer, std::ref(pq));

    producer_thread.join();
    consumer_thread.join();

    return 0;
}
