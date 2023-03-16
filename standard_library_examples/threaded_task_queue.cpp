#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <csignal>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

#define PRINT_DEBUG_INFO 0

namespace
{
std::atomic<bool> stop_status = false;
}

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    stop_status = true;
}

class TaskDispatchQueue
{
  public:
    explicit TaskDispatchQueue(unsigned int num_threads = std::thread::hardware_concurrency())
    {
        creation_time_ = std::chrono::steady_clock::now();
        for (unsigned int thread_no = 0; thread_no < num_threads; ++thread_no)
        {
            threads_.emplace_back([this] {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        condition_variable_.wait(lock, [this] { return stop_status || !tasks_.empty(); });

                        if (stop_status && tasks_.empty())
                        {
                            break;
                        }
                        task = std::move(tasks_.front());
                        tasks_.pop();

#if PRINT_DEBUG_INFO
                        std::cout << "Thread " << std::this_thread::get_id() << " received task\n";
#endif
                    }
                    task();
                }
            });
        }
    };

    ~TaskDispatchQueue()
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            std::cout << "Stopped main thread " << std::this_thread::get_id() << std::endl;
            stop_status = true;
        }
        condition_variable_.notify_all();
        for (auto &thread : threads_)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        deletion_time_ = std::chrono::steady_clock::now();
        std::cout << "Elapsed time: " << (deletion_time_ - creation_time_).count() / 1e9 << " seconds\n";
    };

    template <typename Predicate, typename... Args> void enqueue(Predicate &&func, Args &&...args)
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace(std::bind(std::forward<Predicate>(func), std::forward<Args>(args)...));
        };
        condition_variable_.notify_one();
    }

  private:
    std::condition_variable condition_variable_;
    std::mutex mutex_;
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::chrono::time_point<std::chrono::steady_clock> creation_time_;
    std::chrono::time_point<std::chrono::steady_clock> deletion_time_;
};

int main()
{
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGKILL, signalHandler);

    auto task = [](const int task_no) {
        double sum = 0.0;
        for (int i = 0; i < 1000; ++i)
        {
            for (int j = 0; j < 1000; ++j)
            {
                sum += std::sin(2 * i) + std::cos(3 * j);
            }
        }
        printf("Task %d completed with result %f\n", task_no, sum);
    };

    TaskDispatchQueue task_queue{};
    for (int task_no = 0; task_no < 10000; ++task_no)
    {
        task_queue.enqueue(task, task_no);
    }

    return EXIT_SUCCESS;
}