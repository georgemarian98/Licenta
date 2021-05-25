#pragma once

#include "core/Camera.h"
#include "OpenGL/Shader.h"
#include "Renderer/Model.h"
#include "Controllers/LightController.h"

namespace SceneEditor{

	struct SceneParameters{
		std::vector<std::shared_ptr<Model>>& SceneModels;
		Camera& SceneCamera;
		std::shared_ptr<LightController> SceneLight;

		SceneParameters(std::vector<std::shared_ptr<Model>>& SceneModels, Camera& SceneCamera, std::shared_ptr<LightController> SceneLight) :
			SceneModels{SceneModels}, SceneCamera{SceneCamera}, SceneLight{SceneLight} {}
	};

	class Serializer;

	class Pass{
	public:
		Pass( ) = default;
		Pass(const char* Name,const char* ShaderVertexPath, const char* ShaderFragmentPath) : m_Name(Name)
		{
			m_Shader = std::make_unique<Shader>(ShaderVertexPath, ShaderFragmentPath);
		};

		virtual void Execute(const SceneParameters& ) = 0;

	protected:
		std::string m_Name;
		std::unique_ptr<Shader> m_Shader;
	};
}

