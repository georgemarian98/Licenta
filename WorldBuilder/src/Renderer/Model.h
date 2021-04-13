#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include <UI/ModelPanel.h>

/// <summary>
/// Code inspired from: https://learnopengl.com/Model-Loading/Assimp
/// </summary>
class MeshNode;

class Model{
public:
    Model(const char* Path)
    {
        m_ModelView = std::make_shared<ModelPanel>( );
        loadModel(Path);

    }
    void Draw(const std::unique_ptr<Shader>& shader);
    std::shared_ptr<ModelPanel> GetModelView( ) { return m_ModelView; };

private:
    void PrintTree(std::unique_ptr<MeshNode>& Node, int level);
    void DrawNodes(const std::unique_ptr<MeshNode>& Node, const std::unique_ptr<Shader>& shader, Transforms NodeMatricies);

    void loadModel(const std::string_view& Path);
    std::unique_ptr<MeshNode> processNode(aiNode* Node, const aiScene* Scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* Mesh, const aiScene* Scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* Material, aiTextureType Type, const std::string_view& TypeName);

    uint32_t TextureFromFile(const char* Path);
private:
    std::unique_ptr<MeshNode> m_RootMesh;
    std::string m_Directory;

    std::shared_ptr<ModelPanel> m_ModelView;
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