#pragma once
#include <memory>

#include <core/Camera.h>
#include <OpenGL/Shader.h>
#include <Renderer/Model.h>

class Serializer;

class Pass{
public:
	Pass( ) = default;
	Pass(const char* Name,const char* ShaderVertexPath, const char* ShaderFragmentPath) : m_Name(Name)
	{
		m_Shader = std::make_unique<Shader>(ShaderVertexPath, ShaderFragmentPath);
	};

	virtual void Execute(const std::vector< std::shared_ptr< Model>>& Models,const Camera& ) = 0;

protected:
	std::string m_Name;
	std::unique_ptr<Shader> m_Shader;
};

