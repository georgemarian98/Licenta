#pragma once
#include "Renderer/Pass.h"

namespace SceneEditor{
	class SkyboxPass : public Pass{
	public:
		SkyboxPass( ) = default;
		SkyboxPass(const char* ShaderVertexPath, const char* ShaderFragmentPath) : 
			Pass{"Skybox", ShaderVertexPath, ShaderFragmentPath} { };

		virtual void Execute(const SceneParameters& Parameter) override;

		void LoadSkybox(const std::array<const GLchar*, 6>& CubeMapFaces) { m_Skybox.Load(CubeMapFaces); m_LoadSkybox = true; };

	private:
		SkyBox m_Skybox;
		bool m_LoadSkybox = false;
	};
}


