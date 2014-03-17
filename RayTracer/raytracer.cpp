#include <utility>
#include <QDebug>
#include "raytracer.h"
#include "readfile.h"

using namespace std;

RayTracer::RayTracer(const char *filename, int numOfThreads, int octreeLeafChildren, int octreeDepth,  QObject *parent) :
    QObject(parent),
    progress(0),
    nThreads(numOfThreads)
{
    qDebug() << "scene filename: " << filename;
    qDebug() << "threads: " << numOfThreads;
    qDebug() << "children: " << octreeLeafChildren;
    qDebug() << "depth: " << octreeDepth;

    pair<Camera*, Scene*> s = readfile(filename);
    camera = s.first;
    scene = s.second;
    scene->buildOctree(octreeLeafChildren, octreeDepth);
    if (nThreads <= 0)
        nThreads = QThread::idealThreadCount();

    threads.resize(nThreads);
    int thread_subRenderHeight = camera->getHeight() / nThreads;
    int k = camera->getHeight() % nThreads;

    for(int i = 0; i < nThreads; ++i)
        threads[i] = new RayTracingWorker(0,
                                          camera->getWidth(),
                                          i * thread_subRenderHeight + (i < k),
                                          min((i + 1) * thread_subRenderHeight + (i < k), camera->getHeight()),
                                          camera,
                                          scene);

    for(auto thread : threads)
    {
        connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
        connect(thread, SIGNAL(pixelProcessed()), this, SLOT(updateProgress()));
    }
}

RayTracer::~RayTracer()
{
    delete camera;
    delete scene;
    for(auto thread : threads)
    {
        delete thread;
    }
}


void RayTracer::start()
{
    emit started();
    for(int i = 0; i < nThreads; ++i)
        threads[i]->start();
    nThreadsFinished = 0;
}

void RayTracer::threadFinished()
{
    mutex.lock();
    if(++nThreadsFinished == nThreads)
    {
        saveImage();
        emit finished();
    }
    mutex.unlock();
}

void RayTracer::updateProgress()
{
    mutex.lock();
    ++progress;
    emit progressUpdated(float(progress) / camera->getWidth() / camera->getHeight());
    mutex.unlock();
}

QImage RayTracer::getImage()
{
    QImage res(camera->getWidth(), camera->getHeight(), QImage::Format_RGB32);
    int y = 0;

    for(auto thread : threads)
    {
        for(int j = 0; j < thread->getImg().height(); ++j, ++y)
        {
            for(int i = 0; i < thread->getImg().width(); ++i)
            {
                res.setPixel(i, y, thread->getImg().pixel(i, j));
            }
        }
    }
    return res;
}

void RayTracer::saveImage()
{
    getImage().save(scene->getFilename().c_str());
}
