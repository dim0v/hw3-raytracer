#include "intersection.h"

Intersection::Intersection(glm::vec3 _normal, Ray _ray, float _t, Object* obj):
    normal(_normal), ray(_ray), t(_t), object(obj)
{
}
