#ifndef SPHERE_H
#define SPHERE_H

#include <object.h>
#include <glm/glm.hpp>

class Sphere : public Object
{
    glm::vec3 center;
    float radius;
public:
    Sphere(glm::vec3 _center, float _radius);
    virtual Intersection intersect(const Ray& ray);
};

#endif // SPHERE_H
