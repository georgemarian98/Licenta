#pragma once
#include <unordered_map>
#include "glm/glm.hpp"

class ModelPanel{
public:
    ModelPanel( ) = default;
    void AddChild(std::string Name);
    void SetModelName(std::string Name) { m_ModelName = Name; };
    const std::tuple<glm::vec3, glm::vec3, glm::vec3>& GetMatrices(std::string Name);

    void Draw( );
private:
    std::unordered_map < std::string, std::tuple<glm::vec3, glm::vec3, glm::vec3> > m_Panels;
    std::string m_ModelName;
};
