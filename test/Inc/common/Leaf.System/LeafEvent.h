#ifndef __LEAF_EVENT_H__
#define __LEAF_EVENT_H__

#include <pthread.h>

namespace Leaf
{
	namespace System
	{
		typedef struct {
			bool		bState;
			bool		bAutoReset;
			char		szEventName[MAX_PATH];

			union {
				struct {
					void	*ptAttr;
					void	*ptCond;
					void	*ptCattr;
					void	*ptMutex;
				}Event, *pEvent;
				void	*pHandle;
			};			
		}EVENT;

		typedef EVENT *HEVENT;
		class CEvent
		{
		public:
			CEvent();
			~CEvent();
			
		public:
			bool			CreateEvent(bool bManualReset, bool bInitialState, const char *pEventName = NULL);
			void			CloseEvent();
			
			v_uint32_t		WaitForEvent(v_uint64_t uMilliseconds);
			v_uint32_t		WaitForMultipleEvent(HEVENT *hEvents, v_uint32_t uCount, bool bWaitAll, v_uint64_t uMilliseconds);
			
			v_uint32_t		SetEvent();
			v_uint32_t		ResetEvent();
			v_uint32_t		PulseEvent();

		private:
			HEVENT			m_pEvent;
		};
	}
}

#endif