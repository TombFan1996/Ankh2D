#include "transform.h"

transform transform_create(vec2 _pos, float _rotation, vec2 _scale)
{
	transform newTrans;
	newTrans.position = _pos;
	newTrans.rotation = _rotation;
	newTrans.scale = _scale;
	return newTrans;
}

mat4 transform_getModelMatrix(transform _trans)
{
	mat4 model, trans, scale, rot;
	//get each individual matrix for the transform
	trans = mat4_getTranslate(_trans.position);
	scale = mat4_getScale(_trans.scale);
	rot = mat4_getRotate(_trans.rotation);
	//model = trans * rotation * scale
	model = mat4_getModelMatrix(trans, rot, scale);
	return model;
}