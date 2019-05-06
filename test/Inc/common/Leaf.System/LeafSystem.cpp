#include "stdafx.h"
#include "LeafSystem.h"

using namespace Leaf::System;
Leaf::System::CSystem::CSystem()
{

}

Leaf::System::CSystem::~CSystem()
{

}

bool Leaf::System::CSystem::DeleteSelfFile()
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