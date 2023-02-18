#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

std::random_device rd;
std::mt19937 gen(rd());

inline bool isPrime(const int number)
{
    // 0 and 1 are not prime numbers
    if (number == 0 || number == 1)
    {
        return false;
    }

    // loop to check if n is prime
    for (std::size_t i = 2; i <= number / 2; ++i)
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

void vectorBenchmarksSimple(const std::size_t number_of_elements)
{
    std::uniform_int_distribution dist(0, 10000000);

    std::vector<int> vec;
    vec.reserve(number_of_elements);
    for (std::size_t i = 0; i < number_of_elements; ++i)
    {
        vec.emplace_back(dist(gen));
    }

    std::chrono::_V2::high_resolution_clock::time_point start_time;
    std::chrono::_V2::high_resolution_clock::time_point stop_time;

    // Using index access
    start_time = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < number_of_elements; ++i)
    {
        isPrime(vec[i]);
    }
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (accessing by index): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using element by reference access
    start_time = std::chrono::high_resolution_clock::now();
    for (auto &v : vec)
    {
        isPrime(v);
    }
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (accessing by reference): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using iterator
    start_time = std::chrono::high_resolution_clock::now();
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        isPrime(*it);
    }
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (accessing by iterator): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using std::for_each with std::execution::seq
    // Execution may not be parallelized
    start_time = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::seq, vec.begin(), vec.end(), [&](auto &element) -> void { isPrime(element); });
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (std::for_each sequential): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using std::for_each with std::execution::par
    // Execution may be parallelized
    start_time = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par, vec.begin(), vec.end(), [&](auto &element) -> void { isPrime(element); });
    stop_time = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time (std::for_each parallel): " << (stop_time - start_time).count() / 1e9 << std::endl;

    // Using std::for_each with std::execution::par_unseq
    // Execution may be parallelized, vectorized, or migrated across threads
    start_time = std::chrono::high_resolution_clock::now();
    std::for_each(std::execution::par_unseq, vec.begin(), vec.end(), [&](auto &element) -> void { isPrime(element); });
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