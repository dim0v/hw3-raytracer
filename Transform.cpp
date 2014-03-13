// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    // YOUR CODE FOR HW2 HERE
      float radians = degrees * pi / 180;
      vec3 _axis = glm::normalize(axis);
      mat3 res = mat3(cos(radians)) + (1 - cos(radians))*mat3(_axis[0]*_axis, _axis[1]*_axis, _axis[2]*_axis) + sin(radians)*mat3(0, -_axis[2], _axis[1], _axis[2], 0, -_axis[0], -_axis[1], _axis[0], 0);
    // Please implement this.  Likely the same as in HW 1.  
      return res;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
    // YOUR CODE FOR HW2 HERE
    eye = rotate(-degrees, up)*eye;
    up = glm::normalize(up);
    // Likely the same as in HW 1.  
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    // YOUR CODE FOR HW2 HERE 
    vec3 axis = glm::normalize(glm::cross(up, eye));
    eye = rotate(degrees, axis)*eye;
    up = rotate(degrees, axis)*up;
    up = glm::normalize(up);
    // Likely the same as in HW 1.  
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    // YOUR CODE FOR HW2 HERE
    vec3 w = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w, u);
    mat4 rotate(vec4(u, 0), vec4(v, 0), vec4(w, 0), vec4(0, 0, 0, 1));
    mat4 translate(1, 0, 0, -eye[0], 0, 1, 0, -eye[1], 0, 0, 1, -eye[2], 0, 0, 0, 1);
    // Likely the same as in HW 1.  
    return translate * rotate;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    float theta = fovy / 2 * pi / 180;
    float d = 1 / tan(theta);
    float A = (zFar + zNear) / (zNear - zFar), B = 2*zFar*zNear / (zNear - zFar);
    ret[0][0] /= aspect;
    ret[3][3] = 0;
    ret[3][2] = -1/d;
    ret /= -ret[3][2];
    ret[2][2] = A;
    ret[2][3] = B;
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    return glm::scale(mat4(), vec3(sx,sy,sz));
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    return mat4(mat3(sx, 0, 0, 0, sy, 0, 0, 0, sz));
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
    return glm::translate(mat4(), vec3(tx, ty, tz));
    // YOUR CODE FOR HW2 HERE
    // Implement translation
    mat4 res;
    res[3][0] = tx;
    res[3][1] = ty;
    res[3][2] = tz;
    return res;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
    vec3 x = glm::cross(up,zvec);
    vec3 y = glm::cross(zvec,x);
    vec3 ret = glm::normalize(y);
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
