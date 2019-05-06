#include "stdafx.h"
#include "LeafDirectory.h"

using namespace Leaf::IO;
Leaf::IO::CDirectory::CDirectory()
{

}

Leaf::IO::CDirectory::~CDirectory()
{

}

CString Leaf::IO::CDirectory::GetAppPath()
{
	CString  strAppPath;
	TCHAR    szModuleFileName[MAX_PATH +1];
	ZeroMemory(szModuleFileName, MAX_PATH +1);

	::GetModuleFileName(NULL, szModuleFileName, sizeof(szModuleFileName)/sizeof(TCHAR));
	strAppPath = CString(szModuleFileName);

	int nPos   = strAppPath.ReverseFind('\\');
	strAppPath = strAppPath.Left(nPos);

	return strAppPath;
}

bool Leaf::IO::CDirectory::IsFileExists(LPCTSTR lpszFilePath)
{
	if (lpszFilePath == NULL  || *lpszFilePath == '\0')
	{
		return false;
	}

	if ((_access(lpszFilePath, 0)) == -1)
	{
		return false;
	}

	return true;
}

