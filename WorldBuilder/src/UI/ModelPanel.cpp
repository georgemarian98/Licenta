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

void ModelPanel::Draw(std::pair<std::string, uint32_t>& SelectedEntity)
{	
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	if(ImGui::TreeNodeEx((void*)(intptr_t)m_Id, base_flags, m_Name.c_str( ))){

		if(ImGui::IsItemClicked( ))	
			SelectedEntity = {m_Name, m_Id};

		for(auto& [key, _] : m_Panels){

			if(ImGui::Selectable(key.c_str( ), SelectedEntity.first == key)){
				SelectedEntity = {key, m_Id};
			}
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

Transforms* ModelPanel::GetMatrices(std::string Name)
{
	bool status;
	if(m_Name != Name){
		return &FindMatricies(Name, status);
	}
	else{
		status = true;
		return &m_MainTransforms;
	}
}
