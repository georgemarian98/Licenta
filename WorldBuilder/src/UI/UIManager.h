#pragma once
#include <vector>

#include "core/Window.h"
#include "UI/ModelPanel.h"

class UIManager{
public:
	static void Initiliaze(Window& );
	static void Draw(const uint32_t SceneId, bool ShowDemo = false);

	static void AddPannel(std::shared_ptr<ModelPanel>& Panel);
private:
	static void DrawModels( );
	static void DrawProperties( );

	static Transforms* GetNodeMatricies( );
private:
	static std::vector<std::shared_ptr<ModelPanel>> m_Panels;
	static std::string m_SelectedNode;
};

