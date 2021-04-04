#pragma once
#include <string>

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

class Mesh{
public:
    Mesh(const std::vector<Vertex>& Vertices, const std::vector<uint32_t>& Indices, const std::vector<Texture>& Textures);
    Mesh(Mesh&& Mesh) noexcept;

    void Draw(Shader& Shader);
private:
    void SetupMesh( );

private:
    uint32_t VAO, VBO, EBO;

    std::vector<Vertex>   m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::vector<Texture>  m_Textures;
};

