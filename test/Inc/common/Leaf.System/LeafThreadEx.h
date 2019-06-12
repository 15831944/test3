#ifndef __LEAF_THREADEX_H__
#define __LEAF_THREADEX_H__

#include "LeafEvent.h"

namespace Leaf
{
	namespace System
	{
		#ifdef WIN32
			#define STACKFLAG	v_uint32_t WINAPI
			#define STACKRETURN	0
		#elif POSIX
			#define STACKFLAG	void*
			#define STACKRETURN	NULL
		#endif

		typedef enum {
			PRIORITY_IDLE = -1,
			PRIORITY_NORMAL = 0,
			PRIORITY_ABOVE_NORMAL = 1,
			PRIORITY_HIGH = 2,
		}THREAD_PRIORITY_T;

		typedef enum {
			FLAGS_NORMAL =  0,
			FLAGS_SUSPENDED = 1,
		}THREAD_FLAGS_T;

		class CThreadEx
		{
		public:
			CThreadEx();
			~CThreadEx();
			
		public:
			virtual	bool		IsRunning()	{ return m_suspend == 0; }

			bool				CreateThread(THREAD_FLAGS_T emThreadFlag, v_uint32_t uiStackSize, void *pSecurityAttrs = NULL);	//THREAD_PRIORITY_T emThreadPriority	//LPSECURITY_ATTRIBUTES
			virtual bool		CloseThread();

			virtual	bool		InitInstance() = 0;
			virtual bool		ExitInstance() = 0;
			virtual v_int32_t	Run() = 0;
			
			v_uint32_t			ResumeThread();
			v_uint32_t			SuspendThread();
			
			bool				SetThreadPriority(THREAD_PRIORITY_T emPriority);
			THREAD_PRIORITY_T	GetThreadPriority();			
			
		protected:
			static STACKFLAG	_ThreadEntry(LPVOID pParam);
			
		protected:
			CEvent				m_event;

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