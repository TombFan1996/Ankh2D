#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "bmath.h"

typedef struct
{
	mat4* model_matrix;
	vec2 position;
	float rotation;
	vec2 scale;
} transform;

transform transform_create(vec2 _pos, float _rotation, vec2 _scale);
void transform_get_model_matrix(transform _trans);

#endif