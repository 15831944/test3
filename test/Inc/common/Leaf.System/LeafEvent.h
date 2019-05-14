#ifndef __LEAF_EVENT_H__
#define __LEAF_EVENT_H__

namespace Leaf
{
	namespace System
	{
		typedef struct{
			bool		bState;
			bool		bAutoReset;

			void		*m_ptAttr;
			void		*m_ptCond;
			void		*m_ptCattr;
			void		*m_ptMutex;
		}EVENT;

		typedef EVENT *HEVENT;
		class CEvent
		{
		public:
			CEvent();
			~CEvent();
			
		public:
			HEVENT			CreateEvent(bool bManualReset, bool bInitialState, const std::string &strEventName);
			bool			CloseEvent(HEVENT hEvent);
			
			v_uint32_t		WaitForEvent(HEVENT hEvent, v_uint64_t uMilliseconds);
			v_uint32_t		WaitForMultipleEvent(HEVENT *hEvents, v_uint32_t uCount, bool bWaitAll, v_uint64_t uMilliseconds);
			
			v_uint32_t		SetEvent(HEVENT hEvent);
			v_uint32_t		ResetEvent(HEVENT hEvent);
			v_uint32_t		PulseEvent(HEVENT hEvent);
		};
	}
}

#endif