#include <iostream>
#include <string>
#include <QImage>
#include <utility>
#include <cmath>

#include <chrono>
#include <thread>

#include "readfile.h"
#include "raytracer.h"
#include "camera.h"
#include "scene.h"
using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout << "Usage :" << endl << argv[0] << " <sceneFile.txt> [<number of threads>]" << endl;
        return 0;
    }
    pair<Camera*, Scene*> s = readfile(argv[1]);

    int nThreads = argc > 2 ? atoi(argv[2]) : QThread::idealThreadCount();

    QVector<RayTracer *> threads;
    threads.resize(nThreads);

    int thread_subRenderHeight = s.first->getHeight() / nThreads;

    for(int i = 0; i < nThreads; ++i)
        threads[i] = new RayTracer(0,                          s.first->getWidth(),
                                   i * thread_subRenderHeight, (i + 1) * thread_subRenderHeight,
                                   s.first, s.second);
    for(int i = 0; i < nThreads; ++i)
        threads[i]->start();

    bool t = false;
    while(!t)
    {
        int progress = 0;
        for(int i = 0; i < nThreads; ++i)
            progress += threads[i]->getProgress();
        cerr << float(progress) / s.first->getWidth() / s.first->getHeight() * 100 << "%" << endl;
        this_thread::sleep_for( chrono::seconds(3) );
        t = true;
        for(int i = 0; i < nThreads && t; ++i)
            t = threads[i]->isFinished();
    }
    for(int i = 0; i < nThreads; ++i)
        threads[i]->wait();


    QImage res(s.first->getWidth(), s.first->getHeight(), QImage::Format_RGB32);

    for(int i = 0; i < s.first->getWidth(); ++i)
    {
        for(int j = 0; j < s.first->getHeight(); ++j)
        {
            res.setPixel(i, j, threads[j / thread_subRenderHeight]->getImg().pixel(i, j % thread_subRenderHeight));
        }
    }
    res.save(s.second->getFilename().c_str());
}
