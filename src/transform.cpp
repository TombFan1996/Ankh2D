#include "transform.h"

transform* transform_create()
{
	transform* newTrans = new transform;
	newTrans->position = glm::vec2(0.0f, 0.0f);
	newTrans->rotation = 0.0f;
	newTrans->scale = glm::vec2(50.0f, 50.0f);
	return newTrans;
}

transform* transform_create(glm::vec2 _pos)
{
	transform* newTrans = new transform;
	newTrans->position = _pos;
	newTrans->rotation = 0.0f;
	newTrans->scale = glm::vec2(50.0f, 50.0f);
	return newTrans;
}

transform* transform_create(glm::vec2 _pos, float _rotation)
{
	transform* newTrans = new transform;
	newTrans->position = _pos;
	newTrans->rotation = _rotation;
	newTrans->scale = glm::vec2(50.0f, 50.0f);
	return newTrans;
}

transform* transform_create(glm::vec2 _pos, float _rotation, glm::vec2 _scale)
{
	transform* newTrans = new transform;
	newTrans->position = _pos;
	newTrans->rotation = _rotation;
	newTrans->scale = _scale;
	return newTrans;
}

glm::mat4 transform_getModelMatrix(transform* _trans)
{
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(_trans->position, 0.0f)); 
	model = glm::translate(model, glm::vec3(0.5f * _trans->scale.x, 0.5f * _trans->scale.y, 0.0f)); 
	model = glm::rotate(model, _trans->rotation, glm::vec3(0.0f, 0.0f, 1.0f)); 
	model = glm::translate(model, glm::vec3(-0.5f * _trans->scale.x, -0.5f * _trans->scale.y, 0.0f));
	model = glm::scale(model, glm::vec3(_trans->scale, 1.0f));
	return model;
}