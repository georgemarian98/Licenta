#include "pch.h"
#include "Window.h"

Window::Window(const char* Name, int Width, int Height) : m_Size(Width,Height)
{
	strncpy(m_Name, Name, sizeof(m_Name));

	if(!glfwInit( )){
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		glfwTerminate( );
		return;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(Width, Height, Name, NULL, NULL);
	if(m_Window == NULL){
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glCheckError();
		glfwTerminate( );
		return;
	}

	glfwMakeContextCurrent(m_Window);

	glfwWindowHint(GLFW_SAMPLES, 4);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		fprintf(stderr, "Failed to init GLAD\n");
		return;
	}
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(0);

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
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

void Window::Update( )
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents( );
}