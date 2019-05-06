#ifndef __LEAF_DIRECTORY_H__
#define __LEAF_DIRECTORY_H__

namespace Leaf
{
	namespace IO
	{
		class CDirectory
		{
		public:
			CDirectory();
			~CDirectory();

		public:	
			CString		GetAppPath();
			bool		IsFileExists(LPCTSTR lpszFilePath);
		};
	}
}

#endif