// Since C++20
#include <iostream>
#include <utility>

int main()
{
    int x{-3};
    unsigned y{7};

    // Unsafe comparison of integer types
    std::cout << "x < y (unsafe): " << (x < y) << std::endl;

    // Safe comparison of integer types
    std::cout << "x == y: " << std::cmp_equal(x, y) << std::endl;
    std::cout << "x != y: " << std::cmp_not_equal(x, y) << std::endl;
    std::cout << "x < y: " << std::cmp_less(x, y) << std::endl;
    std::cout << "x <= y: " << std::cmp_less_equal(x, y) << std::endl;
    std::cout << "x > y: " << std::cmp_greater(x, y) << std::endl;
    std::cout << "x >= y: " << std::cmp_greater_equal(x, y) << std::endl;

    return 0;
}