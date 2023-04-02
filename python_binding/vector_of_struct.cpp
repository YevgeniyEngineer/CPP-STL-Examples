#include "vector_of_struct.hpp"
#include <iostream>

std::vector<MyStruct> createStructVector(int size)
{
    std::vector<MyStruct> result;
    for (int i = 0; i < size; ++i)
    {
        MyStruct s;
        s.x = static_cast<std::uint16_t>(i);
        s.y = static_cast<long double>(i * 0.5);
        s.vec = {i * 1.f, i * 3.f, i * 5.f, i * 7.f};
        result.push_back(s);
    }
    return result;
}

void printStructVector(const std::vector<MyStruct> &vector)
{
    std::cout << "Printing (x, y) values: \n";
    for (const auto &element : vector)
    {
        std::cout << "(" << element.x << ", " << element.y << ")\n";
    }
    std::cout << std::endl;

    std::cout << "Printing vector: \n";
    for (const auto &element : vector)
    {
        for (const auto &v : element.vec)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}