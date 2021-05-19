#include "pch.h"

#include "ModelController.h"

namespace SceneEditor{

	void ModelController::AddChild(const std::string& Name)
	{
		m_MeshControllers.insert({Name, Transforms()});
	}

	const MeshProperties& ModelController::GetNodeProperties(std::string& Name, bool& Status)
	{
		return FindNodeProperties(Name, Status);
	}

	void ModelController::Draw(std::pair<std::string, uint32_t>& SelectedEntity)
	{	
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if(ImGui::TreeNodeEx((void*)(intptr_t)m_Id, base_flags, m_Name.c_str( ))){

			if(ImGui::IsItemClicked( ))	
				SelectedEntity = {m_Name, m_Id};

			for(auto& [key, _] : m_MeshControllers){
			
				if(ImGui::Selectable(key.c_str( ), SelectedEntity.first == key)){
					SelectedEntity = {key, m_Id};
				}
			}
			ImGui::TreePop( );
		}
	}

	MeshProperties& ModelController::FindNodeProperties(const std::string& Name, bool& Status)
	{
		Status = m_MeshControllers.find(Name) != m_MeshControllers.end( );
		if(Status == true){
			return  m_MeshControllers[Name];
		}

		static MeshProperties nullTransf;
		return nullTransf;
	}

	MeshProperties* ModelController::GetNodeProperties(const std::string& Name)
	{
		bool status;
		if(m_Name != Name){
			return &FindNodeProperties(Name, status);
		}
		else
			return &m_MainTransforms;
	
	}
}
