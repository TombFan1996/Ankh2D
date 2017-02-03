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
	mat4 model;

	//TODO - get the model matrix for 2d transformation
	//https://robertcspencer.files.wordpress.com/2013/08/computer-graphics-7.png

	//model = glm::translate(model, vec3_create(_trans->position.x, _trans->position.y, 0.0f)); 
	//model = glm::translate(model, vec3_create(0.5f * _trans->scale.x, 0.5f * _trans->scale.y, 0.0f)); 
	//model = glm::rotate(model, _trans->rotation, vec3_create(0.0f, 0.0f, 1.0f)); 
	//model = glm::translate(model, vec3_create(-0.5f * _trans->scale.x, -0.5f * _trans->scale.y, 0.0f));
	//model = glm::scale(model, vec3_create(_trans->scale.x, _trans->scale.y, 1.0f));
	return model;
}