#pragma once
#include <memory>

#include <core/Camera.h>
#include <OpenGL/Shader.h>
#include <Renderer/Model.h>

class Pass{
public:
	Pass( ) = default;
	Pass(const char* ShaderVertexPath, const char* ShaderFragmentPath) 
	{
		m_Shader = std::make_unique<Shader>(ShaderVertexPath, ShaderFragmentPath);
	};

	virtual void Execute(const std::vector< std::shared_ptr< Model>>& Models,const Camera& ) = 0;

protected:
	std::unique_ptr<Shader> m_Shader;
};

