#include "pch.h"
#include "SkyboxPass.h"

namespace SceneEditor{
	void SceneEditor::SkyboxPass::Execute(const SceneParameters& Parameter)
	{
		auto& SceneCamera = Parameter.SceneCamera;

		glDepthFunc(GL_LEQUAL);
		m_Shader->Bind( );

		glm::mat4 transformedView = glm::mat4(glm::mat3(SceneCamera.GetViewMatrix()));
		m_Shader->UploadUniformMat4("view", transformedView);
		m_Shader->UploadUniformMat4("projection", SceneCamera.GetPojection( ));

		m_Skybox.Bind( );
		m_Skybox.Draw( );
		m_Skybox.Unbind( );
		glDepthFunc(GL_LESS);
	}
}
