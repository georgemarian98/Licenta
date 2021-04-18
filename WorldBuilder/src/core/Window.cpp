#include "pch.h"
#include "Window.h"

Window::Window(const char* Name) : m_Name(Name)
{
	if(!glfwInit( )){
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		glfwTerminate( );
		return;
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor( );
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

	m_Width = mode->width;
	m_Height = mode->height;
	CreateApplicationWindow();
}

Window::Window(const char* Name, uint32_t Width, uint32_t Height) : m_Width(Width), m_Height(Height), m_Name(Name)
{

	if(!glfwInit( )){
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		glfwTerminate( );
		return;
	}

	CreateApplicationWindow();
}

Window::~Window( )
{
	Destroy( );
}

void Window::Destroy( )
{
	glfwSetWindowShouldClose(m_Window, GL_TRUE);
	glfwDestroyWindow(m_Window);
	glfwTerminate( );
}

void Window::CreateApplicationWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Name.c_str() , NULL, NULL);
	if(m_Window == NULL){
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glCheckError( );
		glfwTerminate( );
		return;
	}

	glfwMakeContextCurrent(m_Window);

	glfwWindowHint(GLFW_SAMPLES, 4);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		fprintf(stderr, "Failed to init GLAD\n");
		return;
	}

	glfwSwapInterval(0);

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}

void Window::Update( )
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents( );
}