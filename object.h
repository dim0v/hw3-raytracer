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
    struct Material
    {
        glm::vec3 specular;
        glm::vec3 emission;
        glm::vec3 diffuse;
        glm::vec3 ambient;
        float shininess;
    };
    Object();
    virtual Intersection intersect(const Ray& ray) = 0;
    const Material getMat() const {return mat;}
    void setMat(Material _mat) {mat = _mat;}
    const glm::mat4 getTransform() const {return transform;}
    const glm::mat4 getInverseTransform() const {return inverseTransform;}
    void setTransform(glm::mat4 _transform) {transform = _transform; inverseTransform = glm::inverse(transform);}
protected:
    Material mat;
};

#endif // OBJECT_H
