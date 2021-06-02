#include "pch.h"
#include "UIManager.h"

#include <ShlObj.h>

#include "utility/FileDialog.h"
#include "utility/FolderDialog.h"

//#define IMGUI_EXAMPLE

namespace SceneEditor{

	int32_t										  UIManager::m_ModelPanelWidth = 0;
	uint32_t									  UIManager::m_NumVertices = 0;
	bool										  UIManager::m_Clear = false;

	bool                                          UIManager::m_ShowPopUp = false;
	std::string                                   UIManager::m_PopUpText;
	Component                                     UIManager::m_SelectedNode;
	std::vector<std::shared_ptr<ModelController>> UIManager::m_Controllers;
	std::shared_ptr<LightController>              UIManager::m_LightController;

	std::function<void(void)>                UIManager::m_NewSceneFunction;
	std::function<void(uint32_t)>            UIManager::m_DeleteModel;
	std::function<void(const std::string&)>  UIManager::m_SetSkybox;
	std::function<void(const std::string&)>  UIManager::m_ImportModelFunction;
	std::function<void(const std::string&)>  UIManager::m_ImportSceneFunction;
	std::function<void(const std::string&)>  UIManager::m_ExportSceneFunction;

	void UIManager::Initiliaze(Window& Window)
	{
		ImGui::CreateContext( );
		ImGui::StyleColorsDark( );
		ImGui_ImplGlfw_InitForOpenGL(Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void UIManager::Draw(const uint32_t SceneId)
	{
		static bool active = true;
		ImGuiIO& io = ImGui::GetIO( );
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplGlfw_NewFrame( );
		ImGui::NewFrame( );

	#ifdef IMGUI_EXAMPLE
		ImGui::ShowDemoWindow( );
	#endif // IMGUI_EXAMPLE


		{//Docking

			// Note: Switch this to true to enable dockspace
			static bool dockspaceOpen = true;
			bool opt_fullscreen = true;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if(opt_fullscreen){
				ImGuiViewport* viewport = ImGui::GetMainViewport( );
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar( );

			if(opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

		}

		if(ImGui::BeginMenuBar( )){
			if(ImGui::BeginMenu("File")){

				if(ImGui::MenuItem("New Scene")) UIManager::m_NewSceneFunction();
				if(ImGui::BeginMenu("Import")){
					if (ImGui::MenuItem("Import Skybox")) UIManager::SetSkybox();
					if(ImGui::MenuItem("Import Object")) UIManager::ImportModel( );
					if(ImGui::MenuItem("Import Scene")) UIManager::ImportScene( );
					ImGui::EndMenu( );
				}
				if(ImGui::MenuItem("Export Scene")) UIManager::ExportScene();
				if(ImGui::MenuItem("Exit")) exit(0);
				ImGui::EndMenu( );
			}

			ImGui::EndMenuBar( );
		}

		ImGui::Begin("View");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail( );
		ImGui::Image((void*)(intptr_t)(SceneId), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End( );


		ImGui::Begin("Scene Models");
		
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax( );
		auto viewportOffset = ImGui::GetWindowPos( );
		UIManager::m_ModelPanelWidth = (int32_t)(viewportMaxRegion.x + viewportOffset.x);
		UIManager::DrawModels( );

		ImGui::End( );

	
		ImGui::Begin("Model Properties");
		UIManager::DrawProperties( );
		ImGui::End( );
		
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::Begin("Stats", (bool*)0, window_flags);
		UIManager::DrawStats( );
		ImGui::End( );
		
		ImGui::Begin("Light");
		UIManager::DrawLightProperties( );
		ImGui::End( );

		if(m_ShowPopUp == true)
			ImGui::OpenPopup("Modal window");

		if(ImGui::BeginPopupModal("Modal window", &m_ShowPopUp, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)){
			ImGui::Text(m_PopUpText.c_str());
			if(ImGui::Button("Close")){
				ImGui::CloseCurrentPopup( );
				m_ShowPopUp = false;
			}

			ImGui::EndPopup( );
		}

		ImGui::End( );// end dock

		ImGui::Render( );
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData( ));
	}

	void UIManager::AddPannel(std::shared_ptr<ModelController>& Panel)
	{
		m_Controllers.emplace_back(Panel);
		m_SelectedNode = {Panel->m_Name, Panel->m_Id};
	}

	void UIManager::DrawModels( )
	{
		for(auto& panel : m_Controllers){
			panel->Draw(m_SelectedNode);
		}
	}

	void UIManager::DrawProperties( )
	{
		if(m_Controllers.size() == 0){
			return;
		}

		static std::string oldSelectedNode = m_SelectedNode.first;
		static uint32_t oldSelectedNodeId = m_SelectedNode.second;
		static MeshProperties* selectedNodeProperties = m_Controllers[m_SelectedNode.second]->GetNodeProperties(m_SelectedNode.first);

		if(oldSelectedNode != m_SelectedNode.first || oldSelectedNodeId != m_SelectedNode.second || m_Clear == true){

			selectedNodeProperties = m_Controllers[m_SelectedNode.second]->GetNodeProperties(m_SelectedNode.first);
			oldSelectedNode = m_SelectedNode.first;

			m_Clear = false;
		}
		Transforms& selectedNodeTransforms = selectedNodeProperties->TransformMatrices;

		ImGui::Text(m_SelectedNode.first.c_str() );
		ImGui::SameLine();

		if (ImGui::Button("Delete")){

			UIManager::m_DeleteModel(m_SelectedNode.second);

			for (auto controller = m_Controllers.begin() + m_SelectedNode.second + 1; controller != m_Controllers.end(); ++controller) {
				(*controller)->m_Id--;
			}
			m_Controllers.erase(m_Controllers.begin() + m_SelectedNode.second);
			m_Clear = true;
			Renderer::DeleteId();
		}

		ImGui::Separator( );
		ImGui::DragFloat3("Translation", glm::value_ptr(selectedNodeTransforms.Translation), 0.5f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(selectedNodeTransforms.Rotation), 0.5f);
		ImGui::DragFloat3("Scale", glm::value_ptr(selectedNodeTransforms.Scale), 0.5f);
		ImGui::Separator( );
		ImGui::ColorEdit3("Tint Color", glm::value_ptr(selectedNodeProperties->TintColor));
		//To be continue
	}

	void UIManager::DrawStats( )
	{
		// get version info
		static const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
		static const GLubyte* version = glGetString(GL_VERSION); // version as a string

		ImGui::Text("Renderer: %s", renderer);
		ImGui::Text("Version: %s", version);
		ImGui::Separator( );
		ImGui::Text("Frame rate: %.1f", ImGui::GetIO( ).Framerate);
		ImGui::Text("Frame time: %.2f ms", 1000.0 / ImGui::GetIO( ).Framerate);
		ImGui::Text("Vertices: %d", m_NumVertices);
	}

	void UIManager::DrawLightProperties( )
	{
		ImGui::DragFloat3("Position", glm::value_ptr(m_LightController->GetPosition( )));
		ImGui::ColorEdit3("Color", glm::value_ptr(m_LightController->GetColor()));
	}

	void UIManager::SetSkybox()
	{
		static FolderDialog dialog;

		dialog.Open();

		std::string folderPath = dialog.GetFolderPath();

		m_SetSkybox(folderPath);
	}

	void UIManager::ImportModel( )
	{
		static FileDialog modelDialog(L"Obj (*.obj)\0*.obj\0All (*.*)\0*.*\0");

		if(modelDialog.Open( ) == true){
			std::string fileName = modelDialog.GetFilePath( );

			m_ImportModelFunction(fileName);
		}
		
	}

	void UIManager::ImportScene( )
	{
		static FileDialog modelDialog(L"Scene (*.yaml)\0*.yaml\0All (*.*)\0*.*\0");

		if(modelDialog.Open() == true){
			std::string fileName = modelDialog.GetFilePath( );

			m_ImportSceneFunction(fileName);
		}
	}

	void UIManager::ExportScene()
	{
		static FolderDialog dialog;

		dialog.Open( );

		std::string folderPath = dialog.GetFolderPath( );

		UIManager::m_ExportSceneFunction(folderPath);
	}
}
