#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Window{

public:
	Window( ) = default;
	Window(const char* name, int width, int height);
	~Window( );

	void Update( );
	void Destroy( );

	bool ShouldClose( ) { return (bool)glfwWindowShouldClose(m_Window); };
	void SetVsync(bool Vsync) { m_VsyncEnabled = Vsync; glfwSwapInterval((int)m_VsyncEnabled);}
	bool IsVsyncEnabled( ) { return m_VsyncEnabled;};
	glm::vec2 GetWindowSize( ) { return m_Size; }

	operator GLFWwindow* () { return m_Window; };

private:
	GLFWwindow* m_Window = nullptr;
	bool m_VsyncEnabled = false;
	glm::vec2 m_Size = {0.0f, 0.0f};
};

struct WindowData{
	const char* title;
	unsigned int width, height;
	bool vsync;
};