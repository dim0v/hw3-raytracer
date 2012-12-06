#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <utility>
#include <GL/glut.h>
#include <glm/glm.hpp>

using namespace std;
// Readfile definitions

class Camera;
class Scene;

void matransform (stack<glm::dmat4> &transfstack, GLdouble * values) ;
void rightmultiply (const glm::dmat4 & M, stack<glm::dmat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, GLdouble * values) ;
pair<Camera*, Scene*> readfile (const char * filename) ;
