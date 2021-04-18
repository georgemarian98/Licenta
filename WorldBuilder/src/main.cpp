#include "pch.h"

#include "core/Application.h"
#include "OpenGL/Errors.h"

int main()
{
    auto app = Application::GetInstance("Licenta");
    glCheckError( );

    app->Run( );

    return 0;
}