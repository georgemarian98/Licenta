#pragma once
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                #pragma once
#include <vector>
#include <functional>

#include "core/Window.h"
#include "UI/ModelPanel.h"

class UIManager{
public:
	/// <summary>
	/// Initialize ImGui context
	/// </summary>
	/// <param name="Window">Reference to the application GLFW window</param>
	static void Initiliaze(Window& Window);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="SceneId">Index for the texture ID or framebuffer ID</param>
	static void Draw(const uint32_t SceneId);

	/// <summary>
	/// Add the model panel to the internal structure
	/// </summary>
	/// <param name="Panel"></param>
	static void AddPannel(std::shared_ptr<ModelPanel>& Panel);

	static void SetNewSceneFunction(std::function<void(void)> Function) { m_NewSceneFunction = std::move(Function); };
	static void SetImportFunction(std::function<void(const char*)> Function) { m_ImportFunction = std::move(Function); };
	static void SetExportSceneFunction(std::function<void(const char*)> Function) { m_ExportSceneFunction = std::move(Function); };

	static void ClearScene( ) { m_Panels.clear( ); };

private:
	static void DrawModels( );
	static void DrawProperties( );

	static void ImportModel( );
	static void ExportScene( );
private:
	static std::vector<std::shared_ptr<ModelPanel>> m_Panels;
	static std::pair<std::string, uint32_t> m_SelectedNode;

	//Functions
	static std::function<void(void)> m_NewSceneFunction;
	static std::function<void(const char*)> m_ImportFunction;
	static std::function<void(const char*)> m_ExportSceneFunction;
};

                                                                                                                                                                                                                                                    