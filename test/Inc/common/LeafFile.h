#ifndef __LEAF_FILE_H__
#define __LEAF_FILE_H__

namespace Leaf
{
	namespace IO
	{
		class File
		{
		public:
			CString		GetAppPath();

			bool		IsFileExists(LPCTSTR lpszFilePath);
			bool		DeleteSelfFile();
		};
	}
}

#endif