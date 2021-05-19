#include "pch.h"
#include "FolderDialog.h"

SceneEditor::FolderDialog::FolderDialog( )
{
	ZeroMemory(&m_Configuration, sizeof(m_Configuration));
	m_Configuration.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_USENEWUI;
}

void SceneEditor::FolderDialog::Open( )
{
	m_ItemList = SHBrowseForFolder(&m_Configuration);
}

std::string SceneEditor::FolderDialog::GetFolderPath( )
{
	wchar_t path[MAX_PATH] = {0};

	if(m_ItemList != 0){
		//get the name of the folder and put it in path
		SHGetPathFromIDList(m_ItemList, path);

		//free memory used
		IMalloc* imalloc = 0;
		if(SUCCEEDED(SHGetMalloc(&imalloc))){
			imalloc->Free(m_ItemList);
			imalloc->Release( );
		}
	}

	char filePathCString[256];
	size_t convertedChars = 0;

	// Put a copy of the converted string into nstring
	wcstombs_s(&convertedChars, filePathCString, 256, path, _TRUNCATE);

	return std::string{filePathCString};
}
