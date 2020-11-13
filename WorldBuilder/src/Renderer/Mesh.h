#pragma once
#include <string>

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

struct Texture{
    uint32_t id;
    std::string type;
    std::string path;  // we store the path of the texture to compare with other textures
};

class Mesh{
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh( );
};

