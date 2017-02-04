#include "math.h"

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

mat4 mat4_create(float _matElem1, float _matElem2, float _matElem3, float _matElem4,
					float _matElem5, float _matElem6, float _matElem7, float _matElem8,
						float _matElem9, float _matElem10, float _matElem11, float _matElem12,
							float _matElem13, float _matElem14, float _matElem15, float _matElem16)
{
	mat4 tempMat4;
	tempMat4.element[0][0] = _matElem1;
	tempMat4.element[1][0] = _matElem2;
	tempMat4.element[2][0] = _matElem3;
	tempMat4.element[3][0] = _matElem4;

	tempMat4.element[0][1] = _matElem5;
	tempMat4.element[1][1] = _matElem6;
	tempMat4.element[2][1] = _matElem7;
	tempMat4.element[3][1] = _matElem8;

	tempMat4.element[0][2] = _matElem9;
	tempMat4.element[1][2] = _matElem10;
	tempMat4.element[2][2] = _matElem11;
	tempMat4.element[3][2] = _matElem12;

	tempMat4.element[0][3] = _matElem13;
	tempMat4.element[1][3] = _matElem14;
	tempMat4.element[2][3] = _matElem15;
	tempMat4.element[3][3] = _matElem16;

	return tempMat4;
}

//http://www.songho.ca/opengl/gl_projectionmatrix.html#ortho
mat4 mat4_getOrthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
{
	mat4 tempMat4 = mat4_createIdentity();
	tempMat4.element[0][0] = 2.0f / (_right - _left);
	tempMat4.element[1][1] = 2.0f / (_top - _bottom);
	tempMat4.element[2][2] = -2.0f / (_zFar - _zNear);
	tempMat4.element[3][0] = -((_right + _left) / (_right - _left)); 
	tempMat4.element[3][1] = -((_top + _bottom) / (_top - _bottom));
	tempMat4.element[3][2] = -((_zFar + _zNear) / (_zFar - _zNear));
	return tempMat4;
}

mat4 mat4_createIdentity()
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

mat4 mat4_getTranslate(vec2 _pos)
{
	mat4 tempMat4 = mat4_createIdentity();
	tempMat4.element[3][0] = _pos.x;
	tempMat4.element[3][1] = _pos.y;
	return tempMat4;
}

mat4 mat4_getScale(vec2 _scale)
{
	mat4 tempMat4 = mat4_createIdentity();
	tempMat4.element[0][0] = _scale.x;
	tempMat4.element[1][1] = _scale.y;
	return tempMat4;
}

mat4 mat4_getRotate(float _rotation)
{
	mat4 tempMat4 = mat4_createIdentity();
	tempMat4.element[0][0] = cos(_rotation);
	tempMat4.element[0][1] = sin(_rotation);
	tempMat4.element[1][0] = -sin(_rotation);
	tempMat4.element[1][1] = cos(_rotation);
	return tempMat4;
}

mat4 mat4_getModelMatrix(mat4 _trans, mat4 _rot, mat4 _sca)
{
	mat4 modelMat;

	for (uint8_t row = 0; row < 4; row++){
		for (uint8_t col = 0; col < 4; col++){
			modelMat.element[col][row] = _trans.element[col][row] * _rot.element[col][row] * _sca.element[col][row];
		}
	}

	return modelMat;
}