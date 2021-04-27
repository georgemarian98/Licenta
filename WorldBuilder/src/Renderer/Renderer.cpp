#include "pch.h"
#include "Renderer.h"

namespace SceneEditor{

	void Renderer::Initiliaze(glm::vec4 ClearColor)
	{
		glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	}

	void Renderer::Clear( )
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::Submit( )
	{
	}
}
