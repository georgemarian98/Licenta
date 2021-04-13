#pragma once
#include "Model.h"

#include "Renderer/RenderPass.h"

class Scene{

public:
	Scene( ) = default;

	void Draw(const Camera& SceneCamera);
	void AddPass(std::unique_ptr<Pass>& Pass_p);
	std::shared_ptr<ModelPanel> AddModel(const std::string_view& Path);

private:
	std::vector<std::shared_ptr<Model>> m_SceneModels;
	std::vector<std::unique_ptr<Pass>> m_Passes;
};

