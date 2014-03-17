#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Camera;

class Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
public:
    Ray(glm::vec3 _start, glm::vec3 _dir);
    static Ray rayThruPixel(const Camera *cam, int x, int y, float angle);

    void applyTransform(const glm::mat4 &transform);
    glm::vec3 getOrigin() const {return origin;}
    glm::vec3 getDirection() const {return direction;}
    glm::vec3 trace(float t) const {return origin + direction * t;}
    bool operator== (const Ray &rhs) const {return origin == rhs.origin && direction == rhs.direction;}
};

Ray rayThruPixel(Camera cam, int X, int Y);

#endif // RAY_H
