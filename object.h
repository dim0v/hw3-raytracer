#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

class Intersection;
class Ray;

class Object
{
protected:
    glm::mat4 transform;
    glm::mat4 inverseTransform;
public:
    virtual Intersection intersect(const Ray& ray) = 0;
    const glm::mat4 getTransform() const {return transform;}
    const glm::mat4 getInverseTransform() const {return inverseTransform;}
    void setTransform(glm::mat4 _transform) {transform = _transform; inverseTransform = glm::inverse(transform);}
};

#endif // OBJECT_H
