#pragma once
#include <unordered_map>

namespace SceneEditor{

	class Shader{
	public:
		Shader( ) = default;
		Shader(const char* VertexShaderFileName, const char* FragmentShaderFileName);
		Shader(const char* VertexShaderFileName, const char* FragmentShaderFileName, const char* GeometryShaderFileName);
		~Shader( );

		std::string GetVertexShaderName( ) { return m_VertexName; };
		std::string GetFragmentShaderName( ) { return m_FragmentName; };
		std::string GetGeometryShaderName( ) { return m_GeometryName; };

		void Create(const char* VertexShaderFileName, const char* FragmentShaderFileName, const char* GeometryShaderFileName = nullptr);
		void Destroy( );
	
		void Bind( );
		void Unbind( );

		void UploadUniformInt(const std::string& Name, int Value);
		void UploadUniformFloat(const std::string& Name, float Value);

		void UploadUniformVec2(const std::string& Name, const glm::vec2& Value);
		void UploadUniformVec3(const std::string& Name, const glm::vec3& Value);
		void UploadUniformVec4(const std::string& Name, const glm::vec4& Value);

		void UploadUniformMat3(const std::string& Name, const glm::mat3& Matrix);
		void UploadUniformMat4(const std::string& Name, const glm::mat4& Matrix);

	private:
		void CreateShader(const char* File, GLuint& Id, GLuint Type);

		void ShaderLog(GLuint ShaderId, bool Compile = true);
		GLint GetUniformLocation(const std::string& Name);

		std::string readShaderFile(const char* FileName);

	private:
		GLuint m_ShaderProgram = 0;

		std::unordered_map<std::string, GLint> m_Uniforms;
		std::string m_VertexName;
		std::string m_FragmentName;
		std::string m_GeometryName;
	};
}