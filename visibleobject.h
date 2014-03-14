#ifndef VISIBLEOBJECT_H
#define VISIBLEOBJECT_H

#include <glm/glm.hpp>
#include "object.h"

class Intersection;
class Ray;

class VisibleObject : public Object
{
public:
    struct Material
    {
        Material():shininess(10){}
        glm::vec3 specular;
        glm::vec3 emission;
        glm::vec3 diffuse;
        glm::vec3 ambient;
        float shininess;
    };
    VisibleObject();
    const Material getMat() const {return mat;}
    void setMat(Material _mat) {mat = _mat;}
protected:
    Material mat;
};

#endif // VISIBLEOBJECT_H
