// Available since C++17
#include <iostream>
#include <map>
#include <string>
#include <tuple>

struct Point
{
    int x, y;
};

std::tuple<int, double, std::string> getTuple()
{
    return {42, 3.14, std::string("Hello, World!")};
}

int main()
{
    // Decompose a tuple into structured bindings
    auto [int_value, double_value, string_value] = getTuple();
    std::cout << "Tuple values: " << int_value << ", " << double_value << ", " << string_value << std::endl;

    // Decompose a pair into structured bindings
    std::map<std::string, int> word_count = {{"apple", 3}, {"banana", 5}, {"orange", 2}};
    for (const auto &[word, count] : word_count)
    {
        std::cout << "Word: " << word << ", Count: " << count << std::endl;
    }

    // Decompose a struct into structured bindings
    Point point{10, 20};
    auto [x, y] = point;
    std::cout << "Point coordinates: (" << x << ", " << y << ")" << std::endl;

    // Decompose a const struct using structured bindings
    const Point const_point = {30, 40};
    const auto &[const_x, const_y] = const_point;
    std::cout << "Const point coordinates: (" << const_x << ", " << const_y << ")" << std::endl;

    return 0;
}