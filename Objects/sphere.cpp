#include <cmath>
#include "sphere.h"
#include "ray.h"
#include "intersection.h"

using namespace glm;

Sphere::Sphere(vec3 _center, float _radius):
    center(_center), radius(_radius)
{
}

Intersection Sphere::intersect(const Ray &ray, const Object *objToPass)
{
    if(this == objToPass)
        return Intersection();

    mat4 invTrans = inverseTransform;
    vec4 newpos = vec4(ray.getOrigin(), 1)*invTrans;
    vec4 newdir = vec4(ray.getDirection(), 0)*invTrans;
    Ray _ray(vec3(newpos)/newpos.w, vec3(newdir));
    float a, b, c;
    a = dot(_ray.getDirection(), _ray.getDirection());
    b = 2 * dot(_ray.getDirection(), _ray.getOrigin() - center);
    c = dot(_ray.getOrigin() - center, _ray.getOrigin() - center) - radius * radius;
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

BoundingBox Sphere::getBoundingBox() const
{
    vec3 points[8] = {center + vec3(-radius, -radius, -radius),
                      center + vec3(-radius, -radius, +radius),
                      center + vec3(-radius, +radius, -radius),
                      center + vec3(-radius, +radius, +radius),
                      center + vec3(+radius, -radius, -radius),
                      center + vec3(+radius, -radius, +radius),
                      center + vec3(+radius, +radius, -radius),
                      center + vec3(+radius, +radius, +radius)};
    for(int i = 0; i < 8; ++i)
    {
        vec4 tmp = vec4(points[i], 1) * transform;
        points[i] = vec3(tmp) / tmp.w;
    }
    vec3 from(points[0]), to(points[0]);
    for(int i = 1; i < 8; ++i)
    {
        from.x = min(from.x, points[i].x);
        from.y = min(from.y, points[i].y);
        from.z = min(from.z, points[i].z);
        to.x = max(to.x, points[i].x);
        to.y = max(to.y, points[i].y);
        to.z = max(to.z, points[i].z);
    }
    return BoundingBox(from, to);
}
