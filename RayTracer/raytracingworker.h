#ifndef RAYTRACINGWORKER_H
#define RAYTRACINGWORKER_H

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

class RayTracingWorker : public QThread
{
    Q_OBJECT

    std::pair<int, int> tl, br;
    QImage res;
    Camera* cam;
    Scene* scene;
    int progress;
    int MSAASamples;
public:
    RayTracingWorker(int sx, int fx, int sy, int fy, Camera *_cam, Scene *_scene, int _MSAASamples);
    const QImage& getImg(){return res;}
    int getProgress(){return progress;}
protected:
    QImage &raytrace();
    glm::vec3 findColor(const Intersection& hit, int depth);
    void run();

signals:
    void pixelProcessed();
};

#endif // RAYTRACINGWORKER_H
