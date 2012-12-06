// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
dmat3 Transform::rotate(const double degrees, const dvec3& axis)
{
    // YOUR CODE FOR HW2 HERE
      double radians = degrees * pi / 180;
      dvec3 _axis = glm::normalize(axis);
      dmat3 res = dmat3(cos(radians)) + (1 - cos(radians))*dmat3(_axis[0]*_axis, _axis[1]*_axis, _axis[2]*_axis) + sin(radians)*dmat3(0, -_axis[2], _axis[1], _axis[2], 0, -_axis[0], -_axis[1], _axis[0], 0);
    // Please implement this.  Likely the same as in HW 1.  
      return res;
}

void Transform::left(double degrees, dvec3& eye, dvec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    eye = rotate(-degrees, up)*eye;
    up = glm::normalize(up);
    // Likely the same as in HW 1.  
}

void Transform::up(double degrees, dvec3& eye, dvec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    dvec3 axis = glm::normalize(glm::cross(up, eye));
    eye = rotate(degrees, axis)*eye;
    up = rotate(degrees, axis)*up;
    up = glm::normalize(up);
    // Likely the same as in HW 1.  
}

dmat4 Transform::lookAt(const dvec3 &eye, const dvec3 &center, const dvec3 &up)
{
    // YOUR CODE FOR HW2 HERE
    dvec3 w = glm::normalize(eye);
    dvec3 u = glm::normalize(glm::cross(up, w));
    dvec3 v = glm::cross(w, u);
    dmat4 rotate(dvec4(u, 0), dvec4(v, 0), dvec4(w, 0), dvec4(0, 0, 0, 1));
    dmat4 translate(1, 0, 0, -eye[0], 0, 1, 0, -eye[1], 0, 0, 1, -eye[2], 0, 0, 0, 1);
    // Likely the same as in HW 1.  
    return translate * rotate;
}

dmat4 Transform::perspective(double fovy, double aspect, double zNear, double zFar)
{
    dmat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    double theta = fovy / 2 * pi / 180;
    double d = 1 / tan(theta);
    double A = (zFar + zNear) / (zNear - zFar), B = 2*zFar*zNear / (zNear - zFar);
    ret[0][0] /= aspect;
    ret[3][3] = 0;
    ret[3][2] = -1/d;
    ret /= -ret[3][2];
    ret[2][2] = A;
    ret[2][3] = B;
    return ret;
}

dmat4 Transform::scale(const double &sx, const double &sy, const double &sz)
{
    return glm::scale(dmat4(), dvec3(sx,sy,sz));
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    return dmat4(dmat3(sx, 0, 0, 0, sy, 0, 0, 0, sz));
}

dmat4 Transform::translate(const double &tx, const double &ty, const double &tz)
{
    return glm::translate(dmat4(), dvec3(tx, ty, tz));
    // YOUR CODE FOR HW2 HERE
    // Implement translation
    dmat4 res;
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

dvec3 Transform::updvector(const dvec3 &up, const dvec3 & zdvec) 
{
    dvec3 x = glm::cross(up,zdvec); 
    dvec3 y = glm::cross(zdvec,x); 
    dvec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
