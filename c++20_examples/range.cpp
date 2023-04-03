#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> input = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto divisible_by_three = [](const int n) { return n % 3 == 0; };
    auto square = [](const int n) { return n * n; };

    // Input | Predicate | Transform
    auto x = input | std::views::filter(divisible_by_three) | std::views::transform(square);

    for (int i : x)
    {
        std::cout << i << '\n';
    }
    return 0;
}