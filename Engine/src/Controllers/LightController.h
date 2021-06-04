#pragma once
#include <glm/glm.hpp>

namespace SceneEditor{

	class LightController{
	public:
		LightController( ) : m_Position{glm::vec3(9.0f, -8.0f, -39.0f)}, m_Color{glm::vec3(1.0f)} { };
		LightController(glm::vec3& Position, glm::vec3 Color) : m_Position(Position), m_Color{Color} { };

		glm::vec3& GetPosition( ) { return m_Position; };
		glm::vec3& GetColor( ) { return m_Color; };

		void SetPosition(glm::vec3 Position) { m_Position = Position; };
		void SetColor(glm::vec3 Color) { m_Color = Color; };

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Color;
	};
}


