#include <algorithm>  // std::for_each
#include <cstdint>    // std::uint32_t
#include <deque>      // std::deque
#include <functional> // std::function
#include <future>     // std::async
#include <iostream>   // std::cout
#include <memory>     // std::shared_ptr
#include <mutex>      // std::mutex, std::unique_lock
#include <thread>     // std::thread
#include <utility>    // std::move
#include <vector>     // std::vector

class WorkStealingThreadPool
{
  public:
    WorkStealingThreadPool(std::uint32_t num_threads = std::thread::hardware_concurrency())
        : done_{false}, queues_{num_threads}
    {
        for (std::uint32_t i = 0; i < num_threads; ++i)
        {
            threads_.emplace_back([this] {
                while (!done_.load())
                {
                    std::function<void()> task;
                    if (try_pop_task(task) || steal_task(task))
                    {
                        task();
                    }
                    else
                    {
                        // Provides a hint to the implementation to reschedule the execution of threads, allowing other
                        // threads to run.
                        std::this_thread::yield();
                    }
                }
            });
        }
    }

    ~WorkStealingThreadPool()
    {
        done_.store(true);
        for (auto &thread : threads_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    template <typename F> void push_task(F &&task)
    {
        std::uint32_t current_thread_index = thread_index_++;
        if (thread_index_ >= threads_.size())
        {
            thread_index_ = 0;
        }
        {
            std::unique_lock<std::mutex> lock(queues_mutex_);
            queues_[current_thread_index].emplace_front(std::forward<F>(task));
        }
    }

  private:
    bool try_pop_task(std::function<void()> &task)
    {
        std::unique_lock<std::mutex> lock(queues_mutex_, std::try_to_lock);
        if (!lock)
        {
            return false;
        }

        for (auto &queue : queues_)
        {
            if (!queue.empty())
            {
                task = std::move(queue.front());
                queue.pop_front();
                return true;
            }
        }
        return false;
    }

    bool steal_task(std::function<void()> &task)
    {
        std::unique_lock<std::mutex> lock(queues_mutex_);
        for (auto it = queues_.rbegin(); it != queues_.rend(); ++it)
        {
            if (!it->empty())
            {
                task = std::move(it->back());
                it->pop_back();
                return true;
            }
        }
        return false;
    }

    std::vector<std::thread> threads_;
    std::vector<std::deque<std::function<void()>>> queues_;
    std::mutex queues_mutex_;
    std::atomic<std::uint32_t> thread_index_{0};
    std::atomic<bool> done_;
};

template <typename InputIt, typename Func>
void parallel_for_each(InputIt first, InputIt last, Func func, bool parallel = true)
{
    if (parallel)
    {
        WorkStealingThreadPool thread_pool;
        std::vector<std::future<void>> futures;

        for (auto it = first; it != last; ++it)
        {
            futures.push_back(std::async(
                std::launch::async, [&thread_pool, &func, it] { thread_pool.push_task([&func, it] { func(*it); }); }));
        }

        for (auto &future : futures)
        {
            future.wait();
        }
    }
    else
    {
        std::for_each(first, last, func);
    }
}

// Function to calculate the factorial of a number
std::uint64_t factorial(std::uint32_t n)
{
    std::uint64_t result = 1;
    for (std::uint32_t i = 1; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}

int main()
{
    std::vector<std::uint32_t> numbers = {10, 5, 8, 12, 6};
    std::vector<std::future<std::uint64_t>> results;

    WorkStealingThreadPool thread_pool;

    for (const auto &number : numbers)
    {
        auto task = std::make_shared<std::packaged_task<std::uint64_t()>>([number] { return factorial(number); });
        results.push_back(task->get_future());
        thread_pool.push_task([task] { (*task)(); });
    }

    for (std::uint32_t i = 0; i < results.size(); ++i)
    {
        std::cout << "Factorial: " << results[i].get() << std::endl;
    }

    return 0;
}