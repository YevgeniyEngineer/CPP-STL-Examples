// Supported since C++20
#include <algorithm>
#include <concepts>
#include <iostream>
#include <list>
#include <vector>

// Define a concept that represent a container that supports methods such as size() and push_back()
template <typename T>
concept Container = requires(T container)
{
    {
        container.size()
        } -> std::same_as<std::size_t>;
    container.push_back(std::declval<typename T::value_type>());
};

// Function template to add elements to a container that satisfies the Container concept
template <typename C, typename T> void addElements(C &container, const T &value, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        container.push_back(value);
    }
}

int main()
{
    std::vector<int> vec;
    std::list<int> lst;

    addElements(vec, 42, 3);
    addElements(lst, 42, 3);

    // Print the contents of the vector and list
    std::cout << "Vector: ";
    for (const auto &val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "List: ";
    for (const auto &val : lst)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Uncomment the following lines to see the error message when using a non-Container type
    // int non_container = 0;
    // addElements(non_container, 42, 3); // Error: constraints not satisfied

    return 0;
}