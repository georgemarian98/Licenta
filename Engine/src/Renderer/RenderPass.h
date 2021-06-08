#pragma once
#include "Pass.h"
#include "OpenGL/Framebuffer.h"

namespace SceneEditor{

	class RenderPass : public Pass{
	public:
		RenderPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath, std::unique_ptr<Framebuffer>& Buffer) : Pass{"Render", VertexShaderPath, FragmentShaderPath },
			m_SceneBuffer{Buffer}
		{
		};

		virtual void Execute(const SceneParameters& Parameter) override;

	public:
		std::unique_ptr<Framebuffer>& m_SceneBuffer;
	};
}

