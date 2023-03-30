#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const int MAX_DEPTH = 5;
const int MAX_OBJECTS = 10;

struct Point
{
    double x, y;
};

struct QuadTree
{
    int depth;
    double x, y, width, height;
    vector<Point> objects;
    QuadTree *children[4];

    QuadTree(int depth, double x, double y, double width, double height)
    {
        this->depth = depth;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        for (int i = 0; i < 4; i++)
        {
            children[i] = nullptr;
        }
    }

    bool isLeaf()
    {
        return children[0] == nullptr;
    }

    void split()
    {
        double child_width = width / 2.0;
        double child_height = height / 2.0;
        children[0] = new QuadTree(depth + 1, x, y, child_width, child_height);
        children[1] = new QuadTree(depth + 1, x + child_width, y, child_width, child_height);
        children[2] = new QuadTree(depth + 1, x, y + child_height, child_width, child_height);
        children[3] = new QuadTree(depth + 1, x + child_width, y + child_height, child_width, child_height);
    }

    int getIndex(Point p)
    {
        double mid_x = x + width / 2.0;
        double mid_y = y + height / 2.0;
        if (p.x < mid_x && p.y < mid_y)
        {
            return 0;
        }
        else if (p.x >= mid_x && p.y < mid_y)
        {
            return 1;
        }
        else if (p.x < mid_x && p.y >= mid_y)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }

    void insert(Point p)
    {
        if (!isLeaf())
        {
            int index = getIndex(p);
            children[index]->insert(p);
        }
        else
        {
            objects.push_back(p);
            if (objects.size() > MAX_OBJECTS && depth < MAX_DEPTH)
            {
                split();
                for (auto obj : objects)
                {
                    int index = getIndex(obj);
                    children[index]->insert(obj);
                }
                objects.clear();
            }
        }
    }

    vector<Point> query(double qx, double qy, double radius)
    {
        vector<Point> result;
        double dx = qx - (x + width / 2.0);
        double dy = qy - (y + height / 2.0);
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist > radius)
        {
            return result;
        }
        for (auto obj : objects)
        {
            double dx = qx - obj.x;
            double dy = qy - obj.y;
            double dist = std::sqrt(dx * dx + dy * dy);
            if (dist <= radius)
            {
                result.push_back(obj);
            }
        }
        if (!isLeaf())
        {
            vector<Point> child_result;
            for (int i = 0; i < 4; i++)
            {
                vector<Point> temp_result = children[i]->query(qx, qy, radius);
                child_result.insert(child_result.end(), temp_result.begin(), temp_result.end());
            }
            result.insert(result.end(), child_result.begin(), child_result.end());
        }
        return result;
    }
    void clear()
    {
        objects.clear();
        for (int i = 0; i < 4; i++)
        {
            if (children[i] != nullptr)
            {
                children[i]->clear();
                delete children[i];
                children[i] = nullptr;
            }
        }
    }

    ~QuadTree()
    {
        clear();
    }
};

int main()
{
    QuadTree *quadtree = new QuadTree(0, 0, 0, 100, 100);
    for (int i = 0; i < 1000; i++)
    {
        Point p = {static_cast<double>(rand() % 100), static_cast<double>(rand() % 100)};
        quadtree->insert(p);
    }
    vector<Point> result = quadtree->query(50, 50, 10);
    for (auto p : result)
    {
        cout << "(" << p.x << ", " << p.y << ")" << endl;
    }
    delete quadtree;
    return 0;
}
