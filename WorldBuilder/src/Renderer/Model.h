#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

/// <summary>
/// Code inspired from the tutorial: https://learnopengl.com/Model-Loading/Assimp
/// </summary>
class MeshNode;

class Model{
public:
    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);

private:
    void DrawNodes(const std::unique_ptr<MeshNode>& Node, Shader& shader);

    void loadModel(std::string path);
    std::unique_ptr<MeshNode> processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
private:
    std::unique_ptr<MeshNode> m_rootMesh;
    std::vector<Texture> m_Textures_loaded;
    std::string m_Directory;
};


class MeshNode{
    friend Model;
public:
    MeshNode( ) : m_Position(0.0f, 5.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f), m_Rotation(0.0f, 5.0f, 0.0f) { };


    void AddChild(std::unique_ptr<MeshNode>& Node);
    //void AddChild(std::shared_ptr<MeshNode> Node);

private:
    std::vector< std::unique_ptr<Mesh> > m_Meshes;
    std::vector< std::unique_ptr<MeshNode> > m_ChildrenNodes;

    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;

    std::string m_Name;
};
