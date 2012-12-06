#include "triangle.h"
#include "ray.h"
#include "intersection.h"

using namespace glm;

const double eps = 1e-6;

Triangle::Triangle(glm::dvec3 _A, glm::dvec3 _B, glm::dvec3 _C, glm::dvec3 _nA, glm::dvec3 _nB, glm::dvec3 _nC):
    A(_A), B(_B), C(_C)
{
    if(_nA == glm::dvec3() || _nB == glm::dvec3() || _nC == glm::dvec3())
        nA = nB = nC = -glm::normalize(glm::cross(C - A, B - A));
    else
        nA = _nA, nB = _nB, nC = _nC;
}

Intersection Triangle::intersect(Ray ray)
{
    dmat4 invTrans = inverse(transform);
    dvec4 newpos = dvec4(ray.getStart(), 1)*invTrans;
    dvec4 newdir = dvec4(ray.getDirection(), 0)*invTrans;
    Ray _ray(dvec3(newpos)/newpos.w, dvec3(newdir));
    dvec3 normal = normalize(cross(C - A, B - A));
    double t = (glm::dot(A, normal) - glm::dot(_ray.getStart(), normal))/glm::dot(_ray.getDirection(), normal);
    if(t < 0)return Intersection();
    glm::dvec3 P = _ray.trace(t);
    double Sabc = glm::length(glm::cross(C - A, B - A));
    double Spbc = glm::length(glm::cross(C - P, B - P)), Sapc = glm::length(glm::cross(A - P, C - P)), Sabp = glm::length(glm::cross(B - P, A - P));
    if(Spbc + Sapc + Sabp - Sabc > eps) return Intersection();
    double alpha = Spbc / Sabc;
    double beta  = Sapc / Sabc;
    double gamma = Sabp / Sabc;
    //assert(abs(alpha + beta + gamma - 1) <= eps);
    if (alpha   < 0 ||  alpha   > 1 ||
        beta    < 0 ||  beta    > 1 ||
        gamma   < 0 ||  gamma   > 1)
    {
        return Intersection();
    }
    normal = alpha * nA + beta * nB + gamma * nC;
    return Intersection(normalize(dvec3(invTrans*dvec4(normal, 0))), ray, t, this);
}
