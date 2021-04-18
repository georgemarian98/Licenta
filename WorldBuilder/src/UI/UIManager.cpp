#include "pch.h"
#include "UIManager.h"

//#define IMGUI_EXAMPLE

std::ostream& operator<<(std::ostream& g, glm::vec3 vec)
{
	g << vec.x << ", " << vec.y << ", " << vec.z;
	return g;
}

std::pair<std::string, uint32_t> UIManager::m_SelectedNode;
std::vector<std::shared_ptr<ModelPanel>> UIManager::m_Panels;

std::function<void(const std::string&)>  UIManager::m_ImportFunction;
std::function<void(void)>                UIManager::m_NewSceneFunction;
std::function<void(void)>                UIManager::m_ExportObjFunction;
std::function<void(void)>                UIManager::m_ExportImgFunction;

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
			if(ImGui::MenuItem("Import Object")) UIManager::ImportModel( );
			if(ImGui::BeginMenu("Export Scene")){
				if(ImGui::MenuItem("Object")) UIManager::m_ExportObjFunction( );
				if(ImGui::MenuItem("Image")) UIManager::m_ExportImgFunction( );
				ImGui::EndMenu( );
			}
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
	UIManager::DrawModels( );
	ImGui::End( );

	
	ImGui::Begin("Properties");
	UIManager::DrawProperties( );
	ImGui::End( );


	ImGui::End( );// end dock

	ImGui::Render( );
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData( ));
}

void UIManager::AddPannel(std::shared_ptr<ModelPanel>& Panel)
{
	m_Panels.emplace_back(Panel);
	m_SelectedNode = {Panel->m_Name, Panel->m_Id};
}

void UIManager::DrawModels( )
{
	for(auto& panel : m_Panels){
		panel->Draw(m_SelectedNode);
	}
}

void UIManager::DrawProperties( )
{
	static std::string oldSelectedNode = m_SelectedNode.first;
	static uint32_t oldSelectedNodeId = m_SelectedNode.second;
	static MeshProperties* selectedNodeProperties = m_Panels[m_SelectedNode.second]->GetNodeProperties(m_SelectedNode.first);

	if(oldSelectedNode != m_SelectedNode.first || oldSelectedNodeId != m_SelectedNode.second){
		selectedNodeProperties = m_Panels[m_SelectedNode.second]->GetNodeProperties(m_SelectedNode.first);;
		oldSelectedNode = m_SelectedNode.first;
	}
	Transforms& selectedNodeTransforms = selectedNodeProperties->TransformMatrices;

	ImGui::Text(m_SelectedNode.first.c_str() );
	ImGui::Separator( );
	ImGui::DragFloat3("Translation", glm::value_ptr(selectedNodeTransforms.Translation), 0.5f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(selectedNodeTransforms.Rotation), 0.5f);
	ImGui::DragFloat3("Scale", glm::value_ptr(selectedNodeTransforms.Scale), 0.5f);
	ImGui::Separator( );
	ImGui::ColorEdit3("Tint Color", glm::value_ptr(selectedNodeProperties->TintColor));
	//To be continue
}

void UIManager::ImportModel( )
{
	static OPENFILENAME ofn;       // common dialog box structure
	static bool initialized = false;

	if(initialized == false){
		char szFile[260];       // buffer for file name
		WCHAR* filter = L"Obj (*.obj)\0*.obj\0";

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = 0;
		ofn.lpstrFile = (LPWSTR)szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		initialized = true;
	}

	// Display the Open dialog box. 
	if(GetOpenFileName(&ofn) == TRUE){
		
		char* filPathCString = new char[260];
		size_t convertedChars = 0;

		// Put a copy of the converted string into nstring
		wcstombs_s(&convertedChars, filPathCString, 260, ofn.lpstrFile, _TRUNCATE);

		m_ImportFunction(filPathCString);

		delete[] filPathCString;
	}
}
