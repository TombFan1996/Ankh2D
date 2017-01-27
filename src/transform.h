#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm\vec3.hpp>
#include <glm\matrix.hpp>
#include <glm\gtx\transform.hpp>

typedef struct
{
	glm::vec2 position;
	float rotation;
	glm::vec2 scale;
} transform;

transform* transform_create();
transform* transform_create(glm::vec2 _pos);
transform* transform_create(glm::vec2 _pos, float _rotation);
transform* transform_create(glm::vec2 _pos, float _rotation, glm::vec2 _scale);
glm::mat4 transform_getModelMatrix(transform* _trans);

#endif