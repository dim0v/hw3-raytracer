#ifndef TRANSFORM_H
#define TRANSFORM_H
// Transform header file to define the interface. 
// The class is all static for simplicity
// You need to implement left, up and lookAt
// Rotate is a helper function

// Include the helper glm library, including matrix transform extensions

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// glm provides dvector, dmatrix classes like glsl
// Typedefs to make code more readable 

typedef glm::dmat3 dmat3 ;
typedef glm::dmat4 dmat4 ; 
typedef glm::dvec3 dvec3 ; 
typedef glm::dvec4 dvec4 ; 
const double pi = 3.14159265 ; // For portability across platforms


class Transform  
{
public:
	Transform();
	virtual ~Transform();
	static void left(double degrees, dvec3& eye, dvec3& up);
	static void up(double degrees, dvec3& eye, dvec3& up);
	static dmat4 lookAt(const dvec3& eye, const dvec3 &center, const dvec3& up);
	static dmat4 perspective(double fovy, double aspect, double zNear, double zFar);
        static dmat3 rotate(const double degrees, const dvec3& axis) ;
        static dmat4 scale(const double &sx, const double &sy, const double &sz) ; 
        static dmat4 translate(const double &tx, const double &ty, const double &tz);
        static dvec3 updvector(const dvec3 &up, const dvec3 &zdvec) ; 
};

#endif
