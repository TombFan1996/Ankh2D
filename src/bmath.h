#ifndef BASIC_MATH_H
#define BASIC_MATH_H

//Port these over eventually!

#if ANKH2D_WIN32
	#if ANKH2D_SSE
		#include <xmmintrin.h>
	#endif

	#include <malloc.h>
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
	mat4 mat4_create();
	void mat4_identity(mat4* _mat_iden);
	mat4 mat4_orthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
	void mat4_translate(mat4* _model, vec2 _pos);
	void mat4_scale(mat4* _model, vec2 _scale);
	void mat4_rotate(mat4* _model, float _degree);

#elif ANKH2D_PSX
	#include "types.h"

	typedef struct
	{
		uint8_t x;
		uint8_t y;
	} uint8_vec2;

	uint8_vec2 uint8_vec2_create(uint8_t _x, uint8_t _y);

	typedef struct
	{
		uint8_t x;
		uint8_t y;
		uint8_t z;
	} uint8_vec3;

	uint8_vec3 uint8_vec3_create(uint8_t _x, uint8_t _y, uint8_t _z);
#endif

#endif