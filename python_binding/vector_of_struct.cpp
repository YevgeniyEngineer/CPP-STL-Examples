#include "vector_of_struct.hpp"
#include <iostream>

std::vector<MyStruct> createStructVector(int size)
{
    std::vector<MyStruct> result;
    for (int i = 0; i < size; ++i)
    {
        MyStruct s{static_cast<std::uint16_t>(i), static_cast<long double>(i * 0.5)};
        result.push_back(s);
    }
    return result;
}

void printStructVector(const std::vector<MyStruct> &vector)
{
    for (const auto &element : vector)
    {
        std::cout << "(" << element.x << ", " << element.y << ")\n";
    }
    std::cout << std::endl;
}