#pragma once

namespace SceneEditor{

	class Window{
	public:
		Window( ) = default;
		Window(const char* Name);
		Window(const char* Name, uint32_t Width, uint32_t Height);
		~Window( );

		void Update( );
		void Destroy( );

		bool IsRunning( ) { return (bool)glfwWindowShouldClose(m_Window); };
		bool IsVsyncEnabled( ) { return m_VsyncEnabled;};
		void SetVsync(bool Vsync) { m_VsyncEnabled = Vsync; glfwSwapInterval((int)m_VsyncEnabled);}
		uint32_t& GetWindowWidth( ) { return m_Width; }
		uint32_t& GetWindowHeight( ) { return m_Height; }

		operator GLFWwindow* () { return m_Window; };

	private:
		void CreateApplicationWindow();

	private:
		bool m_VsyncEnabled = false;
		uint32_t m_Width = 0, m_Height = 0;
		std::string m_Name;
		GLFWwindow* m_Window = nullptr;
	};
}