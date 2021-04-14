#pragma once
#include <unordered_map>
#include "glm/glm.hpp"

#include "Renderer/VertexData.h"

class UIManager;

class ModelPanel{
public:
    friend class UIManager;

    ModelPanel( ) = default;

    void AddChild(std::string Name);
    void SetModelName(std::string Name) { m_Name = Name; };
    const Transforms& GetMatrices(std::string Name, bool& Status);

    const Transforms& GetMainTransfors( ) { return m_MainTransforms; };
    void Draw(std::string& SelectedNode);

private:
    Transforms& FindMatricies(std::string Name, bool& Status);
    Transforms* GetMatricies(std::string Name, bool& Status);
private:
    std::unordered_map < std::string, Transforms > m_Panels; //translation, scale, rotation
    Transforms m_MainTransforms;
    std::string m_Name;
};
