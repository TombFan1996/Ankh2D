#include "shader.h"

shader* shader_create(const char* _filename)
{
	shader* new_shader = (shader*)malloc(sizeof(shader));
	//create shader program
	new_shader->program = glCreateProgram();
	std::string filename(_filename);

	const char* vert_buf = shader_load((filename + ".vert").c_str());
	new_shader->shaders[VERTEX] = shader_get(vert_buf, GL_VERTEX_SHADER);

	const char* frag_buf = shader_load((filename + ".frag").c_str());
	new_shader->shaders[FRAGMENT] = shader_get(frag_buf, GL_FRAGMENT_SHADER);

	//attach shaders to the shader program
	for (uint8_t i = 0; i < NUM_SHADERS; i++){
		glAttachShader(new_shader->program, new_shader->shaders[i]);
	}

	glLinkProgram(new_shader->program);
	shader_check(new_shader->program, GL_LINK_STATUS, true);

	//check if valid (post link)
	glValidateProgram(new_shader->program);
	shader_check(new_shader->program, GL_VALIDATE_STATUS, true);

	log_fprint("'%s' shader successfully loaded", _filename);

	return new_shader;
}

GLuint shader_get(const char* _buffer, GLenum _shaderType)
{
	GLuint shader = glCreateShader(_shaderType);
	//GL out of mem
	if (shader == 0)
		log_fprint("Shader creation failed");

	//push data into shader and compile
	glShaderSource(shader, 1, &_buffer, 0);
	glCompileShader(shader);

	shader_check(shader, GL_COMPILE_STATUS, false);

	return shader;
}

const char* shader_load(const char* _filename)
{
	FILE* file;
	char* buffer;

	file = fopen(_filename, "rb");

	if (file == NULL)
		log_fprint("Shader file: '%s' does not exist", _filename);

	//move file pointer to eof and back
	fseek(file, 0, SEEK_END);
	int32_t filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	//void* to char*, +1 for null terminator
	buffer = (char*)(malloc(filesize+1));

	//read the contents of the shader into our buffer
	fread(buffer, filesize, 1, file);

	fclose(file);

	//add the null terminator
	buffer[filesize] = 0;

	return buffer;
}

void shader_bind_attrib_location(shader* _shader, uint8_t _index, char* _name)
{
	glBindAttribLocation(_shader->program, _index, _name);
}

void shader_set_uniform_mat4(GLuint _uniform, mat4* _matrix4, bool _transpose)
{
	#if USE_SSE
		if (_transpose)
			glUniformMatrix4fv(_uniform, 1, GL_TRUE, &_matrix4->element[0].m128_f32[0]);
		else
			glUniformMatrix4fv(_uniform, 1, GL_FALSE, &_matrix4->element[0].m128_f32[0]);
	#else
		if (_transpose)
			glUniformMatrix4fv(_uniform, 1, GL_TRUE, &_matrix4->element[0][0]);
		else
			glUniformMatrix4fv(_uniform, 1, GL_FALSE, &_matrix4->element[0][0]);
	#endif
}

void shader_set_uniform_float(GLuint _uniform, float _float)
{
	glUniform1f(_uniform, _float);
}

void shader_set_uniform_vec2(GLuint _uniform, vec2 _vec2)
{
	glUniform2f(_uniform, _vec2.x, _vec2.y);
}

void shader_set_uniform_vec3(GLuint _uniform, vec3 _vec3)
{
	glUniform3f(_uniform, _vec3.x, _vec3.y, _vec3.z);
}

GLuint shader_get_uniform_location(shader* _shader, char* _uniform)
{
	return glGetUniformLocation(_shader->program, _uniform);
}

void shader_check(GLuint _shader, GLuint _flag, bool _isProgram)
{
	GLint success = GL_FALSE;
	GLchar error[1024] = {0};

	if (_isProgram)
		glGetProgramiv(_shader, _flag, &success);
	else
		glGetShaderiv(_shader, _flag, &success);

	if (success == GL_FALSE)
	{
		if (_isProgram)
			glGetProgramInfoLog(_shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(_shader, sizeof(error), NULL, error);

		log_fprint("%s", error);
	}
}

void shader_destroy(shader* _shader)
{
	glDetachShader(_shader->program, _shader->shaders[VERTEX]);
	glDeleteShader(_shader->shaders[VERTEX]);

	glDetachShader(_shader->program, _shader->shaders[FRAGMENT]);
	glDeleteShader(_shader->shaders[FRAGMENT]);

	glDeleteProgram(_shader->program);
}