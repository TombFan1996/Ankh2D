#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm\vec3.hpp>
#include <glm\matrix.hpp>
#include <glm\gtx\transform.hpp>

class Transform
{
	public:
		Transform()
		{
			m_pos = glm::vec2(0.0f, 0.0f);
			m_rot = 0.0f;
			m_sca = glm::vec2(50.0f, 50.0f);
		}

		Transform(glm::vec2 _pos, float _rot, 
			glm::vec2 _sca = glm::vec2(50.0f, 50.0f))
		{
			m_pos = _pos;
			m_rot = _rot;
			m_sca = _sca;
		}

		inline glm::mat4 getModelMatrix() const
		{
			glm::mat4 model;

			model = glm::translate(model, glm::vec3(m_pos, 0.0f)); 
			model = glm::translate(model, glm::vec3(0.5f * m_sca.x, 0.5f * m_sca.y, 0.0f)); 
			model = glm::rotate(model, m_rot, glm::vec3(0.0f, 0.0f, 1.0f)); 
			model = glm::translate(model, glm::vec3(-0.5f * m_sca.x, -0.5f * m_sca.y, 0.0f));
			model = glm::scale(model, glm::vec3(m_sca, 1.0f));

			return model;
		}

		inline void setPosition(glm::vec2 _pos){m_pos = _pos;}
		inline void setRotation(float _rot){m_rot = _rot;}
		inline void setScale(glm::vec2 _sca){m_sca = _sca;}

		inline glm::vec2& getPosition(){return m_pos;}
		inline float& getRotation(){return m_rot;}
		inline glm::vec2& getScale(){return m_sca;}
	private:
		glm::vec2 m_pos;
		float m_rot;
		glm::vec2 m_sca;
};

#endif