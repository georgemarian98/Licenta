#pragma once
#include <memory>

#include "Window.h"
#include "Camera.h"
#include "OpenGL/Shader.h"
class Application{

public:
	static std::shared_ptr<Application> GetInstance(const char* Name = "", uint32_t Width = 1280, uint32_t Height = 720);
	
	void Run( );
private:
	Application( ) = default;
	Application(Application& ) = delete;
	Application(const char* Name, uint32_t Width, uint32_t Height);

	void Mouse(GLFWwindow* window, double xpos, double ypos);
	void KeyboardInput( );
private:
	uint32_t m_Width = 0, m_Height = 0;
	Window m_Window;
	Camera m_Camera;
	Shader m_ModelShader;
};

