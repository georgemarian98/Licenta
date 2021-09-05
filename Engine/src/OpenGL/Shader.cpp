#include "pch.h"
#include "Shader.h"

namespace SceneEditor{

	std::filesystem::path Shader::m_Directory;

	std::string Shader::ReadShaderFile(const std::filesystem::path& FileName)
	{
		std::ifstream shaderFile;
		std::stringstream shaderStringStream;

		try{
			shaderFile.open(FileName);
			shaderStringStream << shaderFile.rdbuf( );
			shaderFile.close( );
		}
		catch(const std::exception&){
			std::cerr << "Shader Error: Failed to load shader: " << FileName << std::endl;
			return "ERROR";
		}

		return shaderStringStream.str( );
	}

	void Shader::CreateShader(const std::filesystem::path& File, GLuint& Id, GLuint Type)
	{
		std::string v = ReadShaderFile(File);
		const GLchar* shaderString = v.c_str( );
		Id = glCreateShader(Type);
		glShaderSource(Id, 1, &shaderString, NULL);
		glCompileShader(Id);
		ShaderLog(Id);
	}

	void Shader::Create(const std::filesystem::path& VertexShaderFileName, const std::filesystem::path& FragmentShaderFileName, const std::filesystem::path& GeometryShaderFileName)
	{
		GLuint vertexShaderID = -1;
		GLuint fragmentShaderID = -1;
		GLuint geometryShaderID = -1;
		bool enableGeometryShader = GeometryShaderFileName.empty();

		CreateShader(VertexShaderFileName, vertexShaderID, GL_VERTEX_SHADER);
		CreateShader(FragmentShaderFileName, fragmentShaderID, GL_FRAGMENT_SHADER);
		if(enableGeometryShader == true){
			CreateShader(GeometryShaderFileName, geometryShaderID, GL_GEOMETRY_SHADER);
		}

		this->m_ShaderProgram = glCreateProgram( );
		glAttachShader(this->m_ShaderProgram, vertexShaderID);
		glAttachShader(this->m_ShaderProgram, fragmentShaderID);
		if(enableGeometryShader == true){
			glAttachShader(this->m_ShaderProgram, geometryShaderID);
		}

		glLinkProgram(this->m_ShaderProgram);
		glCheckError( );

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		if(enableGeometryShader == true){
			glDeleteShader(geometryShaderID);
		}
		ShaderLog(this->m_ShaderProgram, false);

		//Populare the map with all uniforms
		int uniformCount;
		glGetProgramiv(this->m_ShaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

		int length, size;
		GLenum type;
		m_Uniforms.reserve(uniformCount);

		for(int i = 0; i < uniformCount; i++){
			char name[64];
			glGetActiveUniform(this->m_ShaderProgram, (GLuint)i, 64, &length, &size, &type, name);

			GLuint uniformLocation = glGetUniformLocation(this->m_ShaderProgram, name);

			m_Uniforms.insert(std::pair<std::string, GLuint>(name, uniformLocation));
		}

		glCheckError( );
	}

	Shader::Shader(const std::string& VertexShaderFileName, const  std::string& FragmentShaderFileName)
		: m_VertexName(VertexShaderFileName), m_FragmentName(FragmentShaderFileName)
	{
		Create(m_Directory / VertexShaderFileName, m_Directory / FragmentShaderFileName);
	}

	Shader::Shader(const std::string& VertexShaderFileName, const std::string& FragmentShaderFileName, const std::string& GeometryShaderFileName)
		: m_VertexName(VertexShaderFileName), m_FragmentName(FragmentShaderFileName), m_GeometryName(GeometryShaderFileName)
	{
		Create(m_Directory / VertexShaderFileName, m_Directory / FragmentShaderFileName, m_Directory / GeometryShaderFileName);
	}

	void Shader::Bind( )
	{
		glUseProgram(this->m_ShaderProgram);
	}

	void Shader::Unbind( )
	{
		glUseProgram(0);
	}

	GLint Shader::GetUniformLocation(const std::string& Name)
	{
		bool found =  m_Uniforms.find(Name) != m_Uniforms.end( );
		if(found == false){
			//m_Uniforms[Name] = glGetUniformLocation(m_ShaderProgram, Name.c_str( ));
			m_Uniforms[Name] = -1;
			std::cerr << Name << " :Uniform name unkown!!\n";
		}

		return m_Uniforms[Name];
	}

	void Shader::UploadUniformInt(const std::string& Name, int Value)
	{
		GLint location = GetUniformLocation(Name);
		glUniform1i(location, Value);
	}

	void Shader::UploadUniformFloat(const std::string& Name, float Value)
	{
		GLint location = GetUniformLocation(Name);
		glUniform1f(location, Value);
	}

	void Shader::UploadUniformVec2(const std::string& Name, const glm::vec2& Value)
	{
		GLint location = GetUniformLocation(Name);
		glUniform2f(location, Value.x, Value.y);
	}

	void Shader::UploadUniformVec3(const std::string& Name, const glm::vec3& Value)
	{
		GLint location = GetUniformLocation(Name);
		glUniform3f(location, Value.x, Value.y, Value.z);
	}

	void Shader::UploadUniformVec4(const std::string& Name, const glm::vec4& Value)
	{
		GLint location = GetUniformLocation(Name);
		glUniform4f(location, Value.x, Value.y, Value.z, Value.w);
	}

	void Shader::UploadUniformMat3(const std::string& Name, const glm::mat3& Matrix)
	{
		GLint location = GetUniformLocation(Name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(Matrix));
	}

	void Shader::UploadUniformMat4(const std::string& Name, const glm::mat4& Matrix)
	{
		GLint location = GetUniformLocation(Name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix));
	}

	Shader::~Shader( )
	{
		Destroy( );
	}

	void Shader::Destroy( )
	{
		Unbind( );
		glDeleteProgram(m_ShaderProgram);
		m_ShaderProgram = -1;
	}

	void Shader::ShaderLog(GLuint ShaderId, bool Compile)
	{
		GLint success;
		GLchar infoLog[512];

		//check info
		if(Compile == true){
			glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &success);
		}
		else{
			glGetProgramiv(ShaderId, GL_LINK_STATUS, &success);
		}

		if(!success){
			glGetShaderInfoLog(ShaderId, 512, NULL, infoLog);
			std::cout << "Vertex Shader: " << m_VertexName << std::endl;
			std::cout << "Fragment Shader: " << m_FragmentName << std::endl;
			std::cout << "Geometry Shader: " << m_GeometryName << std::endl;
			std::cout << "Shader compilation error\n" << infoLog << std::endl;
		}
	}
}