#include "pch.h"
#include "RenderPass.h"

namespace SceneEditor{

	void RenderPass::Execute(const std::vector< std::shared_ptr< Model>>& Models, const Camera& SceneCamera)
	{
		m_Shader->Bind( );
		m_Shader->UploadUniformMat4("projection", SceneCamera.GetPojection( ));
		m_Shader->UploadUniformMat4("view", SceneCamera.GetViewMatrix( ));

		//// render the loaded model
		for(auto& model : Models){
			model->Draw(m_Shader);
		}

		m_Shader->Unbind( );
	}
}
