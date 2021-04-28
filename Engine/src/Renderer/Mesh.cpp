#include "pch.h"
#include "Mesh.h"

namespace SceneEditor{

    Mesh::Mesh(std::vector<Vertex>& Vertices, std::vector<uint32_t>& Indices, std::vector<Texture>& Textures) :
        m_Vertices(std::move(Vertices)), m_Indices(std::move(Indices)), m_Textures(std::move(Textures))
    {
        m_Vertexbuffer = std::make_unique<VertexArray>(m_Vertices, m_Indices);
    }

    void Mesh::Draw(const std::unique_ptr<Shader>& shader)
    {
        // bind appropriate textures
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;
        uint32_t normalNr = 1;
        uint32_t heightNr = 1;

        size_t texturesSize = m_Textures.size( );
        for(uint32_t i = 0; i < texturesSize; i++){
            glActiveTexture(GL_TEXTURE0 + i); 
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = m_Textures[i].type;

            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++); 
            else if(name == "texture_normal")
                number = std::to_string(normalNr++); 
            else if(name == "texture_height")
                number = std::to_string(heightNr++); 

            // now set the sampler to the correct texture unit
            shader->UploadUniformInt((name + number).c_str( ), i);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
        }

        // draw mesh
        m_Vertexbuffer->Bind( );
        glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size( ), GL_UNSIGNED_INT, 0);
        m_Vertexbuffer->Unbind();

        glActiveTexture(GL_TEXTURE0);
    }
}