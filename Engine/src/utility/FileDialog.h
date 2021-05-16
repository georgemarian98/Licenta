#pragma once
//#include <ShlObj.h>

namespace SceneEditor{

	class FileDialog{
	public:
		FileDialog( ) = default;
		FileDialog(const WCHAR* Filter);

		bool Open( );

		std::wstring GetFile( );

	private:
		std::wstring m_Filename;
		OPENFILENAME m_Dialog;
	};
}


