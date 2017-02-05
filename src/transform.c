#include "transform.h"

transform transform_create(vec2 _pos, float _rotation, vec2 _scale)
{
	transform newTrans;
	newTrans.position = _pos;
	newTrans.rotation = _rotation;
	newTrans.scale = _scale;
	return newTrans;
}

//get each individual matrix for the transform
//model = scale * trans * rotation
mat4 transform_getModelMatrix(transform _trans)
{
	mat4 model = mat4_identity();
	mat4_scale(model, _trans.scale);
	//mat4_rotate(model, _trans.rotation);
	mat4_translate(model, _trans.position);
	return model;
}