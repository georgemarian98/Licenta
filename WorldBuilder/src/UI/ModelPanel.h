#pragma once

#include "glm/glm.hpp"

#include <unordered_map>
#include <utility>

#include "Renderer/VertexData.h"

class UIManager;
struct SelectedNode;

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
    const Transforms& GetMatrices(std::string Name, bool& Status);
    const Transforms& GetMainTransfors( ) { return m_MainTransforms; };

    void Draw(std::pair<std::string, uint32_t>& SelectedEntity);

private:
    Transforms& FindMatricies(std::string Name, bool& Status);
    Transforms* GetMatrices(std::string Name);

private:
    std::unordered_map < std::string, Transforms > m_Panels; //translation, scale, rotation
    Transforms m_MainTransforms;
    std::string m_Name;
    uint32_t m_Id;
};