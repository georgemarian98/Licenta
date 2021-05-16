#include "pch.h"
#include "Scene.h"

namespace SceneEditor{

	void Scene::Draw(const Camera& SceneCamera)
	{
		Renderer::Clear( );

		for(auto& pass : m_Passes){
			pass->Execute(m_SceneModels, SceneCamera);
		}
	}

	std::shared_ptr<ModelController> Scene::AddModel(const std::string_view& Path)
	{
		auto&& model = std::make_shared<Model>(Path.data());
		m_SceneModels.emplace_back(model);

		return m_SceneModels.back()->GetModelController();
	}

	std::shared_ptr<ModelController> Scene::AddModel(const std::shared_ptr<Model>& Model)
	{
		m_SceneModels.emplace_back(std::move(Model));
		return m_SceneModels.back( )->GetModelController( );
	}

	void Scene::AddPass(std::unique_ptr<Pass>& Pass_p)
	{
		m_Passes.emplace_back(std::move(Pass_p));
	}
}


