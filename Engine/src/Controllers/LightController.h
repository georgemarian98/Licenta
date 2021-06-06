#pragma once
#include <glm/glm.hpp>

namespace SceneEditor{

	class LightController{
	public:
		LightController( ) : 
			m_Position{glm::vec3(-9.0f, -8.0f, -39.0f)}, m_Color{glm::vec3(1.0f)}, m_Ambient{ 0.2f }, m_Diffuse{ 0.5f }, m_Specular{ 0.5f } { };

		LightController(glm::vec3& Position, glm::vec3 Color, float Ambient, float Diffuse, float Specular) :
			m_Position(Position), m_Color{Color}, m_Ambient{ Ambient }, m_Diffuse{ Diffuse }, m_Specular{ Specular } { };

		glm::vec3& GetPosition( ) { return m_Position; };
		glm::vec3& GetColor( ) { return m_Color; };
		float* GetAmbient( ) { return &m_Ambient; };
		float* GetDiffuse( ) { return &m_Diffuse; };
		float* GetSpecular( ) { return &m_Specular; };

		void SetPosition(glm::vec3 Position) { m_Position = Position; };
		void SetColor(glm::vec3 Color) { m_Color = Color; };
		void SetAmbient(float Ambient) { m_Ambient = Ambient; };
		void SetDiffuse(float Diffuse) { m_Diffuse = Diffuse; };
		void SetSpecular(float Specular) { m_Specular = Specular; };

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Color;
		float m_Ambient;
		float m_Diffuse;
		float m_Specular;
	};
}


