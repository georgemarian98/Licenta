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
		void MoveFiles(const std::string& InputFile, const std::string& OutputPath);
	private:
		YAML::Emitter m_YAMLEmitter;
	};
}

