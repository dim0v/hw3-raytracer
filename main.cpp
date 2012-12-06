#include <iostream>
#include <string>
#include <QImage>
#include <utility>
#include <cmath>
#include "readfile.h"
#include "raytracer.h"
#include "camera.h"
#include "scene.h"
using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        cout << "Usage :" << endl << argv[0] << " sceneFile.txt" << endl;
        return 0;
    }
    pair<Camera*, Scene*> s = readfile(argv[1]);
    QImage res(s.first->getWidth(), s.first->getHeight(), QImage::Format_RGB32);
    rayTracer tl(0, s.first->getWidth()/2, 0, s.first->getHeight()/2, s.first, s.second);
    rayTracer tr(s.first->getWidth()/2, s.first->getWidth(), 0, s.first->getHeight()/2, s.first, s.second);
    rayTracer bl(0, s.first->getWidth()/2, s.first->getHeight()/2, s.first->getHeight(), s.first, s.second);
    rayTracer br(s.first->getWidth()/2, s.first->getWidth(), s.first->getHeight()/2, s.first->getHeight(), s.first, s.second);
    tl.start();
    tr.start();
    bl.start();
    br.start();
    bool t = false;
    while(!t)
    {
        cerr << 100 * double(tl.getProgress() + tr.getProgress() + bl.getProgress() + br.getProgress()) / (s.first->getWidth() * s.first->getHeight());
        cerr << "%" << endl;
        sleep(3);
        t = tl.isFinished() && tr.isFinished() && bl.isFinished() && br.isFinished();
    }
    tl.wait();
    tr.wait();
    bl.wait();
    br.wait();
    for(int i = 0; i < s.first->getWidth(); ++i)
    {
        for(int j = 0; j < s.first->getHeight(); ++j)
        {
            if(i < s.first->getWidth()/2)
            {
                if(j < s.first->getHeight()/2)
                    res.setPixel(i, j, tl.getImg().pixel(i, j));
                else
                    res.setPixel(i, j, bl.getImg().pixel(i, j - s.first->getHeight()/2));
            }
            else
            {
                if(j < s.first->getHeight()/2)
                    res.setPixel(i, j, tr.getImg().pixel(i - s.first->getWidth()/2, j));
                else
                    res.setPixel(i, j, br.getImg().pixel(i - s.first->getWidth()/2, j - s.first->getHeight()/2));
            }
        }
    }
    res.save(s.second->getFilename().c_str());
}
