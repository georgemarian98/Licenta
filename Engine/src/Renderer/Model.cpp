#include "pch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace SceneEditor{

    void Model::Draw(const std::unique_ptr<Shader>& ModelShader)
    {
        MeshProperties mainProperties = m_ModelView->GetModelProperties( );
        DrawNodes(m_RootMesh, ModelShader, mainProperties);
    }


    void Model::DrawNodes(const std::unique_ptr<MeshNode>& Node, const std::unique_ptr<Shader>& ModelShader, MeshProperties NodeMatricies)
    {
        bool status;
        auto& properties = m_ModelView->GetNodeProperties(Node->m_Name, status);

        if(status == true){
            auto& [translation, scale, rotation] = properties.TransformMatrices;
            auto& [Translation, Scale, Rotation] = NodeMatricies.TransformMatrices;

            Translation             += translation;
            Rotation                += rotation;
            Scale                   *= scale;
            NodeMatricies.TintColor *= properties.TintColor;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Translation);
            model = glm::scale(model, Scale);
            model = glm::rotate(model, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

            ModelShader->UploadUniformMat4("u_Model", model);
            ModelShader->UploadUniformVec3("u_TintColor", NodeMatricies.TintColor);
        }

        for(auto& mesh : Node->m_Meshes)
            mesh->Draw(ModelShader);

        for(auto& child : Node->m_ChildrenNodes)
            DrawNodes(child, ModelShader, NodeMatricies);
    }

    void Model::LoadModel(const std::filesystem::path& Path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(Path.string(), aiProcess_Triangulate      |
                                                              aiProcess_GenSmoothNormals | 
                                                              aiProcess_FlipUVs          | 
                                                              aiProcess_CalcTangentSpace | 
                                                              aiProcess_JoinIdenticalVertices);

        assert(scene && scene->mRootNode);
        m_Directory = Path.parent_path().string();

        m_RootMesh = ProcessNode(scene->mRootNode, scene);
        m_ModelView->SetModelName(std::string{scene->mRootNode->mName.C_Str( )});

        PrintTree(m_RootMesh, 0);
    }

    std::unique_ptr<MeshNode> Model::ProcessNode(aiNode* Node, const aiScene* Scene)
    {
        std::unique_ptr<MeshNode> newNode = std::make_unique<MeshNode>();
        newNode->m_Name = Node->mName.C_Str( );

        for(uint32_t i = 0; i < Node->mNumMeshes; i++){
            aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
            auto& tempMesh = ProcessMesh(mesh, Scene);

            m_ModelView->AddChild(mesh->mName.C_Str());
            newNode->m_Meshes.push_back(std::move(tempMesh));
        }

        for(uint32_t i = 0; i < Node->mNumChildren; i++){
            std::unique_ptr<MeshNode> childNode = ProcessNode(Node->mChildren[i], Scene);
            newNode->AddChild(childNode);
        }

        return newNode;
    }

    std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* ImportedMesh, const aiScene* Scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;

        vertices.reserve(ImportedMesh->mNumVertices);
        m_NoVertex += ImportedMesh->mNumVertices;

        for(uint32_t i = 0; i < ImportedMesh->mNumVertices; i++){
            Vertex vertex;

            // positions
            memcpy(glm::value_ptr(vertex.Position), &ImportedMesh->mVertices[i], 3 * sizeof(float));

            //normals
            if(ImportedMesh->HasNormals( )){
                memcpy(glm::value_ptr(vertex.Normal), &ImportedMesh->mNormals[i], 3 * sizeof(float));
            }

            if(ImportedMesh->mTextureCoords[0])
            {
                //texture coordinates
                memcpy(glm::value_ptr(vertex.TexCoords), &ImportedMesh->mTextureCoords[0][i], 2 * sizeof(float));

                // tangent
                memcpy(glm::value_ptr(vertex.Tangent), &ImportedMesh->mTangents[i], 3 * sizeof(float));

                // bitangent
                memcpy(glm::value_ptr(vertex.Bitangent), &ImportedMesh->mBitangents[i], 3 * sizeof(float));
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.emplace_back(std::move(vertex));
        }

        indices.reserve(3ull * ImportedMesh->mNumFaces);
        for(uint32_t i = 0; i < ImportedMesh->mNumFaces; i++){
            aiFace face = ImportedMesh->mFaces[i];

            indices.insert(indices.end(), &face.mIndices[0], &face.mIndices[face.mNumIndices]);
        }

        aiMaterial* material = Scene->mMaterials[ImportedMesh->mMaterialIndex];

        std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::Height);
        textures.insert(textures.end( ), heightMaps.begin( ), heightMaps.end( ));

        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
        textures.insert(textures.end( ), diffuseMaps.begin( ), diffuseMaps.end( ));

        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
        textures.insert(textures.end( ), specularMaps.begin( ), specularMaps.end( ));

        std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::Normal);
        textures.insert(textures.end( ), normalMaps.begin( ), normalMaps.end( ));


        return std::make_unique<Mesh>(vertices, indices, textures);
    }

    std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* Material, aiTextureType Type, TextureType TypeName)
    {
        static std::vector<Texture> loadedTextures;

        std::vector<Texture> textures;
        uint32_t textureCount = Material->GetTextureCount(Type);

        for(uint32_t i = 0; i < textureCount; i++){
            aiString str;
            Material->GetTexture(Type, i, &str);
            std::string texturePath(str.C_Str());

            auto& foundTexture = std::find_if(loadedTextures.begin(), loadedTextures.end(), [&texturePath](const Texture& el) {
                return el.path == texturePath;
            });

            if(foundTexture == loadedTextures.end()){
                uint32_t id = LoadTextureFromFile(texturePath);
                Texture& texture = textures.emplace_back(id, TypeName, texturePath);
                loadedTextures.push_back(texture);
            }
            else {
                textures.push_back(*foundTexture);
            }
        }
        return textures;
    }

    uint32_t  Model::LoadTextureFromFile(const std::string& Path)
    {
        std::string filename = m_Directory + '\\' + Path;

        uint32_t textureID;
     
        int width, height, nrComponents;
        uint8_t* data = stbi_load(filename.c_str( ), &width, &height, &nrComponents, 0);
        if(data != nullptr){
            GLenum format = 0;

            switch (nrComponents)
            {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                format = GL_RGBA;
                break;
            }

            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
            std::cerr << "Texture failed to load at path: " << filename << std::endl;
    

        stbi_image_free(data);

        return textureID;
    }

    void Model::PrintTree(std::unique_ptr<MeshNode>& Node, int level)
    {
        for(int i = 0; i < level; i++){
            std::cout << "   ";
        }
        std::cout << Node->m_Name << std::endl;

        for(auto& child : Node->m_ChildrenNodes){
            PrintTree(child, level + 1);
        }
    }

    ////////////////////////////  MeshNode

    void MeshNode::AddChild(std::unique_ptr<MeshNode>& ChildMesh)
    {
        m_ChildrenNodes.push_back( std::move(ChildMesh));
    }
}
