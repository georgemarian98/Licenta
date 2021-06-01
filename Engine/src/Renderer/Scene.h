#pragma once

#include "Model.h"
#include "Renderer.h"
#include "Renderer/RenderPass.h"

#include "Controllers/LightController.h"

namespace SceneEditor{

	class Serializer;

	class Scene{

	public:
		friend Serializer;
		Scene( ) { Renderer::Initiliaze( ); m_Light = std::make_shared<LightController>( ); };

		void Draw(Camera& SceneCamera);
		void AddPass(std::unique_ptr<Pass>& Pass_p);
		void ClearScene( ) { m_SceneModels.clear( ); };

		std::shared_ptr<ModelController> AddModel(const std::string_view& Path, uint32_t& NoVertices);
		std::shared_ptr<ModelController> AddModel(std::shared_ptr<Model>& Model);

		void DeleteModel(uint32_t Index) { m_SceneModels.erase(m_SceneModels.begin() + Index); };

		std::shared_ptr<ModelController> GetController(int Index) { return m_SceneModels[Index]->GetModelController( ); };
		std::shared_ptr<LightController> GetLightController( ){	return m_Light; };

	private:
		std::vector<std::shared_ptr<Model>>& GetModels( ) { return m_SceneModels; };

	private:
		std::vector<std::shared_ptr<Model>> m_SceneModels;
		std::vector<std::unique_ptr<Pass>> m_Passes;

		std::shared_ptr<LightController> m_Light;
	};
}

