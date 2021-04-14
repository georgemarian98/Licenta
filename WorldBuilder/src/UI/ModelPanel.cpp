#include "pch.h"

#include "ModelPanel.h"

void ModelPanel::AddChild(std::string Name)
{
	m_Panels.insert({Name, Transforms(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(1.0f), glm::vec3(0.0f))});
}

const Transforms& ModelPanel::GetMatrices(std::string Name, bool& Status)
{
	return FindMatricies(Name, Status);
}

void ModelPanel::Draw(std::string& SelectedNode)
{	
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	static int index = rand( );

	if(ImGui::TreeNodeEx((void*)(intptr_t)index, base_flags, m_Name.c_str( ))){
	//if(ImGui::TreeNode(m_Name.c_str( ))){
		if(ImGui::IsItemClicked( ))	
			SelectedNode = m_Name;

		for(auto& [key, value] : m_Panels){

			if(ImGui::Selectable(key.c_str( ), SelectedNode == key))
				SelectedNode = key;
		}
		ImGui::TreePop( );
	}
}

Transforms& ModelPanel::FindMatricies(std::string Name, bool& Status) 
{
	Status = m_Panels.find(Name) != m_Panels.end( );
	if(Status == true){
		return  m_Panels[Name];
	}

	return Transforms( );
}

Transforms* ModelPanel::GetMatricies(std::string Name, bool& Status)
{
	if(m_Name != Name){
		return &FindMatricies(Name,Status);
	}
	else{
		Status = true;
		return &m_MainTransforms;
	}
}
