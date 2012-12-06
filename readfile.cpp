/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  
  

// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 
  
/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "ray.h"
#include "Transform.h" 
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "scene.h"
#include "light.h"

using namespace std;
#include "readfile.h"

dvec3 eye; // The (regularly updated) dvector coordinates of the eye
dvec3 up;  // The (regularly updated) dvector coordinates of the up

dvec3 eyeinit ;
dvec3 upinit ;
dvec3 center ;
int w, h ;
double fovy ;


bool useGlu; // Toggle use of "official" opengl/glm transform vs user
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders
static enum {view, translate, scale} transop ; // which operation to transform
enum shape {cube, sphere, teapot} ;
double sx, sy ; // the scale in x and y
double tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ;
GLdouble lightposn [4*numLights] ; // Light Positions
GLdouble lightcolor[4*numLights] ; // Light Colors
GLdouble lightransf[4*numLights] ; // Lights transformed by modelview
int numused ;                     // How many lights are used

// Materials (read from file)
// With multiple objects, these are colors for each.
GLdouble ambient[4] ;
GLdouble diffuse[4] ;
GLdouble specular[4] ;
GLdouble emission[4] ;
GLdouble shininess ;

// For multiple objects, read from a file.
const int maxobjects = 10 ;
int numobjects ;
struct object {
  shape type ;
  GLdouble size ;
  GLdouble ambient[4] ;
  GLdouble diffuse[4] ;
  GLdouble specular[4] ;
  GLdouble emission[4] ;
  GLdouble shininess ;
  dmat4 transform ;
} objects[maxobjects] ;


// The function below applies the appropriate transform to a 4-dvector
void matransform(stack<dmat4> &transfstack, GLdouble* values)
{
    dmat4 transform = transfstack.top();
    dvec4 valdvec = dvec4(values[0],values[1],values[2],values[3]);
    dvec4 newval = valdvec * transform;
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const dmat4 & M, stack<dmat4> &transfstack)
{
    dmat4 &T = transfstack.top();
    // Right multiply M, but do this left to account for row/column major 
    T = M * T; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, GLdouble* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

pair<Camera *, Scene *> readfile(const char* filename)
{
    Camera* cam = new Camera;
    Scene* scn = new Scene;
    Object::Material cur_mat;
    string str, cmd; 
    ifstream in;
    vector<dvec3> vert;
    vector<pair<dvec3, dvec3> > vertnorm;
    in.open(filename);
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        stack <dmat4> transfstack;
        transfstack.push(dmat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines 

                stringstream s(str);
                s >> cmd;
                GLdouble values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 

                if(cmd == "size") {
                    validinput = readvals(s, 2, values);
                    if(!validinput){cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cam->width = scn->width = values[0];
                    cam->height = scn->height = values[1];
                }
                else if(cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->depth = values[0];
                }
                else if(cmd == "output") {
                    s >> scn->output;
                }

                else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cam->eye    = dvec3(values[0], values[1], values[2]);
                    cam->center = dvec3(values[3], values[4], values[5]);
                    cam->up     = dvec3(values[6], values[7], values[8]);
                    cam->fovy   = values[9];
                    cam->fovx   = 2*atan(cam->width/2./(cam->height/2/tan(cam->fovy*pi/180/2)))*180/pi;
                }

                else if (cmd == "sphere") {
                    validinput = readvals(s,4,values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->objects.push_back(new Sphere(dvec3(values[0], values[1], values[2]), values[3]));
                    scn->objects.back()->setMat(cur_mat);
                    scn->objects.back()->setTransform(transfstack.top());
                }

                else if (cmd == "maxverts" || cmd == "maxvertnorms");

                else if (cmd == "vertex") {
                    validinput = readvals(s,3,values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    vert.push_back(dvec3(values[0], values[1], values[2]));
                }

                else if (cmd == "vertexnormal") {
                    validinput = readvals(s,6,values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    vertnorm.push_back(make_pair(dvec3(values[0], values[1], values[2]), dvec3(values[3], values[4], values[5])));
                }

                else if (cmd == "tri") {
                    validinput = readvals(s,3,values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->objects.push_back(new Triangle(vert[values[0]], vert[values[1]], vert[values[2]]));
                    scn->objects.back()->setMat(cur_mat);
                    scn->objects.back()->setTransform(transfstack.top());
                }

                else if (cmd == "trinormal") {
                    validinput = readvals(s,3,values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->objects.push_back(new Triangle(vertnorm[values[0]].first, vertnorm[values[1]].first,
                                                        vertnorm[values[2]].first, vertnorm[values[0]].second,
                                                        vertnorm[values[1]].second, vertnorm[values[2]].second));
                    scn->objects.back()->setMat(cur_mat);
                    scn->objects.back()->setTransform(transfstack.top());
                }

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        rightmultiply(glm::transpose(Transform::translate(values[0], values[1], values[2])), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {
                        rightmultiply((dmat4(Transform::rotate(values[3], dvec3(values[0], values[1], values[2])))), transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transfstack.pop();
                    }
                }

                else if(cmd == "directional") {
                    validinput = readvals(s, 6, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->lights.push_back(new Light);
                    scn->lights.back()->pos     = dvec3(values[0], values[1], values[2]);
                    scn->lights.back()->color   = dvec3(values[3], values[4], values[5]);
                    scn->lights.back()->type    = Light::directional;
                }

                else if(cmd == "point") {
                    validinput = readvals(s, 6, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->lights.push_back(new Light);
                    scn->lights.back()->pos     = dvec3(values[0], values[1], values[2]);
                    scn->lights.back()->color   = dvec3(values[3], values[4], values[5]);
                    scn->lights.back()->type    = Light::point;
                }

                else if(cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    scn->attenuation = dvec3(values[0], values[1], values[2]);
                }

                else if(cmd == "ambient") {
                    validinput = readvals(s, 3, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cur_mat.ambient = dvec3(values[0], values[1], values[2]);
                }

                else if(cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cur_mat.diffuse = dvec3(values[0], values[1], values[2]);
                }

                else if(cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cur_mat.specular = dvec3(values[0], values[1], values[2]);
                }

                else if(cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cur_mat.emission = dvec3(values[0], values[1], values[2]);
                }

                else if(cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if(!validinput) {cerr << "Something has gone wrong. Skipping command \"" << cmd << "\"" << endl; continue;}
                    cur_mat.shininess = values[0];
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str);
        }

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
    return make_pair(cam, scn);
}
