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

void matransform (stack<glm::mat4> &transfstack, GLfloat * values) ;
void rightmultiply (const glm::mat4 & M, stack<glm::mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, GLfloat * values) ;
pair<Camera*, Scene*> readfile (const char * filename) ;
