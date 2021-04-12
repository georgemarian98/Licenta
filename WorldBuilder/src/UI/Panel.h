#pragma once

class Panel{

public:
	virtual void Draw( ) = 0;

protected:
	std::string m_Name;
};
