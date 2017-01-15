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

	//setup normalised orthographic view
	m_projection = glm::ortho(0.0f, _width, _height, 0.0f, -1.0f, 1.0f);

	log_fprint("'%s' shader successfully loaded", _filename);
}

GLuint Shader::getUniformLocation(char* _uniform)
{
	return glGetUniformLocation(m_program, _uniform);
}

void Shader::getUniformMat4(GLuint _uniform, glm::mat4 _matrix4)
{
	glUniformMatrix4fv(_uniform, 1, GL_FALSE, &_matrix4[0][0]);
}

Shader::~Shader()
{
	glDetachShader(m_program, m_shaders[VERTEX]);
	glDeleteShader(m_shaders[VERTEX]);

	glDetachShader(m_program, m_shaders[FRAGMENT]);
	glDeleteShader(m_shaders[FRAGMENT]);

	glDeleteProgram(m_program);
}