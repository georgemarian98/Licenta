#pragma once

#include "Window.h"
#include "Camera.h"
#include "OpenGL/Framebuffer.h"
#include "Renderer/Scene.h"
#include "ApplicationCamera.h"

namespace SceneEditor{

	class Application{

	public:
		Application(Application&) = delete;
		Application& operator=(Application& Other) = delete;

		static std::shared_ptr<Application> GetInstance(const char* Name = "");

		void Run( );

	private:
		Application( ) = default;
		Application(const char* Name);

		void InitializeHandlers();

		void KeyboardInput( );
		void MouseInput(GLFWwindow* Window, double Xpos, double Ypos);
		void ResizeWindow(GLFWwindow* window, int width, int height);

	private:
		Window m_Window;

		uint32_t& m_Width;
		uint32_t& m_Height;

		ApplicationCamera m_Camera;

		std::unique_ptr<Scene> m_Scene;
		std::unique_ptr<Framebuffer> m_SceneBuffer;
	};
}


