#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename T> class ThreadSafeQueue
{
  public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;

    void push(const T &item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.emplace(item);
        lock.unlock();
        conditional_variable_.notify_one();
    }

    bool tryPop(T &item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return false;
        }

        item = queue_.front();
        queue_.pop();
        return true;
    }

    void waitAndPop(T &item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        conditional_variable_.wait(lock, [this] { return !queue_.empty(); });
        item = queue_.front();
        queue_.pop();
    }

  private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable conditional_variable_;
};

int main()
{
    std::mutex local_mutex;
    ThreadSafeQueue<int> thread_safe_queue;

    std::vector<std::thread> thread_pool;

    auto placeIntoQueue = [&thread_safe_queue, &local_mutex]() {
        for (int i = 0; i < 100; ++i)
        {
            thread_safe_queue.push(i);

            std::unique_lock<std::mutex> lock(local_mutex);
            std::cout << "Thread " << std::this_thread::get_id() << " enqueued " << i << std::endl;
        }
    };

    for (int i = 0; i < 10; ++i)
    {
        thread_pool.emplace_back(placeIntoQueue);
    }

    for (auto &thread : thread_pool)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return EXIT_SUCCESS;
}