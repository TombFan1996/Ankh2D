#include "sprite.h"

Sprite::Sprite(char* _texName, Shader* _shader, Transform* _trans)
{
	m_shader = _shader;
	m_texture = new Texture2D(_texName);
	m_transform = _trans;

	m_model = m_shader->getUniformLocation("model");
	m_projection = m_shader->getUniformLocation("projection");

	GLfloat vertices[] = {
		// Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
	};

	//quad is normalised for positional and tex coords
	glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

void Sprite::setTexture(Texture2D* _newTex)
{
	if (!m_texture)
		delete m_texture;
	else
		m_texture = _newTex;
}

void Sprite::draw()
{
	//bind our program
	glUseProgram(m_shader->getProgram());

	//communicate w/ uniforms
	//send the model matrix off
	m_shader->getUniformMat4(m_model, m_transform->getModelMatrix());

	//send the projection matrix off
	m_shader->getUniformMat4(m_projection, m_shader->getProjection());

	//bind our texture
	m_texture->bind();

	//bind and draw our object
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

Sprite::~Sprite()
{
	delete m_transform;
	delete m_texture;

	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
}