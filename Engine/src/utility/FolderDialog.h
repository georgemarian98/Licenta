#pragma once

#include <ShlObj.h>

namespace SceneEditor{

	class FolderDialog{
	public:
		FolderDialog( );

		void Open( );

		std::string GetFolderPath( );
	private:
		BROWSEINFO m_Configuration;
		LPITEMIDLIST m_ItemList = {};
	};
}

