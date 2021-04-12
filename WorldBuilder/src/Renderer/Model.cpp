#include "pch.h"
#include "Model.h"

void Model::Draw(Shader& shader)
{
    Transforms matricies = m_ModelView->GetMainTransfors( );
    DrawNodes(m_RootMesh, shader, matricies); 
}


void Model::DrawNodes(const std::unique_ptr<MeshNode>& Node, Shader& shader, Transforms NodeMatricies)
{
    bool status;
    auto& [translation, scale, rotation] = m_ModelView->GetMatrices(Node->m_Name, status);

    if(status == true){
        NodeMatricies.Translation += translation;
        NodeMatricies.Scale += scale;
        NodeMatricies.Rotation += rotation;


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, NodeMatricies.Translation);
        model = glm::scale(model, NodeMatricies.Scale);
        model = glm::rotate(model, NodeMatricies.Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, NodeMatricies.Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, NodeMatricies.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        shader.UploadUniformMat4("model", model);
    }
    
    

    for(uint32_t i = 0; i < Node->m_Meshes.size( ); i++)
        Node->m_Meshes[i]->Draw(shader);

    for(uint32_t i = 0; i < Node->m_ChildrenNodes.size( ); i++){
        DrawNodes(Node->m_ChildrenNodes[i], shader, NodeMatricies);
    }
}

void Model::loadModel(const std::string_view& Path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(Path.data(), aiProcess_Triangulate |
                                                 aiProcess_GenSmoothNormals | 
                                                 aiProcess_FlipUVs | 
                                                 aiProcess_CalcTangentSpace | 
                                                 aiProcess_JoinIdenticalVertices);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString( ) << std::endl;
        return;
    }
    m_Directory = Path.substr(0, Path.find_last_of('/'));

    m_RootMesh = processNode(scene->mRootNode, scene);
    m_ModelView->SetModelName( scene->mRootNode->mName.C_Str() );

    PrintTree(m_RootMesh, 0);
}

std::unique_ptr<MeshNode> Model::processNode(aiNode* Node, const aiScene* Scene)
{
    std::unique_ptr<MeshNode> newNode = std::make_unique<MeshNode>();
    newNode->m_Name = Node->mName.C_Str( );

    for(uint32_t i = 0; i < Node->mNumMeshes; i++){
        aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
        auto& tempMesh = processMesh(mesh, Scene);

        m_ModelView->AddChild(mesh->mName.C_Str());
        newNode->m_Meshes.push_back(std::move(tempMesh));
    }

    for(uint32_t i = 0; i < Node->mNumChildren; i++){
        std::unique_ptr<MeshNode> childNode = processNode(Node->mChildren[i], Scene);
        newNode->AddChild(childNode);
    }

    return newNode;
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* ImportedMesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    vertices.reserve(ImportedMesh->mNumVertices);
    
    for(uint32_t i = 0; i < ImportedMesh->mNumVertices; i++){
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = ImportedMesh->mVertices[i].x;
        vector.y = ImportedMesh->mVertices[i].y;
        vector.z = ImportedMesh->mVertices[i].z;
        vertex.Position = vector;

        if(ImportedMesh->HasNormals( )){
            vector.x = ImportedMesh->mNormals[i].x;
            vector.y = ImportedMesh->mNormals[i].y;
            vector.z = ImportedMesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if(ImportedMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = ImportedMesh->mTextureCoords[0][i].x;
            vec.y = ImportedMesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = ImportedMesh->mTangents[i].x;
            vector.y = ImportedMesh->mTangents[i].y;
            vector.z = ImportedMesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangent
            vector.x = ImportedMesh->mBitangents[i].x;
            vector.y = ImportedMesh->mBitangents[i].y;
            vector.z = ImportedMesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    indices.reserve(3u * ImportedMesh->mNumFaces);
    for(uint32_t i = 0; i < ImportedMesh->mNumFaces; i++){
        aiFace face = ImportedMesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for(uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[ImportedMesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end( ), diffuseMaps.begin( ), diffuseMaps.end( ));

    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end( ), specularMaps.begin( ), specularMaps.end( ));

    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end( ), normalMaps.begin( ), normalMaps.end( ));

    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end( ), heightMaps.begin( ), heightMaps.end( ));

    return std::make_unique<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* Material, aiTextureType Type, const std::string_view& TypeName)
{
    static std::vector<Texture> loadedTextures;

    std::vector<Texture> textures;
    uint32_t textureCount = Material->GetTextureCount(Type);

    for(uint32_t i = 0; i < textureCount; i++){
        aiString str;
        Material->GetTexture(Type, i, &str);
        bool skip = false;

        for(auto& texture : loadedTextures){
            if(texture.path == std::string(str.C_Str( ))){
                textures.push_back(texture);
                skip = true; 
                break;
            }
        }

        if(skip == false){ 
            uint32_t id = TextureFromFile(str.C_Str( ));
            std::string type = TypeName.data();
            std::string path = str.C_Str( );

            Texture& texture = textures.emplace_back(id, type, path); 
            loadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

uint32_t  Model::TextureFromFile(const char* Path)
{
    std::string filename = std::string(Path);
    filename = m_Directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    uint8_t* data = stbi_load(filename.c_str( ), &width, &height, &nrComponents, 0);
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
