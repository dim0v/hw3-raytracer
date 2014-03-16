#include "boundingbox.h"
#include "ray.h"
#include "visibleobject.h"
#include <glm/glm.hpp>
#include <iostream>
#include <limits>

using namespace glm;

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

Intersection BoundingBox::intersect(const Ray &ray)
{
    //Intersect ray with the box itself
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (ray.getDirection().x >= 0) {
        tmin = (from.x - ray.getStart().x) / ray.getDirection().x;
        tmax = (to.x - ray.getStart().x) / ray.getDirection().x;
    }
    else {
        tmin = (to.x - ray.getStart().x) / ray.getDirection().x;
        tmax = (from.x - ray.getStart().x) / ray.getDirection().x;
    }
    if (ray.getDirection().y >= 0) {
        tymin = (from.y - ray.getStart().y) / ray.getDirection().y;
        tymax = (to.y - ray.getStart().y) / ray.getDirection().y;
    }
    else {
        tymin = (to.y - ray.getStart().y) / ray.getDirection().y;
        tymax = (from.y - ray.getStart().y) / ray.getDirection().y;
    }
    if ( (tmin > tymax) || (tymin > tmax) )
        return Intersection();

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;
    if (ray.getDirection().z >= 0) {
        tzmin = (from.z - ray.getStart().z) / ray.getDirection().z;
        tzmax = (to.z - ray.getStart().z) / ray.getDirection().z;
    }
    else {
        tzmin = (to.z - ray.getStart().z) / ray.getDirection().z;
        tzmax = (from.z - ray.getStart().z) / ray.getDirection().z;
    }
    if ( (tmin > tzmax) || (tzmin > tmax) )
        return Intersection();

    //ray intersects the box. Check objects inside the box for intersection
    Intersection res;
    for(auto obj : objects)
    {
        Intersection tmp = obj->intersect(ray);
        if(tmp.getObject() && tmp.getRayPos() < res.getRayPos()) res = tmp;
    }
    return res;
}

bool BoundingBox::intersects(const BoundingBox &other) const
{
    float px, py, pz;
    px = max(to.x, other.to.x) - min(from.x, other.from.x) - ((to.x - from.x) + (other.to.x - other.from.x));
    px = -px;
    py = max(to.y, other.to.y) - min(from.y, other.from.y) - ((to.y - from.y) + (other.to.y - other.from.y));
    py = -py;
    pz = max(to.z, other.to.z) - min(from.z, other.from.z) - ((to.z - from.z) + (other.to.z - other.from.z));
    pz = -pz;

    return px > 0 && py > 0 && pz > 0;
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
    for(auto obj : objects)
    {
        for(int i = 0; i < 8; ++i)
        {
            try
            {
                VisibleObject *vobj = dynamic_cast<VisibleObject *>(obj);
                if(children[i]->intersects(vobj->getBoundingBox()))
                    children[i]->objects.push_back(vobj);
            }
            catch(...)
            {
                std::cerr << "Shit happens. Tried to add non-VisibleObject to octree" << std::endl;
            }
        }
    }
    objects.clear();
    objects.insert(objects.begin(), children, children + 8);
    for(int i = 0; i < 8; ++i)
        children[i]->buildOctree(leaf_children_treshold, depth_treshold - 1);
}
