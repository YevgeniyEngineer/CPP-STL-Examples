#include <chrono>           // std::chrono::high_resolution_clock::now()
#include <cmath>            // std::sin
#include <csignal>          // std::signal
#include <cstdlib>          // std::exit
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

class DispatchQueue
{
  public:
    DispatchQueue() : task_arena_(5)
    {
    }

    template <typename F> void enqueue(F &&task)
    {
        task_arena_.execute([task{std::move(task)}, this] { task(); });
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
        for (int i = 0; i < 100000000; ++i)
        {
            for (int j = 0; j < 100000000; ++j)
            {
                sum += std::sin(2 * i) + std::cos(3 * j);
                sum *= std::abs(sum);
            }
        }
        std::cout << sum << std::endl;
    };

    DispatchQueue work_stealing_queue;

    auto t1 = std::chrono::high_resolution_clock::now();
    for (int task_no = 0; task_no < 5; ++task_no)
    {
        work_stealing_queue.enqueue(std::bind(task, task_no));
    }

    // Wait for all tasks to complete
    work_stealing_queue.wait();

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << (t2 - t1).count() / 1e9 << std::endl;

    return 0;
}