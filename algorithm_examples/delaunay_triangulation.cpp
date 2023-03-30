#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "plotting/matplotlibcpp.hpp"

namespace plt = matplotlibcpp;

// Point structure
struct Point
{
    double x, y;
    constexpr Point(double x = 0, double y = 0) : x(x), y(y){};
};

// Edge structure
struct Edge
{
    int u, v;
    constexpr Edge(int u = -1, int v = -1) : u(u), v(v){};
};

// Triangle structure
struct Triangle
{
    int a, b, c;
    constexpr Triangle(int a = -1, int b = -1, int c = -1) : a(a), b(b), c(c){};
};

// Compare points by x-coordinate, then y-coordinate
constexpr static inline bool cmp(const Point &a, const Point &b) noexcept
{
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

// Compute the Euclidean distance between two points
constexpr static inline double dist(const Point &a, const Point &b) noexcept
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Check if the points are in counterclockwise order
constexpr static inline bool ccw(const Point &a, const Point &b, const Point &c) noexcept
{
    return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
}

// Compute the circumcenter of a triangle
constexpr static inline Point circumcenter(const Point &a, const Point &b, const Point &c) noexcept
{
    double ax = a.x, ay = a.y;
    double bx = b.x, by = b.y;
    double cx = c.x, cy = c.y;
    double D = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
    double Ux = (ax * ax + ay * ay) * (by - cy) + (bx * bx + by * by) * (cy - ay) + (cx * cx + cy * cy) * (ay - by);
    double Uy = (ax * ax + ay * ay) * (cx - bx) + (bx * bx + by * by) * (ax - cx) + (cx * cx + cy * cy) * (bx - ax);
    if (std::abs(D) > std::numeric_limits<double>::epsilon())
    {
        return Point(Ux / D, Uy / D);
    }
    else
    {
        return Point(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    }
}

// Recursive function to construct the Delaunay triangulation of a set of points
// @param points input points used for construction of triangulation
// @param l and r index of leftmost middle and rightmost point of the triangle being constructed
// @param result list of triangles
static inline void triangulate(const std::vector<Point> &points, int l, int r, std::vector<Triangle> &result)
{
    if (r - l == 2)
    {
        // Base case: triangle with three points
        int a = l;
        int b = l + 1;
        int c = r;
        if (ccw(points[a], points[b], points[c]))
        {
            result.emplace_back(a, b, c);
        }
        else
        {
            result.emplace_back(a, c, b);
        }
    }
    else if (r - l > 2)
    {
        // Divide and conquer: recursively triangulate left and right subsets
        int m = (l + r) / 2;

        std::vector<Triangle> left;
        std::vector<Triangle> right;

        triangulate(points, l, m, left);
        triangulate(points, m, r, right);

        int i = 0;
        int j = 0;
        while (i < left.size() && j < right.size())
        {
            // Merge: merge left and right subsets into a single triangulation
            auto &a = left[i];
            auto &b = right[j];
            if (ccw(points[a.a], points[a.b], points[b.c]))
            {
                result.emplace_back(a);
                ++i;
            }
            else
            {
                result.emplace_back(b.c, b.b, a.b);
                result.emplace_back(b.c, a.b, a.a);
                ++j;
            }
        }
        while (i < left.size())
        {
            result.emplace_back(left[i]);
            ++i;
        }
        while (j < right.size())
        {
            result.emplace_back(right[j]);
            ++j;
        }
    }
}

// Construct the Delaunay triangulation of a set of points
static void delaunayTriangulation(std::vector<Point> &points, std::vector<Triangle> &triangles)
{
    // Sort points by x-coordinate, then y-coordinate
    std::sort(points.begin(), points.end(), cmp);

    // Construct the Delaunay triangles
    triangulate(points, 0, points.size(), triangles);
}

// Generate a random double between min and max
static inline double randomDouble(double min, double max) noexcept
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
}

// Generate an array of n random 2D points
static inline std::vector<Point> generateRandomPoints(int n, double minX, double maxX, double minY, double maxY)
{
    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i)
    {
        double x = randomDouble(minX, maxX);
        double y = randomDouble(minY, maxY);
        points[i] = Point(x, y);
    }
    return points;
}

// Sample usage
int main()
{
    bool print_results = false;

    int N = 500;
    std::vector<Point> points = generateRandomPoints(N, -5, 5, -3, 7);

    std::vector<Triangle> triangles;

    auto t1 = std::chrono::high_resolution_clock::now();

    delaunayTriangulation(points, triangles);

    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << (t2 - t1).count() / 1e9 << " seconds\n";

    if (print_results)
    {
        for (const auto &t : triangles)
        {
            std::cout << "(" << points[t.a].x << "," << points[t.a].y << ") ";
            std::cout << "(" << points[t.b].x << "," << points[t.b].y << ") ";
            std::cout << "(" << points[t.c].x << "," << points[t.c].y << ")" << std::endl;
        }
    }

    std::vector<std::vector<Point>> triangulations;
    for (const auto &t : triangles)
    {
        std::vector<Point> triangle;
        triangle.emplace_back(points[t.a].x, points[t.a].y);
        triangle.emplace_back(points[t.b].x, points[t.b].y);
        triangle.emplace_back(points[t.c].x, points[t.c].y);
        triangulations.emplace_back(triangle);
    }

    for (const auto &triangle : triangulations)
    {
        std::vector<double> x_coords;
        std::vector<double> y_coords;
        for (const auto &point : triangle)
        {
            x_coords.emplace_back(point.x);
            y_coords.emplace_back(point.y);
        }
        plt::plot(x_coords, y_coords);
    }
    plt::show();
    plt::detail::_interpreter::kill();

    return EXIT_SUCCESS;
}