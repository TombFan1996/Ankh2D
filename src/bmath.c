#include "bmath.h"

#if ANKH2D_WIN32
	float deg_to_rad(float _deg)
	{
		return _deg * (PI / 180.0f);
	}

	float rad_to_deg(float _rad)
	{
		return _rad * (180.0f / PI);
	}

	vec2 vec2_create(float _x, float _y)
	{
		vec2 temp_vec2;
		temp_vec2.x = _x;
		temp_vec2.y = _y;
		return temp_vec2;
	}

	int32_vec2 int32_vec2_create(uint32_t _x, uint32_t _y)
	{
		int32_vec2 temp_vec2;
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

	//when using SSE, the calculations on the mat4 are performed in reverse
	//which is terrible for GL, this reverses back for us
	void mat4_reverse(mat4* _old_mat4)
	{
		#if ANKH2D_SSE
			_old_mat4->element[0] = _mm_set_ps(_old_mat4->element[0].m128_f32[0], _old_mat4->element[0].m128_f32[1],
				_old_mat4->element[0].m128_f32[2], _old_mat4->element[0].m128_f32[3]);

			_old_mat4->element[1] = _mm_set_ps(_old_mat4->element[1].m128_f32[0], _old_mat4->element[1].m128_f32[1],
				_old_mat4->element[1].m128_f32[2], _old_mat4->element[1].m128_f32[3]);

			_old_mat4->element[2] = _mm_set_ps(_old_mat4->element[2].m128_f32[0], _old_mat4->element[2].m128_f32[1],
				_old_mat4->element[2].m128_f32[2], _old_mat4->element[2].m128_f32[3]);

			_old_mat4->element[3] = _mm_set_ps(_old_mat4->element[3].m128_f32[0], _old_mat4->element[3].m128_f32[1],
				_old_mat4->element[3].m128_f32[2], _old_mat4->element[3].m128_f32[3]);
		#endif
	}

	void mat4_translate(mat4* _model, vec2 _pos)
	{
		#if ANKH2D_SSE
			//we have to reverse these position to prevent conflict with the rotation
			__m128 new_pos_0 = _mm_set_ps(_model->element[3].m128_f32[3] * _pos.x,
				_model->element[3].m128_f32[2] * _pos.x, _model->element[3].m128_f32[1] * _pos.x, 
					_model->element[3].m128_f32[0] * _pos.x);
			_model->element[0] = _mm_add_ps(_model->element[0], new_pos_0);

			__m128 new_pos_1 = _mm_set_ps(_model->element[3].m128_f32[3] * _pos.y,
				_model->element[3].m128_f32[2] * _pos.y, _model->element[3].m128_f32[1] * _pos.y, 
					_model->element[3].m128_f32[0] * _pos.y);
			_model->element[1] = _mm_add_ps(_model->element[1], new_pos_1);

			__m128 new_pos_2 = _mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f);
			_model->element[2] = _mm_add_ps(_model->element[2], new_pos_2);
		#else
			/*_model->element[0][0] += _model->element[3][0] * _pos.x;
			_model->element[1][0] += _model->element[3][0] * _pos.y;
			_model->element[2][0] += _model->element[3][0] * 0.0f;

			_model->element[0][1] += _model->element[3][1] * _pos.x;
			_model->element[1][1] += _model->element[3][1] * _pos.y;
			_model->element[2][1] += _model->element[3][1] * 0.0f;

			_model->element[0][2] += _model->element[3][2] * _pos.x;
			_model->element[1][2] += _model->element[3][2] * _pos.y;
			_model->element[2][2] += _model->element[3][2] * 0.0f;

			_model->element[0][3] += _model->element[3][3] * _pos.x;
			_model->element[1][3] += _model->element[3][3] * _pos.y;
			_model->element[2][3] += _model->element[3][3] * 0.0f;*/

			_model->element[0][3] = _pos.x;
			_model->element[1][3] = _pos.y;
			_model->element[2][3] = 0.0f;
		#endif
	}

	void mat4_scale(mat4* _model, vec2 _scale)
	{
		#if ANKH2D_SSE
			_model->element[0] = _mm_set_ps(_scale.x, 0.0f, 0.0f, 0.0f);
			_model->element[1] = _mm_set_ps(0.0f, _scale.y, 0.0f, 0.0f);
			_model->element[2] = _mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f);
		#else
			_model->element[0][0] = _scale.x;
			_model->element[1][1] = _scale.y;
			_model->element[2][2] = 0.0f;
		#endif
	}

	//http://www.songho.ca/opengl/gl_matrix.html#transform
	//z-axis rotation, allows manipulation of the x and y axis.
	void mat4_rotate(mat4* _model, float _degree)
	{
		//degree was rotating wrong way
		float radians = deg_to_rad(_degree);

		#if ANKH2D_SSE
			__m128 new_elem_0 = _mm_set_ps(_model->element[0].m128_f32[0] * cosf(radians) + _model->element[0].m128_f32[1] * -sinf(radians),
				_model->element[0].m128_f32[0] * sinf(radians) + _model->element[0].m128_f32[1] * cosf(radians), 0.0f, 0.0f);
			_model->element[0] = _mm_add_ps(_model->element[0], new_elem_0);
		
			__m128 new_elem_1 = _mm_set_ps(_model->element[1].m128_f32[0] * cosf(radians) + _model->element[1].m128_f32[1] * -sinf(radians),
				_model->element[1].m128_f32[0] * sinf(radians) + _model->element[1].m128_f32[1] * cosf(radians), 0.0f, 0.0f);
			_model->element[1] = _mm_add_ps(_model->element[1], new_elem_1);
		#else
			//_model->element[0][0] = _model->element[0][0] * cosf(radians) + _model->element[0][1] * -sinf(radians);
			//_model->element[0][1] = _model->element[0][0] * sinf(radians) + _model->element[0][1] * cosf(radians);
			//_model->element[1][0] = _model->element[1][0] * cosf(radians) + _model->element[1][1] * -sinf(radians);
			//_model->element[1][1] = _model->element[1][0] * sinf(radians) + _model->element[1][1] * cosf(radians);

			_model->element[0][0] *= cosf(radians);// + _model->element[0][1] * -sinf(radians);
			_model->element[0][1] *= -sinf(radians);// + _model->element[0][1] * cosf(radians);
			_model->element[1][0] *= sinf(radians);// + _model->element[1][1] * -sinf(radians);
			_model->element[1][1] *= cosf(radians);// + _model->element[1][1] * cosf(radians);
		#endif
	}

	mat4 mat4_create()
	{
		//SSE = __m128 (16 byte bound), float[4][4] also 16 bytes
		mat4 mat_new;
		mat4_identity(&mat_new);
		return mat_new;
	}

	//http://www.songho.ca/opengl/gl_projectionmatrix.html#ortho
	mat4 mat4_orthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar)
	{
		mat4 mat_ortho;
		mat4_identity(&mat_ortho);

		#if ANKH2D_SSE
			mat_ortho.element[0] = _mm_set_ps(2.0f / (_right - _left), 0.0f, 0.0f, 0.0f);
			mat_ortho.element[1] = _mm_set_ps(0.0f, 2.0f / (_top - _bottom), 0.0f, 0.0f);
			mat_ortho.element[2] = _mm_set_ps(0.0f, 0.0f, -2.0f / (_zFar - _zNear), 0.0f);
			mat_ortho.element[3] = _mm_set_ps(
				-((_right + _left) / (_right - _left)),
				-((_top + _bottom) / (_top - _bottom)),
				-((_zFar + _zNear) / (_zFar - _zNear)),
				1.0f);

			mat4_reverse(&mat_ortho);
		#else
			mat_ortho.element[0][0] = 2.0f / (_right - _left);
			mat_ortho.element[1][1] = 2.0f / (_top - _bottom);
			mat_ortho.element[2][2] = -2.0f / (_zFar - _zNear);
			mat_ortho.element[3][0] = -((_right + _left) / (_right - _left)); 
			mat_ortho.element[3][1] = -((_top + _bottom) / (_top - _bottom));
			mat_ortho.element[3][2] = -((_zFar + _zNear) / (_zFar - _zNear));
		#endif

		return mat_ortho;
	}

	void mat4_identity(mat4* _mat_iden)
	{
		#if ANKH2D_SSE
			_mat_iden->element[0] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			_mat_iden->element[1] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
			_mat_iden->element[2] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
			_mat_iden->element[3] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
		#else
			_mat_iden->element[0][0] = 1.0f;
			_mat_iden->element[1][0] = 0.0f;
			_mat_iden->element[2][0] = 0.0f;
			_mat_iden->element[3][0] = 0.0f;

			_mat_iden->element[0][1] = 0.0f;
			_mat_iden->element[1][1] = 1.0f;
			_mat_iden->element[2][1] = 0.0f;
			_mat_iden->element[3][1] = 0.0f;

			_mat_iden->element[0][2] = 0.0f;
			_mat_iden->element[1][2] = 0.0f;
			_mat_iden->element[2][2] = 1.0f;
			_mat_iden->element[3][2] = 0.0f;

			_mat_iden->element[0][3] = 0.0f;
			_mat_iden->element[1][3] = 0.0f;
			_mat_iden->element[2][3] = 0.0f;
			_mat_iden->element[3][3] = 1.0f;
		#endif
	}

#elif ANKH2D_PSX
	
#endif