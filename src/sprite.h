#ifndef SPRITE_H
#define SPRITE_H

#include <glm\vec2.hpp>
#include "texture.h"
#include "transform.h"

class Sprite
{
	public:
		Sprite(char* _texName, Transform* _trans);
		~Sprite();

		inline void setTransform(Transform* _trans){m_transform = _trans;}
		void setTexture(Texture2D* _newTex);

		inline Transform* getTransform(){return m_transform;}
		inline Texture2D* getTexture(){return m_texture;}
	
		//should only be called by the shader manager
		void draw();

	private:
		Transform* m_transform;
		Texture2D* m_texture;
		GLuint m_VAO, m_VBO;
};

#endif