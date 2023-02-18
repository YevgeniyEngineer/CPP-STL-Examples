#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

class DetachableThreads
{
  private:
    std::vector<std::thread> threads;
    std::atomic<bool> running_ = true;

  public:
    DetachableThreads() = default;
    ~DetachableThreads()
    {
        for (auto &thread : threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

    const bool isRunning() const
    {
        return running_;
    }

    template <typename Predicate, typename... Args> void startTask(Predicate &&f, Args &&...args)
    {
        threads.emplace_back(std::forward<Predicate>(f), std::forward<Args>(args)...);
        threads.back().detach();
    }

    void stopDaemonThreads()
    {
        running_ = false;
    }
};

int main()
{
    DetachableThreads dt;
    std::mutex mutex;

    for (int thread_no = 0; thread_no < std::thread::hardware_concurrency(); ++thread_no)
    {
        dt.startTask([&dt, &mutex, thread_no] {
            while (dt.isRunning())
            {
                {
                    std::unique_lock lock(mutex);
                    std::cout << "Hello from thread " + std::to_string(thread_no) << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }

            {
                std::unique_lock lock(mutex);
                std::cout << "Thread " << std::this_thread::get_id() << " finished\n";
            }
        });
    }

    std::cin.get();

    dt.stopDaemonThreads();

    std::this_thread::sleep_for(1s);

    return 0;
}