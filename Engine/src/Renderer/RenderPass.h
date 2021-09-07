#pragma once
#include "Pass.h"
#include "OpenGL/Framebuffer.h"

namespace SceneEditor{

	class RenderPass : public Pass{
	public:
		RenderPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath) : Pass("RenderPass", VertexShaderPath, FragmentShaderPath) {};

		virtual void Execute(const SceneParameters& Parameter) override;
	};
}

