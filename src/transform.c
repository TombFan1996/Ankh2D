#include "transform.h"

transform transform_create(vec2 _pos, float _rotation, vec2 _scale)
{
	transform new_transform;
	new_transform.model_matrix = (mat4*)malloc(sizeof(mat4));
	mat4_identity(new_transform.model_matrix);
	new_transform.position = _pos;
	new_transform.rotation = _rotation;
	new_transform.scale = _scale;
	return new_transform;
}

//get each individual matrix for the transform
//model = scale * trans * rotation
void transform_get_model_matrix(transform _trans)
{
	mat4_identity(_trans.model_matrix);
	mat4_scale(_trans.model_matrix, _trans.scale);
	mat4_rotate(_trans.model_matrix, _trans.rotation);
	mat4_translate(_trans.model_matrix, _trans.position);
	#if ANKH2D_SSE
		mat4_reverse(_trans.model_matrix);
	#endif
}