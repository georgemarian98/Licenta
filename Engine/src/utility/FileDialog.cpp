#include "pch.h"
#include "FileDialog.h"

namespace SceneEditor{

	FileDialog::FileDialog(const WCHAR* Filter)
	{
		m_Filename = std::wstring(MAX_PATH, L'\0');

		// Initialize OPENFILENAME
		ZeroMemory(&m_Dialog, sizeof(m_Dialog));

		m_Dialog.lStructSize = sizeof(m_Dialog);
		m_Dialog.hwndOwner = 0;
		m_Dialog.lpstrFile = &m_Filename[0];
		m_Dialog.nMaxFile = MAX_PATH;
		m_Dialog.lpstrFilter = Filter;
		m_Dialog.nFilterIndex = 1;
		m_Dialog.lpstrTitle = L"Select a File";
		m_Dialog.nMaxFileTitle = 0;
		m_Dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	}

	bool FileDialog::Open( )
	{
		return GetOpenFileName(&m_Dialog);
	}

	std::string FileDialog::GetFilePath( )
	{
		char filePathCString[MAX_PATH];
		size_t convertedChars = 0;

		wcstombs_s(&convertedChars, filePathCString, MAX_PATH, m_Filename.c_str() , _TRUNCATE);

		return std::string{filePathCString};
	}
}
