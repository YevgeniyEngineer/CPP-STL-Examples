#include <chrono>           // std::chrono::high_resolution_clock::now()
#include <cmath>            // std::sin
#include <csignal>          // std::signal
#include <functional>       // std::bind
#include <iostream>         // std::cout
#include <tbb/task_arena.h> // tbb::task_arena
#include <tbb/task_group.h> // tbb::task_group
#include <utility>          // std::move

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    std::exit(signum);
}

class WorkStealingQueue
{
  public:
    WorkStealingQueue(size_t num_threads = std::thread::hardware_concurrency()) : task_arena_(num_threads)
    {
    }

    void enqueue(std::function<void()> &&task)
    {
        task_group_.run([task{std::move(task)}, this] { task(); });
    }

    void wait()
    {
        task_group_.wait();
    }

  private:
    tbb::task_arena task_arena_;
    tbb::task_group task_group_;
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

    WorkStealingQueue work_stealing_queue;

    auto start_time = std::chrono::steady_clock::now();
    for (int task_no = 0; task_no < 10000; ++task_no)
    {
        work_stealing_queue.enqueue(std::bind(task, task_no));
    }

    // Wait for all tasks to complete
    work_stealing_queue.wait();

    auto stop_time = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: " << (stop_time - start_time).count() / 1e9 << std::endl;

    return 0;
}