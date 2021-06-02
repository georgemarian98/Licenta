#include "pch.h"
#include "SkyboxPass.h"

namespace SceneEditor{
	void SceneEditor::SkyboxPass::Execute(const SceneParameters& Parameter)
	{
		auto& SceneCamera = Parameter.SceneCamera;

		if (m_Skybox->IsLoaded() == false)
			return;

		glDepthFunc(GL_LEQUAL);
		m_Shader->Bind( );

		glm::mat4 transformedView = glm::mat4(glm::mat3(SceneCamera.GetViewMatrix()));
		m_Shader->UploadUniformMat4("u_View", transformedView);
		m_Shader->UploadUniformMat4("u_Projection", SceneCamera.GetPojection( ));

		m_Skybox->Draw( );
		glDepthFunc(GL_LESS);
	}
}
