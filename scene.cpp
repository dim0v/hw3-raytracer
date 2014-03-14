#include <vector>
#include "scene.h"
#include "ray.h"
#include "intersection.h"
#include "object.h"
using namespace std;
using namespace glm;

Scene::Scene():
    depth(5), attenuation(vec3(1, 0, 0))
{
}

Intersection intersect(Ray &ray, const Scene* scene, const Object* obj)
{
    Intersection res;
    const vector<Object*>& vObj = scene->getObjectsList();
    for(vector<Object*>::const_iterator i = vObj.begin(), j = vObj.end(); i!= j; ++i)
    {
        Intersection tmp = (*i)->intersect(ray);
        if(tmp.getObject() && tmp.getObject() != obj && tmp.getRayPos() < res.getRayPos()) res = tmp;
    }
    return res;
}
