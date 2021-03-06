#include <cmath>
#include "ray.h"
#include "camera.h"
#include "Transform.h"

Ray::Ray(glm::vec3 _start, glm::vec3 _dir):
    origin(_start), direction(_dir)
{
}

void Ray::applyTransform(const glm::mat4 &transform)
{
    vec4 newpos = vec4(origin, 1)*transform;
    vec4 newdir = vec4(direction, 0)*transform;
    origin = vec3(newpos)/newpos.w;
    direction = vec3(newdir);
}

Ray Ray::rayThruPixel(const Camera *cam, int X, int Y, float angle)
{
    float margin = 0.8;
    float addX = margin * cos(angle), addY = margin * sin(angle);
    addX = (addX + 1) / 2;
    addY = (addY + 1) / 2;
    float x = X+addX, y = Y+addY;
    Ray res(cam->getEye(), glm::vec3());
    glm::vec3 a = cam->getCenter() - cam->getEye();
    glm::vec3 u, v, w;
    w = glm::normalize(a);
    u = glm::normalize(glm::cross(cam->getUp(), w));
    v = glm::cross(w, u);
    float alpha, beta;
    alpha = tanf(cam->getFovx()*pi/180/2.)*(x - cam->getWidth()/2.)/(cam->getWidth()/2.);
    beta  = tanf(cam->getFovy()*pi/180/2.)*(y - cam->getHeight()/2.)/(cam->getHeight()/2.);
    res.direction = -glm::normalize(alpha*u + beta*v - w);
    return res;
}
