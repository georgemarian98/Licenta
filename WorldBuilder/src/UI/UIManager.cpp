#include "pch.h"
#include "UIManager.h"

std::ostream& operator<<(std::ostream& g, glm::vec3 vec)
{
	g << vec.x << ", " << vec.y << ", " << vec.z << "\n";
	return g;
}

std::vector<std::shared_ptr<ModelPanel>> UIManager::m_Panels;
std::string UIManager::m_SelectedNode;

void UIManager::Initiliaze(Window& Window)
{
	ImGui::CreateContext( );
	ImGui::StyleColorsDark( );
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void UIManager::Draw(const uint32_t SceneId, bool ShowDemo)
{
	static bool active = true;
	ImGuiIO& io = ImGui::GetIO( );
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplOpenGL3_NewFrame( );
	ImGui_ImplGlfw_NewFrame( );
	ImGui::NewFrame( );

	if(ShowDemo == true){
		ImGui::ShowDemoWindow( );
	}

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

			if(ImGui::MenuItem("New Scene")) std::cout << "New Scene\n";
			if(ImGui::MenuItem("Import Object")) std::cout << "Import Object\n";
			if(ImGui::BeginMenu("Export Scene")){
				if(ImGui::MenuItem("Object")) std::cout << "obj\n";
				if(ImGui::MenuItem("Image")) std::cout << "img\n";
				ImGui::EndMenu( );
			}
			if(ImGui::MenuItem("Exit")) exit(0);
			ImGui::EndMenu( );
		}

		ImGui::EndMenuBar( );
	}

	ImGui::Begin("View");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail( );
	ImGui::Image(reinterpret_cast<void*>(SceneId), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
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
	m_SelectedNode = Panel->m_Name;
}

void UIManager::DrawModels( )
{
	for(auto& panel : m_Panels){
		panel->Draw(m_SelectedNode);
	}
}

void UIManager::DrawProperties( )
{
	static std::string oldSelectedNode = m_SelectedNode;
	static Transforms* selectedNodeMatricies = UIManager::GetNodeMatricies( );

	if(oldSelectedNode != m_SelectedNode){
		selectedNodeMatricies = UIManager::GetNodeMatricies( );
		oldSelectedNode = m_SelectedNode;
	}
	
	ImGui::Text(m_SelectedNode.c_str() );
	ImGui::Separator( );
	ImGui::DragFloat3("Translation", glm::value_ptr(selectedNodeMatricies->Translation), 0.5f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(selectedNodeMatricies->Rotation), 0.5f);
	ImGui::DragFloat3("Scale", glm::value_ptr(selectedNodeMatricies->Scale), 0.5f);
}

Transforms* UIManager::GetNodeMatricies( )
{
	bool status;
	for(auto& panel : m_Panels){
		auto selectedNodeMatricies = panel->GetMatricies(m_SelectedNode, status);
		if(status == true){
			return selectedNodeMatricies;
		}
	}

	return nullptr;
}