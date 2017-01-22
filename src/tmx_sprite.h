#ifndef TMX_RENDERER_H
#define TMX_RENDERER_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "texture2d.h"
#include "transform.h"
#include "tmx_parser.h"

#include "shader.h"

//WARNING: tilemap of equal width and height only!
//the offset vs tile size is not sorted yet

class TMX_Sprite
{
	public:
		TMX_Sprite(TMX_MAP& _map, Shader* _shader, Transform* _trans);
		void draw(glm::mat4 _projection);
		~TMX_Sprite();
	private:
		GLuint m_model, m_projection;
		Shader* m_shader;
		Transform* m_transform;
		uint32_t m_vboSize;
		GLuint* m_VAO;
		GLuint* m_VBO;
		Texture2D* m_tilemap;
		TMX_MAP* m_map;
};

#endif