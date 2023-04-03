#include <compare>
#include <iostream>

struct Point
{
    int x;
    int y;

    auto operator<=>(const Point &other) const = default; // Default three-way comparison operator
};

int main()
{
    Point a{1, 2};
    Point b{2, 1};
    Point c{1, 2};

    auto result1 = a <=> b;
    auto result2 = a <=> c;

    if (result1 < 0)
    {
        std::cout << "a is less than b\n";
    }
    else if (result1 == 0)
    {
        std::cout << "a is equal to b\n";
    }
    else
    {
        std::cout << "a is greater than b\n";
    }

    if (result2 < 0)
    {
        std::cout << "a is less than c\n";
    }
    else if (result2 == 0)
    {
        std::cout << "a is equal to c\n";
    }
    else
    {
        std::cout << "a is greater than c\n";
    }

    return 0;
}