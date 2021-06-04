#include "pch.h"
#include "RenderPass.h"

namespace SceneEditor{

	void RenderPass::Execute(const SceneParameters& Parameter)
	{
		auto& Models = Parameter.SceneModels;
		auto& SceneCamera = Parameter.SceneCamera;
		auto& SceneLight = Parameter.SceneLight;

		m_Shader->Bind( );
		m_Shader->UploadUniformMat4("u_Projection", SceneCamera.GetPojection( ));
		m_Shader->UploadUniformMat4("u_View", SceneCamera.GetViewMatrix( ));

		//Lighting
		m_Shader->UploadUniformVec3("u_LightProperties.position", SceneLight->GetPosition());
		m_Shader->UploadUniformVec3("u_LightProperties.color", SceneLight->GetColor());
		m_Shader->UploadUniformVec3("u_LightProperties.ambient", glm::vec3(*SceneLight->GetAmbient()));
		m_Shader->UploadUniformVec3("u_LightProperties.diffuse", glm::vec3(*SceneLight->GetDiffuse()));
		m_Shader->UploadUniformVec3("u_LightProperties.specular", glm::vec3(*SceneLight->GetSpecular()));

		m_Shader->UploadUniformVec3("u_CameraPosition", SceneCamera.GetPosition());

		//// render the loaded model
		for(auto& model : Models){
			model->Draw(m_Shader);
		}

		m_Shader->Unbind( );
	}
}
