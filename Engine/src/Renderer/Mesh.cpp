#include "pch.h"
#include "Mesh.h"

namespace SceneEditor{

    Mesh::Mesh(std::vector<Vertex>& Vertices, std::vector<uint32_t>& Indices, std::vector<Texture>& Textures) :
        m_Vertices(std::move(Vertices)), m_Indices(std::move(Indices)), m_Textures(std::move(Textures))
    {
        m_Vertexbuffer = std::make_unique<VertexArray>(m_Vertices, m_Indices);

        // bind appropriate textures
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;
        uint32_t normalNr = 1;
        uint32_t heightNr = 1;

        size_t texturesSize = m_Textures.size();
        for (uint32_t i = 0; i < texturesSize; i++) {

            std::string name;

            switch (m_Textures[i].type)
            {
            case Diffuse:
                name = "u_TextureDiffuse" + std::to_string(diffuseNr++);
                break;
            case Specular:
                name = "u_TextureSpecular" + std::to_string(specularNr++);
                break;
            case Normal:
                name = "u_TextureNormal" + std::to_string(normalNr++);
                break;
            case Height:
                name = "u_TextureHeight" + std::to_string(heightNr++);
                break;
            }

            m_TexturesLocations[name] = m_Textures[i].id;
        }

    }

    Mesh::~Mesh()
    {
        for (Texture& texture : m_Textures) {
            glDeleteTextures(1, &texture.id);
        }
    }

    void Mesh::Draw(const std::unique_ptr<Shader>& shader)
    {    
        uint32_t i = 0;
        for(auto& [name, id] : m_TexturesLocations){

            glActiveTexture(GL_TEXTURE0 + i); 
            
            shader->UploadUniformInt(name, i);
            glBindTexture(GL_TEXTURE_2D, id);
            i++;
        }

        // draw mesh
        m_Vertexbuffer->Bind( );
        glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size( ), GL_UNSIGNED_INT, 0);
        m_Vertexbuffer->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }
}