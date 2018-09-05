#include "stdafx.h"
#include "ImageButton.h"

#include "../../Inc/common/MemDCEx.h"
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

CImageButton::CImageButton()
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	m_bIsDisabled = FALSE;

	m_bIsDownFlag = FALSE;
	m_bIsBtnDown = FALSE;
	m_bInitedBitmap = FALSE;
	
	m_pImageNormal = NULL;
	m_pImageHover = NULL;
	m_pImageDown = NULL;
	m_pImageDisable = NULL;
	
	m_pCurBtnDC = NULL;

	m_strBtnText = _T("");
	m_crTextColor = GetSysColor(COLOR_WINDOW);
	memset(&m_lfFont, 0x0, sizeof(LOGFONT));
}

CImageButton::~CImageButton()
{
	SafeDelete(m_pImageNormal);
	SafeDelete(m_pImageHover);
	SafeDelete(m_pImageDown);
	SafeDelete(m_pImageDisable);
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CImageButton, CButton)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	
	//ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP()
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
	CMemDCEx MemDC(pScreenDC, rcWndCtrl, FALSE);
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
				grect.X=0;
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

		if (m_dcStandDisableDC.m_hDC == NULL)
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
				grect.X=0;
				grect.Y=0; 
				grect.Width = width; 
				grect.Height = height;
				graphics.DrawImage(m_pImageNormal, grect, 0, 0, width, height, UnitPixel, &ia);
			}
			else
			{
				graphics.DrawImage(m_pImageDisable, 0, 0, m_pImageDisable->GetWidth(), m_pImageDisable->GetHeight());
			}

			m_dcStandDisableDC.CreateCompatibleDC(MemDC);
			bitmap.CreateCompatibleBitmap(MemDC, rcWndCtrl.Width(), rcWndCtrl.Height());
			pOldBitmap = m_dcStandDisableDC.SelectObject(&bitmap);
			m_dcStandDisableDC.BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), MemDC, 0, 0, SRCCOPY);
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
		m_pCurBtnDC = &m_dcStandDisableDC; 
	}
	else
	{
		bIsPressed = (lpDIS->itemState & ODS_SELECTED);
		
		if (m_bIsDownFlag)
		{
			m_pCurBtnDC = &m_dcStandPressDC;
		}
		else
		{
			if (bIsPressed)
			{
				m_pCurBtnDC = &m_dcStandPressDC;
			}
			else
			{
				if (m_bIsHovering)
				{
					m_pCurBtnDC = &m_dcStandHotLightDC;
				}
				else
				{
					m_pCurBtnDC = &m_dcStandDC;
				}
			}
		}
	}

	DrawWndBtn(pDC);
}

LRESULT CImageButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bIsTracking = FALSE;
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
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;

		if (::_TrackMouseEvent(&tme))
		{
			m_bIsTracking = TRUE;
		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}

#if 0
void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwFlags = TME_CANCEL|TME_LEAVE;
		tme.dwHoverTime = 0;

		if (::_TrackMouseEvent(&tme))
		{
			m_bIsTracking = FALSE;
		}
	}

	m_bIsButtonDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}

void CImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bIsButtonDown = FALSE;
	CButton::OnLButtonUp(nFlags, point);
}
#endif
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
	CRect rcCenter;
	CFont *pOldFont = NULL;
	if (pDC == NULL)
	{
		return;
	}

	CRect rcWndCtrl;
	GetClientRect(&rcWndCtrl);

	if (m_strBtnText != _T("") && m_strBtnText.GetLength() > 0)
	{
		rcCenter = rcWndCtrl;
		int iTextHeight = pDC->DrawText(m_strBtnText, -1, rcCenter, DT_WORDBREAK|DT_EDITCONTROL|DT_CENTER|DT_CALCRECT);
		rcCenter.CopyRect(rcWndCtrl);
		if (rcCenter.Height() > iTextHeight)
		{
			rcCenter.top += (rcCenter.Height() - iTextHeight)/2;
		}
	}

	pDC->BitBlt(0, 0, rcWndCtrl.Width(), rcWndCtrl.Height(), m_pCurBtnDC, 0, 0, SRCCOPY);

	if (m_strBtnText != _T("") && m_strBtnText.GetLength() > 0)
	{
		if (m_Font.m_hObject != NULL)
		{
			pOldFont = pDC->SelectObject(&m_Font);
		}
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_crTextColor);

		pDC->DrawText(m_strBtnText, -1, rcCenter, DT_WORDBREAK|DT_EDITCONTROL|DT_CENTER);
		pDC->SelectObject(pOldFont);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CImageButton::SetBtnDownFlag(BOOL bFlag)
{
	m_bIsDownFlag = bFlag;
}

BOOL CImageButton::SetImage(LPCTSTR lpszImageNormal, LPCTSTR lpszImageHover, LPCTSTR lpszImageDown, LPCTSTR lpszImageDisable)
{
	BOOL bRet = FALSE;
	CRect rcBtnWnd;
	
	USES_CONVERSION;
	do
	{
		this->GetWindowRect(&rcBtnWnd);
		GetParent()->ScreenToClient(&rcBtnWnd);

		if (lpszImageNormal != NULL && *lpszImageNormal != '\0')
		{
			m_pImageNormal = Bitmap::FromFile(A2CW(lpszImageNormal));
		}
		
		if (lpszImageHover != NULL && *lpszImageHover != '\0')
		{
			m_pImageHover = Bitmap::FromFile(A2CW(lpszImageHover));
		}
		
		if (lpszImageDown != NULL && *lpszImageDown != '\0')
		{
			m_pImageDown = Bitmap::FromFile(A2CW(lpszImageDown));
		}
		
		if (lpszImageDisable != NULL && *lpszImageDisable != '\0')
		{
			m_pImageDisable = Bitmap::FromFile(A2CW(lpszImageDisable));
		}

		rcBtnWnd.right = rcBtnWnd.left + m_pImageNormal->GetWidth();
		rcBtnWnd.bottom = rcBtnWnd.top + m_pImageNormal->GetHeight();
		MoveWindow(rcBtnWnd.left, rcBtnWnd.top, rcBtnWnd.Width(), rcBtnWnd.Height());

		bRet = TRUE;
	} while(FALSE);

	return bRet;
}

BOOL CImageButton::SetImageText(int iFontSize, LPCTSTR lpszBtnText, LPCTSTR lpszFaceName, COLORREF crTextColor)
{
	BOOL bRet = FALSE;
	HFONT hfont = NULL;
	
	do
	{
		if (lpszBtnText == NULL || *lpszBtnText == '\0')
		{
			bRet = FALSE;
			break;
		}
		
		m_Font.CreateFont(iFontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, lpszFaceName);
		
		if (m_Font.m_hObject == NULL)
		{
			bRet = FALSE;
			break;
		}
		
		m_strBtnText = lpszBtnText;
		m_crTextColor = crTextColor;
		
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}