#pragma once
#include <memory>

#include "Window.h"
#include "Camera.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Framebuffer.h"

#include "Renderer/Scene.h"
#include "UI/ModelPanel.h"

class Application{

public:
	static std::shared_ptr<Application> GetInstance(const char* Name = "", uint32_t Width = 1280, uint32_t Height = 720);
	
	void Run( );
private:
	Application( ) = default;
	Application(Application& ) = delete;
	Application(const char* Name, uint32_t Width, uint32_t Height);

	void Mouse(GLFWwindow* Window, double Xpos, double Ypos);
	void KeyboardInput( );
	void ResizeWindow(GLFWwindow* window, int width, int height);

private:
	uint32_t m_Width = 0, m_Height = 0;
	Window m_Window;
	Camera m_Camera;

	std::unique_ptr<Framebuffer> m_SceneBuffer;
	std::unique_ptr<Scene> m_Scene;
};


