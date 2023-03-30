#include <iostream>
#include <memory_resource>
#include <vector>

template <typename T> struct Allocator
{
    using value_type = T;
    T *allocate(std::size_t n)
    {
        std::cout << "Allocating " << n << " objects of size " << sizeof(T) << std::endl;
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    void deallocate(T *p, std::size_t n)
    {
        std::cout << "Deallocating " << n << " objects of size " << sizeof(T) << std::endl;
        ::operator delete(p);
    }
};

template <typename ValueType, typename AllocatorType>
void printVector(const std::vector<ValueType, AllocatorType> &vector)
{
    // Output the contents of the vector
    for (const auto &element : vector)
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main()
{
    {
        // Declare a vector with a custom allocator
        std::vector<int, Allocator<int>> vector;

        // Reserve space for 10 element
        vector.reserve(10);

        // Add some integers
        vector.push_back(1);
        vector.push_back(2);
        vector.push_back(3);

        printVector(vector);
    }

    {
        // Create a memory resource for the vector
        std::pmr::monotonic_buffer_resource buffer(1024);

        // Create a vector using the memory resource
        std::pmr::vector<int> pmr_vector(&buffer);

        // Add some elements to the vector
        pmr_vector.push_back(11);
        pmr_vector.push_back(12);
        pmr_vector.push_back(13);

        printVector(pmr_vector);
    }

    return 0;
}