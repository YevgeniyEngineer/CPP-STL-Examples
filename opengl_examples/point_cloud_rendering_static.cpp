#include <GL/glut.h> // OpenGL Utility Toolkit
#include <cmath>
#include <iostream>
#include <vector>

// Define a simple point structure
struct Point
{
    float x, y, z;
    Point(float x, float y, float z) : x(x), y(y), z(z)
    {
    }
};

// Define a simple function to generate a random point cloud
std::vector<Point> generatePointCloud(int num_points)
{
    std::vector<Point> point_cloud;
    for (int i = 0; i < num_points; i++)
    {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
        float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
        float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
        point_cloud.emplace_back(x, y, z);
    }
    return point_cloud;
}

// Define a function to render the point cloud
void renderPointCloud(const std::vector<Point> &point_cloud)
{
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (const auto &point : point_cloud)
    {
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}

// Define a function to render the scene
void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    renderPointCloud(generatePointCloud(10000));

    glutSwapBuffers();
}

// Define a function to handle window resizing
void resize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    gluPerspective(45.0f, aspect_ratio, 0.1f, 100.0f);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Point Cloud Viewer");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);

    glutMainLoop();

    return 0;
}