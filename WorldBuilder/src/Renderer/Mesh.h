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
    std::unique_ptr<VertexArray> m_Vertexbuffer;

    std::vector<Vertex>   m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::vector<Texture>  m_Textures;
};

