#include <algorithm>
#include <chrono>
#include <cstdint>
#include <execution>
#include <iostream>
#include <vector>

void vectorBenchmarksSimple(const std::size_t number_of_elements)
{
    std::vector<int> vec(number_of_elements);

    std::chrono::_V2::high_resolution_clock::time_point start_time;
    std::chrono::_V2::high_resolution_clock::time_point stop_time;

    // Using index access
    start_time = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < number_of_elements; ++i)
    {
        vec[i] += 5;
    }
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (accessing by index): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using element by reference access
    start_time = std::chrono::high_resolution_clock::now();
    for (auto &v : vec)
    {
        v += 5;
    }
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (accessing by reference): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using iterator
    start_time = std::chrono::high_resolution_clock::now();
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        *it += 5;
    }
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (accessing by iterator): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using std::for_each with std::execution::seq
    // Execution may not be parallelized
    start_time = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::seq, vec.begin(), vec.end(), [&](auto &element) -> void { element += 5; });
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (std::for_each sequential): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using std::for_each with std::execution::par
    // Execution may be parallelized
    start_time = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par, vec.begin(), vec.end(), [&](auto &element) -> void { element += 5; });
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (std::for_each parallel): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using std::for_each with std::execution::par_unseq
    // Execution may be parallelized, vectorized, or migrated across threads
    start_time = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par_unseq, vec.begin(), vec.end(), [&](auto &element) -> void { element += 5; });
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (std::for_each parallel, unseq): " << (stop_time - start_time).count() / 1e9
              << std::endl;
}

int main()
{
    std::size_t number_of_elements;

    std::cout << "Number of elements: ";
    std::cin >> number_of_elements;

    vectorBenchmarksSimple(number_of_elements);

    return EXIT_SUCCESS;
}