#include "stdafx.h"
#include "LeafSystem.h"

#include <tlhelp32.h>

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

#if 0
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
#endif
	return bRet;
}

bool Leaf::System::CSystem::SetExecPrivilege()
{
	bool bRet = false;

	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp;

#if 0
	do 
	{
		//打开当前程序的权限令牌
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
		{
			bRet = false;
			break;
		}

		//获得某一特定权限的权限标识LUID，保存在tkp中
		if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
		{
			bRet = false;
			break;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		//调用AdjustTokenPrivileges来提升我们需要的系统权限 
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);
#endif
	if (hToken != NULL)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}

	return bRet;
}

v_uint32_t Leaf::System::CSystem::GetParentProcessId(v_uint32_t uiCurProcessId)
{
	bool bRet = false;
	bool bProcess = false;

	v_uint32_t uiParentId = -1;

	HANDLE hProcess = NULL;
	PROCESSENTRY32 pe32 = {0};

	do 
	{
		hProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32);
		bProcess = ::Process32First(hProcess, &pe32);
		while (bProcess)
		{
			if (pe32.th32ProcessID == uiCurProcessId)
			{
				uiParentId = pe32.th32ParentProcessID;
				break;
			}

			bProcess = ::Process32Next(hProcess, &pe32);
		}

		bRet = true;
	} while (false);

	if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}

	return uiParentId;
}

#define ProcessBasicInformation	 0 
v_uint32_t Leaf::System::CSystem::GetParentProcessIdEx(v_uint32_t uiCurProcessId)
{
	bool bRet = false;

	v_uint32_t uiStatus = 0;
	v_uint32_t uiParentId = -1;

	HANDLE hProcess = NULL;
	PROCESS_BASIC_INFORMATION pbi = {0};
	typedef LONG(__stdcall *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);

	do 
	{
		PROCNTQSIP NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle("ntdll"), "NtQueryInformationProcess");
		if (NtQueryInformationProcess == NULL)
		{
			bRet = false;
			break;
		}

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, uiCurProcessId);
		if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		uiStatus = NtQueryInformationProcess(hProcess, ProcessBasicInformation, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
		if (uiStatus != 0)
		{
			bRet = FALSE;
			break;
		}

		uiParentId = pbi.InheritedFromUniqueProcessId;

		bRet = true;
	} while (false);

	if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}

	return uiParentId;
}

bool Leaf::System::CSystem::GetParentProcess(v_uint32_t uiCurProcessId, HANDLE &hProcess)
{
	bool bRet = false;

	v_uint32_t uiParentId = -1;

	do 
	{
		uiParentId = GetParentProcessId(uiCurProcessId);
		if (uiParentId == -1)
		{
			bRet = false;
			break;
		}

		hProcess = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, uiParentId);
		if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}

		bRet = true;
	} while (false);

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
		{//提升权限
		}

		uiFlags |= (bForce ? EWX_FORCE : EWX_FORCEIFHUNG);
		ExitWindowsEx(uiFlags, 0);

		bRet = true;
	} while (false);

	return bRet;
}
