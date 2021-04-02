#include "pch.h"
#include "Model.h"

void Model::Draw(Shader& shader)
{
    DrawNodes(m_rootMesh, shader); 
}

void Model::DrawNodes(const std::unique_ptr<MeshNode>& Node, Shader& shader)
{
    //Begin     ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
    //Get matrix from imGui
    for(uint32_t i = 0; i < Node->m_Meshes.size( ); i++)
        Node->m_Meshes[i]->Draw(shader);
    //Pop Tree

    for(uint32_t i = 0; i < Node->m_ChildrenNodes.size( ); i++){
        DrawNodes(Node->m_ChildrenNodes[i], shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | 
                                                 aiProcess_GenSmoothNormals | 
                                                 aiProcess_FlipUVs | 
                                                 aiProcess_CalcTangentSpace | 
                                                 aiProcess_JoinIdenticalVertices);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString( ) << std::endl;
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));

    m_rootMesh = processNode(scene->mRootNode, scene);
}

std::unique_ptr<MeshNode> Model::processNode(aiNode* node, const aiScene* scene)
{
    std::unique_ptr<MeshNode> newNode = std::make_unique<MeshNode>();
    newNode->m_Name = node->mName.C_Str( );

    for(uint32_t i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        auto tempMesh = processMesh(mesh, scene);

        newNode->m_Meshes.push_back( std::make_unique<Mesh>( std::move(tempMesh) ));
    }

    for(uint32_t i = 0; i < node->mNumChildren; i++){
        std::unique_ptr<MeshNode> childNode = processNode(node->mChildren[i], scene);
        newNode->AddChild(childNode);
    }

    return newNode;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    vertices.reserve(mesh->mNumVertices);
    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if(mesh->HasNormals( )){
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    indices.reserve(3u * mesh->mNumFaces);
    for(uint32_t i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for(uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end( ), diffuseMaps.begin( ), diffuseMaps.end( ));

    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end( ), specularMaps.begin( ), specularMaps.end( ));

    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end( ), normalMaps.begin( ), normalMaps.end( ));

    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end( ), heightMaps.begin( ), heightMaps.end( ));

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < m_Textures_loaded.size( ); j++){
            if(std::strcmp(m_Textures_loaded[j].path.c_str( ), str.C_Str( )) == 0){
                textures.push_back(m_Textures_loaded[j]);
                skip = true; 
                break;
            }
        }

        if(skip == false){   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str( ), m_Directory);
            texture.type = typeName;
            texture.path = str.C_Str( );
            textures.push_back(texture);
            m_Textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

unsigned int  Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str( ), &width, &height, &nrComponents, 0);
    if(data){
        GLenum format = 0;
        if(nrComponents == 1)
            format = GL_RED;
        else if(nrComponents == 3)
            format = GL_RGB;
        else if(nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

////////////////////////////  MeshNode

void MeshNode::AddChild(std::unique_ptr<MeshNode>& ChildMesh)
{
    m_ChildrenNodes.push_back( std::move(ChildMesh));
}
