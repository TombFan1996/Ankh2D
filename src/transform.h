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

glm::mat4 transform_getModelMatrix(transform* _trans);

#endif