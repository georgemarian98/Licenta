#pragma once
#include <unordered_map>

namespace SceneEditor{

	class Shader{
	public:
		static std::filesystem::path m_Directory;

		Shader( ) = default;
		Shader(const std::string& VertexShaderFileName, const std::string& FragmentShaderFileName);
		Shader(const std::string& VertexShaderFileName, const std::string& FragmentShaderFileName, const std::string& GeometryShaderFileName);
		~Shader( );

		std::string GetVertexShaderName( )   { return m_VertexName; };
		std::string GetFragmentShaderName( ) { return m_FragmentName; };
		std::string GetGeometryShaderName( ) { return m_GeometryName; };

		void Create(const std::filesystem::path& VertexShaderFileName, const std::filesystem::path& FragmentShaderFileName, const std::filesystem::path& GeometryShaderFileName = "");
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
		void CreateShader(const std::filesystem::path& File, GLuint& Id, GLuint Type);

		void ShaderLog(GLuint ShaderId, bool Compile = true);
		GLint GetUniformLocation(const std::string& Name);

		std::string ReadShaderFile(const std::filesystem::path& FileName);

	private:
		GLuint m_ShaderProgram = 0;
		std::string m_VertexName;
		std::string m_FragmentName;
		std::string m_GeometryName;		

		std::unordered_map<std::string, GLint> m_Uniforms;
	};
}
