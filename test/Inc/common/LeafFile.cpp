#include "stdafx.h"
#include "LeafFile.h"

using namespace Leaf::IO;
CString File::GetAppPath()
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

bool File::IsFileExists(LPCTSTR lpszFilePath)
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

bool File::DeleteSelfFile()
{
	bool bRet = false;

	CString strCmdLine;

	do 
	{
		if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
		{
			bRet = false;
			break;
		}

		if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL))
		{
			bRet = false;
			break;
		}

		SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, _pgmptr, NULL);
		strCmdLine.Format(_T("/c del /q %s"), _pgmptr);

		ShellExecute(NULL, "open", "cmd.exe", strCmdLine, NULL, SW_HIDE);
		ExitProcess(0);

		bRet = true;
	} while (false);

	return bRet;
}