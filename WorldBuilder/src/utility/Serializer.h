#pragma once
#include "Renderer/Scene.h"
#include <yaml-cpp/yaml.h>

class Serializer{
public:
	Serializer() = default;
	Serializer(const std::unique_ptr<Scene>& CurrentScene);

	void ExportScene(const std::string& Path);
	std::unique_ptr<Scene> ImportScene(const std::string& FolderPath);
private:
	YAML::Emitter m_YAMLEmitter;
};

