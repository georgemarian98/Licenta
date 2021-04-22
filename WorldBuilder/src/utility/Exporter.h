#pragma once
#include "Renderer/Scene.h"
#include <yaml-cpp/yaml.h>

class Exporter{
public:
	Exporter(const std::unique_ptr<Scene>& CurrentScene);

	void writeToFile(const std::string& Path);

private:
	YAML::Emitter m_YAMLEmitter;
};

