#pragma once
#include <string>
#include <vector>

#include <assimp/scene.h>

#include "Mesh.h"

#include <Controllers/ModelController.h>

/// <summary>
/// Code inspired from: https://learnopengl.com/Model-Loading/Assimp
/// </summary>

namespace SceneEditor{

    class MeshNode;

    class Model{
    public:
        Model(const char* Path)
        {
            m_ModelView = std::make_shared<ModelController>( );
            LoadModel(Path);
        }
        void Draw(const std::unique_ptr<Shader>& ModelShader);
        std::shared_ptr<ModelController> GetModelController( ) { return m_ModelView; };
        std::string GetModelName( ) { return m_Directory + "\\" + m_ModelView->GetModelName( ); };

    private:
        void PrintTree(std::unique_ptr<MeshNode>& Node, int level);
        void DrawNodes(const std::unique_ptr<MeshNode>& Node, const std::unique_ptr<Shader>& ModelShader, MeshProperties NodeMatricies);

        void LoadModel(const std::string_view& Path);
        std::unique_ptr<MeshNode> ProcessNode(aiNode* Node, const aiScene* Scene);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* Mesh, const aiScene* Scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* Material, aiTextureType Type, const std::string_view& TypeName);

        uint32_t TextureFromFile(const char* Path);
    private:
        std::unique_ptr<MeshNode> m_RootMesh;
        std::string m_Directory;

        std::shared_ptr<ModelController> m_ModelView;
    };

    class MeshNode{
        friend Model;
    public:
        MeshNode( ) = default;

        void AddChild(std::unique_ptr<MeshNode>& Node);

    private:
        std::vector< std::unique_ptr<Mesh> > m_Meshes;
        std::vector< std::unique_ptr<MeshNode> > m_ChildrenNodes;

        std::string m_Name;
    };
}

