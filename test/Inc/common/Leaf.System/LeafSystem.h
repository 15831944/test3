#ifndef __LEAF_SYSTEM_H__
#define __LEAF_SYSTEM_H__

namespace Leaf
{
	namespace System
	{
		class CSystem
		{
		public:
			CSystem();
			~CSystem();

		public:	
			bool		DeleteSelfFile();
			bool		SetExecPrivilege();

			bool		SetExitComputer(v_uint32_t uiFlags, bool bForce);
		};
	}
}

#endif