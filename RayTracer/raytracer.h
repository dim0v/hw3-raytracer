#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include <QMutex>
#include "camera.h"
#include "scene.h"
#include "raytracingworker.h"

class RayTracer : public QObject
{
    Q_OBJECT

    Camera *camera;
    Scene *scene;
    QVector<RayTracingWorker *> threads;
    int progress;
    int nThreadsFinished;
    int nThreads;
    int MSAASamples;

    QMutex mutex;
public:
    explicit RayTracer(const char *filename, int _MSAASamples, int numOfThreads, int octreeLeafChildren, int octreeDepth,  QObject *parent = 0);
    ~RayTracer();

protected:
    QImage getImage();
    void saveImage();

signals:
    void started();
    void finished();
    void progressUpdated(float percent);

public slots:
    void start();
    void threadFinished();
    void updateProgress();
};

#endif // RAYTRACER_H
