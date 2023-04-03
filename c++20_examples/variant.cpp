// Available since C++17
#include <iostream>
#include <string>
#include <variant>
#include <vector>

// Define a custom visitor for std::variant
struct VariantVisitor
{
    void operator()(int i) const
    {
        std::cout << "Integer value: " << i << std::endl;
    }

    void operator()(double d) const
    {
        std::cout << "Double value: " << d << std::endl;
    }

    void operator()(const std::string &s) const
    {
        std::cout << "String value: " << s << std::endl;
    }
};

int main()
{
    // Create a variant to store int, double and string
    std::variant<int, double, std::string> my_variant;

    // Assign a value to the variant and print its contents
    my_variant = 42;
    std::visit(VariantVisitor{}, my_variant);

    my_variant = 3.14149;
    std::visit(VariantVisitor{}, my_variant);

    my_variant = std::string("Hello, World!");
    std::visit(VariantVisitor{}, my_variant);

    // Store std::variant in std::vector
    std::vector<std::variant<int, double, std::string>> variant_list;

    variant_list.emplace_back(10);
    variant_list.emplace_back(2.71828);
    variant_list.emplace_back(std::string("Another String"));

    for (const auto &value : variant_list)
    {
        std::visit(VariantVisitor{}, value);
    }

    return 0;
}