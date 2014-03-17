#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <utility>

class Object;
class VisibleObject;
class Light;
class Ray;
class Camera;
class Intersection;

class Scene
{
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    std::string output;
    int width, height;
    int depth;
    glm::vec3 attenuation;
public:
    Scene();
    const std::vector<Object*>& getObjectsList() const {return objects;}
    const std::vector<Light*>& getLightsList() const {return lights;}
    const std::string& getFilename() const {return output;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    int getDepth() const {return depth;}
    const glm::vec3 getAttenuation() const {return attenuation;}
    friend std::pair<Camera *, Scene *> readfile(const char* filename);

    void buildOctree(unsigned leaf_child_treshold = 8, unsigned depth_treshold = 13);
};

Intersection intersect(Ray& ray, const Scene *scene, const VisibleObject *obj);

#endif // SCENE_H
