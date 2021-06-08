#include "pch.h"
#include "RenderPass.h"

namespace SceneEditor{

	void RenderPass::Execute(const SceneParameters& Parameter)
	{
		auto& models = Parameter.SceneModels;
		auto& sceneCamera = Parameter.SceneCamera;
		auto& sceneLight = Parameter.SceneLight;

		m_SceneBuffer->Bind();

		m_Shader->Bind( );
		m_Shader->UploadUniformMat4("u_Projection", sceneCamera.GetPojection( ));
		m_Shader->UploadUniformMat4("u_View", sceneCamera.GetViewMatrix( ));

		//Lighting
		m_Shader->UploadUniformVec3("u_LightProperties.position", sceneLight->GetPosition());
		m_Shader->UploadUniformVec3("u_LightProperties.color", sceneLight->GetColor());
		m_Shader->UploadUniformVec3("u_LightProperties.ambient", glm::vec3(*sceneLight->GetAmbient()));
		m_Shader->UploadUniformVec3("u_LightProperties.diffuse", glm::vec3(*sceneLight->GetDiffuse()));
		m_Shader->UploadUniformVec3("u_LightProperties.specular", glm::vec3(*sceneLight->GetSpecular()));

		m_Shader->UploadUniformMat4("u_LightSpaceMatrix", sceneLight->GetSpaceMatrix());

		m_Shader->UploadUniformVec3("u_CameraPosition", sceneCamera.GetPosition());

		//Shadow
		m_Shader->UploadUniformInt("u_ShadowTexture", 31);
		glActiveTexture(GL_TEXTURE31);
		glBindTexture(GL_TEXTURE_2D, 3);

		//// render the scene
		for(auto& model : models){
			model->Draw(m_Shader);
		}

		m_Shader->Unbind( );
	}
}
