#include "transform.h"

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