#include "pch.h"
#include "Serializer.h"
#include "UI/UIManager.h"

std::ostream& operator<<(std::ostream& g, glm::vec3 vec)
{
	g << vec.x << ", " << vec.y << ", " << vec.z;
	return g;
}

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
		for(auto& panel : UIManager::m_Panels){
			m_YAMLEmitter << YAML::BeginMap; // Model properties
			m_YAMLEmitter << YAML::Key << "Name" << YAML::Value << panel->m_Name; 
			auto& mainProperties = panel->m_MainTransforms;

			m_YAMLEmitter << YAML::Key << "Translation" << YAML::Value << mainProperties.TransformMatrices.Translation;
			m_YAMLEmitter << YAML::Key << "Scale"       << YAML::Value << mainProperties.TransformMatrices.Scale;
			m_YAMLEmitter << YAML::Key << "Rotation"    << YAML::Value << mainProperties.TransformMatrices.Rotation;
			m_YAMLEmitter << YAML::Key << "Color"       << YAML::Value << mainProperties.TintColor;

			m_YAMLEmitter << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq; // Meshes sequence
			for(auto& mesh : panel->m_Panels){
				m_YAMLEmitter << YAML::BeginMap; // Mesh Properties
				auto& meshProperties = mesh.second;

				m_YAMLEmitter << YAML::Key << "Mesh Name"   << YAML::Value << mesh.first;
				m_YAMLEmitter << YAML::Key << "Translation" << YAML::Value << meshProperties.TransformMatrices.Translation;
				m_YAMLEmitter << YAML::Key << "Scale"       << YAML::Value << meshProperties.TransformMatrices.Translation;
				m_YAMLEmitter << YAML::Key << "Rotation"    << YAML::Value << meshProperties.TransformMatrices.Translation;
				m_YAMLEmitter << YAML::Key << "Color"       << YAML::Value << meshProperties.TintColor;
				m_YAMLEmitter << YAML::EndMap; // Mesh Properties
			}
			m_YAMLEmitter << YAML::EndSeq;  // Meshes sequence

			m_YAMLEmitter << YAML::EndMap; // Model properties
		}
	}
	m_YAMLEmitter << YAML::EndSeq; //Models Properties sequence

	m_YAMLEmitter << YAML::EndMap; //Start

	if(m_YAMLEmitter.good() == false)
		std::cout << "Emitter error: " << m_YAMLEmitter.GetLastError( ) << "\n";

}

void Serializer::ExportScene(const std::string& Path)
{
	std::string filePath{Path};
	filePath += "\\Scene.yaml";

	//copy shaders in this folder

	std::ofstream outputFile(filePath);

	if(outputFile.is_open() == true){
		outputFile << m_YAMLEmitter.c_str( );
		outputFile.close( );
	}
}

std::unique_ptr<Scene> Serializer::ImportScene(const std::string& FolderPath)
{
	std::string filePath{FolderPath};
	filePath += "\\Scene.yaml";

	std::unique_ptr<Scene> importedScene = std::make_unique<Scene>( );
	YAML::Node rootData = YAML::LoadFile(filePath);

	YAML::Node models = rootData["Models"];
	if(!models)
		std::cout << "YAML corupt\n";

	std::cout << "-------------IMPORT----------\n";

	for(auto& model : models){
		std::string modelPath = model["Path"].as<std::string>( );
		std::cout << modelPath << std::endl;
	}
	std::cout << "-----------------------\n";

	YAML::Node modelsProperties = rootData["Models Properties"];
	if(!modelsProperties)
		std::cout << "YAML corrupt models properties\n";
	
	for(auto& modelProperties : modelsProperties){
		std::string modelName = modelProperties["Name"].as<std::string>( );
		glm::vec3 translation = modelProperties["Translation"].as<glm::vec3>( );
		glm::vec3 scale = modelProperties["Scale"].as<glm::vec3>( );
		glm::vec3 rotation = modelProperties["Rotation"].as<glm::vec3>( );
		glm::vec3 color = modelProperties["Color"].as<glm::vec3>( );

		std::cout << "Name: " << modelName << std::endl;
		std::cout << "Translation: " << translation << std::endl;
		std::cout << "Scale: " << scale << std::endl;
		std::cout << "Rotation: " << rotation << std::endl;
		std::cout << "Color: " << color << std::endl;

		YAML::Node meshes = modelProperties["Meshes"];
		if(meshes){
			for(auto& meshProperties : meshes){
				std::string meshName = meshProperties["Mesh Name"].as<std::string>( );
				glm::vec3 meshtranslation = meshProperties["Translation"].as<glm::vec3>( );
				glm::vec3 meshscale = meshProperties["Scale"].as<glm::vec3>( );
				glm::vec3 meshrotation = meshProperties["Rotation"].as<glm::vec3>( );
				glm::vec3 meshcolor = meshProperties["Color"].as<glm::vec3>( );

				std::cout << "Mesh Name: " << meshName << std::endl;
				std::cout << "Mesh Translation: " << meshtranslation << std::endl;
				std::cout << "Mesh Scale: " << meshscale << std::endl;
				std::cout << "Mesh Rotation: " << meshrotation << std::endl;
				std::cout << "Mesh Color: " << meshcolor << std::endl;
			}
		}
		std::cout << "-----------------------\n";
	}
	return importedScene;
}
