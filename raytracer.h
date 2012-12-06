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

QImage raytrace(Camera &cam, Scene &scene, int sx, int sy, int fx, int fy, QImage &res, int *progress);
glm::dvec3 findColor(Intersection hit, Scene scene, int depth);

class rayTracer : public QThread
{
    std::pair<int, int> tl, br;
    QImage res;
    Camera* cam;
    Scene* scene;
    int progress;
public:
    rayTracer(int sx, int fx, int sy, int fy, Camera *_cam, Scene *_scene);
    const QImage& getImg(){return res;}
    int getProgress(){return progress;}
protected:
    void run();
};

#endif // RAYTRACER_H
