#include "math.h"

float deg_to_rad(float _deg)
{
	return _deg * (PI / 180);
}

float rad_to_deg(float _rad)
{
	return _rad * (180 / PI);
}

vec2 vec2_create(float _x, float _y)
{
	vec2 temp_vec2;
	temp_vec2.x = _x;
	temp_vec2.y = _y;
	return temp_vec2;
}

vec3 vec3_create(float _x, float _y, float _z)
{
	vec3 temp_vec3;
	temp_vec3.x = _x;
	temp_vec3.y = _y;
	temp_vec3.z = _z;
	return temp_vec3;
}

void mat4_translate(mat4& _model, vec2 _pos)
{
	_model.element[0][0] += _model.element[3][0] * _pos.x;
	_model.element[1][1] += _model.element[3][0] * _pos.y;
	_model.element[2][0] += _model.element[3][0] * 0.0f;

	_model.element[0][1] += _model.element[3][1] * _pos.x;
	_model.element[1][1] += _model.element[3][1] * _pos.y;
	_model.element[2][1] += _model.element[3][1] * 0.0f;

	_model.element[0][2] += _model.element[3][2] * _pos.x;
	_model.element[1][2] += _model.element[3][2] * _pos.y;
	_model.element[2][2] += _model.element[3][2] * 0.0f;

	_model.element[0][3] += _model.element[3][3] * _pos.x;
	_model.element[1][3] += _model.element[3][3] * _pos.y;
	_model.element[2][3] += _model.element[3][3] * 0.0f;
}

void mat4_scale(mat4& _model, vec2 _scale)
{
	_model.element[0][0] = _scale.x;
	_model.element[1][1] = _scale.y;
	_model.element[2][2] = 0.0f;
}

//http://www.songho.ca/opengl/gl_matrix.html#transform
void mat4_rotate(mat4& _model, float _degree)
{
	//degree was rotating wrong way
	float radians = -deg_to_rad(_degree);
	//z-axis rotation, allows manipulation of the x and y axis.
    _model.element[0][0] = _model.element[0][0] * cosf(radians) + _model.element[0][1] * -sinf(radians);
    _model.element[0][1] = _model.element[0][0] * sinf(radians) + _model.element[0][1] * cosf(radians);
    _model.element[1][0] = _model.element[1][0] * cosf(radians) + _model.element[1][1] * -sinf(radians);
    _model.element[1][1] = _model.element[1][0] * sinf(radians) + _model.element[1][1] * cosf(radians);
}

mat4 mat4_create()
{
	return mat4_identity();
}

//http://www.songho.ca/opengl/gl_projectionmatrix.html#ortho
mat4 mat4_orthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
{
	mat4 mat_ortho = mat4_identity();
	mat_ortho.element[0][0] = 2.0f / (_right - _left);
	mat_ortho.element[1][1] = 2.0f / (_top - _bottom);
	mat_ortho.element[2][2] = -2.0f / (_zFar - _zNear);
	mat_ortho.element[3][0] = -((_right + _left) / (_right - _left)); 
	mat_ortho.element[3][1] = -((_top + _bottom) / (_top - _bottom));
	mat_ortho.element[3][2] = -((_zFar + _zNear) / (_zFar - _zNear));
	return mat_ortho;
}

mat4 mat4_identity()
{
	mat4 mat_iden;

	mat_iden.element[0][0] = 1.0f;
	mat_iden.element[1][0] = 0.0f;
	mat_iden.element[2][0] = 0.0f;
	mat_iden.element[3][0] = 0.0f;

	mat_iden.element[0][1] = 0.0f;
	mat_iden.element[1][1] = 1.0f;
	mat_iden.element[2][1] = 0.0f;
	mat_iden.element[3][1] = 0.0f;

	mat_iden.element[0][2] = 0.0f;
	mat_iden.element[1][2] = 0.0f;
	mat_iden.element[2][2] = 1.0f;
	mat_iden.element[3][2] = 0.0f;

	mat_iden.element[0][3] = 0.0f;
	mat_iden.element[1][3] = 0.0f;
	mat_iden.element[2][3] = 0.0f;
	mat_iden.element[3][3] = 1.0f;

	return mat_iden;
}