#include "transform.h"

transform transform_create(vec2 _pos, float _rotation, vec2 _scale)
{
	transform new_transform;
	new_transform.position = _pos;
	new_transform.rotation = _rotation;
	new_transform.scale = _scale;
	return new_transform;
}

//get each individual matrix for the transform
//model = scale * trans * rotation
mat4 transform_get_model_matrix(transform _trans)
{
	mat4 model = mat4_identity();
	mat4_scale(&model, _trans.scale);
	mat4_rotate(&model, _trans.rotation);
	mat4_translate(&model, _trans.position);
	#if USE_SSE
		mat4_reverse(&model);
	#endif
	return model;
}