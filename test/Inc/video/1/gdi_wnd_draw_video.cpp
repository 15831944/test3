#include "stdafx.h"
#include "gdi_wnd_draw_video.h"

/************************************************************************/
/* author : wl
 * email  : lysgwl@163.com
 * date   : 2017.08.02 11:34
 */
/************************************************************************/

gdi_wnd_draw_video::gdi_wnd_draw_video()
{
	m_hDC = NULL;
	m_hWnd = NULL;
	
	m_bExit = FALSE;
	
	m_nProcTimeOver = 80;
	m_nCloseTimeOver = 500;
	
	m_nBufferLen = 0;
	m_dwThreadID = 0;
	
	m_nWndWidth	 = 0;
	m_nWndHeight = 0;
	
	m_nBitCount = 0;
	m_nPixelWidth = 0;
	m_nPixelHeight = 0;
	
	m_hThread = NULL;
	m_pBuffer = NULL;
	m_rect.SetRectEmpty();
	
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

gdi_wnd_draw_video::~gdi_wnd_draw_video()
{
	if (!CloseVideoProc())
	{
		return;
	}
}

gdi_wnd_draw_video& gdi_wnd_draw_video::Instance()
{
	static gdi_wnd_draw_video inst;
	return inst;
}

DWORD gdi_wnd_draw_video::DrawSceneThreadProc(LPVOID lpParam)
{
	gdi_wnd_draw_video* pDrawSceneProc = (gdi_wnd_draw_video*)lpParam;
	if (pDrawSceneProc != NULL)
	{
		pDrawSceneProc->displayvideo();
	}
	
	return 0;
}

void gdi_wnd_draw_video::displayvideo()
{
	if (m_hWnd == NULL)
	{
		return;
	}

	if (m_hDC == NULL)
	{
		return;
	}

	while(WaitForSingleObject(m_hEndEvent, m_nProcTimeOver) != WAIT_OBJECT_0)
	{
		if (!m_bExit)
		{
			if (!drawscene())
			{
				m_nProcTimeOver = 500;
				continue;
			}
		}
		else
		{
			SetEvent(m_hEndEvent);
		}
	}
}

BOOL gdi_wnd_draw_video::CreateVideoProc(CRect* pWndRect, HWND hWnd)
{
	BOOL bRet = FALSE;
	HDC hDC = NULL;
	
	if(pWndRect == NULL || pWndRect->IsRectEmpty())
	{
		return FALSE;
	}
	
	if(hWnd == NULL)
	{
		return FALSE;
	}
	
	hDC = ::GetDC(hWnd);
	if (hDC == NULL)
	{
		return FALSE;
	}

	if (m_hThread != NULL)
	{
		return FALSE;
	}
	
	if(WaitForSingleObject(m_hStartEvent, 0) != WAIT_OBJECT_0)
	{
		SetEvent(m_hStartEvent);
		ResetEvent(m_hEndEvent);
		
		m_hDC = hDC;
		m_hWnd = hWnd;
		
		m_rect.CopyRect(pWndRect);
		
		m_hThread = CreateThread(NULL, 0, DrawSceneThreadProc, (LPVOID)this, CREATE_SUSPENDED, &m_dwThreadID);
		if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		{
			ResetEvent(m_hStartEvent);
			return FALSE;
		}

		bRet = TRUE;
		m_bExit = FALSE;
	}
	
	return bRet;
}

BOOL gdi_wnd_draw_video::CloseVideoProc()
{
	m_bExit = TRUE;

	WaitForSingleObject(m_hEndEvent, m_nCloseTimeOver);
	
	if (m_hDC != NULL)
	{
		::ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	
	while(!m_threadFrameQueue.IsEmpty())
	{
		FRAME_DATA_BUFFER* pFrameBuffer = (FRAME_DATA_BUFFER*)m_threadFrameQueue.Front();
		if (pFrameBuffer != NULL)
		{
			if (pFrameBuffer->pFrameData != NULL)
			{
				delete[] pFrameBuffer->pFrameData;
				pFrameBuffer->pFrameData = NULL;
			}

			delete pFrameBuffer;
			pFrameBuffer = NULL;
		}

		m_threadFrameQueue.Pop();
	}

	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_nBitCount = 0;
	m_nPixelWidth = 0;
	m_nPixelHeight = 0;

	ResetEvent(m_hStartEvent);

	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	return TRUE;
}

BOOL gdi_wnd_draw_video::SetProcState()
{
	DWORD dwRet = 0;

	if (m_hThread == NULL)
	{
		return FALSE;
	}

	dwRet = ResumeThread(m_hThread);
	if (dwRet == 0)
	{
		return TRUE;
	}
	else if (dwRet == 1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void gdi_wnd_draw_video::SetProcTimeOver(int nProcTime, int nCloseTime)
{
	m_nProcTimeOver = nProcTime;
	m_nCloseTimeOver = nCloseTime;
}

BOOL gdi_wnd_draw_video::SetFrameData(const unsigned char* pFrameData, unsigned long ulDataLen, unsigned long ulVideoWidth, unsigned long ulVideoHeight, unsigned long ulBitCount)
{
	if (pFrameData == NULL || ulDataLen == 0)
	{
		return FALSE;
	}

	if (ulVideoWidth == 0 || ulVideoHeight == 0)
	{
		return FALSE;
	}

	if (ulBitCount == 0)
	{
		return FALSE;
	}

	FRAME_DATA_BUFFER* pFrameBuffer = new FRAME_DATA_BUFFER;
	if (pFrameBuffer == NULL)
	{
		return FALSE;
	}
	memset(pFrameBuffer, 0x0, sizeof(FRAME_DATA_BUFFER));

	pFrameBuffer->pFrameData = new unsigned char[ulDataLen];
	if (pFrameBuffer->pFrameData == NULL)
	{
		if (pFrameBuffer != NULL)
		{
			delete pFrameBuffer;
			pFrameBuffer = NULL;
		}
		return FALSE;
	}
	memset(pFrameBuffer->pFrameData, 0x0, ulDataLen);

	pFrameBuffer->uFrameDataLen = ulDataLen;
	pFrameBuffer->ulBitCount    = ulBitCount;
	pFrameBuffer->uPixelWidth   = ulVideoWidth;
	pFrameBuffer->uPixelHeight  = ulVideoHeight;
	memcpy(pFrameBuffer->pFrameData, pFrameData, ulDataLen);

	m_threadFrameQueue.Push(pFrameBuffer);
	return TRUE;
}

BOOL gdi_wnd_draw_video::getframedata()
{
	BOOL bRet = FALSE;

	unsigned long uPixelWidth  = 0;
	unsigned long uPixelHeight = 0;
	unsigned long uBitCount    = 0;
	unsigned long ulDataLen    = 0;

	FRAME_DATA_BUFFER* pFrameBuffer = NULL;

	if (m_threadFrameQueue.IsEmpty())
	{
		return FALSE;
	}

	pFrameBuffer = (FRAME_DATA_BUFFER*)m_threadFrameQueue.Front();
	if (pFrameBuffer == NULL)
	{
		return FALSE;
	}

	if (pFrameBuffer->pFrameData == NULL || pFrameBuffer->uFrameDataLen == 0 || pFrameBuffer->uPixelWidth == 0 || pFrameBuffer->uPixelHeight == 0)
	{
		bRet = FALSE;
		goto part1;
	}

	uBitCount    = pFrameBuffer->ulBitCount;
	uPixelWidth  = pFrameBuffer->uPixelWidth;
	uPixelHeight = pFrameBuffer->uPixelHeight;

	if (uPixelWidth != m_nPixelWidth || uPixelHeight != m_nPixelHeight)
	{
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}

		m_nBufferLen = (uPixelWidth*uBitCount/8+3)/4*4*uPixelHeight;//uPixelWidth*uPixelHeight*uBitCount/8;
		m_pBuffer = new unsigned char[m_nBufferLen];
		if (m_pBuffer == NULL)
		{
			bRet = FALSE;
			goto part1;
		}
		memset(m_pBuffer, 0x0, m_nBufferLen);

		m_nPixelWidth  = uPixelWidth;
		m_nPixelHeight = uPixelHeight;
	}

	if (m_pBuffer == NULL)
	{
		bRet = FALSE;
		goto part1;
	}
	else
	{
		memset(m_pBuffer, 0x0, m_nBufferLen);
	}

	if (m_nBufferLen < pFrameBuffer->uFrameDataLen)
	{
		bRet = FALSE;
		goto part1;
	}

	bRet = TRUE;
	memcpy(m_pBuffer, pFrameBuffer->pFrameData, pFrameBuffer->uFrameDataLen);

part1:
	if (pFrameBuffer != NULL)
	{
		if (pFrameBuffer->pFrameData != NULL)
		{
			delete[] pFrameBuffer->pFrameData;
			pFrameBuffer->pFrameData = NULL;
		}

		delete pFrameBuffer;
		pFrameBuffer = NULL;
	}

	m_threadFrameQueue.Pop();
	return bRet;
}

BOOL gdi_wnd_draw_video::drawscene()
{
	int nRet = 0;
	BITMAPINFOHEADER bih = {0};

	if (m_hWnd == NULL)
	{
		return FALSE;
	}
	
	if (m_hDC == NULL)
	{
		return FALSE;
	}

	if (!getframedata())
	{
//		return FALSE;
	}

	if (m_pBuffer == NULL)
	{
		return FALSE;
	}

	BITMAPINFO bmi = {0};
	bmi.bmiHeader.biSize  = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_nPixelWidth;
	bmi.bmiHeader.biHeight = m_nPixelHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	nRet = SetStretchBltMode(m_hDC, COLORONCOLOR);
	if (nRet == 0)
	{
		return FALSE;
	}

	nRet = StretchDIBits(m_hDC,
						 0,
						 0,
						 m_rect.Width(),
						 m_rect.Height(),
						 0,
						 0,
						 m_nPixelWidth,
						 m_nPixelHeight,
						 m_pBuffer,
						 &bmi,
						 DIB_RGB_COLORS,
						 SRCCOPY
						 );
	if (nRet == GDI_ERROR)
	{
		return FALSE;
	}

	return 0;
}