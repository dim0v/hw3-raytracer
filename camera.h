#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <utility>

class Scene;

class Camera
{
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
    float fovx, fovy;
    int width, height;
public:
    Camera(glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, float _fovx, float _fovy, int _width, int _height);
    Camera();
    glm::vec3 getEye() const {return eye;}
    glm::vec3 getCenter() const {return center;}
    glm::vec3 getUp() const {return up;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    float getFovx() const {return fovx;}
    float getFovy() const {return fovy;}
    friend std::pair<Camera *, Scene *> readfile(const char* filename);
};

#endif // CAMERA_H
