#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Camera;

class Ray
{
    glm::dvec3 start;
    glm::dvec3 direction;
public:
    Ray(glm::dvec3 _start, glm::dvec3 _dir);
    friend Ray rayThruPixel(Camera cam, int x, int y);
    glm::dvec3 getStart() const {return start;}
    glm::dvec3 getDirection() const {return direction;}
    glm::dvec3 trace(double t) const {return start + direction * t;}
    bool operator== (const Ray &rhs) const {return start == rhs.start && direction == rhs.direction;}
};

Ray rayThruPixel(Camera cam, int X, int Y);

#endif // RAY_H
