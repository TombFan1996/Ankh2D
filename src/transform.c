#include "transform.h"

#if ANKH2D_WIN32
	transform transform_create(vec2 _pos, float _rotation, vec2 _scale)
	{
		transform new_transform;
		new_transform.position = _pos;
		new_transform.rotation = _rotation;
		new_transform.scale = _scale;

		//new_transform.model_matrix = (mat4*)malloc(sizeof(mat4));
		//mat4_identity(&new_transform.model_matrix);

		return new_transform;
	}

	//get each individual matrix for the transform
	//model = scale * trans * rotation
	mat4 transform_get_model_matrix(transform _trans)
	{
		mat4 model_matrix;
		mat4_identity(&model_matrix);
		mat4_scale(&model_matrix, _trans.scale);
		mat4_rotate(&model_matrix, _trans.rotation);
		mat4_translate(&model_matrix, _trans.position);

		#if ANKH2D_SSE
			mat4_reverse(&model_matrix);
		#endif

		return model_matrix;
	}

#elif ANKH2D_PSX

#endif