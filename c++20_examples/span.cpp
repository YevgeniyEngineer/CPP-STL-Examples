#include <algorithm>
#include <array>
#include <iostream>
#include <span>
#include <vector>

void printSum(const std::span<int> &numbers)
{
    int sum = 0;
    for (const auto &number : numbers)
    {
        sum += number;
    }
    std::cout << "Sum: " << sum << std::endl;
}

int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::array<int, 5> arr{6, 7, 8, 9, 10};

    // Create spans from containers
    std::span vec_span(vec);
    std::span arr_span(arr);

    // Subspan
    printSum(vec_span);
    printSum(arr_span);

    // Subspan
    std::span sub_vec_span = vec_span.subspan(1, 3);
    printSum(sub_vec_span);

    // Create a span from C array
    int c_array[] = {11, 12, 13};
    std::span c_array_span(c_array, 3);
    printSum(c_array_span);

    // Modify elements through a span
    std::cout << "Vector elements: ";
    for (const auto &element : vec)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    std::transform(vec_span.begin(), vec_span.end(), vec_span.begin(), [](int x) { return x * 2; });

    // Print modified elements
    std::cout << "Modified vector elements: ";
    for (const auto &element : vec)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}