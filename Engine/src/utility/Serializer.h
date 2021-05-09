#pragma once
#include "Renderer/Scene.h"
#include <yaml-cpp/yaml.h>

namespace SceneEditor{

	class Serializer{
	public:
		Serializer() = default;
		Serializer(const std::unique_ptr<Scene>& CurrentScene);

		void ExportScene(const std::string& Path);
		std::unique_ptr<Scene> ImportScene(const std::string& FolderPath);

	private:
		void SerializeModel(std::shared_ptr<ModelController> Controller);

		void CopyShaders(const std::string& Path);
		void CopyHeaders(const std::string& Path);
	private:
		YAML::Emitter m_YAMLEmitter;

		std::unordered_map<std::string, std::string> m_DependencyDirectories;;
	};
}

