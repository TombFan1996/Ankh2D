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

	m_speed = 0.25f;
	spriteState = SDir::SPRITE_STILL;
	m_keys = SDL_GetKeyboardState(NULL);
}

void Sprite::setTexture(Texture2D* _newTex)
{
	if (!m_texture)
		delete m_texture;
	else
		m_texture = _newTex;
}

void Sprite::update()
{
	SDL_PumpEvents();

	if (m_keys[SDL_SCANCODE_W])
		spriteState = SDir::SPRITE_UP;

	else if (m_keys[SDL_SCANCODE_A])
		spriteState = SDir::SPRITE_LEFT;

	else if (m_keys[SDL_SCANCODE_S])
		spriteState = SDir::SPRITE_DOWN;

	else if (m_keys[SDL_SCANCODE_D])
		spriteState = SDir::SPRITE_RIGHT;

	else
		spriteState = SDir::SPRITE_STILL;
	
	switch(spriteState)
	{
		case SDir::SPRITE_UP:
			m_transform->setPosition(glm::vec2(m_transform->getPosition().x, 
				m_transform->getPosition().y - m_speed));
			break;
		case SDir::SPRITE_LEFT:
			m_transform->setPosition(glm::vec2(m_transform->getPosition().x - m_speed, 
				m_transform->getPosition().y));
			break;
		case SDir::SPRITE_DOWN:
			m_transform->setPosition(glm::vec2(m_transform->getPosition().x, 
				m_transform->getPosition().y + m_speed));
			break;
		case SDir::SPRITE_RIGHT:
			m_transform->setPosition(glm::vec2(m_transform->getPosition().x + m_speed, 
				m_transform->getPosition().y));
			break;
	}
}

void Sprite::draw(glm::mat4 _projection)
{
	//bind our program
	glUseProgram(m_shader->getProgram());

	//communicate w/ uniforms
	//send the model matrix off
	m_shader->getUniformMat4(m_model, m_transform->getModelMatrix());

	//send the projection matrix off
	m_shader->getUniformMat4(m_projection, _projection);

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