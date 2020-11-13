#include "pch.h"

#include "Application.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Model.h"

std::shared_ptr<Application> Application::GetInstance(const char* Name, uint32_t Width, uint32_t Height)
{	
	static std::shared_ptr<Application> m_AppInstance = nullptr;

	if(m_AppInstance == nullptr){
		m_AppInstance = std::shared_ptr<Application>(new Application(Name, Width, Height));
	}

	return m_AppInstance;
}

Application::Application(const char* Name, uint32_t Width, uint32_t Height) :
	m_Width(Width), m_Height(Height), m_Window(Name, Width, Height), m_Camera(Width, Height), 
	m_ModelShader("D:\\Proiecte\\Licenta\\WorldBuilder\\shaders\\vertex.glsl", "D:\\Proiecte\\Licenta\\WorldBuilder\\shaders\\fragment.glsl")
{
	//m_Window.SetVsync(false);

	// Setup ImGui
	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );
	ImGui::StyleColorsDark( );
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	//temp
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glfwSetCursorPosCallback(m_Window, [ ](GLFWwindow* window, double xpos, double ypos){
		auto app = Application::GetInstance( );
		app->Mouse(window, xpos, ypos);
	});

	m_ModelShader.Bind( );
	m_ModelShader.UploadUniformMat4("projection", m_Camera.getPojection());
	m_ModelShader.Unbind( );
}

void Application::Run( )
{
	//stbi_set_flip_vertically_on_load(true);
	
	// load models
	// -----------
	//Model ourModel("D:\\Facultate\\An 3\\Grafica\\models\\chandelier\\Lamp150(OBJ).obj");
	//Model ourModel("D:\\Facultate\\An 3\\Grafica\\models\\backpack\\backpack.obj");
	Model ourModel("D:\\Facultate\\An 3\\Grafica\\models\\nanosuit\\nanosuit.obj");

	while(m_Window.ShouldClose( ) == false){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );

		KeyboardInput( );

		m_ModelShader.Bind( );

		m_ModelShader.UploadUniformMat4("view", m_Camera.getViewMatrix());

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		m_ModelShader.UploadUniformMat4("model", model);
		ourModel.Draw(m_ModelShader);
		m_ModelShader.Unbind( );
		
		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData( ));

		m_Window.Update( );
	}
}

void Application::Mouse(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static double lastX = m_Width / 2;
	static double lastY = m_Height / 2;
	const float sensitivity = 0.05f;

	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_Camera.rotate(yoffset, xoffset);

	m_ModelShader.Bind();
	m_ModelShader.UploadUniformMat4("view", m_Camera.getViewMatrix( ));
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