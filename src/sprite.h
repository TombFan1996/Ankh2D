#ifndef SPRITE_H
#define SPRITE_H

#include <glm\vec2.hpp>
#include "texture.h"
#include "transform.h"

#include "shader.h"

class Sprite
{
	public:
		Sprite(char* _texName, Shader* _shader, Transform* _trans);
		~Sprite();

		inline void setTransform(Transform* _trans){m_transform = _trans;}
		void setTexture(Texture2D* _newTex);

		inline Transform* getTransform(){return m_transform;}
		inline Texture2D* getTexture(){return m_texture;}
	
		void draw();
	private:
		GLuint m_model, m_projection;
		Shader* m_shader;
		Transform* m_transform;
		Texture2D* m_texture;
		GLuint m_VAO, m_VBO;
};

#endif