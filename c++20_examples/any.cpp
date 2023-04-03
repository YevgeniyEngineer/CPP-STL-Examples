// Supported since C++17
#include <any>
#include <iostream>
#include <string>
#include <vector>

// Function to print the content of std::any object
void printAny(const std::any &value)
{
    if (!value.has_value())
    {
        std::cout << "Empty std::empty\n";
        return;
    }

    try
    {
        const int &int_value = std::any_cast<int>(value);
        std::cout << "Integer value: " << int_value << std::endl;
    }
    catch (const std::bad_any_cast &ex)
    {
    }

    try
    {
        const double &double_value = std::any_cast<double>(value);
        std::cout << "Double value: " << double_value << std::endl;
    }
    catch (const std::bad_any_cast &ex)
    {
    }

    try
    {
        const std::string &string_value = std::any_cast<std::string>(value);
        std::cout << "String value: " << string_value << std::endl;
    }
    catch (const std::bad_any_cast &ex)
    {
    }
}

struct MyStruct
{
    int x;
    int y;
};

class MyClass
{
  public:
    MyClass(int a, int b) : a_(a), b_(b)
    {
    }

    int getSum() const
    {
        return (a_ + b_);
    }

  private:
    int a_;
    int b_;
};

int main()
{
    // The type of std::any has to be CopyConstructible
    // The type must be destructible
    std::vector<std::any> any_list;

    // Store different types in the std::any container
    any_list.push_back(42);
    any_list.push_back(3.14159);
    any_list.push_back(std::string("Hello, World!"));

    // Print contents of each std::any in std::vector
    for (const auto &value : any_list)
    {
        printAny(value);
    }

    // Modify std::any object
    std::any any_value = 42;
    printAny(any_value);

    any_value = 3.14;
    printAny(any_value);

    any_value = std::string("Value changed");
    printAny(any_value);

    // Reset std::any to empty state
    any_value.reset();
    printAny(any_value);

    // Create instances of MyStruct and MyClass
    MyStruct my_struct = {1, 2};
    MyClass my_class(3, 4);

    // Store instances in std::any
    std::any any_struct = my_struct;
    std::any any_class = my_class;

    // Access the values stored in std::any
    try
    {
        MyStruct &stored_struct = std::any_cast<MyStruct &>(any_struct);
        std::cout << "MyStruct: x = " << stored_struct.x << ", y = " << stored_struct.y << std::endl;
    }
    catch (const std::bad_any_cast &e)
    {
        std::cout << "Failed to cast std::any to MyStruct" << std::endl;
    }

    try
    {
        MyClass &stored_class = std::any_cast<MyClass &>(any_class);
        std::cout << "MyClass: sum = " << stored_class.getSum() << std::endl;
    }
    catch (const std::bad_any_cast &e)
    {
        std::cout << "Failed to cast std::any to MyClass" << std::endl;
    }

    return 0;
}