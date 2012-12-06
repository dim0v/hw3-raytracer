#include <cmath>
#include "ray.h"
#include "camera.h"
#include "Transform.h"

Ray::Ray(glm::dvec3 _start, glm::dvec3 _dir):
    start(_start), direction(_dir)
{
}

Ray rayThruPixel(Camera cam, int X, int Y)
{
    double x = X+.5, y = Y+.5;
    Ray res(cam.getEye(), glm::dvec3());
    glm::dvec3 a = cam.getCenter() - cam.getEye();
    glm::dvec3 u, v, w;
    w = glm::normalize(a);
    u = glm::normalize(glm::cross(cam.getUp(), w));
    v = glm::cross(w, u);
    double alpha, beta;
    alpha = tanf(cam.getFovx()*pi/180/2.)*(x - cam.getWidth()/2.)/(cam.getWidth()/2.);
    beta  = tanf(cam.getFovy()*pi/180/2.)*(y - cam.getHeight()/2.)/(cam.getHeight()/2.);
    res.direction = -glm::normalize(alpha*u + beta*v - w);
    return res;
}
