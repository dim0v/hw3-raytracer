#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
    enum Light_type{point, directional};
    glm::dvec3 pos;
    glm::dvec3 color;
    Light_type type;
    Light();
};

#endif // LIGHT_H
