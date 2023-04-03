// Supported since C++17
#include <cstring>
#include <iostream>
#include <optional>
#include <string>

std::optional<int> parseInt(const std::string &str)
{
    try
    {
        int value = std::stoi(str);
        return value;
    }
    catch (const std::invalid_argument &ex)
    {
        return std::nullopt;
    }
    catch (const std::out_of_range &ex)
    {
        return std::nullopt;
    }
}

int main()
{
    std::string input1{"42"};
    std::string input2{"not a number"};

    auto num1 = parseInt(input1);
    auto num2 = parseInt(input2);

    if (num1.has_value())
    {
        std::cout << "Parsed '" << input1 << "' as " << num1.value() << std::endl;
    }
    else
    {
        std::cout << "Failed to parse '" << input1 << "' \n";
    }

    if (num2.has_value())
    {
        std::cout << "Parsed '" << input2 << "' as " << num2.value() << std::endl;
    }
    else
    {
        std::cout << "Failed to parse '" << input2 << "' \n";
    }

    return 0;
}