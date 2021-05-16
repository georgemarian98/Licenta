#pragma once
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                #pragma once
#include <vector>
#include <functional>

#include "core/Window.h"
#include "Controllers/ModelController.h"

namespace SceneEditor{

	class Serializer;

	class UIManager{
	public:
		friend Serializer;
		/// <summary>
		/// Initialize ImGui context
		/// </summary>
		/// <param name="Window">: reference to the application GLFW window</param>
		static void Initiliaze(Window& Window);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="SceneId">: index for the texture ID or framebuffer ID</param>
		static void Draw(const uint32_t SceneId);

		/// <summary>
		/// Add the model panel to the internal structure
		/// </summary>
		/// <param name="Panel"></param>
		static void AddPannel(std::shared_ptr<ModelController>& Panel);

		static void SetNewSceneFunction(std::function<void(void)> Function) { m_NewSceneFunction = std::move(Function); };
		static void SetImportFunction(std::function<void(const char*)> Function) { m_ImportFunction = std::move(Function); };
		static void SetImportSceneFunction(std::function<void(const char*)> Function) { m_ImportSceneFunction = std::move(Function); };
		static void SetExportSceneFunction(std::function<void(const char*)> Function) { m_ExportSceneFunction = std::move(Function); };

		static void ClearScene( ) { m_Controllers.clear( ); m_NumVertices = 0;  m_Clear = true; };

		static void ShowPopUp(const std::string& Message) { m_ShowPopUp = true; m_PopUpText = Message; };
		static void UpdateNumberVertices(int Vertices) { m_NumVertices += Vertices;};

	private:
		static void DrawModels( );
		static void DrawProperties( );
		static void DrawStats( );

		static void ImportModel( );
		static void FolderDialog(std::function<void(const char*)>& Function);

	private:
		static uint32_t m_NumVertices;
		static bool m_Clear;

		static bool m_ShowPopUp;
		static std::string m_PopUpText;

		static std::pair<std::string, uint32_t> m_SelectedNode;
		static std::vector<std::shared_ptr<ModelController>> m_Controllers;

		//Functions
		static std::function<void(void)>        m_NewSceneFunction;
		static std::function<void(const char*)> m_ImportFunction;
		static std::function<void(const char*)> m_ImportSceneFunction;
		static std::function<void(const char*)> m_ExportSceneFunction;
	};
}

                                                                                                                                                                                                                                                    