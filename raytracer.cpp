#include <vector>
#include <iostream>
#include <iomanip>
#include <ios>
#include <QThread>
#include "raytracer.h"
#include <glm/glm.hpp>
#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "intersection.h"
#include "light.h"
#include "object.h"
#include "triangle.h"
#include "sphere.h"

using namespace glm;
using namespace std;

const double eps = 1e-9;

rayTracer::rayTracer(int sx, int fx, int sy, int fy, Camera *_cam, Scene *_scene):
    tl(std::make_pair(sx, sy)), br(std::make_pair(fx, fy)), res(fx-sx, fy-sy, QImage::Format_RGB32),
    cam(_cam), scene(_scene), progress(0)
{}

void rayTracer::run()
{
    raytrace(*cam, *scene, tl.first, tl.second, br.first, br.second, res, &progress);
}

QImage raytrace(Camera &cam, Scene &scene, int sx, int sy, int fx, int fy, QImage& res, int* progress)
{
    for(int i = sx; i < fx; ++i)
    {
        for(int j = sy; j < fy; ++j)
        {
            Ray ray = rayThruPixel(cam, i, j);
            Intersection hit = intersect(ray, scene, NULL);
            if(!hit.getObject()) res.setPixel(i - sx, j - sy, 0);
            else
            {
                dvec3 color = findColor(hit, scene, scene.getDepth());
                res.setPixel(i - sx, j - sy, (int(color.r * 255)<<16) + (int(color.g * 255)<<8) + int(color.b * 255));
            }
            (*progress)++;
            //cerr << double(i*h + j) / (w*h) * 100 << "%" << endl;
        }
    }
    return res;
}

dvec3 findColor(Intersection hit, Scene scene, int depth)
{
    if(!depth) return dvec3();
    dvec3 color(0, 0, 0);
    dvec3 P = hit.getRay().trace(hit.getRayPos());
    vector<dvec3> attenuation(scene.getLightsList().size());
    dvec3 normal = /*scene.getLightsList()[i]->type == Light::point ? -hit.getNormal() : */hit.getNormal();
    for(int i = 0, ___i = scene.getLightsList().size(); i != ___i; ++i)
    {
        bool shaded = false;
        for(int j = 0; j < scene.getObjectsList().size() && !shaded; ++j)
        {
            dvec3 dir = scene.getLightsList()[i]->type == Light::point ? normalize(scene.getLightsList()[i]->pos - P) :
                                                                         normalize(scene.getLightsList()[i]->pos);
            Intersection t = scene.getObjectsList()[j]->intersect(Ray(P, dir));
            dvec3 rayP = t.getRay().trace(t.getRayPos());
            if(t.getObject() &&
                    length(rayP - P) > eps &&
                    (length(normalize(scene.getLightsList()[i]->pos - rayP) - normalize(t.getRay().getDirection())) < eps || scene.getLightsList()[i]->type == Light::directional))
                shaded = true;
        }
        if(shaded)continue;
        double d = length(scene.getLightsList()[i]->pos - P);
        if(scene.getLightsList()[i]->type == Light::point)
            attenuation[i] = (scene.getLightsList()[i]->color)/(dot(scene.getAttenuation(), dvec3(1, d, d*d)));
        else
            attenuation[i] = scene.getLightsList()[i]->color;

        dvec3 direction = scene.getLightsList()[i]->type == Light::point ?      //diffuse
                    normalize(scene.getLightsList()[i]->pos - P) :              //diffuse
                    normalize(scene.getLightsList()[i]->pos);                   //diffuse
        color += attenuation[i] * hit.getObject()->getMat().diffuse *           //diffuse
                std::max(0., dot(direction, normal));                           //diffuse

        dvec3 halfdvec = normalize(direction - hit.getRay().getDirection());    //specular
        color += attenuation[i]*hit.getObject()->getMat().specular *            //specular
                pow(std::max(0., dot(halfdvec, normal)),                        //specular
                    hit.getObject()->getMat().shininess);                       //specular
    }
    color += hit.getObject()->getMat().ambient;                                 //ambient

    color += hit.getObject()->getMat().emission;                                //emission

    Ray refl(P, normalize(hit.getRay().getDirection() -                         //reflected
                          2*dot(hit.getRay().getDirection(),                    //reflected
                                normal)*normal));                               //reflected
    Intersection reflHit = intersect(refl, scene, hit.getObject());             //reflected
    if(reflHit.getObject() && length(hit.getObject()->getMat().specular) > 0)   //reflected
        color += hit.getObject()->getMat().specular *                           //reflected
                findColor(reflHit, scene, depth - 1);                           //reflected

    if(color.r > 1)color.r = 1;
    if(color.g > 1)color.g = 1;
    if(color.b > 1)color.b = 1;
    if(color.r < 0)color.r = 0;
    if(color.g < 0)color.g = 0;
    if(color.b < 0)color.b = 0;
    return color;
}
