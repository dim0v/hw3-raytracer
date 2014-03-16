#include "triangle.h"
#include "ray.h"
#include "intersection.h"

using namespace glm;

const float eps = 1e-6;

Triangle::Triangle(glm::vec3 _A, glm::vec3 _B, glm::vec3 _C, glm::vec3 _nA, glm::vec3 _nB, glm::vec3 _nC):
    A(_A), B(_B), C(_C)
{
    if(_nA == glm::vec3() || _nB == glm::vec3() || _nC == glm::vec3())
        nA = nB = nC = -glm::normalize(glm::cross(C - A, B - A));
    else
        nA = _nA, nB = _nB, nC = _nC;
}

Intersection Triangle::intersect(const Ray &ray)
{
    mat4 invTrans = inverseTransform;
    vec4 newpos = vec4(ray.getStart(), 1)*invTrans;
    vec4 newdir = vec4(ray.getDirection(), 0)*invTrans;
    Ray _ray(vec3(newpos)/newpos.w, vec3(newdir));
    vec3 normal = normalize(cross(C - A, B - A));
    float t = (glm::dot(A, normal) - glm::dot(_ray.getStart(), normal))/glm::dot(_ray.getDirection(), normal);
    if(t < 0 || fabs(glm::dot(_ray.getDirection(), normal)) < eps)return Intersection();
    glm::vec3 P = _ray.trace(t);
    float Sabc = glm::length(glm::cross(C - A, B - A));
    float Spbc = glm::length(glm::cross(C - P, B - P)), Sapc = glm::length(glm::cross(A - P, C - P)), Sabp = glm::length(glm::cross(B - P, A - P));
    if(Spbc + Sapc + Sabp - Sabc > eps) return Intersection();
    float alpha = Spbc / Sabc;
    float beta  = Sapc / Sabc;
    float gamma = Sabp / Sabc;
    //assert(abs(alpha + beta + gamma - 1) <= eps);
    if (alpha   < 0 ||  alpha   > 1 ||
        beta    < 0 ||  beta    > 1 ||
        gamma   < 0 ||  gamma   > 1)
    {
        return Intersection();
    }
    normal = alpha * nA + beta * nB + gamma * nC;
    if(length(normal) == 0) return Intersection();
    return Intersection(normalize(vec3(invTrans*vec4(normal, 0))), ray, t, this);
}

BoundingBox Triangle::getBoundingBox() const
{
    vec3 points[3] = {A, B, C};
    for(int i = 0; i < 3; ++i)
    {
        vec4 tmp = vec4(points[i], 1) * transform;
        points[i] = vec3(tmp) / tmp.w;
    }
    vec3 from(points[0]), to(points[0]);
    for(int i = 1; i < 2; ++i)
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
