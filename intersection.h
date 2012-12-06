#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
#include <limits>
#include "ray.h"

class Object;
class Ray;

class Intersection
{
    glm::dvec3 normal;
    Ray ray;
    double t;
    Object *object;
public:
    Intersection(glm::dvec3 _normal = glm::dvec3(), Ray _ray = Ray(glm::dvec3(), glm::dvec3()),
                 double _t = std::numeric_limits<double>::infinity(), Object *obj = NULL);
    glm::dvec3 getNormal() const {return normal;}
    Ray getRay() const {return ray;}
    double getRayPos() const {return t;}
    Object* getObject() const {return object;}
};
#endif // INTERSECTION_H
