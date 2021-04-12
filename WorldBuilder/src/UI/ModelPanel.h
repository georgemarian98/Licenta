#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "UI/Panel.h"
#include "Renderer/VertexData.h"

class ModelPanel : public Panel{
public:
    ModelPanel( ) = default;

    void AddChild(std::string Name);
    void SetModelName(std::string Name) { m_Name = Name; };
    const std::tuple<glm::vec3, glm::vec3, glm::vec3>& GetMatrices(std::string Name, bool& Status);

    Transforms GetMainTransfors( ) { return m_MainTransforms; };
    void Draw( ) override;
private:
    std::unordered_map < std::string, std::tuple<glm::vec3, glm::vec3, glm::vec3> > m_Panels; //translation, scale, rotation
    Transforms m_MainTransforms;
};
