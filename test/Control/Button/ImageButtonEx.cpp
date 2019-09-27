#include "stdafx.h"
#include "ImageButtonEx.h"
#include "RztTooltip.h"

CImageButtonEx::CImageButtonEx()
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	m_bHaveBitmaps = FALSE;

	m_pPreBtnDC = NULL;
	m_pStdImage = NULL;
	m_pStdHImage = NULL;
	m_pStdPImage = NULL;
	m_pStdDImage = NULL;
	
	m_strBtnText = _T("");
	m_rcText.SetRectEmpty();

	m_crTextColor = GetSysColor(COLOR_WINDOW);
}

CImageButtonEx::~CImageButtonEx()
{
	m_Font.DeleteObject();

	SafeDelete(m_pStdImage);
	SafeDelete(m_pStdHImage);
	SafeDelete(m_pStdPImage);
	SafeDelete(m_pStdDImage);
}

BEGIN_MESSAGE_MAP(CImageButtonEx, CButton)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()

	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
void CImageButtonEx::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);	//SWP_FRAMECHANGED
	CButton::PreSubclassWindow();
}

BOOL CImageButtonEx::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		pMsg->message = WM_LBUTTONDOWN;
	}
	
	return CButton::PreTranslateMessage(pMsg);
}

HBRUSH CImageButtonEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (!m_bHaveBitmaps)
	{
		GetParentWndBk();
		SetImageWndBk(pDC);

		m_bHaveBitmaps = TRUE;
	}
	
	return (HBRUSH)::GetStockObject(GRAY_BRUSH);
}

void CImageButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pCurBtnDC = NULL;

	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	if (pDC == NULL)
	{
		return;
	}
	
	if (lpDIS->itemState & ODS_DISABLED)
	{//禁用
		pCurBtnDC = &m_dcStandD; 
	}
	else
	{
		if (lpDIS->itemState & ODS_SELECTED)
		{//选中
			pCurBtnDC = &m_dcStandP;
		}
		else
		{
			if (m_bIsHovering)
			{//滑过
				pCurBtnDC = &m_dcStandH;//&m_dcStandDC;
			}
			else
			{//正常
				pCurBtnDC = &m_dcStand;
			}
		}
	}

	DrawImageWndBk(pDC, pCurBtnDC);
}

BOOL CImageButtonEx::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

LRESULT CImageButtonEx::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bIsTracking = FALSE;
	m_bIsHovering = TRUE;
	
	//Invalidate();
	return 0;
}

LRESULT CImageButtonEx::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	
	//Invalidate();
	return 0;
}

void CImageButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = this->GetSafeHwnd();
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;

		if (::_TrackMouseEvent(&tme))
		{
			m_bIsTracking = TRUE;
		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
//
void CImageButtonEx::GetParentWndBk()
{
	CBitmap bitmap;
	CBitmap *pOldBitmap = NULL;

	CRect rect;
	GetClientRect(&rect);

	if (m_dcWndBkDC.m_hDC == NULL)
	{
		CClientDC clDC(GetParent());

		CRect rcParentRect;
		GetWindowRect(&rcParentRect);
		GetParent()->ScreenToClient(rcParentRect);

		m_dcWndBkDC.CreateCompatibleDC(&clDC);
		bitmap.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());

		pOldBitmap = m_dcWndBkDC.SelectObject(&bitmap); 
		
		m_dcWndBkDC.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rcParentRect.left, rcParentRect.top, SRCCOPY);
		bitmap.DeleteObject();
	}
}

void CImageButtonEx::DrawParentWndBk(CDC *pDC)
{
	if (pDC == NULL || m_dcWndBkDC == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcWndBkDC, 0, 0, SRCCOPY);
}

void CImageButtonEx::SetImageWndBk(CDC *pDC)
{
	CBitmap bitmap;
	CBitmap *pOldBitmap = NULL;

	if (pDC == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CMemDC MemDC(pDC, rect);
	Graphics graphics(MemDC.GetSafeHdc());

	if (m_dcStand.m_hDC == NULL)
	{//正常状态
		DrawParentWndBk(MemDC);
		graphics.DrawImage(m_pStdImage, 0, 0, m_pStdImage->GetWidth(), m_pStdImage->GetHeight());

		m_dcStand.CreateCompatibleDC(MemDC);
		bitmap.CreateCompatibleBitmap(MemDC, rect.Width(), rect.Height());

		pOldBitmap = m_dcStand.SelectObject(&bitmap);
		m_dcStand.BitBlt(0, 0, rect.Width(), rect.Height(), MemDC, 0, 0, SRCCOPY);
		bitmap.DeleteObject();
	}

	if (m_dcStandP.m_hDC == NULL)
	{//按下状态
		DrawParentWndBk(MemDC);
		if (m_pStdPImage != NULL)
		{
			graphics.DrawImage(m_pStdPImage, 1, 1, m_pStdPImage->GetWidth(), m_pStdPImage->GetHeight());
		}
		else
		{
			graphics.DrawImage(m_pStdImage, 1, 1, m_pStdImage->GetWidth(), m_pStdImage->GetHeight());
		}

		m_dcStandP.CreateCompatibleDC(MemDC);
		bitmap.CreateCompatibleBitmap(MemDC, rect.Width(), rect.Height());

		pOldBitmap = m_dcStandP.SelectObject(&bitmap);
		m_dcStandP.BitBlt(0, 0, rect.Width(), rect.Height(), MemDC, 0, 0, SRCCOPY);
		bitmap.DeleteObject();
	}

	if (m_dcStandH.m_hDC == NULL)
	{//滑过状态
		DrawParentWndBk(MemDC);
		if (m_pStdHImage != NULL)
		{
			graphics.DrawImage(m_pStdHImage, 0, 0, m_pStdHImage->GetWidth(), m_pStdHImage->GetHeight());
		}
		else
		{
			ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
				0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
				0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
				0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
				0.05f, 0.05f, 0.05f, 0.00f, 1.00f};

			ImageAttributes ia;
			ia.SetColorMatrix(&HotMat);	

			float width = (float)m_pStdImage->GetWidth();
			float height = (float)m_pStdImage->GetHeight();

			RectF grect; 
			grect.X=0;
			grect.Y=0; 
			grect.Width = width; 
			grect.Height = height;
			graphics.DrawImage(m_pStdImage, grect, 0, 0, width, height, UnitPixel, &ia);
		}

		m_dcStandH.CreateCompatibleDC(MemDC);
		bitmap.CreateCompatibleBitmap(MemDC, rect.Width(), rect.Height());

		pOldBitmap = m_dcStandH.SelectObject(&bitmap);
		m_dcStandH.BitBlt(0, 0, rect.Width(), rect.Height(), MemDC, 0, 0, SRCCOPY);
		bitmap.DeleteObject();	
	}

	if (m_dcStandD.m_hDC == NULL)
	{//禁用状态
		DrawParentWndBk(MemDC);
		if (m_pStdDImage != NULL)
		{
			graphics.DrawImage(m_pStdDImage, 0, 0, m_pStdDImage->GetWidth(), m_pStdDImage->GetHeight());
		}
		else
		{
			ColorMatrix GrayMat = {	0.30f, 0.30f, 0.30f, 0.00f, 0.00f,
				0.59f, 0.59f, 0.59f, 0.00f, 0.00f,
				0.11f, 0.11f, 0.11f, 0.00f, 0.00f,
				0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
				0.00f, 0.00f, 0.00f, 0.00f, 1.00f};

			ImageAttributes ia;
			ia.SetColorMatrix(&GrayMat);

			float width = (float)m_pStdImage->GetWidth();
			float height = (float)m_pStdImage->GetHeight();

			RectF grect; 
			grect.X=0;
			grect.Y=0; 
			grect.Width = width; 
			grect.Height = height;
			graphics.DrawImage(m_pStdImage, grect, 0, 0, width, height, UnitPixel, &ia);
		}

		m_dcStandD.CreateCompatibleDC(MemDC);
		bitmap.CreateCompatibleBitmap(MemDC, rect.Width(), rect.Height());

		pOldBitmap = m_dcStandD.SelectObject(&bitmap);
		m_dcStandD.BitBlt(0, 0, rect.Width(), rect.Height(), MemDC, 0, 0, SRCCOPY);
		bitmap.DeleteObject();
	}

	if (m_pPreBtnDC == NULL)
	{
		m_pPreBtnDC = &m_dcStand;
	}
}

void CImageButtonEx::DrawImageWndBk(CDC *pDC, CDC *pCurBtnDC)
{
	if (pDC == NULL || pCurBtnDC == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(rect);

	CMemDC MemDC(pDC, rect);
	Graphics graphics(MemDC.GetSafeHdc());

	MemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pCurBtnDC, 0, 0, SRCCOPY);

	m_pPreBtnDC = pCurBtnDC;
}

/////////////////////////////////////////////////////////////////////////////
//
void CImageButtonEx::SetImage(LPCTSTR lpszStdImage, LPCTSTR lpszStdHImage, LPCTSTR lpszStdPImage, LPCTSTR lpszStdDImage)
{
	int iWidth = -1;
	int iHeight = -1;

	USES_CONVERSION;
	if (lpszStdImage != NULL && *lpszStdImage != '\0')
	{
		m_pStdImage = Bitmap::FromFile(A2CW(lpszStdImage));
	}

	if (lpszStdHImage != NULL && *lpszStdHImage != '\0')
	{
		m_pStdHImage = Bitmap::FromFile(A2CW(lpszStdHImage));
	}

	if (lpszStdPImage != NULL && *lpszStdPImage != '\0')
	{
		m_pStdPImage = Bitmap::FromFile(A2CW(lpszStdPImage));
	}

	if (lpszStdDImage != NULL && *lpszStdDImage != '\0')
	{
		m_pStdDImage = Bitmap::FromFile(A2CW(lpszStdDImage));
	}

	if (m_pStdImage != NULL)
	{
		iWidth = m_pStdImage->GetWidth();
		iHeight = m_pStdImage->GetHeight();

		MoveWindow(0, 0, iWidth, iHeight);
	}
}

void CImageButtonEx::SetText(UINT uiFontSize, UINT uiWeight, LPCTSTR lpszBtnText, LPCTSTR lpszFaceName, COLORREF crTextColor, CRect rcText)
{
	LOGFONT	lfFont;
	if (lpszBtnText == NULL || *lpszBtnText == '\0')
	{
		return;
	}

	memset(&lfFont, 0x0, sizeof(LOGFONT));
	lfFont.lfHeight = uiFontSize;
	lfFont.lfWeight = uiWeight;
	lfFont.lfCharSet=GB2312_CHARSET;
	_tcscpy_s(lfFont.lfFaceName, lpszFaceName);

	if (m_Font.m_hObject == NULL)
	{
		m_Font.CreateFontIndirect(&lfFont);
		if (m_Font.m_hObject == NULL)
		{
			return;
		}
	}

	m_strBtnText = lpszBtnText;
	m_crTextColor = crTextColor;

	m_rcText.CopyRect(rcText);
	Invalidate(TRUE);
}