#pragma once
#include <vector>
#include <functional>

#include "core/Window.h"
#include "UI/ModelPanel.h"

class UIManager{
public:
	static void Initiliaze(Window& );
	static void Draw(const uint32_t SceneId, bool ShowDemo = false);

	static void AddPannel(std::shared_ptr<ModelPanel>& Panel);
	static void SetImportFunction(std::function<void(const std::string&)> Function) { m_ImportFunction = std::move(Function); };

private:
	static void DrawModels( );
	static void DrawProperties( );

	static void ImportModel( );
private:
	static std::vector<std::shared_ptr<ModelPanel>> m_Panels;
	static std::pair<std::string, uint32_t> m_SelectedNode;

	//Functions
	//TODO: New Scene, Export Scene, etc
	static std::function<void(const std::string&)> m_ImportFunction;
};

