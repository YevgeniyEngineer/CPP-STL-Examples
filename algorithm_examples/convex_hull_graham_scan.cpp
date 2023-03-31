#include <algorithm>
#include <chrono>
#include <cstdint>
#include <execution>
#include <iostream>
#include <random>
#include <tuple>
#include <type_traits>
#include <vector>

template <typename T> struct Point
{
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                  "Type must be either integral or floating-point");

    T x, y;

    constexpr inline bool operator<(const Point &other) const noexcept
    {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

template <typename T>
constexpr inline T crossProduct(const Point<T> &p1, const Point<T> &p2, const Point<T> &p3) noexcept
{
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

template <typename T> inline bool compare(const Point<T> &p1, const Point<T> &p2, const Point<T> &origin) noexcept
{
    const T cp = crossProduct(origin, p1, p2);

    const T p1x = p1.x - origin.x;
    const T p1y = p1.y - origin.y;
    const T p2x = p2.x - origin.x;
    const T p2y = p2.y - origin.y;

    return (cp > 0) || ((cp == 0) && ((p1x * p1x + p1y * p1y) < (p2x * p2x + p2y * p2y)));
}

template <typename T> std::vector<Point<T>> grahamScan(std::vector<Point<T>> points)
{
    if (points.size() < 3)
    {
        return points;
    }

    std::sort(std::execution::par, points.begin(), points.end());
    Point<T> origin = points[0];

    std::sort(std::execution::par, points.begin() + 1, points.end(),
              [&](const Point<T> &p1, const Point<T> &p2) { return compare(p1, p2, origin); });

    std::vector<Point<T>> hull;
    hull.reserve(points.size());
    hull.emplace_back(points[0]);
    hull.emplace_back(points[1]);
    hull.emplace_back(points[2]);

    std::size_t hull_size = 3;
    for (std::size_t i = 3; i < points.size(); ++i)
    {
        const auto &point = points[i];
        while ((hull_size > 1) && crossProduct(hull[hull_size - 2], hull.back(), point) <= 0)
        {
            hull.pop_back();
            --hull_size;
        }
        hull.emplace_back(point);
        ++hull_size;
    }

    return hull;
}

template <typename T> std::vector<Point<T>> generate_random_points(std::size_t n, T min_value, T max_value)
{
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                  "Type must be either integral or floating-point");

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Point<T>> points;
    points.reserve(n);

    if constexpr (std::is_integral<T>::value)
    {
        std::uniform_int_distribution<T> dist(min_value, max_value);

        for (std::size_t i = 0; i < n; ++i)
        {
            points.emplace_back(Point<T>{dist(gen), dist(gen)});
        }
    }
    else
    {
        std::uniform_real_distribution<T> dist(min_value, max_value);

        for (std::size_t i = 0; i < n; ++i)
        {
            points.emplace_back(Point<T>{dist(gen), dist(gen)});
        }
    }

    return points;
}

int main()
{
    constexpr std::size_t N = 100'000;
    auto points = generate_random_points<double>(N, -1000.0, 1000.0);

    auto start_time = std::chrono::high_resolution_clock::now();
    auto hull = grahamScan(points);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    std::cout << "Convex hull calculation for " << N << " points took " << elapsed.count() << " ms\n";

    return 0;
}