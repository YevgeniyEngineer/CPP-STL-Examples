#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

constexpr int num_threads = 4;
constexpr int num_iterations = 100000;

std::atomic<int> atomic_counter{0};
std::mutex print_mutex;

void worker()
{
    for (int i = 0; i < num_iterations; ++i)
    {
        // Increment the atomic counter
        ++atomic_counter;

        // Print the current value of the counter every 10,000 iterations
        if (i % 10000 == 0)
        {
            std::unique_lock<std::mutex> lock(print_mutex);
            std::cout << "Thread " << std::this_thread::get_id() << ": " << atomic_counter.load() << std::endl;
        }
    }
}

int main()
{
    // Create and launch jthreads
    std::vector<std::jthread> threads;
    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(worker);
    }

    // No need to join jthreads explicitly, they will join automatically upon destruction
    return 0;
}
