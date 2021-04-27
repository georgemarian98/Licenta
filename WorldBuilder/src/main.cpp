#include "pch.h"

#include "core/Application.h"
#include "OpenGL/Errors.h"

#include "Test.h"

int main()
{
    //auto app = SceneEditor::Application::GetInstance("Licenta");
    //glCheckError( );

    //app->Run( );


    SceneEditor::TestClass test;
    test.setVar(54353453);
    test.printVar( );
    return 0;
}