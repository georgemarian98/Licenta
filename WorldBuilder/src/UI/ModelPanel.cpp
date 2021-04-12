#include "pch.h"

#include "ModelPanel.h"

void ModelPanel::AddChild(std::string Name)
{
	m_Panels.insert({Name, 
					std::make_tuple<glm::vec3, glm::vec3, glm::vec3>(glm::vec3(0.0f, 0.0f, -15.0f), glm::vec3(1.0f), glm::vec3(0.0f))});
}

const std::tuple<glm::vec3, glm::vec3, glm::vec3>& ModelPanel::GetMatrices(std::string Name, bool& Status)
{
	Status = m_Panels.find(Name) != m_Panels.end( );
	if(Status == true){
		return  m_Panels[Name];
	}
}

void ModelPanel::Draw( ) 
{	
	ImGui::DragFloat3("Translation", glm::value_ptr(m_MainTransforms.Translation), 0.5f);
	ImGui::DragFloat3("Rotation", glm::value_ptr(m_MainTransforms.Rotation), 0.5f);
	ImGui::DragFloat3("Scale", glm::value_ptr(m_MainTransforms.Scale), 0.5f);

	if(ImGui::TreeNode(m_Name.c_str( ))){

		for(auto& [key, value] : m_Panels){

			if(ImGui::TreeNode(key.c_str( ))){
				auto& [transltion, scale, rotation] = value;
				ImGui::DragFloat3("Translation", glm::value_ptr(transltion), 0.5f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.5f);
				ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.5f);

				ImGui::TreePop( );
			}
		}
		ImGui::TreePop( );
	}
}
