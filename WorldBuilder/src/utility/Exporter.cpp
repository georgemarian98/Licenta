#include "pch.h"
#include "Exporter.h"

Exporter::Exporter(const std::unique_ptr<Scene>& CurrentScene)
{
	m_YAMLEmitter << YAML::BeginMap;
	
	m_YAMLEmitter << YAML::Key << "TESTEST" << YAML::Value << "YAYAAYAYA";
	m_YAMLEmitter << YAML::Key << "Models" << YAML::BeginSeq; // Models
	m_YAMLEmitter << YAML::BeginMap; 
	for(auto& model : CurrentScene->m_SceneModels){
		m_YAMLEmitter << YAML::Key << "Path" << YAML::Value << model->GetModelName();
	}
	m_YAMLEmitter << YAML::EndMap; 
	m_YAMLEmitter << YAML::EndSeq; //Models

	m_YAMLEmitter << YAML::EndMap;
}

void Exporter::writeToFile(const std::string& Path)
{
	std::string filePath{Path};
	filePath += "\\test.yaml";

	std::ofstream outputFile(filePath);

	if(outputFile.is_open() == true){
		outputFile << m_YAMLEmitter.c_str( );
		outputFile.close( );
	}
}
