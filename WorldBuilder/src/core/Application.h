#pragma once
#include <memory>

#include "Window.h"

class Application{

public:
	static std::shared_ptr<Application> GetInstance(const char* Name, uint32_t Width, uint32_t Height);
	
	void Run( );
private:
	Application( ) = default;
	Application(Application& ) = delete;
	Application(const char* Name, uint32_t Width, uint32_t Height);

private:
	uint32_t m_Witdh = 0, m_Height = 0;
	Window m_Window;
};

