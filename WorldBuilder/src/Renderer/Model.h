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
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
private:
    // model data
    std::vector<Mesh> m_Meshes;
    std::vector<Texture> m_Textures_loaded;
    std::string m_Directory;
};


class MeshNode{

public:
    MeshNode( ) = default;

    MeshNode(Mesh&);

    void AddChild(std::shared_ptr<MeshNode> Node);
private:
    std::shared_ptr<Mesh> m_Mesh;
    std::vector< std::shared_ptr<MeshNode>> m_ChildrenNodes;

    glm::mat4 translate;
};
