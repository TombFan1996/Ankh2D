#include "camera.h"

Camera::Camera(SDL_Window* _window)
{
	int width, height;
	SDL_GetWindowSize(_window, &width, &height);
	m_screenHeight = height;
	m_screenWidth = width;
	//setup normalised orthographic view
	m_projection = glm::ortho(0.0f, m_screenWidth, m_screenHeight, 0.0f, -1.0f, 1.0f);

	m_speed = 0.25f;
	camState = CDir::CAM_STILL;
	m_keys = SDL_GetKeyboardState(NULL);
	m_freeCamera = true;
}

void Camera::update(Sprite& _sprite)
{
	this->movement();

	//update orthographic camera following the sprite
	glm::vec2 targetPos = _sprite.getTransform()->getPosition();
	this->m_transform.setPosition(glm::vec2(targetPos.x - 
		(m_screenWidth/2), targetPos.y - (m_screenHeight/2)));
	
	glm::vec2 position = this->m_transform.getPosition();
	m_projection = glm::ortho(position.x, position.x + m_screenWidth, 
		position.y + m_screenHeight, position.y, -1.0f, 1.0f);

}

void Camera::update()
{
	this->movement();

	//update orthographic camera normally
	glm::vec2 position = this->m_transform.getPosition();
	m_projection = glm::ortho(position.x, position.x + m_screenWidth, 
		position.y + m_screenHeight, position.y, -1.0f, 1.0f);
}

void Camera::movement()
{
	SDL_PumpEvents();

	if (m_keys[SDL_SCANCODE_W])
		camState = CDir::CAM_UP;

	else if (m_keys[SDL_SCANCODE_A])
		camState = CDir::CAM_LEFT;

	else if (m_keys[SDL_SCANCODE_S])
		camState = CDir::CAM_DOWN;

	else if (m_keys[SDL_SCANCODE_D])
		camState = CDir::CAM_RIGHT;

	else
		camState = CDir::CAM_STILL;
	
	switch(camState)
	{
		case CDir::CAM_UP:
			m_transform.setPosition(glm::vec2(m_transform.getPosition().x, 
				m_transform.getPosition().y - m_speed));
			break;
		case CDir::CAM_LEFT:
			m_transform.setPosition(glm::vec2(m_transform.getPosition().x - m_speed, 
				m_transform.getPosition().y));
			break;
		case CDir::CAM_DOWN:
			m_transform.setPosition(glm::vec2(m_transform.getPosition().x, 
				m_transform.getPosition().y + m_speed));
			break;
		case CDir::CAM_RIGHT:
			m_transform.setPosition(glm::vec2(m_transform.getPosition().x + m_speed, 
				m_transform.getPosition().y));
			break;
	}
}

Camera::~Camera()
{

}