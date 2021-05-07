#pragma once
#include "Pass.h"

namespace SceneEditor{

	class RenderPass : public Pass{
	public:
		RenderPass(const char* ShaderVertexPath, const char* ShaderFragmentPath) : Pass{"Render Pass",ShaderVertexPath, ShaderFragmentPath} { };

		virtual void Execute(const std::vector< std::shared_ptr< Model>>& Models, const Camera& SceneCamera ) override;
	};
}
