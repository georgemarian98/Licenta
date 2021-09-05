#pragma once
#include "Pass.h"
#include "OpenGL/Framebuffer.h"

namespace SceneEditor{

	class RenderPass : public Pass{
	public:
		RenderPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath, std::unique_ptr<Framebuffer>& Buffer);

		virtual void Execute(const SceneParameters& Parameter) override;

	public:
		std::unique_ptr<Framebuffer>& m_SceneBuffer;
		uint32_t m_Framebuffer;
		uint32_t m_DepthTexture;
		std::unique_ptr<Shader> m_Shadow;

		bool PLsss = true;
	};
}

