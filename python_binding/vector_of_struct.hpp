// my_structs.h
#pragma once

#include <cstdint>
#include <vector>

struct MyStruct
{
    std::uint16_t x;
    long double y;
};

std::vector<MyStruct> createStructVector(int size);

void printStructVector(const std::vector<MyStruct> &vector);