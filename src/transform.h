#ifndef TRANSFORM_H
#define TRANSFORM_H

#if ANKH2D_WIN32
	#include "bmath.h"

	typedef struct
	{
		vec2 position;
		float rotation;
		vec2 scale;
	} transform;

	transform transform_create(vec2 _pos, float _rotation, vec2 _scale);
	mat4 transform_get_model_matrix(transform _trans);

#elif ANKH2D_PSX
	#include "bmath.h"

	typedef struct
	{
		int16_vec2 position;
	} transform;

	void transform_create(transform* _trans, int16_vec2 _pos);
	
#endif

#endif