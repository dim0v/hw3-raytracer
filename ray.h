#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Camera;

class Ray
{
    glm::vec3 start;
    glm::vec3 direction;
public:
    Ray(glm::vec3 _start, glm::vec3 _dir);
    static Ray rayThruPixel(const Camera *cam, int x, int y);

    void applyTransform(const glm::mat4 &transform);
    glm::vec3 getStart() const {return start;}
    glm::vec3 getDirection() const {return direction;}
    glm::vec3 trace(float t) const {return start + direction * t;}
    bool operator== (const Ray &rhs) const {return start == rhs.start && direction == rhs.direction;}
};

Ray rayThruPixel(Camera cam, int X, int Y);

#endif // RAY_H
