#include "pch.h"

#include "Test.h"
#include <iostream>

void SceneEditor::printMessage(const char* Message)
{
	std::cout << Message << std::endl;
}

void SceneEditor::TestClass::printVar( )
{
	std::cout << m_var << std::endl;
}
