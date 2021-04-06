#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include <UI/ModelPanel.h>

/// <summary>
/// Code inspired from the tutorial: https://learnopengl.com/Model-Loading/Assimp
/// </summary>
class MeshNode;

class Model{
public:
    Model(const char* Path)
    {
        m_ModelView = std::make_shared<ModelPanel>( );
        loadModel(Path);

    }
    void Draw(Shader& shader);
    std::shared_ptr<ModelPanel> GetModelView( ) { return m_ModelView; };

private:
    void DrawNodes(const std::unique_ptr<MeshNode>& Node, Shader& shader);

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
    MeshNode( ) : m_Position(0.0f, 5.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f), m_Rotation(0.0f, 5.0f, 0.0f) { };

    void AddChild(std::unique_ptr<MeshNode>& Node);

private:
    std::vector< std::unique_ptr<Mesh> > m_Meshes;
    std::vector< std::unique_ptr<MeshNode> > m_ChildrenNodes;

    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

    std::string m_Name;
};