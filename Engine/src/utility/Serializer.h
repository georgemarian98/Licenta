#pragma once
#include "Renderer/Scene.h"
#include <yaml-cpp/yaml.h>

namespace SceneEditor{

#define SHADER_FOLDER Serializer::m_RootFolder / "..\\Engine\\shaders\\"

	class Serializer{
	public:
		static std::filesystem::path m_RootFolder;

		Serializer() = default;
		Serializer(const std::unique_ptr<Scene>& CurrentScene);

		void ExportScene(const std::filesystem::path& Path);
		static std::unique_ptr<Scene> ImportScene(const std::filesystem::path& FolderPath);
		static std::vector<std::shared_ptr<Model>> ImportModels(const std::filesystem::path& ScenePath, YAML::Node* Root = nullptr);

	private:
		void SerializeModel(std::shared_ptr<ModelController> Controller);

		void CopyShaders(const std::filesystem::path& Path);
		void CopyHeaders(const std::filesystem::path& Path);

	private:
		YAML::Emitter m_YAMLEmitter;
	};
}

