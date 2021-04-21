#pragma once
#include <string>

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

class Mesh{
public:
    Mesh(std::vector<Vertex>& Vertices, std::vector<uint32_t>& Indices, std::vector<Texture>& Textures);

    void Draw(const std::unique_ptr<Shader>& Shader);

private:
    std::unique_ptr<VertexArray> m_Vertexbuffer;

    std::vector<Vertex>   m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::vector<Texture>  m_Textures;
};

