#pragma once
#include <string>

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

struct Texture{
    uint32_t id;
    std::string type;
    std::string path;  
};

class Mesh{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    Mesh(Mesh&& Mesh);

    void Draw(Shader& shader);
private:
    void setupMesh( );

private:
    unsigned int VAO, VBO, EBO;

    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture>  textures;
};

