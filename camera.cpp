#include "camera.h"

Camera::Camera(glm::dvec3 _eye, glm::dvec3 _center, glm::dvec3 _up, double _fovx, double _fovy, int _width, int _height):
    eye(_eye), center(_center), up(_up), fovx(_fovx), fovy(_fovy), width(_width), height(_height)
{
}

Camera::Camera()
{
}
