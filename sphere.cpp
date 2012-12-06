#include <cmath>
#include "sphere.h"
#include "ray.h"
#include "intersection.h"

using namespace glm;

Sphere::Sphere(dvec3 _center, double _radius):
    center(_center), radius(_radius)
{
}

Intersection Sphere::intersect(Ray ray)
{
    dmat4 invTrans = inverse(transform);
    dvec4 newpos = dvec4(ray.getStart(), 1)*invTrans;
    dvec4 newdir = dvec4(ray.getDirection(), 0)*invTrans;
    Ray _ray(dvec3(newpos)/newpos.w, dvec3(newdir));
    double a, b, c;
    a = dot(_ray.getDirection(), _ray.getDirection());
    b = 2 * dot(_ray.getDirection(), _ray.getStart() - center);
    c = dot(_ray.getStart() - center, _ray.getStart() - center) - radius * radius;
    double D = b*b - 4*a*c, t;
    if(D < 0) return Intersection();
    t = (- b - std::sqrt(D))/(2*a);
    if(t > 0)
    {
        dvec3 normal = _ray.trace(t) - center;
        return Intersection(normalize(dvec3(invTrans*dvec4(normal, 0))), ray, t, this);
    }
    t = (- b + std::sqrt(D))/(2*a);
    if(t > 0)
    {
        dvec3 normal = _ray.trace(t) - center;
        return Intersection(normalize(dvec3(invTrans*dvec4(normal, 0))), ray, t, this);
    }
    return Intersection();
}
