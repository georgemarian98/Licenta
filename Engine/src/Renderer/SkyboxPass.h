#pragma once
#include "Renderer/Pass.h"

namespace SceneEditor{
	class SkyboxPass : public Pass{
	public:
		SkyboxPass( ) = default;
		SkyboxPass(const char* ShaderVertexPath, const char* ShaderFragmentPath, std::shared_ptr<SkyBox> Skybox) :
			Pass{ "Skybox", ShaderVertexPath, ShaderFragmentPath }, m_Skybox{Skybox} { };

		virtual void Execute(const SceneParameters& Parameter) override;

	private:
		std::shared_ptr<SkyBox> m_Skybox;
	};
}


