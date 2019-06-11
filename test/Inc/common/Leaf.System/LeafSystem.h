#ifndef __LEAF_SYSTEM_H__
#define __LEAF_SYSTEM_H__

namespace Leaf
{
	namespace System
	{
		typedef struct
		{
			DWORD ExitStatus;
			DWORD PebBaseAddress;
			DWORD AffinityMask;
			DWORD BasePriority;
			ULONG UniqueProcessId;
			ULONG InheritedFromUniqueProcessId;
		} PROCESS_BASIC_INFORMATION;

		class CSystem
		{
		public:
			CSystem();
			~CSystem();

		public:	
			bool		DeleteSelfFile();
			bool		SetExecPrivilege();

			v_uint32_t	GetParentProcessId(v_uint32_t uiCurProcessId);
			v_uint32_t	GetParentProcessIdEx(v_uint32_t uiCurProcessId);

			bool		GetParentProcess(v_uint32_t uiCurProcessId, HANDLE &hProcess);
			bool		SetExitComputer(v_uint32_t uiFlags, bool bForce);
		};
	}
}

#endif