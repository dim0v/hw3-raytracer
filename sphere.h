#ifndef SPHERE_H
#define SPHERE_H

#include <object.h>
#include <glm/glm.hpp>

class Sphere : public Object
{
    glm::dvec3 center;
    double radius;
public:
    Sphere(glm::dvec3 _center, double _radius);
    virtual Intersection intersect(Ray ray);
};

#endif // SPHERE_H
