#pragma once

namespace SceneEditor{
	void printMessage(const char* Message);

	class TestClass{
	public:
		void printVar( );
		void setVar(int var) { m_var = var; };
	private:
		int m_var;

	};
}