#include "pch.h"

#include "Application.h"

std::shared_ptr<Application> Application::GetInstance(const char* Name, uint32_t Width, uint32_t Height)
{	
	static std::shared_ptr<Application> m_AppInstance = nullptr;

	if(m_AppInstance == nullptr){
		m_AppInstance = std::shared_ptr<Application>(new Application(Name, Width, Height));
	}

	return m_AppInstance;
}

Application::Application(const char* Name, uint32_t Width, uint32_t Height):
	m_Window(Name, Width, Height), m_Witdh(Width), m_Height(Height)
{
	m_Window.SetVsync(true);

	// Setup ImGui
	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );
	ImGui::StyleColorsDark( );
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void Application::Run( )
{
	while(m_Window.ShouldClose( ) == false){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );

		//ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData( ));

		m_Window.Update( );
	}
}