#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include "visibleobject.h"

class Sphere : public VisibleObject
{
    glm::vec3 center;
    float radius;
public:
    Sphere(glm::vec3 _center, float _radius);
    virtual Intersection intersect(const Ray& ray);
};

#endif // SPHERE_H
