#pragma once
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                #pragma once
#include <vector>
#include <functional>

#include "core/Window.h"
#include "Controllers/ModelController.h"
#include "Controllers/LightController.h"

namespace SceneEditor{

	class Serializer;

	class UIManager{
	public:
		friend Serializer;
		/// <summary>
		/// Initialize ImGui context
		/// </summary>
		/// <param name="Window">: reference to the application GLFW window</param>
		static void Initiliaze(Window& Window, float* CameraSpeed);

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

		static void SetLightController(std::shared_ptr<LightController> LightController) { m_LightController = LightController; };

		static void SetDeleteModelFunction(std::function<void(uint32_t)> Function)           { m_DeleteModel         = std::move(Function); };
		static void SetSkyboxFunction(std::function<void(const std::string&)> Function)      { m_SetSkybox           = std::move(Function); };
		static void SetNewSceneFunction   (std::function<void(void)> Function)               { m_NewSceneFunction    = std::move(Function); };
		static void SetImportFunction     (std::function<void(const std::string&)> Function) { m_ImportModelFunction = std::move(Function); };
		static void SetImportSceneFunction(std::function<void(const std::string&)> Function) { m_ImportSceneFunction = std::move(Function); };
		static void SetExportSceneFunction(std::function<void(const std::string&)> Function) { m_ExportSceneFunction = std::move(Function); };

		static void ClearScene( ) { m_Controllers.clear( ); m_NumVertices = 0;  m_Clear = true; };

		static void ShowPopUp(const std::string& Message) { m_ShowPopUp = true; m_PopUpText = Message; };
		static void UpdateNumberVertices(int Vertices) { m_NumVertices += Vertices;};

		static int GetModelPanelWidth() { return m_ModelPanelWidth;};
		static bool IsSceneFocused() { return m_IsSceneFocused;};

	private:
		static void DrawModels( );
		static void DrawProperties( );
		static void DrawStats( );
		static void DrawLightProperties( );
		static void DrawCameraProperties( );

		static void SetSkybox( );
		static void ImportModel( );
		static void ImportScene( );
		static void ExportScene( );

	private:
		static float* m_CameraSpeed;

		static bool m_IsSceneFocused;
		static int32_t m_ModelPanelWidth;

		static uint32_t m_NumVertices;
		static bool m_Clear;

		static bool m_ShowPopUp;
		static std::string m_PopUpText;

		static Component m_SelectedNode;
		static std::vector<std::shared_ptr<ModelController>> m_Controllers;
		static std::shared_ptr<LightController> m_LightController;

		//Functions
		static std::function<void(uint32_t)>           m_DeleteModel;
		static std::function<void(const std::string&)> m_SetSkybox;

		static std::function<void(void)>               m_NewSceneFunction;
		static std::function<void(const std::string&)> m_ImportModelFunction;
		static std::function<void(const std::string&)> m_ImportSceneFunction;
		static std::function<void(const std::string&)> m_ExportSceneFunction;
	};
}

                                                                                                                                                                                                                                                    