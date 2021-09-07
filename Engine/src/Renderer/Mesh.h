#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"

namespace SceneEditor{

    class Mesh{
    public:
        Mesh(std::vector<Vertex>& Vertices, std::vector<uint32_t>& Indices, std::vector<Texture>& Textures);
        ~Mesh();

        void Draw(const std::unique_ptr<Shader>& Shader);

    private:
        std::vector<Vertex>   m_Vertices;
        std::vector<uint32_t> m_Indices;
        std::vector<Texture>  m_Textures;

        std::unordered_map<std::string, uint32_t> m_TexturesLocations;

        std::unique_ptr<VertexArray> m_Vertexbuffer;
    };
}

