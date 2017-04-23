#if ANKH2D_WIN32

	#include "transform.h"
	
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
		mat4 mat4_comp1, mat4_comp2;
		mat4 translate, pivot, scale, rot;

		mat4_empty(&mat4_comp1);
		mat4_empty(&mat4_comp2);

		//set all to identity
		mat4_identity(&translate);
		mat4_identity(&pivot);
		mat4_identity(&scale);
		mat4_identity(&rot);

		//create the matrices from the transform property of the sprite
		mat4_scale(&scale, _trans.scale);
		mat4_rotate(&rot, _trans.rotation);
		mat4_translate(&translate, _trans.position);
		mat4_translate(&pivot, vec2_create(-_trans.scale.x / 2, -_trans.scale.y / 2));

		mat4_mul(&mat4_comp1, &scale, &pivot);
		mat4_mul(&mat4_comp2, &mat4_comp1, &rot);
		mat4_mul(&mat4_comp2, &mat4_comp2, &translate);

		#if ANKH2D_SSE
			mat4_reverse(&model_matrix);
		#endif

		return mat4_comp2;
	}

#elif ANKH2D_PSX

#endif