#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Window{

public:
	Window( ) = default;
	Window(const char* Name, int Width, int Height);
	~Window( );

	void Update( );
	void Destroy( );

	bool ShouldClose( ) { return (bool)glfwWindowShouldClose(m_Window); };
	bool IsVsyncEnabled( ) { return m_VsyncEnabled;};
	void SetVsync(bool Vsync) { m_VsyncEnabled = Vsync; glfwSwapInterval((int)m_VsyncEnabled);}
	glm::vec2 GetWindowSize( ) { return m_Size; }

	operator GLFWwindow* () { return m_Window; };

private:
	char m_Name[100];
	bool m_VsyncEnabled = false;
	glm::vec2 m_Size = {0.0f, 0.0f};
	GLFWwindow* m_Window = nullptr;
};