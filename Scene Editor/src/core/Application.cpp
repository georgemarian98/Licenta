#include "pch.h"

#include "Application.h"

#include "Renderer/Renderer.h"
#include "../UI/UIManager.h"
#include "utility/Serializer.h"
#include "Renderer/SkyboxPass.h"
#include "Renderer/ShadowPass.h"

//#define TESTING

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
		m_Window(Name), m_Width(m_Window.GetWindowWidth()), m_Height(m_Window.GetWindowHeight()),
		m_Camera(m_Width, m_Height)
	{

		Shader::m_Directory = SHADER_FOLDER;

		m_Scene = std::make_unique<Scene>( );
		m_SceneBuffer = std::make_unique<Framebuffer>(m_Width, m_Height);

#ifndef TESTING				
		std::unique_ptr<Pass> renderPass = std::make_unique<RenderPass>("texture_vertex.glsl", "texture_fragment.glsl");
		m_Scene->AddPass(renderPass);

		std::shared_ptr<SkyBox> Skybox = std::make_shared<SkyBox>();
		std::unique_ptr<Pass> skyboxPass = std::make_unique<SkyboxPass>("skybox_vertex.glsl", "skybox_fragment.glsl", Skybox);
		m_Scene->SetSkybox(Skybox);
		m_Scene->AddPass(skyboxPass);

		uint64_t aux;
		auto temp = m_Scene->AddModel("D:\\3D Models\\nanosuit\\nanosuit.obj", aux);
		UIManager::AddPannel(temp);
		UIManager::UpdateNumberVertices(aux);

#else 
		//Serializer imp;
		m_Scene = Serializer::ImportScene("C:\\Users\\georg\\Desktop\\Scene");
#endif // 1
		
		UIManager::SetLightController(m_Scene->GetLightController( ));

		glfwSetWindowSizeCallback(m_Window, [ ](GLFWwindow* window, int width, int height){
			auto app = Application::GetInstance( );
			app->ResizeWindow(window, width, height);
		});

		glfwSetCursorPosCallback(m_Window, [ ](GLFWwindow* window, double xpos, double ypos){
			auto app = Application::GetInstance( );
			app->MouseInput(window, xpos, ypos);
		});

		InitializeUI();
		InitializeHandlers();
	}

	void Application::Run( )
	{		
		m_Window.SetVsync(true);
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
		if(glfwGetKey(m_Window, GLFW_KEY_F1) == GLFW_PRESS){
			Renderer::SetDrawMode(DrawMode::Normal);
		}

		if(glfwGetKey(m_Window, GLFW_KEY_F2) == GLFW_PRESS){
			Renderer::SetDrawMode(DrawMode::Line);
		}

		if(glfwGetKey(m_Window, GLFW_KEY_F3) == GLFW_PRESS){
			Renderer::SetDrawMode(DrawMode::Point);
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

	void Application::MouseInput(GLFWwindow* Window, double Xpos, double Ypos)
	{
		static bool firstMouse = true;
		
		if(glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE || Xpos < UIManager::GetModelPanelWidth() || UIManager::IsSceneFocused() == false){
			firstMouse = true;
			return;
		}

		static double lastX = m_Width / 2.0;
		static double lastY = m_Height / 2.0;
		const float sensitivity = 0.05f;

		if(firstMouse == true){
			lastX = Xpos;
			lastY = Ypos;
			firstMouse = false;
		}

		double xoffset = Xpos - lastX;
		double yoffset = lastY - Ypos;
		lastX = Xpos;
		lastY = Ypos;

		xoffset *= -sensitivity;
		yoffset *= -sensitivity;

		m_Camera.Rotate(yoffset, xoffset);
	}

	void Application::ResizeWindow(GLFWwindow* window, int width, int height)
	{
		fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);

		if(width != 0 && height != 0){
			m_Width = width;
			m_Height = height;
			m_Camera.SetProjection(width, height);
			m_SceneBuffer->Resize(width, height);
		}

	}

	void Application::InitializeUI()
	{
		UIManager::Initiliaze(m_Window, m_Camera.GetMovementSpeed());
	}

	void Application::InitializeHandlers()
	{
		// Setup ImGui
		UIManager::SetSkyboxFunction([&](const std::string& Path) {

			if (Path.empty() == true)
				return;

			std::array<std::string, 6> faces;

			for (auto& p : std::filesystem::directory_iterator(Path)) {

				std::string filename = p.path().filename().string().substr(0, 1);
				int index = std::stoi(filename) - 1;

				if (0 <= index && index <= 5) {
					faces[index] = p.path().string();
				}
				
			}

			m_Scene->GetSkybox()->LoadTextures(faces);
		});

		UIManager::SetDeleteModelFunction([&](uint32_t Index) {
			m_Scene->DeleteModel(Index);
		});

		UIManager::SetNewSceneFunction([&]() {
			m_Scene->ClearScene();
			UIManager::ClearScene();
			Renderer::Reset();
		});

		UIManager::SetImportModelFunction([&](const std::string& Path) {

			if (Path.empty() == true) {
				return;
			}

			uint64_t noVertices;
			auto controller = m_Scene->AddModel(Path, noVertices);

			UIManager::AddPannel(controller);
			UIManager::UpdateNumberVertices(noVertices);
		});

		UIManager::SetImportSceneFunction([&](const std::string& Path) {

			if (Path.empty() == true) {
				return;
			}

			Serializer srl;
			auto models = srl.ImportModels(Path);

			for (auto& model : models) {
				UIManager::UpdateNumberVertices(model->GetNoVertices());

				auto controller = m_Scene->AddModel(model);

				UIManager::AddPannel(controller);
			}
		});

		UIManager::SetExportSceneFunction([&](const std::string& Path) {

			if (Path.empty() == true) {
				return;
			}

			Serializer exp(m_Scene);
			exp.ExportScene(Path);

			std::string message = "Scene exported successfully to " + std::string{ Path };
			UIManager::ShowPopUp(message);
		});
	}
}
