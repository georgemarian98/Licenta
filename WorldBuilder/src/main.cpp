#include "pch.h"

#include "core/Application.h"
#include "OpenGL/Errors.h"

int main()
{
    auto app = Application::GetInstance("Licenta", 1280, 720);
    glCheckError( );

    app->Run( );
}