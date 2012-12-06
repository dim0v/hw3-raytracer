#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "object.h"

class Object;
class Intersection;

class Triangle : public Object
{
    glm::dvec3 A, B, C;
    glm::dvec3 nA, nB, nC;
public:
    Triangle(glm::dvec3 A, glm::dvec3 B, glm::dvec3 C, glm::dvec3 nA = glm::dvec3(),
             glm::dvec3 nB = glm::dvec3(), glm::dvec3 nC = glm::dvec3());
    virtual Intersection intersect(Ray ray);
};

#endif // TRIANGLE_H
