#include "shader.h"

const char* loadShader(const char* _filename)
{
	FILE* file;
	char* buffer;

	file = fopen(_filename, "rb");

	if (file == NULL)
		log_fprint("Shader file: '%s' does not exist", _filename);

	//move file pointer to eof and back
	fseek(file, 0, SEEK_END);
	uint16_t filesize = ftell(file);
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

void checkShader(GLuint _shader, GLuint _flag, bool _isProgram)
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

GLuint createShader(const char* _buffer, GLenum _shaderType)
{
	GLuint shader = glCreateShader(_shaderType);
	//GL out of mem
	if (shader == 0)
		log_fprint("Shader creation failed");

	//push data into shader and compile
	glShaderSource(shader, 1, &_buffer, 0);
	glCompileShader(shader);

	checkShader(shader, GL_COMPILE_STATUS, false);

	return shader;
}

Shader::Shader(char* _filename, float _width, float _height)
{
	//create shader program
	m_program = glCreateProgram();
	std::string filename(_filename);

	const char* vertBuf = loadShader((filename + ".vert").c_str());
	m_shaders[VERTEX] = createShader(vertBuf, GL_VERTEX_SHADER);

	const char* fragBuf = loadShader((filename + ".frag").c_str());
	m_shaders[FRAGMENT] = createShader(fragBuf, GL_FRAGMENT_SHADER);

	//attach shaders to the shader program
	for (uint8_t i = 0; i < NUM_SHADERS; i++){
		glAttachShader(m_program, m_shaders[i]);
	}

	//tells GL what part of data to read in shader
	glBindAttribLocation(m_program, 0, "model");
	glBindAttribLocation(m_program, 1, "projection");

	glLinkProgram(m_program);
	checkShader(m_program, GL_LINK_STATUS, true);

	//check if valid (post link)
	glValidateProgram(m_program);
	checkShader(m_program, GL_VALIDATE_STATUS, true);

	//get our transform uniform from vertex shader
	m_uniforms[MODEL] = glGetUniformLocation(m_program, "model");
	m_uniforms[PROJECTION] = glGetUniformLocation(m_program, "projection");

	//setup normalised orthographic view
	m_perspective = glm::ortho(0.0f, _width, _height, 0.0f, -1.0f, 1.0f);

	log_fprint("'%s' shader successfully loaded", _filename);
}

void Shader::update(Sprite& _sprite)
{
	//bind our program
	glUseProgram(m_program);

	//provide model data to the shader via the sprite transform
	glUniformMatrix4fv(m_uniforms[MODEL], 1, GL_FALSE, &_sprite.getTransform()->getModelMatrix()[0][0]);

	//provide projection data to the shader via the sprite transform
	glUniformMatrix4fv(m_uniforms[PROJECTION], 1, GL_FALSE, &m_perspective[0][0]);

	//bind our texture for rendering
	_sprite.getTexture()->bind();
	//draw our sprite to the window
	_sprite.draw();
}

Shader::~Shader()
{
	glDetachShader(m_program, m_shaders[VERTEX]);
	glDeleteShader(m_shaders[VERTEX]);

	glDetachShader(m_program, m_shaders[FRAGMENT]);
	glDeleteShader(m_shaders[FRAGMENT]);

	glDeleteProgram(m_program);
}