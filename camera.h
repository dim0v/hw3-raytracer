#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <utility>

class Scene;

class Camera
{
    glm::dvec3 eye;
    glm::dvec3 center;
    glm::dvec3 up;
    double fovx, fovy;
    int width, height;
public:
    Camera(glm::dvec3 _eye, glm::dvec3 _center, glm::dvec3 _up, double _fovx, double _fovy, int _width, int _height);
    Camera();
    glm::dvec3 getEye() const {return eye;}
    glm::dvec3 getCenter() const {return center;}
    glm::dvec3 getUp() const {return up;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    double getFovx() const {return fovx;}
    double getFovy() const {return fovy;}
    friend std::pair<Camera *, Scene *> readfile(const char* filename);
};

#endif // CAMERA_H
