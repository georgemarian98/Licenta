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
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                         std::string typeName);

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;

};

