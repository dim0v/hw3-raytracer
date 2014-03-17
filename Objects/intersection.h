#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
#include <limits>
#include "ray.h"

class Ray;
class VisibleObject;

class Intersection
{
    glm::vec3 normal;
    Ray ray;
    float t;
    VisibleObject *object;
public:
    Intersection(glm::vec3 _normal = glm::vec3(), Ray _ray = Ray(glm::vec3(), glm::vec3()),
                 float _t = std::numeric_limits<float>::infinity(), VisibleObject *obj = NULL);
    const glm::vec3& getNormal() const {return normal;}
    const Ray& getRay() const {return ray;}
    const float& getRayPos() const {return t;}
    const VisibleObject* getObject() const {return object;}
};
#endif // INTERSECTION_H
