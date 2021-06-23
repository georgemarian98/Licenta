#include "pch.h"
#include "ShadowPass.h"

namespace SceneEditor {

	ShadowPass::ShadowPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath, uint32_t& Width, uint32_t& Height) :
		Pass{ "Shadow", VertexShaderPath , FragmentShaderPath }, m_Width{ Width }, m_Height{ Height }
	{
		glGenFramebuffers(1, &m_Framebuffer);

		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);


		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};

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

