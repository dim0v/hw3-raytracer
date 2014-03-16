#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <glm/glm.hpp>
#include <vector>
#include <utility>

#include "object.h"
#include "ray.h"

class VisibleObject;

class BoundingBox : public Object
{
    glm::vec3 from;
    glm::vec3 to;
    std::vector<Object *> objects;
public:
    BoundingBox(glm::vec3 _from, glm::vec3 _to):from(_from), to(_to) {}
    BoundingBox(float sx, float fx, float sy, float fy, float sz, float fz):from(sx, sy, sz), to(fx, fy, fz) {}
    BoundingBox(const std::vector<VisibleObject *> &objects, unsigned leaf_children_treshold = 8, unsigned depth_treshold = 16);

    virtual Intersection intersect(const Ray& ray);

    bool intersects(const BoundingBox &other) const;
    void buildOctree(unsigned leaf_children_treshold = 8, unsigned depth_treshold = 4);
};

#endif // BOUNDINGVOLUME_H
