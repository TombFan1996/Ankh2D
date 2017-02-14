#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#if ANKH2D_SSE
	#include <xmmintrin.h>
#else
	#include <stdlib.h> //needed for malloc
#endif

#include <math.h>

#define PI 3.14159265359f

float deg_to_rad(float _deg);
float rad_to_deg(float _rad);

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

//[COLUMN] [ROW]

#if ANKH2D_SSE
	typedef struct
	{
		__m128 element[4];
	} mat4;
#else
	typedef struct
	{
		float element[4][4];
	} mat4;
#endif

void mat4_reverse(mat4* _old_mat4);
mat4* mat4_create();
void mat4_identity(mat4* _mat_iden);
void mat4_orthographic(mat4* _ortho, float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
void mat4_translate(mat4* _model, vec2 _pos);
void mat4_scale(mat4* _model, vec2 _scale);
void mat4_rotate(mat4* _model, float _degree);

#endif