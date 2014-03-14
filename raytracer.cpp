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

const float eps = 1e-6;

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
            Ray ray = Ray::rayThruPixel(cam, i, j);
            Intersection hit = intersect(ray, scene, NULL);
            if(!hit.getObject()) res.setPixel(i - sx, j - sy, 0);
            else
            {
                vec3 color = findColor(hit, scene, scene.getDepth());
                res.setPixel(i - sx, j - sy, (int(color.r * 255)<<16) + (int(color.g * 255)<<8) + int(color.b * 255));
            }
            (*progress)++;
            //cerr << float(i*h + j) / (w*h) * 100 << "%" << endl;
        }
    }
    return res;
}

vec3 findColor(Intersection hit, Scene scene, int depth)
{
    if(!depth) return vec3();
    vec3 color(0, 0, 0);
    vec3 P = hit.getRay().trace(hit.getRayPos());
    vector<vec3> attenuation(scene.getLightsList().size());
    vec3 normal = hit.getNormal();
    for(int i = 0, ___i = scene.getLightsList().size(); i != ___i; ++i)
    {
        vec3 rpos, rdirn;
        if(scene.getLightsList()[i]->type == Light::point)
            rpos = scene.getLightsList()[i]->pos, rdirn = normalize(P - scene.getLightsList()[i]->pos);
        else if(scene.getLightsList()[i]->type == Light::directional)
            rpos = P, rdirn = normalize(scene.getLightsList()[i]->pos);

        Ray lRay(rpos, rdirn);
        Intersection lightHit = intersect(lRay, scene, NULL);

        bool shadowed = false;

        if(scene.getLightsList()[i]->type == Light::point)
            shadowed = lightHit.getObject() != hit.getObject();
        else if(scene.getLightsList()[i]->type == Light::directional)
            shadowed = bool(lightHit.getObject());

        if(shadowed) continue;

        float d = length(scene.getLightsList()[i]->pos - P);
        if(scene.getLightsList()[i]->type == Light::point)
            attenuation[i] = (scene.getLightsList()[i]->color)/(dot(scene.getAttenuation(), vec3(1, d, d*d)));
        else
            attenuation[i] = scene.getLightsList()[i]->color;

        vec3 direction = scene.getLightsList()[i]->type == Light::point ?      //diffuse
                   normalize(scene.getLightsList()[i]->pos - P) :              //diffuse
                   normalize(scene.getLightsList()[i]->pos);                   //diffuse
        color += attenuation[i] * hit.getObject()->getMat().diffuse *           //diffuse
                std::max(0.f, dot(direction, normal));                           //diffuse

        vec3 halfvec = normalize(direction - hit.getRay().getDirection());    //specular
        color += attenuation[i]*hit.getObject()->getMat().specular *            //specular
                pow(std::max(0.f, dot(halfvec, normal)),                        //specular
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
