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
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);

private:
    void DrawNodes(MeshNode* Node, Shader& shader);

    void loadModel(std::string path);
    MeshNode* processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
private:
    MeshNode* m_rootMesh;
    std::vector<Texture> m_Textures_loaded;
    std::string m_Directory;
};


class MeshNode{
    friend Model;
public:
    MeshNode( ) : position(0.0f, 5.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rotation(0.0f, 5.0f, 0.0f) { };

    //MeshNode(Mesh&);

    void AddChild(MeshNode* Node);
    //void AddChild(std::shared_ptr<MeshNode> Node);

private:
    std::vector< Mesh > m_Meshes;
    std::vector< MeshNode* > m_ChildrenNodes;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    std::string name;
};
