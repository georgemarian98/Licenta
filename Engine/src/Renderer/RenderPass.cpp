#include "pch.h"
#include "RenderPass.h"

namespace SceneEditor{

	RenderPass::RenderPass(const std::string& VertexShaderPath, const std::string& FragmentShaderPath, std::unique_ptr<Framebuffer>& Buffer) : Pass{ "Render", VertexShaderPath, FragmentShaderPath },
		m_SceneBuffer{ Buffer }
	{
		if (m_SceneBuffer == nullptr) {
			PLsss = false;
		}
		m_Shadow = std::make_unique<Shader>("shadow_vertex.glsl", "shadow_fragment.glsl");
		glGenFramebuffers(1, &m_Framebuffer);

		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1920, 1080, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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

	void RenderPass::Execute(const SceneParameters& Parameter)
	{
		auto& sceneModels = Parameter.SceneModels;
		auto& sceneCamera = Parameter.SceneCamera;
		auto& sceneLight = Parameter.SceneLight;

#if 1
		//Shadow
		glViewport(0, 0, 1920, 1080);
		glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		m_Shadow->Bind();
		m_Shadow->UploadUniformMat4("u_LightSpaceMatrix", sceneLight->GetSpaceMatrix());

		//glCullFace(GL_FRONT);

		for (auto& model : sceneModels) {
			model->Draw(m_Shadow);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 1920, 1080);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_BACK);

		m_Shadow->Unbind();

		if( PLsss == true)
			m_SceneBuffer->Bind();


#endif

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
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

		//// render the scene
		for(auto& model : sceneModels){
			model->Draw(m_Shader);
		}

		m_Shader->Unbind( );
	}
}
