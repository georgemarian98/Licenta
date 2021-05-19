#include "pch.h"

#include "Application.h"

#include "Renderer/Renderer.h"
#include "../UI/UIManager.h"
#include "utility/Serializer.h"

namespace SceneEditor{

	std::shared_ptr<Application> Application::GetInstance(const char* Name)
	{
		static std::shared_ptr<Application> m_AppInstance = nullptr;

		if(m_AppInstance == nullptr){
			m_AppInstance = std::shared_ptr<Application>(new Application(Name));
		}

		return m_AppInstance;
	}

	Application::Application(const char* Name) :
		m_Window(Name), m_Camera(m_Window.GetWindowSize( )[0], m_Window.GetWindowSize( )[1])
	{
		auto windowSize = m_Window.GetWindowSize( );
		m_Width = windowSize[0];
		m_Height = windowSize[1];

		// Setup ImGui
		UIManager::Initiliaze(m_Window);

		UIManager::SetNewSceneFunction([ & ]( ){
			m_Scene->ClearScene( );
			UIManager::ClearScene( );
			Renderer::Reset( );
		});

		UIManager::SetImportFunction([ & ](const std::string& Path){

			if(Path.empty( ) == true){
				return;
			}

			uint32_t noVertices;
			auto controller = m_Scene->AddModel(Path, noVertices);

			UIManager::AddPannel(controller);
			UIManager::UpdateNumberVertices(noVertices);
		});

		UIManager::SetImportSceneFunction([ & ](const std::string& Path){

			if(Path.empty( ) == true){
				return;
			}

			Serializer srl;
			auto models = srl.ImportModels(Path);

			for(auto& model : models){
				UIManager::UpdateNumberVertices(model->GetNoVertices( ));

				auto controller = m_Scene->AddModel(model);

				UIManager::AddPannel(controller);
			}
		});

		UIManager::SetExportSceneFunction([ & ](const std::string& Path){

			if(Path.empty() == true){
				return;
			}

			Serializer exp(m_Scene);
			exp.ExportScene(Path);

			std::string message = "Scene exported successfully to " + std::string{Path};
			UIManager::ShowPopUp(message);
		});

		m_SceneBuffer = std::make_unique<Framebuffer>(m_Width, m_Height);
		m_Scene = std::make_unique<Scene>( );

		std::unique_ptr<Pass> renderPass = std::make_unique<RenderPass>("D:\\Proiecte\\Licenta\\Engine\\shaders\\vertex.glsl", "D:\\Proiecte\\Licenta\\Engine\\shaders\\fragment.glsl");
		m_Scene->AddPass(renderPass);

		glfwSetWindowSizeCallback(m_Window, [ ](GLFWwindow* window, int width, int height){
			auto app = Application::GetInstance( );
			app->ResizeWindow(window, width, height);
		});
	}

	void Application::Run( )
	{
		//auto temp = m_Scene->AddModel("D:\\3D Models\\nanosuit\\nanosuit.obj");
		//UIManager::AddPannel(temp);

		m_Window.SetVsync(false);
		while(m_Window.IsRunning( ) == false){
			KeyboardInput( );

			m_SceneBuffer->Bind( );

			m_Scene->Draw(m_Camera);

			m_SceneBuffer->Unbind( );

			UIManager::Draw(m_SceneBuffer->GetColorId( ));
			m_Window.Update( );
		}
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
			m_Camera.Move(MOVE_DIRECTION::MOVE_FORWARD, TS);
		}

		if(glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS){
			m_Camera.Move(MOVE_DIRECTION::MOVE_BACKWARD, TS);
		}

		if(glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS){
			m_Camera.Move(MOVE_DIRECTION::MOVE_RIGHT, TS);
		}

		if(glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS){
			m_Camera.Move(MOVE_DIRECTION::MOVE_LEFT, TS);
		}
	}

	void Application::ResizeWindow(GLFWwindow* window, int width, int height)
	{
		fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);

		if(width != 0 && height != 0){
			m_Camera.SetProjection(width, height);
			m_SceneBuffer->Resize(width, height);
		}

	}
}
