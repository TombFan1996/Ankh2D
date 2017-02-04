#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdint.h>

//matrices: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

typedef struct
{
	float x;
	float y;
} vec2;

vec2 vec2_create(float _x, float _y);

typedef struct
{
	float x;
	float y;
	float z;
} vec3;

vec3 vec3_create(float _x, float _y, float _z);

//OpenGL is column major when dealing w/ mat4's
//http://www.songho.ca/opengl/gl_matrix.html
typedef struct
{
	float element[4][4];
} mat4;

mat4 mat4_create(float _matElem1, float _matElem2, float _matElem3, float _matElem4,
					float _matElem5, float _matElem6, float _matElem7, float _matElem8,
						float _matElem9, float _matElem10, float _matElem11, float _matElem12,
							float _matElem13, float _matElem14, float _matElem15, float _matElem16);

mat4 mat4_getOrthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);

mat4 mat4_createIdentity();
mat4 mat4_getTranslate(vec2 _pos);
mat4 mat4_getScale(vec2 _scale);
mat4 mat4_getRotate(float _rotation);
mat4 mat4_getModelMatrix(mat4 _trans, mat4 _rot, mat4 _sca);

#endif