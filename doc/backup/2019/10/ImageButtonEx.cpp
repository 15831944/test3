#include "stdafx.h"
#include "ImageButtonEx.h"

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
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

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
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	if (pDC == NULL)
	{
		return;
	}
	
	CDC* pCurBtnDC = NULL;
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
				pCurBtnDC = &m_dcStandH;
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
	CRect rect;
	GetClientRect(&rect);

	if (m_dcWndBkDC.m_hDC == NULL)
	{
		CClientDC clDC(GetParent());

		CRect rcParentRect;
		GetWindowRect(&rcParentRect);
		GetParent()->ScreenToClient(rcParentRect);

		m_dcWndBkDC.CreateCompatibleDC(&clDC);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());

		CBitmap *pOldBitmap = m_dcWndBkDC.SelectObject(&bitmap);
		m_dcWndBkDC.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rcParentRect.left, rcParentRect.top, SRCCOPY);
		bitmap.DeleteObject();

#if 0
		Bitmap*	pWndBkImage = ::new Bitmap((HBITMAP)::GetCurrentObject(m_dcWndBkDC, OBJ_BITMAP),NULL);
		if (pWndBkImage != NULL)
		{
			CLSID pngClsid;
			GetEncoderClsid(L"image/bmp", &pngClsid);

			pWndBkImage->Save(L"C:\\11.bmp", &pngClsid, NULL);
		}
#endif
	}
}

void CImageButtonEx::DrawParentWndBk(CDC *pDC, CDC *pWndBkDC, Bitmap* pSrcImage, CDC &dcDrawImage)
{
	if (pDC == NULL || pSrcImage == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	CMemDC MemDC(pDC, rect);
	Graphics graphics(MemDC.GetSafeHdc());

	//画父窗口背景
	MemDC.BitBlt(0, 0, rect.Width(), rect.Height(), pWndBkDC, 0, 0, SRCCOPY);

	//画图片信息
	graphics.DrawImage(pSrcImage, 0, 0, pSrcImage->GetWidth(), pSrcImage->GetHeight());

	dcDrawImage.CreateCompatibleDC(MemDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(MemDC, rect.Width(), rect.Height());

	CBitmap *pOldBitmap = dcDrawImage.SelectObject(&bitmap);
	dcDrawImage.BitBlt(0, 0, rect.Width(), rect.Height(), MemDC, 0, 0, SRCCOPY);
	bitmap.DeleteObject();
}

void CImageButtonEx::SetImageWndBk(CDC *pDC)
{
	Bitmap* pImage = NULL;
	if (pDC == NULL)
	{
		return;
	}

	//正常状态
	if (m_dcStand.m_hDC == NULL)
	{
		pImage = m_pStdImage;
		DrawParentWndBk(pDC, &m_dcWndBkDC, pImage, m_dcStand);
	}

	if (m_dcStandP.m_hDC == NULL)
	{
		if (m_pStdPImage != NULL)
		{
			pImage = m_pStdPImage;
		}
		else
		{
			pImage = m_pStdImage;
		}

		DrawParentWndBk(pDC, &m_dcWndBkDC, pImage, m_dcStandP);
	}

	if (m_dcStandH.m_hDC == NULL)
	{
		if (m_pStdHImage != NULL)
		{
			pImage = m_pStdHImage;
		}
		else
		{
			pImage = m_pStdImage;
		}

		DrawParentWndBk(pDC, &m_dcWndBkDC, pImage, m_dcStandH);
	}

	if (m_dcStandD.m_hDC == NULL)
	{
		if (m_pStdDImage != NULL)
		{
			pImage = m_pStdDImage;
		}
		else
		{
			pImage = m_pStdImage;
		}

		DrawParentWndBk(pDC, &m_dcWndBkDC, pImage, m_dcStandD);
	}

	if (m_pPreBtnDC == NULL)
	{
		m_pPreBtnDC = &m_dcStand;
	}
}

void CImageButtonEx::DrawImageWndBk(CDC *pDC, CDC *pCurBtnDC)
{
	CRect rcText;
	CStringW strText;
	StringFormat stringformat1;

	USES_CONVERSION;
	if (pDC == NULL || pCurBtnDC == NULL)
	{
		return;
	}

	CRect rect;
	GetClientRect(rect);

	CMemDC MemDC(pDC, rect);
	Graphics graphics(MemDC.GetSafeHdc());
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias); 

	MemDC->BitBlt(0, 0, rect.Width(), rect.Height(), pCurBtnDC, 0, 0, SRCCOPY);

	if (m_strBtnText != _T(""))
	{
		if (m_rcText != CRect(0, 0, 0, 0))
		{
			rcText = m_rcText;
		}
		else
		{
			rcText = rect;
		}

		strText = A2CW(m_strBtnText);
		stringformat1.SetAlignment(StringAlignmentCenter);
		stringformat1.SetLineAlignment(StringAlignmentCenter);

		//FontFamily fontfamily(L"微软雅黑");
		//Font font1(&fontfamily, 26, FontStyleRegular, UnitPixel);
		Gdiplus::Font font1(MemDC->GetSafeHdc(), &m_lfFont);

		graphics.DrawString(strText.GetBuffer(0), strText.GetLength(), &font1, RectF(rcText.left, rcText.top, rcText.Width(), rcText.Height()), &stringformat1, &SolidBrush(Color::White));
	}

	m_pPreBtnDC = pCurBtnDC;
}

int CImageButtonEx::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
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
	if (lpszBtnText == NULL)
	{
		return;
	}

	memset(&m_lfFont, 0x0, sizeof(LOGFONT));

	m_lfFont.lfHeight = uiFontSize;
	m_lfFont.lfWeight = uiWeight;
	m_lfFont.lfCharSet=GB2312_CHARSET;
	_tcscpy_s(m_lfFont.lfFaceName, lpszFaceName);

// 	if (m_Font.m_hObject == NULL)
// 	{
// 		m_Font.CreateFontIndirect(&lfFont);
// 		if (m_Font.m_hObject == NULL)
// 		{
// 			return;
// 		}
// 	}

	m_strBtnText = lpszBtnText;
	m_crTextColor = crTextColor;

	m_rcText.CopyRect(rcText);
	Invalidate(TRUE);
}