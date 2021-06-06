#include "pch.h"

#include "core/Application.h"
#include "OpenGL/Errors.h"

#include "utility/Serializer.h"

int main( )
{
    SceneEditor::Serializer::m_RootFolder = std::filesystem::current_path().string();

    auto app = SceneEditor::Application::GetInstance("Licenta");
    glCheckError( );

    app->Run( );

    return 0;
}