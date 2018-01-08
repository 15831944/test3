#include "stdafx.h"
#include "ImageButton.h"

#include "../../Inc/common/MemDC.h"

CImageButton::CImageButton()
{
	m_bInitedBitmap = FALSE;
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	m_bIsDisabled = FALSE;
	
	m_pImageNormal = NULL;
	m_pImageHover = NULL;
	m_pImageDown = NULL;
	m_pImageDisable = NULL;
	
	m_pCurBtnDC = NULL;
	
	m_strImageNormal = _T("");
	m_strImageHover = _T("");
	m_strImageDown = _T("");
	m_strImageDisable = _T("");
}

CImageButton::~CImageButton()
{
	if (m_pImageNormal != NULL)
	{
		delete m_pImageNormal;
		m_pImageNormal = NULL;
	}
	
	if (m_pImageHover != NULL)
	{
		delete m_pImageHover;
		m_pImageHover = NULL;
	}
	
	if (m_pImageDown != NULL)
	{
		delete m_pImageDown;
		m_pImageDown = NULL;
	}
	
	if (m_pImageDisable != NULL)
	{
		delete m_pImageDisable;
		m_pImageDisable = NULL;
	}
}

BEGIN_MESSAGE_MAP(CImageButton, CButton)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

void CImageButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);
	CButton::PreSubclassWindow();
}

BOOL CImageButton::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;
	}
	
	return CButton::PreTranslateMessage(pMsg);
}

BOOL CImageButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

HBRUSH CImageButton::CtlColor(CDC* pScreenDC, UINT nCtlColor) 
{
	CRect rcWndCtrl;
	CBitmap bitmap, *pOldBitmap = NULL;
	
	if (m_bInitedBitmap || m_pImageNormal == NULL)
	{
		return NULL;
	}
	
	GetClientRect(&rcWndCtrl);
	CMemDC MemDC(pScreenDC, rcWndCtrl);
	Graphics graphics(MemDC->GetSafeHdc());
	
	//
	if (m_dcWndBkDC.m_hDC == NULL)
	{
		CRect rect;
		CClientDC clDC(GetParent());
		GetWindowRect(rect);
		GetParent()->ScreenToClient(rect);
		
		m_dcWndBkDC.CreateCompatibleDC(&clDC);
		bitmap.CreateCompatibleBitmap(&clDC, rcWndCtrl.Width(), rcWndCtrl.Height());
		pOldBitmap = m_dcWndBkDC.SelectObject(&bitmap);
		m_dcWndBkDC.BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), &clDC, rect.left, rect.top, SRCCOPY);
		bitmap.DeleteObject();
	}
	
	if (m_dcStandDC.m_hDC == NULL)
	{
		DrawWndBk(MemDC);
		graphics.DrawImage(m_pImageNormal, 0, 0, m_pImageNormal->GetWidth(), m_pImageNormal->GetHeight());

		m_dcStandDC.CreateCompatibleDC(MemDC);
		bitmap.CreateCompatibleBitmap(MemDC, rcWndCtrl.Width(), rcWndCtrl.Height());
		pOldBitmap = m_dcStandDC.SelectObject(&bitmap);
		m_dcStandDC.BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), MemDC, 0, 0, SRCCOPY);
		bitmap.DeleteObject();

		if (m_dcStandPressDC.m_hDC == NULL)
		{
			DrawWndBk(MemDC);

			if (m_pImageDown == NULL)
			{
				graphics.DrawImage(m_pImageNormal, 1, 1, m_pImageNormal->GetWidth(), m_pImageNormal->GetHeight());
			}
			else
			{
				graphics.DrawImage(m_pImageDown, 1, 1, m_pImageDown->GetWidth(), m_pImageDown->GetHeight());
			}

			m_dcStandPressDC.CreateCompatibleDC(MemDC);
			bitmap.CreateCompatibleBitmap(MemDC, rcWndCtrl.Width(), rcWndCtrl.Height());
			pOldBitmap = m_dcStandPressDC.SelectObject(&bitmap);
			m_dcStandPressDC.BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), MemDC, 0, 0, SRCCOPY);
			bitmap.DeleteObject();
		}

		if (m_dcStandHotLightDC.m_hDC == NULL)
		{
			DrawWndBk(MemDC);

			if (m_pImageHover == NULL)
			{
				ColorMatrix HotMat = {	1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
					0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
					0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.05f, 0.05f, 0.05f, 0.00f, 1.00f};

				ImageAttributes ia;
				ia.SetColorMatrix(&HotMat);	

				float width = (float)m_pImageNormal->GetWidth();
				float height = (float)m_pImageNormal->GetHeight();

				RectF grect; 
				grect.X=0, 
					grect.Y=0; 
				grect.Width = width; 
				grect.Height = height;
				graphics.DrawImage(m_pImageNormal, grect, 0, 0, width, height, UnitPixel, &ia);
			}
			else
			{
				graphics.DrawImage(m_pImageHover, 0, 0, m_pImageHover->GetWidth(), m_pImageHover->GetHeight());
			}
			
			m_dcStandHotLightDC.CreateCompatibleDC(MemDC);
			bitmap.CreateCompatibleBitmap(MemDC, rcWndCtrl.Width(), rcWndCtrl.Height());
			pOldBitmap = m_dcStandHotLightDC.SelectObject(&bitmap);
			m_dcStandHotLightDC.BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), MemDC, 0, 0, SRCCOPY);
			bitmap.DeleteObject();	
		}

		if (m_dcDisableDC.m_hDC == NULL)
		{
			DrawWndBk(MemDC);

			if (m_pImageDisable == NULL)
			{
				ColorMatrix GrayMat = {	0.30f, 0.30f, 0.30f, 0.00f, 0.00f,
					0.59f, 0.59f, 0.59f, 0.00f, 0.00f,
					0.11f, 0.11f, 0.11f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 0.00f, 1.00f};

				ImageAttributes ia;
				ia.SetColorMatrix(&GrayMat);

				float width = (float)m_pImageNormal->GetWidth();
				float height = (float)m_pImageNormal->GetHeight();

				RectF grect; 
				grect.X=0, 
					grect.Y=0; 
				grect.Width = width; 
				grect.Height = height;
				graphics.DrawImage(m_pImageNormal, grect, 0, 0, width, height, UnitPixel, &ia);
			}
			else
			{
				graphics.DrawImage(m_pImageDisable, 0, 0, m_pImageDisable->GetWidth(), m_pImageDisable->GetHeight());
			}

			m_dcDisableDC.CreateCompatibleDC(MemDC);
			bitmap.CreateCompatibleBitmap(MemDC, rcWndCtrl.Width(), rcWndCtrl.Height());
			pOldBitmap = m_dcDisableDC.SelectObject(&bitmap);
			m_dcDisableDC.BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), MemDC, 0, 0, SRCCOPY);
			bitmap.DeleteObject();
		}
	}

	if (m_pCurBtnDC == NULL)
	{
		m_pCurBtnDC = &m_dcStandDC;
	}

	m_bInitedBitmap = TRUE;
	return NULL;
}

void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	BOOL bIsPressed = FALSE;
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	if (pDC == NULL)
	{
		return;
	}

	if (lpDIS->itemState & ODS_DISABLED)
	{
		m_pCurBtnDC = &m_dcDisableDC; 
	}
	else
	{
		bIsPressed = (lpDIS->itemState & ODS_SELECTED);
		if (bIsPressed)
		{
			m_pCurBtnDC = &m_dcStandPressDC;
		}
		else if (m_bIsHovering)
		{
			m_pCurBtnDC = &m_dcStandHotLightDC;
		}
		else
		{
			m_pCurBtnDC = &m_dcStandDC;
		}
	}

	DrawWndBtn(pDC);
}

LRESULT CImageButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bIsHovering = TRUE;
	
	Invalidate();
	return 0;
}

LRESULT CImageButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	
	Invalidate();
	return 0;
}

void CImageButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
void CImageButton::DrawWndBk(CDC *pDC)
{
	if (pDC == NULL)
	{
		return;
	}

	CRect rcWndCtrl;
	GetClientRect(&rcWndCtrl);

	pDC->BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), &m_dcWndBkDC, 0, 0, SRCCOPY);
}

void CImageButton::DrawWndBtn(CDC *pDC)
{
	if (pDC == NULL)
	{
		return;
	}

	CRect rcWndCtrl;
	GetClientRect(&rcWndCtrl);

	pDC->BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), m_pCurBtnDC, 0, 0, SRCCOPY);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CImageButton::SetImage(LPCTSTR lpszImageNormal, LPCTSTR lpszImageHover, LPCTSTR lpszImageDown, LPCTSTR lpszImageDisable)
{
	BOOL bRet = FALSE;
	
	do
	{
		if (lpszImageNormal != NULL && *lpszImageNormal != '\0')
		{
			m_strImageNormal = lpszImageNormal;
			
			CT2CW strFileName(lpszImageNormal);
			m_pImageNormal = Bitmap::FromFile((LPCWSTR)strFileName);
		}
		
		if (lpszImageHover != NULL && *lpszImageHover != '\0')
		{
			m_strImageHover = lpszImageHover;
			
			CT2CW strFileName(lpszImageHover);
			m_pImageHover = Bitmap::FromFile((LPCWSTR)strFileName);
		}
		
		if (lpszImageDown != NULL && *lpszImageDown != '\0')
		{
			m_strImageDown = lpszImageDown;
			
			CT2CW strFileName(lpszImageDown);
			m_pImageDown = Bitmap::FromFile((LPCWSTR)strFileName);
		}
		
		if (lpszImageDisable != NULL && *lpszImageDisable != '\0')
		{
			m_strImageDisable = lpszImageDisable;
			
			CT2CW strFileName(lpszImageDisable);
			m_pImageDisable = Bitmap::FromFile((LPCWSTR)strFileName);
		}

		bRet = TRUE;
		MoveWindow(0,0, m_pImageNormal->GetWidth(), m_pImageNormal->GetHeight());
		
	} while(FALSE);
	
	return bRet;
}