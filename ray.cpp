#include <cmath>
#include "ray.h"
#include "camera.h"
#include "Transform.h"

Ray::Ray(glm::vec3 _start, glm::vec3 _dir):
    start(_start), direction(_dir)
{
}

void Ray::applyTransform(const glm::mat4 &transform)
{
    vec4 newpos = vec4(start, 1)*transform;
    vec4 newdir = vec4(direction, 0)*transform;
    start = vec3(newpos)/newpos.w;
    direction = vec3(newdir);
}

Ray Ray::rayThruPixel(const Camera *cam, int X, int Y)
{
    float x = X+.5, y = Y+.5;
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
