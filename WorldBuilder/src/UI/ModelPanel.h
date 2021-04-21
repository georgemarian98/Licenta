#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <utility>

#include "Renderer/VertexData.h"

class UIManager;

class ModelPanel{
public:
    friend class UIManager;

    ModelPanel( ) 
    {
        static int id = 0;
        m_Id = id++;
    };

    void AddChild(std::string Name);
    void SetModelName(std::string Name) { m_Name = Name; };
    const MeshProperties& GetNodeProperties(std::string Name, bool& Status);
    const MeshProperties& GetMainNodeProperties( ) { return m_MainTransforms; };

    void Draw(std::pair<std::string, uint32_t>& SelectedEntity);

private:
    MeshProperties& FindNodeProperties(std::string Name, bool& Status);
    MeshProperties* GetNodeProperties(std::string Name);

private:
    std::unordered_map < std::string, MeshProperties > m_Panels; //translation, scale, rotation
    MeshProperties m_MainTransforms;
    std::string m_Name;
    uint32_t m_Id;
};