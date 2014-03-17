#include "camera.h"

Camera::Camera(glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up, float _fovx, float _fovy, int _width, int _height):
    eye(_eye), center(_center), up(_up), fovx(_fovx), fovy(_fovy), width(_width), height(_height)
{
}

Camera::Camera()
{
}
