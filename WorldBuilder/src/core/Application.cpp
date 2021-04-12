#include "pch.h"

#include "Application.h"

#include "Renderer/Model.h"

std::ostream& operator<<(std::ostream& g, glm::vec3 vec)
{
	g << vec.x << ", " << vec.y << ", " << vec.z << "\n";
	return g;
}

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
	m_Window.SetVsync(true);

	// Setup ImGui
	//IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );
	ImGui::StyleColorsDark( );
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	//temp
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glfwSetCursorPosCallback(m_Window, [ ](GLFWwindow* window, double xpos, double ypos){
		auto app = Application::GetInstance( );
		app->Mouse(window, xpos, ypos);
	});

	glfwSetWindowSizeCallback(m_Window, [ ](GLFWwindow* window, int width, int height){
		auto app = Application::GetInstance( );
		app->ResizeWindow(window, width, height);
	});

	m_ModelShader.Bind( );
	m_ModelShader.UploadUniformMat4("projection", m_Camera.GetPojection());
	m_ModelShader.Unbind( );
}

void Application::Run( )
{
	//stbi_set_flip_vertically_on_load(true);
	
	// load models
	//Model ourModel("D:/Proiecte/hw3d/hw3d/Models/Sponza/sponza.obj");
	//Model ourModel("D:/Facultate/An 3/Grafica/models/chandelier/Lamp150(OBJ).obj");
	//Model ourModel("D:/Facultate/An 3/Grafica/models/backpack/backpack.obj");
	Model ourModel("D:/Proiecte/hw3d/hw3d/Models/nano_textured/nanosuit.obj");
	//Model ourModel("D:/Facultate/An 3/Grafica/models/nanosuit/nanosuit.obj");

	auto modelView = ourModel.GetModelView( );
	m_Window.SetVsync(false);

	while(m_Window.ShouldClose( ) == false){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		KeyboardInput( );

		m_ModelShader.Bind( );

		m_ModelShader.UploadUniformMat4("view", m_Camera.GetViewMatrix());


		// render the loaded model
		ourModel.Draw(m_ModelShader);
		m_ModelShader.Unbind( );		

		ImGUIDraw(modelView);
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

	//m_Camera.rotate(yoffset, xoffset);

	//m_ModelShader.Bind();
	//m_ModelShader.UploadUniformMat4("view", m_Camera.GetViewMatrix( ));
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

	//glfwGetFramebufferSize(m_Window, &m_width, &m_height);
	m_Camera.setProjection(width, height);

	glm::mat4 projection = m_Camera.GetPojection( );

	m_ModelShader.Bind( );
	m_ModelShader.UploadUniformMat4("projection", projection);

	//textureShader.useShaderProgram( );
	//textureShader.UploadUniformMat4("projection", projection);

	//sky.setProjection(projection);

	glViewport(0, 0, width, height);
}

void Application::ImGUIDraw(std::shared_ptr<ModelPanel>& ModelView)
{
	static bool active = true;

	ImGui_ImplOpenGL3_NewFrame( );
	ImGui_ImplGlfw_NewFrame( );
	ImGui::NewFrame( );

	//ImGui::ShowDemoWindow( );

	ImGui::Begin("Properties");
	ModelView->Draw();
	ImGui::End( );


	ImGui::Render( );
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData( ));
}
