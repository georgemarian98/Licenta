#include "pch.h"

#include "core/Application.h"
#include "OpenGL/Errors.h"

#include <string>
#include <string_view>

int main()
{
    auto app = Application::GetInstance("Licenta", 1280, 720);
    glCheckError( );

    app->Run( );
}