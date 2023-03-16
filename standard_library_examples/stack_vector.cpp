#include <algorithm> // std::move_backward
#include <array>     // std::array
#include <cstddef>   // std::ptrdiff_t
#include <cstdint>   // std::size_t
#include <iostream>  // std::cout
#include <iterator>  // std::reverse_iterator
#include <stdexcept> // std::overflow_error, std::underflow_error
#include <utility>   // std::move

template <typename T, std::size_t N> class StackVector
{
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    StackVector() : size_(0){};

    bool empty() const noexcept
    {
        return (size_ == 0UL);
    }
    size_type size() const noexcept
    {
        return size_;
    }
    size_type max_size() const noexcept
    {
        return N;
    }
    void clear() noexcept
    {
        size_ = 0UL;
    }
    void push_back(const T &value)
    {
        if (size_ >= N)
        {
            throw std::overflow_error("StackVector is full");
        }
        data_[size_++] = value;
    }
    void push_back(T &&value)
    {
        if (size_ >= N)
        {
            throw std::overflow_error("StackVector is full");
        }
        data_[size_++] = std::move(value);
    }
    template <typename... Args> void emplace_back(Args &&...args)
    {
        if (size_ >= N)
        {
            throw std::overflow_error("StackVector is full");
        }
        data_[size_++] = T(std::forward<Args>(args)...);
    }
    void pop_back()
    {
        if (empty())
        {
            throw std::underflow_error("StackVector is empty");
        }
        --size_;
    }
    iterator insert(iterator pos, const T &value)
    {
        if (size_ >= N)
        {
            throw std::overflow_error("StackVector is full");
        }
        if (pos < begin() || pos > end())
        {
            throw std::out_of_range("Insert position out of range");
        }

        std::move_backward(pos, end(), end() + 1);
        *pos = value;
        ++size_;

        return pos;
    }
    iterator insert(iterator pos, T &&value)
    {
        if (size_ >= N)
        {
            throw std::overflow_error("StackVector is full");
        }
        if (pos < begin() || pos > end())
        {
            throw std::out_of_range("Insert position out of range");
        }

        std::move_backward(pos, end(), end() + 1);
        *pos = std::move(value);
        ++size_;

        return pos;
    }
    reference operator[](size_type index) noexcept
    {
        return data_[index];
    }
    const_reference operator[](size_type index) const noexcept
    {
        return data_[index];
    }
    reference at(size_type index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("StackVector index out of range");
        }
        return data_[index];
    }
    iterator begin() noexcept
    {
        return data_.data();
    }
    const_iterator cbegin() const noexcept
    {
        return data_.data();
    }
    iterator end() noexcept
    {
        return data_.data() + size_;
    }
    const_iterator cend() const noexcept
    {
        return data_.data() + size_;
    }
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

  private:
    std::array<T, N> data_;
    size_type size_;
};

int main()
{
    StackVector<int, 10> vec;

    // Add elements to the StackVector
    for (int i = 0; i < 5; ++i)
    {
        vec.push_back(i);
    }

    // Access elements
    std::cout << "vec[2] = " << vec[2] << std::endl;

    // Iterate over elements and print them
    for (const auto &val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Remove the last element
    vec.pop_back();

    // Check the size of the StackVector
    std::cout << "Size: " << vec.size() << std::endl;

    // for (auto it = vec.cbegin(); it != vec.cend(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }

    // for (auto it = vec.rbegin(); it != vec.rend(); ++it)
    // {
    //     std::cout << *it << std::endl;
    // }

    // Insert elements
    vec.insert(vec.begin() + 2, 42);
    vec.insert(vec.begin() + 4, 99);
    vec.emplace_back(5);

    // Iterate over elements and print them
    for (const auto &val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}