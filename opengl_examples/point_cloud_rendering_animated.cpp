#include <GL/glut.h> // OpenGL Utility Toolkit
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

// Define a simple point structure
struct Point
{
    float x, y, z;
    float dx, dy, dz;
    Point(float x, float y, float z) : x(x), y(y), z(z), dx(0.0f), dy(0.0f), dz(0.0f)
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

// Define a function to update the position of each point in the point cloud
void updatePointCloud(std::vector<Point> &point_cloud)
{
    const float speed = 0.01f;
    for (auto &point : point_cloud)
    {
        point.dx = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
        point.dy = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
        point.dz = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
        point.x += point.dx * speed;
        point.y += point.dy * speed;
        point.z += point.dz * speed;
        if (std::abs(point.x) > 0.5f || std::abs(point.y) > 0.5f || std::abs(point.z) > 0.5f)
        {
            point.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
            point.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
            point.z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f;
            point.dx = 0.0f;
            point.dy = 0.0f;
            point.dz = 0.0f;
        }
    }
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

    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    // Calculate the time since the last frame
    float delta_time =
        std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time).count() / 1000000.0f;

    static std::vector<Point> point_cloud = generatePointCloud(10000);
    updatePointCloud(point_cloud);

    glColor3f(1.0f, 1.0f, 1.0f);
    renderPointCloud(point_cloud);

    glutSwapBuffers();

    last_time = current_time;

    // Request a redraw of the window
    glutPostRedisplay();
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
    glutCreateWindow("Animated Point Cloud Viewer");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);

    glutMainLoop();

    return 0;
}