#include "math.h"

float degToRad(float _deg)
{
	return _deg * (PI / 180);
}

float radToDeg(float _rad)
{
	return _rad * (180 / PI);
}

vec2 vec2_create(float _x, float _y)
{
	vec2 tempVec2;
	tempVec2.x = _x;
	tempVec2.y = _y;
	return tempVec2;
}

vec3 vec3_create(float _x, float _y, float _z)
{
	vec3 tempVec3;
	tempVec3.x = _x;
	tempVec3.y = _y;
	tempVec3.z = _z;
	return tempVec3;
}

void mat4_translate(mat4& _model, vec2 _pos)
{
	_model.element[3][0] = _pos.x;
	_model.element[3][1] = _pos.y;
	_model.element[3][2] = 0.0f;
}

void mat4_scale(mat4& _model, vec2 _scale)
{
	_model.element[0][0] = _scale.x;
	_model.element[1][1] = _scale.y;
	_model.element[2][2] = 0.0f;
}

//rotate around the z-axis, allows manipulation of the x and y axis.
void mat4_rotate(mat4& _model, float _degree)
{
	//TODO!

	/*float radians = degToRad(_degree);
	_model.element[0][0] = cos(radians);
	_model.element[1][0] = -sin(radians);
	_model.element[0][1] = sin(radians);
	_model.element[1][1] = cos(radians);*/
}

mat4 mat4_create()
{
	return mat4_identity();
}

//http://www.songho.ca/opengl/gl_projectionmatrix.html#ortho
mat4 mat4_orthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
{
	mat4 tempMat4 = mat4_identity();
	tempMat4.element[0][0] = 2.0f / (_right - _left);
	tempMat4.element[1][1] = 2.0f / (_top - _bottom);
	tempMat4.element[2][2] = -2.0f / (_zFar - _zNear);
	tempMat4.element[3][0] = -((_right + _left) / (_right - _left)); 
	tempMat4.element[3][1] = -((_top + _bottom) / (_top - _bottom));
	tempMat4.element[3][2] = -((_zFar + _zNear) / (_zFar - _zNear));
	return tempMat4;
}

mat4 mat4_identity()
{
	mat4 tempMat4;

	tempMat4.element[0][0] = 1.0f;
	tempMat4.element[1][0] = 0.0f;
	tempMat4.element[2][0] = 0.0f;
	tempMat4.element[3][0] = 0.0f;

	tempMat4.element[0][1] = 0.0f;
	tempMat4.element[1][1] = 1.0f;
	tempMat4.element[2][1] = 0.0f;
	tempMat4.element[3][1] = 0.0f;

	tempMat4.element[0][2] = 0.0f;
	tempMat4.element[1][2] = 0.0f;
	tempMat4.element[2][2] = 1.0f;
	tempMat4.element[3][2] = 0.0f;

	tempMat4.element[0][3] = 0.0f;
	tempMat4.element[1][3] = 0.0f;
	tempMat4.element[2][3] = 0.0f;
	tempMat4.element[3][3] = 1.0f;

	return tempMat4;
}