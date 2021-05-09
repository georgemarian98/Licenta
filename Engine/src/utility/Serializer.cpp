#include "pch.h"
#include "Serializer.h"
#include "UI/UIManager.h"

#include <regex>

#define VERTEX_SHADER "..\\Engine\\shaders\\vertex.glsl"
#define PIXEL_SHADER "..\\Engine\\shaders\\fragment.glsl"
#define LIB_PATH "..\\bin\\Release\\Engine.lib"
#define SRC_PATH "..\\Engine\\src"

namespace YAML{

	template<>
	struct convert<glm::vec3>	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if(!node.IsSequence( ) || node.size( ) != 3)
				return false;

			rhs.x = node[0].as<float>( );
			rhs.y = node[1].as<float>( );
			rhs.z = node[2].as<float>( );
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if(!node.IsSequence( ) || node.size( ) != 4)
				return false;

			rhs.x = node[0].as<float>( );
			rhs.y = node[1].as<float>( );
			rhs.z = node[2].as<float>( );
			rhs.w = node[3].as<float>( );
			return true;
		}
	};

}

namespace SceneEditor{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	Serializer::Serializer(const std::unique_ptr<Scene>& CurrentScene)
	{
		m_YAMLEmitter << YAML::BeginMap; //Start
	
		m_YAMLEmitter << YAML::Key << "Models" << YAML::Value <<YAML::BeginSeq; // Models paths
		{
			for(auto& model : CurrentScene->m_SceneModels){
				m_YAMLEmitter << YAML::BeginMap;
				m_YAMLEmitter << YAML::Key << "Path" << YAML::Value << model->GetModelName( );
				m_YAMLEmitter << YAML::EndMap;
			}
		}
		m_YAMLEmitter << YAML::EndSeq; //Models paths

		m_YAMLEmitter << YAML::Key << "Models Properties" << YAML::Value << YAML::BeginSeq; // Models Properties sequence
		{
			for(auto& controller : UIManager::m_Panels){
				SerializeModel(controller);
			}
		}
		m_YAMLEmitter << YAML::EndSeq; //Models Properties sequence

		m_YAMLEmitter << YAML::EndMap; //Start

		if(m_YAMLEmitter.good() == false)
			std::cout << "Emitter error: " << m_YAMLEmitter.GetLastError( ) << "\n";

		m_DependencyDirectories["..\\Engine\\deps\\assimp\\include\\assimp"] = "assimp";
		m_DependencyDirectories["..\\Engine\\deps\\glad\\include\\glad"] = "glad";
		m_DependencyDirectories["..\\Engine\\deps\\glad\\include\\KHR"] = "KHR";
		m_DependencyDirectories["..\\Engine\\deps\\glfw\\include\\GLFW"] = "glfw";
		m_DependencyDirectories["..\\Engine\\deps\\glm\\glm"] = "glm";
		m_DependencyDirectories["..\\Engine\\deps\\yaml\\include\\yaml-cpp"] = "yaml-cpp";
	}

	void Serializer::ExportScene(const std::string& Path)
	{
		std::string filePath = Path + "\\Scene.yaml";

		std::cout << std::filesystem::current_path( ) << std::endl;

		//Shaders
		CopyShaders(Path);

		//Lib
		std::string libDirectory = Path + "\\lib";
		if(std::filesystem::exists(libDirectory) == false)
			assert(std::filesystem::create_directories(libDirectory));
		
		std::string libPath = libDirectory + "\\Engine.lib";
		assert(std::filesystem::copy_file(LIB_PATH, libPath, std::filesystem::copy_options::overwrite_existing));

		//Dependency Directories
		CopyHeaders(Path);

		//Scene file
		std::ofstream outputFile(filePath);
		if(outputFile.is_open() == true){
			outputFile << m_YAMLEmitter.c_str( );
			outputFile.close( );
		}

		std::string message = "Scene exported successfully to " + Path;
		UIManager::ShowPopUp(message);
	}

	std::unique_ptr<Scene> Serializer::ImportScene(const std::string& FolderPath)
	{
		std::string filePath = FolderPath + "\\Scene.yaml";

		std::unique_ptr<Scene> importedScene = std::make_unique<Scene>( );

		YAML::Node rootData;
		try	{
			rootData = YAML::LoadFile(filePath);
		}
		catch(const std::exception& e){
			std::cerr << e.what( );
		}		

		YAML::Node models = rootData["Models"];
		if(!models)
			std::cout << "YAML corupt\n";

		std::cout << "-------------IMPORT----------\n";

		for(auto& model : models){
			std::string modelPath = model["Path"].as<std::string>( );
			UIManager::AddPannel(importedScene->AddModel(modelPath));
		}

		YAML::Node modelsProperties = rootData["Models Properties"];
		if(!modelsProperties)
			std::cout << "YAML corrupt models properties\n";
	
		int i = 0;
		for(auto& modelProperties : modelsProperties){
			std::string modelName = modelProperties["Name"].as<std::string>( );

			MeshProperties mainProp;
			mainProp.TransformMatrices.Translation = modelProperties["Translation"].as<glm::vec3>( );
			mainProp.TransformMatrices.Scale = modelProperties["Scale"].as<glm::vec3>( );
			mainProp.TransformMatrices.Rotation = modelProperties["Rotation"].as<glm::vec3>( );
			mainProp.TintColor = modelProperties["Color"].as<glm::vec3>( );

			UIManager::m_Panels[i]->m_MainTransforms = mainProp;

			YAML::Node meshes = modelProperties["Meshes"];
			if(meshes){
				for(auto& meshProperties : meshes){
					std::string meshName = meshProperties["Mesh Name"].as<std::string>( );
					MeshProperties meshProp;
					meshProp.TransformMatrices.Translation = meshProperties["Translation"].as<glm::vec3>( );
					meshProp.TransformMatrices.Scale = meshProperties["Scale"].as<glm::vec3>( );
					meshProp.TransformMatrices.Rotation = meshProperties["Rotation"].as<glm::vec3>( );
					meshProp.TintColor = meshProperties["Color"].as<glm::vec3>( );

					UIManager::m_Panels[i]->m_Panels[meshName] = meshProp;
				}
			}
			i++;
		}

		std::string vertexFile = FolderPath + "\\shaders\\vertex.glsl";
		std::string pixelFile = FolderPath + "\\shaders\\fragment.glsl";
		std::unique_ptr<Pass> renderPass = std::make_unique<RenderPass>(vertexFile.data(), pixelFile.data());
		importedScene->AddPass(renderPass);

		return importedScene;
	}
	
	void Serializer::SerializeModel(std::shared_ptr<ModelController> Controller)
	{
		m_YAMLEmitter << YAML::BeginMap; // Model properties
		m_YAMLEmitter << YAML::Key << "Name" << YAML::Value << Controller->m_Name;
		auto& mainProperties = Controller->m_MainTransforms;

		m_YAMLEmitter << YAML::Key << "Translation" << YAML::Value << mainProperties.TransformMatrices.Translation;
		m_YAMLEmitter << YAML::Key << "Scale" << YAML::Value << mainProperties.TransformMatrices.Scale;
		m_YAMLEmitter << YAML::Key << "Rotation" << YAML::Value << mainProperties.TransformMatrices.Rotation;
		m_YAMLEmitter << YAML::Key << "Color" << YAML::Value << mainProperties.TintColor;

		m_YAMLEmitter << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq; // Meshes sequence
		for(auto& mesh : Controller->m_Panels){
			m_YAMLEmitter << YAML::BeginMap; // Mesh Properties
			auto& meshProperties = mesh.second;

			m_YAMLEmitter << YAML::Key << "Mesh Name" << YAML::Value << mesh.first;
			m_YAMLEmitter << YAML::Key << "Translation" << YAML::Value << meshProperties.TransformMatrices.Translation;
			m_YAMLEmitter << YAML::Key << "Scale" << YAML::Value << meshProperties.TransformMatrices.Scale;
			m_YAMLEmitter << YAML::Key << "Rotation" << YAML::Value << meshProperties.TransformMatrices.Rotation;
			m_YAMLEmitter << YAML::Key << "Color" << YAML::Value << meshProperties.TintColor;
			m_YAMLEmitter << YAML::EndMap; // Mesh Properties
		}
		m_YAMLEmitter << YAML::EndSeq;  // Meshes sequence

		m_YAMLEmitter << YAML::EndMap; // Model properties
	}

	void Serializer::CopyShaders(const std::string& Path)
	{
		std::string folderDirectory = Path + "\\shaders";
		if(std::filesystem::exists(folderDirectory) == false)
			assert(std::filesystem::create_directories(folderDirectory));

		std::string vertexShaderPath = folderDirectory + "\\vertex.glsl";
		std::string fragmentShaderPath = folderDirectory + "\\fragment.glsl";

		assert(std::filesystem::copy_file(VERTEX_SHADER, vertexShaderPath, std::filesystem::copy_options::overwrite_existing));
		assert(std::filesystem::copy_file(PIXEL_SHADER, fragmentShaderPath, std::filesystem::copy_options::overwrite_existing));
	}
	void Serializer::CopyHeaders(const std::string& Path)
	{
		std::string depsPath = Path + "\\include\\";
		std::filesystem::create_directories(depsPath);

		const std::regex headerRegex("[^\\s]+(\\.(h|hpp))$");

		for(const auto& entry : std::filesystem::recursive_directory_iterator(SRC_PATH)){
			std::string path = entry.path( ).u8string( );

			if(path.find("pch") == std::string::npos && std::regex_match(path, headerRegex) == true){
				std::string parentDirectory = depsPath + entry.path( ).parent_path( ).filename( ).u8string( );
				std::string destPath = parentDirectory + "\\" + entry.path( ).filename( ).u8string( );

				if(std::filesystem::exists(parentDirectory) == false)
					assert(std::filesystem::create_directories(parentDirectory));

				std::error_code code;
				std::filesystem::copy(path, destPath, std::filesystem::copy_options::overwrite_existing, code);

				assert(code != std::error_condition( ));
			}
		}

		for(auto& directory : m_DependencyDirectories){

			std::error_code code;
			std::string destPath = depsPath + directory.second;
			std::filesystem::copy(directory.first, destPath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive, code);

			assert(code != std::error_condition( ));
		}
	}
}
