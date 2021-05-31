#pragma once
#include "Renderer/Pass.h"

namespace SceneEditor{
	class SkyboxPass : public Pass{
	public:
		SkyboxPass( ) = default;
		SkyboxPass(const char* ShaderVertexPath, const char* ShaderFragmentPath, const std::array<const GLchar*, 6>& CubeMapFaces) : 
			Pass{"Skybox", ShaderVertexPath, ShaderFragmentPath} 
		{
			m_Skybox.Load(CubeMapFaces);
		};

		virtual void Execute(const SceneParameters& Parameter) override;

	private:
		SkyBox m_Skybox;
	};
}


