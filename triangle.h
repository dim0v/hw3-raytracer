#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "visibleobject.h"

class Object;
class Intersection;

class Triangle : public VisibleObject
{
    glm::vec3 A, B, C;
    glm::vec3 nA, nB, nC;
public:
    Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 nA = glm::vec3(),
             glm::vec3 nB = glm::vec3(), glm::vec3 nC = glm::vec3());
    virtual Intersection intersect(const Ray &ray);

    virtual BoundingBox getBoundingBox() const;
};

#endif // TRIANGLE_H
