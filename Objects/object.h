#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "intersection.h"
#include "ray.h"

class Object
{
protected:
    glm::mat4 transform;
    glm::mat4 inverseTransform;
public:
    const glm::mat4 getTransform() const {return transform;}
    const glm::mat4 getInverseTransform() const {return inverseTransform;}
    void setTransform(glm::mat4 _transform) {transform = _transform; inverseTransform = glm::inverse(transform);}

    virtual Intersection intersect(const Ray& ray, const Object* objToPass = nullptr) = 0;
};

#endif // OBJECT_H
