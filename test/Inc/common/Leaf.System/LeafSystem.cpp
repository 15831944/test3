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

bool Leaf::System::CSystem::SetExecPrivilege()
{
	bool bRet = false;

	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp;

	do 
	{
		//�򿪵�ǰ�����Ȩ������
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
		{
			bRet = false;
			break;
		}

		//���ĳһ�ض�Ȩ�޵�Ȩ�ޱ�ʶLUID��������tkp��
		if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
		{
			bRet = false;
			break;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		//����AdjustTokenPrivileges������������Ҫ��ϵͳȨ�� 
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

	if (hToken != NULL)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}

	return bRet;
}

bool Leaf::System::CSystem::SetExitComputer(v_uint32_t uiFlags, bool bForce)
{
	bool bRet = false;
	
	OSVERSIONINFO osvi={0};

	do 
	{
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(&osvi))
		{
			bRet = false;
			break;
		}

		if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{//����Ȩ��
		}

		uiFlags |= (bForce ? EWX_FORCE : EWX_FORCEIFHUNG);
		ExitWindowsEx(uiFlags, 0);

		bRet = true;
	} while (false);

	return bRet;
}