#pragma once
#include "Renderer/Pass.h"

namespace SceneEditor{
	class SkyboxPass : public Pass{
	public:
		SkyboxPass( ) = default;
		SkyboxPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath, std::shared_ptr<SkyBox> Skybox) :
			Pass{ "Skybox", VertexShaderPath, FragmentShaderPath }, m_Skybox{Skybox} { };

		virtual void Execute(const SceneParameters& Parameter) override;

	private:
		std::shared_ptr<SkyBox> m_Skybox;
	};
}


