#pragma once
#include "Renderer/Pass.h"

namespace SceneEditor {

	class ShadowPass : public Pass{
	public:
		ShadowPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath, uint32_t& Width, uint32_t& Height);

		void Execute(const SceneParameters& Parameters) override;

	private:
		uint32_t& m_Width;
		uint32_t& m_Height;
		uint32_t m_Framebuffer;
		uint32_t m_DepthTexture;
	};

}

