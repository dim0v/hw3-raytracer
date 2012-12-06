#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

class Intersection;
class Ray;

class Object
{
protected:
    glm::dmat4 transform;
public:
    struct Material
    {
        glm::dvec3 specular;
        glm::dvec3 emission;
        glm::dvec3 diffuse;
        glm::dvec3 ambient;
        double shininess;
    };
    Object();
    virtual Intersection intersect(Ray ray) = 0;
    const Material getMat() const {return mat;}
    void setMat(Material _mat) {mat = _mat;}
    const glm::dmat4 getTransform() const {return transform;}
    void setTransform(glm::dmat4 _transform) {transform = _transform;}
protected:
    Material mat;
};

#endif // OBJECT_H
