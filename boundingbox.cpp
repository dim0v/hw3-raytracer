#include "boundingbox.h"
#include "ray.h"
#include "visibleobject.h"
#include <glm/glm.hpp>
#include <iostream>
#include <limits>

using namespace glm;

const float eps = 1e-6;

BoundingBox::BoundingBox(const std::vector<VisibleObject *> &objects, unsigned leaf_children_treshold /*= 8*/, unsigned depth_treshold /*= 16*/)
{
    if(objects.empty()) return;
    BoundingBox b = objects.front()->getBoundingBox();
    from = b.from;
    to = b.to;
    for(auto obj : objects)
    {
        b = obj->getBoundingBox();
        float inf = std::numeric_limits<float>::infinity();
        float nan = std::numeric_limits<float>::quiet_NaN();
        if(length(b.from) == inf || length(b.from) == nan ||        //object is invisible anyway. Can be safely passed
                length(b.to) == inf || length(b.to) == nan)
        {
            continue;
        }

        from.x = min(from.x, b.from.x);
        from.y = min(from.y, b.from.y);
        from.z = min(from.z, b.from.z);
        to.x = max(to.x, b.to.x);
        to.y = max(to.y, b.to.y);
        to.z = max(to.z, b.to.z);
        this->objects.push_back(obj);
    }
    buildOctree(leaf_children_treshold, depth_treshold);
}

bool BoundingBox::intersectedByRay(const Ray &ray)
{
    float t0 = 0, t1 = std::numeric_limits<float>::infinity();
    for (int i = 0; i < 3; ++i) {
        // Update interval for _i_th bounding box slab
        float invRayDir = 1.f / ray.getDirection()[i];
        float tNear = (from[i] - ray.getOrigin()[i]) * invRayDir;
        float tFar = (to[i] - ray.getOrigin()[i]) * invRayDir;
        // Update parametric interval from slab intersection $t$s
        if (tNear > tFar) std::swap(tNear, tFar);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1) return false;
    }
    return true;
}

Intersection BoundingBox::intersect(const Ray &ray, const Object *objToPass)
{
    //Intersect ray with the box itself
    if(!intersectedByRay(ray))
        return Intersection();

    //ray intersects the box. Check objects inside the box for intersection
    Intersection res;
    for(auto obj : objects)
    {
        Intersection tmp = obj->intersect(ray, objToPass);
        if(tmp.getObject() && tmp.getRayPos() < res.getRayPos()) res = tmp;
    }
    return res;
}

bool BoundingBox::intersects(const BoundingBox &other) const
{
    for(int i = 0; i < 3; ++i)
    {
        if(from[i] - other.to[i] > eps || other.from[i] - to[i] > eps)
            return false;
    }
    return true;
}

void BoundingBox::buildOctree(unsigned leaf_children_treshold /*= 8*/, unsigned depth_treshold /*= 4*/)
{
    if(objects.size() <= leaf_children_treshold || !depth_treshold)
        return;
    vec3 halfway = (from + to) / 2.0f;
    BoundingBox *children[8] = {new BoundingBox(vec3(from.x,    from.y,    from.z   ), vec3(halfway.x, halfway.y, halfway.z)),
                                new BoundingBox(vec3(halfway.x, from.y,    from.z   ), vec3(to.x,      halfway.y, halfway.z)),
                                new BoundingBox(vec3(from.x,    halfway.y, from.z   ), vec3(halfway.x, to.y,      halfway.z)),
                                new BoundingBox(vec3(from.x,    from.y,    halfway.z), vec3(halfway.x, halfway.y, to.z     )),
                                new BoundingBox(vec3(halfway.x, halfway.y, from.z   ), vec3(to.x,      to.y,      halfway.z)),
                                new BoundingBox(vec3(halfway.x, from.y,    halfway.z), vec3(to.x,      halfway.y, to.z     )),
                                new BoundingBox(vec3(from.x,    halfway.y, halfway.z), vec3(halfway.x, to.y,      to.z     )),
                                new BoundingBox(vec3(halfway.x, halfway.y, halfway.z), vec3(to.x,      to.y,      to.z     ))};
    std::vector<Object *> vec;
    for(auto obj : objects)
    {
        try
        {
            VisibleObject *vobj = dynamic_cast<VisibleObject *>(obj);
            BoundingBox box = vobj->getBoundingBox();
            bool f = false;
            for(int i = 0; i < 8; ++i)
            {
                if(children[i]->intersects(box))
                    children[i]->objects.push_back(vobj), f = true;
            }
            if(!f)
                vec.push_back(vobj);
        }
        catch(...)
        {
            std::cerr << "Shit happens. Tried to add non-VisibleObject to octree" << std::endl;
        }
    }
    objects.clear();
    objects.insert(objects.begin(), vec.begin(), vec.end());
    objects.insert(objects.end(), children, children + 8);
    for(int i = 0; i < 8; ++i)
        children[i]->buildOctree(leaf_children_treshold, depth_treshold - 1);
}
