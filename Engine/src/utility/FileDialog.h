#pragma once

namespace SceneEditor{

	class FileDialog{
	public:
		FileDialog( ) = default;
		FileDialog(const WCHAR* Filter);

		bool Open( );

		std::string GetFilePath( );

	private:
		std::wstring m_Filename;
		OPENFILENAME m_Dialog;
	};
}


