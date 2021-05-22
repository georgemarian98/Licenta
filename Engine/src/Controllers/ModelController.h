#pragma once

#include <unordered_map>
#include <utility>

#include "Renderer/VertexData.h"
#include "Renderer/Renderer.h"

namespace SceneEditor{
    class UIManager;
    class Serializer;
    class Model;

    using Component = std::pair<std::string, uint32_t>;

    class ModelController{
    public:
        friend class UIManager;
        friend class Serializer;
        friend class Model;

        ModelController( ) 
        {
            m_Id = Renderer::GenerateID( );
        };

        void SetModelName(std::string& Name) { m_Name = Name; };
        void SetMainProperties(const MeshProperties& Properties) { m_MainTransforms = Properties; };

        const MeshProperties& GetNodeProperties(std::string& Name, bool& Status);
        MeshProperties& GetModelProperties( ) { return m_MainTransforms; };

        std::string GetModelName( ) { return m_Name; };

        void Draw(Component& SelectedEntity);

    private:
        void AddChild(const std::string& Name);

        MeshProperties& FindNodeProperties(const std::string& Name, bool& Status);
        MeshProperties* GetNodeProperties(const std::string& Name);

    private:
        uint32_t m_Id;
        std::string m_Name;
        MeshProperties m_MainTransforms;
        std::unordered_map < std::string, MeshProperties > m_MeshControllers;
    };

}