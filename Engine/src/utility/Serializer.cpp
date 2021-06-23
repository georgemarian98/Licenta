#include "pch.h"
#include "Serializer.h"

#include "Renderer/SkyboxPass.h"
#include "Renderer/ShadowPass.h"

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
	std::filesystem::path Serializer::m_RootFolder;

#define LIB_PATH m_RootFolder / "..\\bin\\Release\\Engine.lib"
#define SRC_PATH m_RootFolder / "..\\Engine\\src"
#define DEPENDENCY_PATH m_RootFolder / "..\\Engine\\deps\\"

	static bool endsWith(const std::string& str, const std::string& suffix)
	{
		return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
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
				m_YAMLEmitter << model->GetModelName( );
			}
		}
		m_YAMLEmitter << YAML::EndSeq; //Models paths

		m_YAMLEmitter << YAML::Key << "Models Properties" << YAML::Value << YAML::BeginSeq; // Models Properties sequence
		{
			for(auto& controller : CurrentScene->GetModels()){
				SerializeModel(controller->GetModelController());
			}
		}
		m_YAMLEmitter << YAML::EndSeq; //Models Properties sequence

		m_YAMLEmitter << YAML::Key << "Light Position" << YAML::Value << CurrentScene->m_Light->GetPosition( );
		m_YAMLEmitter << YAML::Comment("Light Properties"); 
		m_YAMLEmitter << YAML::Key << "Light Color" << YAML::Value << CurrentScene->m_Light->GetColor( );
		m_YAMLEmitter << YAML::Key << "Light Ambient" << YAML::Value << *CurrentScene->m_Light->GetAmbient( );
		m_YAMLEmitter << YAML::Key << "Light Diffuse" << YAML::Value << *CurrentScene->m_Light->GetDiffuse( );
		m_YAMLEmitter << YAML::Key << "Light Specular" << YAML::Value << *CurrentScene->m_Light->GetSpecular( );

		if (CurrentScene->m_Skybox->IsLoaded() == true) {
			m_YAMLEmitter << YAML::Key << "Skybox" << YAML::Value << YAML::BeginSeq; // Skybox cube textures path

			for (auto& path : CurrentScene->m_Skybox->GetTexturePaths()) {
				m_YAMLEmitter << path;
			}

			m_YAMLEmitter << YAML::EndSeq;
		}
		
		m_YAMLEmitter << YAML::EndMap; //Start


		if(m_YAMLEmitter.good() == false)
			std::cout << "Emitter error: " << m_YAMLEmitter.GetLastError( ) << "\n";
		
	}

	void Serializer::ExportScene(const std::filesystem::path& Path)
	{
		//Shaders
		CopyShaders(Path);

		//Lib
		std::filesystem::path libDirectory = Path / "lib";
		if(std::filesystem::exists(libDirectory) == false)
			assert(std::filesystem::create_directories(libDirectory));
		
		std::filesystem::path libPath = libDirectory / "Engine.lib";
		assert(std::filesystem::copy_file(LIB_PATH, libPath, std::filesystem::copy_options::overwrite_existing));

		//Dependency Directories
		CopyHeaders(Path);

		//Scene file
		std::filesystem::path filePath = Path / "Scene.yaml";
		std::ofstream outputFile(filePath);
		if(outputFile.is_open() == true){
			outputFile << m_YAMLEmitter.c_str( );
			outputFile.close( );
		}
	}

	std::unique_ptr<Scene> Serializer::ImportScene(const std::filesystem::path& FolderPath)
	{
		std::filesystem::path filePath = FolderPath / "Scene.yaml";

		std::unique_ptr<Scene> importedScene = std::make_unique<Scene>( );

		YAML::Node rootData;
		try	{
			rootData = YAML::LoadFile(filePath.string());
		}
		catch(const std::exception& e){
			std::cerr << e.what( );
		}		

		std::cout << "-------------IMPORT----------\n";

		auto&& models = ImportModels(FolderPath, &rootData);
		importedScene->m_SceneModels = std::move(models);

		importedScene->m_Light->SetPosition(rootData["Light Position"].as<glm::vec3>( ));
		importedScene->m_Light->SetColor(rootData["Light Color"].as<glm::vec3>( ));
		importedScene->m_Light->SetAmbient(rootData["Light Ambient"].as<float>( ));
		importedScene->m_Light->SetDiffuse(rootData["Light Diffuse"].as<float>( ));
		importedScene->m_Light->SetSpecular(rootData["Light Specular"].as<float>( ));

		Shader::m_Directory = FolderPath / "shaders";
		//uint32_t width = 1920, height = 1080;
		//std::unique_ptr<Pass> shadowPass = std::make_unique<ShadowPass>("shadow_vertex.glsl", "shadow_fragment.glsl", width, height);
		//importedScene->AddPass(shadowPass);
		std::unique_ptr<Framebuffer> aux;
		std::unique_ptr<Pass> renderPass = std::make_unique<RenderPass>("texture_vertex.glsl", "texture_fragment.glsl", aux);
		importedScene->AddPass(renderPass);

		YAML::Node skybox = rootData["Skybox"];
		if (skybox) {
			std::array<std::string, 6> cubeTexturesPaths;
			int index = 0;

			for (auto& texturePath : skybox) {
				std::string path = texturePath.as<std::string>();
				cubeTexturesPaths[index++] = path;
			}

			std::shared_ptr<SkyBox> skybox = std::make_shared<SkyBox>();
			skybox->LoadTextures(cubeTexturesPaths);
			importedScene->SetSkybox(skybox);

			std::unique_ptr<Pass> skyboxPass = std::make_unique<SkyboxPass>("skybox_vertex.glsl", "skybox_fragment.glsl" , skybox);
			importedScene->AddPass(skyboxPass);
		}


		return importedScene;
	}

	std::vector<std::shared_ptr<Model>> Serializer::ImportModels(const std::filesystem::path& ScenePath, YAML::Node* Root)
	{
		std::vector<std::shared_ptr<Model>> importedModels;
		YAML::Node root;

		if(Root == nullptr){
			try{
				root = YAML::LoadFile(ScenePath.string());
			}
			catch(const std::exception& e){
				std::cerr << e.what( );
			}
		}
		else{
			root = *Root;
		}

		YAML::Node models = root["Models"];
		assert(models);

		for(auto& model : models){
			std::string modelPath = model.as<std::string>( );

			auto&& model = std::make_shared<Model>(modelPath.c_str());
			importedModels.emplace_back(model);
		}

		YAML::Node modelsProperties = root["Models Properties"];
		assert(modelsProperties);

		int i = 0;
		for(auto& modelProperties : modelsProperties){

			MeshProperties mainProp;
			mainProp.TransformMatrices.Translation = modelProperties["Translation"].as<glm::vec3>( );
			mainProp.TransformMatrices.Scale = modelProperties["Scale"].as<glm::vec3>( );
			mainProp.TransformMatrices.Rotation = modelProperties["Rotation"].as<glm::vec3>( );
			mainProp.TintColor = modelProperties["Color"].as<glm::vec3>( );

			auto modelController = importedModels[i++]->GetModelController( );;
			modelController->m_MainTransforms = mainProp;

			YAML::Node meshes = modelProperties["Meshes"];
			if(meshes){
				for(auto& meshProperties : meshes){
					std::string meshName = meshProperties["Mesh Name"].as<std::string>( );
					MeshProperties meshProp;
					meshProp.TransformMatrices.Translation = meshProperties["Translation"].as<glm::vec3>( );
					meshProp.TransformMatrices.Scale = meshProperties["Scale"].as<glm::vec3>( );
					meshProp.TransformMatrices.Rotation = meshProperties["Rotation"].as<glm::vec3>( );
					meshProp.TintColor = meshProperties["Color"].as<glm::vec3>( );

					modelController->m_MeshControllers[meshName] = meshProp;
				}
			}
		}

		return importedModels;
	}
	
	void Serializer::SerializeModel(std::shared_ptr<ModelController> Controller)
	{
		m_YAMLEmitter << YAML::BeginMap; // Model properties
		auto& mainProperties = Controller->m_MainTransforms;

		m_YAMLEmitter << YAML::Key << "Translation" << YAML::Value << mainProperties.TransformMatrices.Translation;
		m_YAMLEmitter << YAML::Key << "Scale" << YAML::Value << mainProperties.TransformMatrices.Scale;
		m_YAMLEmitter << YAML::Key << "Rotation" << YAML::Value << mainProperties.TransformMatrices.Rotation;
		m_YAMLEmitter << YAML::Key << "Color" << YAML::Value << mainProperties.TintColor;

		m_YAMLEmitter << YAML::Key << "Meshes" << YAML::Value << YAML::BeginSeq; // Meshes sequence
		for(auto& mesh : Controller->m_MeshControllers){
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

	void Serializer::CopyShaders(const std::filesystem::path& Path)
	{
		std::filesystem::path folderDirectory = Path / "shaders";
		if(std::filesystem::exists(folderDirectory) == false)
			assert(std::filesystem::create_directories(folderDirectory));

		for (auto& path : std::filesystem::directory_iterator(SHADER_FOLDER)) {

			std::filesystem::path outputFile = folderDirectory / path.path().filename();

			assert(std::filesystem::copy_file(path, outputFile, std::filesystem::copy_options::overwrite_existing));
		}
	}

	void Serializer::CopyHeaders(const std::filesystem::path& Path)
	{
		std::filesystem::path depsPath = Path / "include";

		if (std::filesystem::exists(depsPath) == false)
			assert(std::filesystem::create_directories(depsPath));

		for(const auto& entry : std::filesystem::recursive_directory_iterator(SRC_PATH)){
			std::string path = entry.path( ).string( );
			

			if(path.find("pch") == std::string::npos && endsWith(path, ".h") == true){

				std::filesystem::path parentDirectory = depsPath / entry.path( ).parent_path( ).filename( );
				std::filesystem::path destPath = parentDirectory / entry.path( ).filename( );

				if(std::filesystem::exists(parentDirectory) == false)
					assert(std::filesystem::create_directories(parentDirectory));

				std::error_code code;
				std::filesystem::copy(path, destPath, std::filesystem::copy_options::overwrite_existing, code);

				assert(code != std::error_condition( ));
			}
		}

		static std::unordered_map<std::string, std::string> dependencyDirectories = {
			std::make_pair<std::string, std::string>("assimp\\include\\assimp", "assimp"),
			std::make_pair<std::string, std::string>("glad\\include\\glad", "glad"),
			std::make_pair<std::string, std::string>("glad\\include\\KHR", "KHR"),
			std::make_pair<std::string, std::string>("glfw\\include\\GLFW", "glfw"),
			std::make_pair<std::string, std::string>("glm\\glm", "glm"),
			std::make_pair<std::string, std::string>("yaml\\include\\yaml-cpp", "yaml-cpp"),
		};

		for(auto& directory : dependencyDirectories){

			std::error_code code;
			std::filesystem::path destPath = depsPath / directory.second;
			std::filesystem::copy(DEPENDENCY_PATH / directory.first, destPath, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive, code);

			assert(code != std::error_condition( ));
		}
	}
}
