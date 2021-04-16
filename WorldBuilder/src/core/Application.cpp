#include "pch.h"

#include "Application.h"
#include "Renderer/Model.h"

#include "Renderer/Renderer.h"
#include "UI/UIManager.h"

#include <functional>

std::shared_ptr<Application> Application::GetInstance(const char* Name, uint32_t Width, uint32_t Height)
{	
	static std::shared_ptr<Application> m_AppInstance = nullptr;

	if(m_AppInstance == nullptr){
		m_AppInstance = std::shared_ptr<Application>(new Application(Name, Width, Height));
	}

	return m_AppInstance;
}

Application::Application(const char* Name, uint32_t Width, uint32_t Height) :
	m_Width(Width), m_Height(Height), m_Window(Name, Width, Height), m_Camera(Width, Height)
{
	// Setup ImGui
	Renderer::Initiliaze( );
	UIManager::Initiliaze(m_Window);

	UIManager::SetImportFunction([ & ](const std::string& Path)	{
		UIManager::AddPannel(m_Scene->AddModel(Path));
	});

	m_SceneBuffer = std::make_unique<Framebuffer>(m_Width, m_Height);
	m_Scene = std::make_unique<Scene>();

	std::unique_ptr<Pass> renderPass = std::make_unique<RenderPass>("D:\\Proiecte\\Licenta\\WorldBuilder\\shaders\\vertex.glsl", "D:\\Proiecte\\Licenta\\WorldBuilder\\shaders\\fragment.glsl");
	m_Scene->AddPass(renderPass);

	//glfwSetCursorPosCallback(m_Window, [ ](GLFWwindow* window, double xpos, double ypos){
	//	auto app = Application::GetInstance( );
	//	app->Mouse(window, xpos, ypos);
	//});

	glfwSetWindowSizeCallback(m_Window, [ ](GLFWwindow* window, int width, int height){
		auto app = Application::GetInstance( );
		app->ResizeWindow(window, width, height);
	});
}

void Application::Run( )
{
	//stbi_set_flip_vertically_on_load(true);
	
	UIManager::AddPannel(m_Scene->AddModel("D:\\3D Models\\nanosuit\\nanosuit.obj"));
	//UIManager::AddPannel(m_Scene->AddModel("D:\\3D Models\\muro\\muro.obj"));
	//UIManager::AddPannel(m_Scene->AddModel("D:\\3D Models\\gobber\\GoblinX.obj"));
	//UIManager::AddPannel(m_Scene->AddModel("D:\\3D Models\\brick_wall\\brick_wall.obj"));
	m_Window.SetVsync(false);

	while(m_Window.ShouldClose( ) == false){
		KeyboardInput( );

		m_SceneBuffer->Bind( );
		Renderer::Clear( );

		m_Scene->Draw(m_Camera);

		m_SceneBuffer->Unbind( );

		UIManager::Draw(m_SceneBuffer->GetColorId());
		m_Window.Update( );
	}
}

void Application::Mouse(GLFWwindow* Window, double Xpos, double Ypos)
{
	static bool firstMouse = true;
	static double lastX = m_Width / 2.0;
	static double lastY = m_Height / 2.0;
	constexpr float sensitivity = 0.05f;

	if(firstMouse){
		lastX = Xpos;
		lastY = Ypos;
		firstMouse = false;
	}

	double xoffset = Xpos - lastX;
	double yoffset = lastY - Ypos;
	lastX = Xpos;
	lastY = Ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;
}

void Application::KeyboardInput( )
{
	static double TimeStep = 0;
	double currTS = glfwGetTime( );
	double TS = currTS - TimeStep;
	TimeStep = currTS;

	if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(m_Window, GL_TRUE);
	}

	//Camera
	if(glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS){
		m_Camera.move(MOVE_DIRECTION::MOVE_FORWARD, TS);
	}

	if(glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS){
		m_Camera.move(MOVE_DIRECTION::MOVE_BACKWARD, TS);
	}

	if(glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS){
		m_Camera.move(MOVE_DIRECTION::MOVE_RIGHT, TS);
	}

	if(glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS){
		m_Camera.move(MOVE_DIRECTION::MOVE_LEFT, TS);
	}
}

void Application::ResizeWindow(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);

	m_Camera.setProjection(width, height);
	m_SceneBuffer->Resize(width, height);
}
