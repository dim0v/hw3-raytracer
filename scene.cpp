#include <vector>
#include "scene.h"
#include "ray.h"
#include "intersection.h"
#include "object.h"
#include "boundingbox.h"
#include "visibleobject.h"
using namespace std;
using namespace glm;

Scene::Scene():
    depth(5), attenuation(vec3(1, 0, 0))
{
}

void Scene::buildOctree(unsigned leaf_child_treshold, unsigned depth_treshold)
{
    vector<VisibleObject *> vobjVec;
    vector<Object *> objVec;
    for(auto obj : objects)
    {
        try
        {
            VisibleObject *vobj = dynamic_cast<VisibleObject *>(obj);
            vobjVec.push_back(vobj);
        }
        catch (...)
        {
            objVec.push_back(obj);
        }
    }
    BoundingBox *tree = new BoundingBox(vobjVec, leaf_child_treshold, depth_treshold);
    objects.clear();
    objects.insert(objects.begin(), objVec.begin(), objVec.end());
    objects.push_back(tree);
}

Intersection intersect(Ray &ray, const Scene* scene, const VisibleObject *obj)
{
    Intersection res;
    const vector<Object*>& vObj = scene->getObjectsList();
    for(vector<Object*>::const_iterator i = vObj.begin(), j = vObj.end(); i!= j; ++i)
    {
        Intersection tmp = (*i)->intersect(ray, obj);
        if(tmp.getObject() && tmp.getRayPos() < res.getRayPos()) res = tmp;
    }
    return res;
}
