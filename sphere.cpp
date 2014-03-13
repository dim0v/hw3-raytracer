#include <cmath>
#include "sphere.h"
#include "ray.h"
#include "intersection.h"

using namespace glm;

Sphere::Sphere(vec3 _center, float _radius):
    center(_center), radius(_radius)
{
}

Intersection Sphere::intersect(const Ray &ray)
{
    mat4 invTrans = inverseTransform;
    vec4 newpos = vec4(ray.getStart(), 1)*invTrans;
    vec4 newdir = vec4(ray.getDirection(), 0)*invTrans;
    Ray _ray(vec3(newpos)/newpos.w, vec3(newdir));
    float a, b, c;
    a = dot(_ray.getDirection(), _ray.getDirection());
    b = 2 * dot(_ray.getDirection(), _ray.getStart() - center);
    c = dot(_ray.getStart() - center, _ray.getStart() - center) - radius * radius;
    float D = b*b - 4*a*c, t;
    if(D < 0) return Intersection();
    t = (- b - std::sqrt(D))/(2*a);
    if(t > 0)
    {
        vec3 normal = _ray.trace(t) - center;
        return Intersection(normalize(vec3(invTrans*vec4(normal, 0))), ray, t, this);
    }
    t = (- b + std::sqrt(D))/(2*a);
    if(t > 0)
    {
        vec3 normal = _ray.trace(t) - center;
        return Intersection(normalize(vec3(invTrans*vec4(normal, 0))), ray, t, this);
    }
    return Intersection();
}
