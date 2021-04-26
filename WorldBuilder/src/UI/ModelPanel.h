#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <utility>

#include "Renderer/VertexData.h"

class UIManager;
class Serializer;

class ModelPanel{
public:
    friend class UIManager;
    friend class Serializer;

    ModelPanel( ) 
    {
        static int id = 0;
        m_Id = id++;
    };

    void AddChild(const std::string& Name);
    void SetModelName(std::string& Name) { m_Name = Name; };
    void SetMainProperties(const MeshProperties& Properties) { m_MainTransforms = Properties; };

    const MeshProperties& GetNodeProperties(std::string& Name, bool& Status);
    const MeshProperties& GetMainNodeProperties( ) { return m_MainTransforms; };

    void Draw(std::pair<std::string, uint32_t>& SelectedEntity);

    std::string GetModelName( ) { return m_Name; };

private:
    void SetModelProperties(const std::string Name, const MeshProperties& Properties);

    MeshProperties& FindNodeProperties(const std::string& Name, bool& Status);
    MeshProperties* GetNodeProperties(const std::string& Name);

private:
    std::unordered_map < std::string, MeshProperties > m_Panels;
    MeshProperties m_MainTransforms;
    std::string m_Name;
    uint32_t m_Id;
};