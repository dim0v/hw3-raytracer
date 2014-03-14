#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <string>
#include <QThread>
#include <QImage>
#include <cmath>
#include <glm/glm.hpp>
#include <utility>

class Intersection;
class Camera;
class Scene;

class RayTracer : public QThread
{
    std::pair<int, int> tl, br;
    QImage res;
    Camera* cam;
    Scene* scene;
    int progress;
public:
    RayTracer(int sx, int fx, int sy, int fy, Camera *_cam, Scene *_scene);
    const QImage& getImg(){return res;}
    int getProgress(){return progress;}
protected:
    QImage &raytrace();
    glm::vec3 findColor(const Intersection& hit, int depth);
    void run();
};

#endif // RAYTRACER_H
