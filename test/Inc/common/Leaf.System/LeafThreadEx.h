#ifndef __LEAF_THREADEX_H__
#define __LEAF_THREADEX_H__

#include "LeafEvent.h"

namespace Leaf
{
	namespace System
	{
		typedef enum {
			PRIORITY_IDLE = -1,
			PRIORITY_NORMAL = 0,
			PRIORITY_ABOVE_NORMAL = 1,
			PRIORITY_HIGH = 2,
		}THREAD_PRIORITY_T;

		#ifdef WIN32
			#define STACKFLAG	v_uint32_t WINAPI
			#define STACKRETURN	0
		#elif POSIX
			#define STACKFLAG	void*
			#define STACKRETURN	NULL
		#endif

		class CThreadEx
		{
		public:
			CThreadEx();
			~CThreadEx();
			
		public:
			bool				CreateThread(v_uint32_t uiCreateFlags, v_uint32_t uiStackSize, void *pSecurityAttrs);
			virtual bool		CloseThread();
			
			virtual v_int32_t	Run() = 0;
			bool				IsRunning();
			
			v_uint32_t			ResumeThread();
			v_uint32_t			SuspendThread();
			
			bool				SetThreadPriority(v_int32_t uiPriority);
			v_int32_t			GetThreadPriority();			
			
		protected:
			static STACKFLAG	_ThreadEntry(LPVOID pParam);
			
		private:
			#ifdef WIN32
				HANDLE			m_thread;
				v_uint32_t		m_thread_t;
			#elif POSIX
				pthread_t 		m_thread_t;
			#endif

			v_uint32_t			m_suspend;
		};
	}
}

#endif