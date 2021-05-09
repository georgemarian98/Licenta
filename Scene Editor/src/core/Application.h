#pragma once

#include "Window.h"
#include "Camera.h"
#include "OpenGL/Framebuffer.h"
#include "Renderer/Scene.h"
#include "ApplicationCamera.h"

namespace SceneEditor{

	class Application{

	public:
		static std::shared_ptr<Application> GetInstance(const char* Name = "");

		void Run( );
	private:
		Application( ) = default;
		Application(Application&) = delete;
		Application(const char* Name);

		void Mouse(GLFWwindow* Window, double Xpos, double Ypos);
		void KeyboardInput( );
		void ResizeWindow(GLFWwindow* window, int width, int height);

	private:
		uint32_t m_Width = 0, m_Height = 0;
		Window m_Window;
		ApplicationCamera m_Camera;

		std::unique_ptr<Framebuffer> m_SceneBuffer;
		std::unique_ptr<Scene> m_Scene;
	};
}


