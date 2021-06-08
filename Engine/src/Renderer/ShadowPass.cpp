#include "pch.h"
#include "ShadowPass.h"

namespace SceneEditor {

	void SceneEditor::ShadowPass::Execute(const SceneParameters& Parameters)
	{
		auto& sceneModels = Parameters.SceneModels;
		auto& sceneLight = Parameters.SceneLight;


		glViewport(0, 0, m_Width, m_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		m_Shader->Bind();
		m_Shader->UploadUniformMat4("u_LightSpaceMatrix", sceneLight->GetSpaceMatrix());

		glCullFace(GL_FRONT);

		for (auto& model : sceneModels) {
			model->Draw(m_Shader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, m_Width, m_Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);

		m_Shader->Unbind();
	}
}

