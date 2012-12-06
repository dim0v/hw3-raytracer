#include "intersection.h"

Intersection::Intersection(glm::dvec3 _normal, Ray _ray, double _t, Object* obj):
    normal(_normal), ray(_ray), t(_t), object(obj)
{
}
