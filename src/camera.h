#ifndef CAMERA_H
#define CAMERA_H

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\gtx\transform.hpp>
#include "transform.h"
#include <SDL.h>

#include "sprite.h"

enum CDir
{
	CAM_STILL,
	CAM_LEFT,
	CAM_RIGHT,
	CAM_UP,
	CAM_DOWN
};

class Sprite;

class Camera
{
	public:
		Camera(SDL_Window* _window);
		void update();
		void movement();
		void update(Sprite& _sprite);
		void setFreeCamera(bool _free){m_freeCamera = _free;}
		bool getFreeCamera(){return m_freeCamera;}
		Transform* getTransform(){return &m_transform;}
		glm::mat4 getProjection(){return m_projection;}
		~Camera();
	private:
		bool m_freeCamera;
		Transform m_transform;
		CDir camState;
		const uint8_t* m_keys;
		float m_speed; //camera speed when free (not attached to player)
		float m_screenWidth, m_screenHeight;
		glm::mat4 m_projection;
};

#endif