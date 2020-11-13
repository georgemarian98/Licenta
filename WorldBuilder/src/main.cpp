#include "pch.h"

#include "core/Application.h"
#include "OpenGL/Errors.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main()
{

    Assimp::Importer imp;
    auto model = imp.ReadFile("D:\\Facultate\\An 3\\Grafica\\models\\nanosuit\\nanosuit.obj",
                              aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if(!model){
        std::cout << "probleme\n";
        return false;
    }

    auto app = Application::GetInstance("Licenta", 1280, 720);
    glCheckError( );

    app->Run( );
}