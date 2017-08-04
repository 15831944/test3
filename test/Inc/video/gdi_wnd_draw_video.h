#ifndef __GDI_WND_DRAW_VIDEO_H__
#define __GDI_WND_DRAW_VIDEO_H__

#include "../algorithm/thread_queue.h"

typedef struct tagFrame_data_buffer{
	unsigned char*	pFrameData;
	unsigned long	uFrameDataLen;
	unsigned long	ulBitCount;
	unsigned long   uPixelWidth;
	unsigned long   uPixelHeight;
}FRAME_DATA_BUFFER;

class gdi_wnd_draw_video
{
public:
	gdi_wnd_draw_video();
	~gdi_wnd_draw_video();
	
public:
	static gdi_wnd_draw_video& Instance();
	static DWORD WINAPI DrawSceneThreadProc(LPVOID lpParam);

	BOOL					CreateVideoProc(CRect* pWndRect, HWND hWnd);	
	BOOL					CloseVideoProc();

	BOOL					SetProcState();
	void					SetProcTimeOver(int nProcTime, int nCloseTime = 500);
	BOOL					SetFrameData(const unsigned char* pFrameData, unsigned long ulDataLen, unsigned long ulVideoWidth, unsigned long ulVideoHeight, unsigned long ulBitCount = 24);
	
protected:
	BOOL					drawscene();
	BOOL					getframedata();
	
	void					displayvideo();
	
protected:
	HANDLE					m_hThread;
	
	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;
	
	HDC						m_hDC;
	HWND					m_hWnd;	
	CRect					m_rect;
	
private:
	BOOL					m_bExit;

	int						m_nProcTimeOver;
	int						m_nCloseTimeOver;
	int						m_nBufferLen;
	
	int						m_nBitCount;
	int						m_nPixelWidth;
	int						m_nPixelHeight;

	int						m_nWndWidth;
	int						m_nWndHeight;
	
	DWORD					m_dwThreadID;
	
	unsigned char*			m_pBuffer;
	thread_queue<FRAME_DATA_BUFFER*> m_threadFrameQueue;
};

#endif