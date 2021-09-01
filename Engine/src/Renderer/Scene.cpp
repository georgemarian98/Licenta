#include "pch.h"
#include "Scene.h"

namespace SceneEditor{

	void Scene::Draw(Camera& SceneCamera)
	{
		Renderer::Clear( );

		SceneParameters parameters{m_SceneModels, SceneCamera, m_Light};

		for(auto& pass : m_Passes){
			pass->Execute(parameters);
		}
	}

	std::shared_ptr<ModelController> Scene::AddModel(const std::filesystem::path& Path, uint64_t& NoVertices)
	{
		auto model = std::make_shared<Model>(Path);
		auto& controller = model->GetModelController();
		NoVertices = model->GetNoVertices( );

		m_SceneModels.emplace_back(std::move(model));

		return controller;
	}

	std::shared_ptr<ModelController> Scene::AddModel(std::shared_ptr<Model>& Model)
	{
		auto& controller = Model->GetModelController();
		m_SceneModels.emplace_back(std::move(Model));
		return controller;
	}

	void Scene::AddPass(std::unique_ptr<Pass>& Pass_p)
	{
		m_Passes.emplace_back(std::move(Pass_p));
	}
}


