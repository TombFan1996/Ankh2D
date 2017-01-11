#include "sprite.h"

Sprite::Sprite(char* _texName, Transform* _trans)
{
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

	m_texture = new Texture2D(_texName);
	m_transform = _trans;
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
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

Sprite::~Sprite()
{
	delete m_texture;
}