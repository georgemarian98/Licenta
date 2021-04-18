#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Window{

public:
	Window( ) = default;
	Window(const char* Name);
	Window(const char* Name, uint32_t Width, uint32_t Height);
	~Window( );

	void Update( );
	void Destroy( );

	bool ShouldClose( ) { return (bool)glfwWindowShouldClose(m_Window); };
	bool IsVsyncEnabled( ) { return m_VsyncEnabled;};
	void SetVsync(bool Vsync) { m_VsyncEnabled = Vsync; glfwSwapInterval((int)m_VsyncEnabled);}
	glm::u32vec2 GetWindowSize( ) { return {m_Width, m_Height}; }

	operator GLFWwindow* () { return m_Window; };

private:
	void CreateApplicationWindow();

private:
	std::string m_Name;
	bool m_VsyncEnabled = false;
	uint32_t m_Width = 0, m_Height = 0;
	GLFWwindow* m_Window = nullptr;
};