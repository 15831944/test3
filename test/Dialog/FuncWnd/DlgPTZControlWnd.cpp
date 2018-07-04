#include "stdafx.h"
#include "../test.h"
#include "DlgPTZControlWnd.h"

#include <GdiPlus.h>
#include "../inc/GlobalInfo.h"

#define PI 3.1415926

typedef BOOL (WINAPI *lpfnUpdateLayeredWindow) (HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF,BLENDFUNCTION*, DWORD);
lpfnUpdateLayeredWindow g_pUpdateLayeredWindow;

IMPLEMENT_DYNAMIC(CDlgPTZControlWnd, CDialog)
CDlgPTZControlWnd::CDlgPTZControlWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPTZControlWnd::IDD, pParent)
{
	m_bRefreshSkin = FALSE;
	m_bTracking = FALSE;

	m_strAppPath = _T("");
	m_pPTZImage = NULL;
}

CDlgPTZControlWnd::~CDlgPTZControlWnd()
{
}

void CDlgPTZControlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgPTZControlWnd, CDialog)
	ON_WM_PAINT()

	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgPTZControlWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgPTZControlWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!InitInfo())
	{
		return FALSE;
	}

	if (!InitCtrl())
	{
		return FALSE;
	}

	return TRUE;
}

void CDlgPTZControlWnd::OnPaint() 
{
	CPaintDC dc(this);
}

void CDlgPTZControlWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackmouseevent.dwFlags =TME_HOVER | TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = 10;
		if (::_TrackMouseEvent(&trackmouseevent))
		{
			m_bTracking  = TRUE;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPTZControlWnd::OnMouseLeave()
{
	m_bTracking = FALSE;
	SetPTZWndImage(CAMERA_PTZ_NORMAL, m_strAppPath);
	CDialog::OnMouseLeave();
}

void CDlgPTZControlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!DrawPTZWndImage(m_pPTZImage, point))
	{
		return;
	}

	if (!SetLayeredWnd(255, NULL, m_pPTZImage))
	{
		return;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgPTZControlWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgPTZControlWnd::InitCtrl()
{
	ModifyStyleEx(0, WS_EX_LAYERED | WS_OVERLAPPED);

	if (!InitWndSkin())
	{
		return FALSE;
	}

	if (!CreateChildWnd())
	{
		return FALSE;
	}

	if (!SetLayeredWnd(255, NULL, m_pPTZImage))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgPTZControlWnd::InitInfo()
{
	DWORD dwExStyle = 0;

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

	m_strAppPath = pGlobal->GetAppPath();
	if (m_strAppPath == _T("") || m_strAppPath.GetLength() <= 0)
	{
		return FALSE;
	}

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pUpdateLayeredWindow = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32, "UpdateLayeredWindow");
	return TRUE;
}

BOOL CDlgPTZControlWnd::InitWndSkin()
{
	if (!SetPTZWndImage(CAMERA_PTZ_NORMAL, m_strAppPath))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgPTZControlWnd::CreateChildWnd()
{
	int iWidth  = 0;
	int iHeight = 0;

	if (m_pPTZImage != NULL)
	{
		iWidth  = m_pPTZImage->GetWidth();
		iHeight = m_pPTZImage->GetHeight();
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		iWidth  = rcClient.Width();
		iHeight = rcClient.Height();
	}

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, iWidth, iHeight, SWP_NOSIZE|SWP_NOMOVE); 
	return TRUE;
}

BOOL CDlgPTZControlWnd::SetPTZWndImage(CAMERA_PTZ_ORG emPTZOrg, LPCTSTR lpszStylePath)
{
	CString strPTZCtrlPath;

	if (emPTZOrg == CAMERA_PTZ_EMPTY)
	{
		return FALSE;
	}

	if (lpszStylePath == NULL || *lpszStylePath == '\0')
	{
		return FALSE;
	}

	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return FALSE;
	}

	switch (emPTZOrg)
	{
	case CAMERA_PTZ_NORMAL:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Normal.png"), lpszStylePath);
		}	
		break;

	case CAMERA_PTZ_LEFT:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Left.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_LEFT_BOTTOM:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Left_Bottom.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_BOTTOM:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Bottom.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_RIGHT_BOTTOM:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Right_Bottom.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_RIGHT:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Right.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_RIGHT_TOP:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Right_Top.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_TOP:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Top.png"), lpszStylePath);
		}
		break;

	case CAMERA_PTZ_LEFT_TOP:
		{
			strPTZCtrlPath.Format(_T("%s\\Skins\\PTZControl\\PTZ_Left_Top.png"), lpszStylePath);
		}
		break;
	}

	if (!pGlobal->IsFileExists(strPTZCtrlPath))
	{
		return FALSE;
	}

	if (m_pPTZImage != NULL)
	{
		delete m_pPTZImage;
		m_pPTZImage = NULL;
	}

	CT2CW strFileName(strPTZCtrlPath.GetBuffer(0));	

	m_pPTZImage=Bitmap::FromFile((LPCWSTR)strFileName);
	if ((m_pPTZImage==NULL)||(m_pPTZImage->GetLastStatus()!=Ok)) 
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgPTZControlWnd::DrawPTZWndImage(Bitmap *pPTZImage, POINT ptMousePos)
{
	BOOL bRet = FALSE;
	int iInOffset = 3;
	int iOutOffset = 8;

	if (pPTZImage == NULL)
	{
		return FALSE;
	}

	do 
	{
		double ixOrg = (double)ptMousePos.x;
		double iYOrg = (double)ptMousePos.y;

		double iRadius = (double)((double)pPTZImage->GetWidth()/(double)2);
		if (iRadius == 0)
		{
			bRet = FALSE;
			break;
		}

		if ((ixOrg - iRadius) == 0)
		{
		}
		else if ((iYOrg - iRadius) == 0)
		{
		}
		else
		{
			double dAngle  = atan2((iRadius - iYOrg),(ixOrg - iRadius));
			double dAngle1 = 180 * dAngle / PI;

			if (dAngle1 <= 0)
			{
				dAngle1 = 360 + (dAngle1);
			}

			double sin1 = sin(dAngle);
			if (sin1 != 0)
			{
				int iRadiusCur = abs((iRadius - iYOrg)/sin(dAngle));
				if (iRadiusCur < 30)
				{
					SetPTZWndImage(CAMERA_PTZ_NORMAL, m_strAppPath);
				}
				else if (iRadiusCur >= 30 && iRadiusCur < (iRadius-iOutOffset))
				{
					int iAverageAngle = 90/4;
					if (dAngle1 < iAverageAngle || ((dAngle1 > (360 - iAverageAngle) ) && iAverageAngle < 360))
					{
						SetPTZWndImage(CAMERA_PTZ_RIGHT, m_strAppPath);
					}
					else if ((dAngle1 < 90 - iAverageAngle) && (dAngle1 > iAverageAngle))
					{
						SetPTZWndImage(CAMERA_PTZ_RIGHT_TOP, m_strAppPath);
					}
					else if ((dAngle1 < (90 + iAverageAngle)) && (dAngle1 > (90 - iAverageAngle)))
					{
						SetPTZWndImage(CAMERA_PTZ_TOP, m_strAppPath);
					}
					else if ((dAngle1 < (180 - iAverageAngle)) && (dAngle1 > (90 + iAverageAngle)))
					{
						SetPTZWndImage(CAMERA_PTZ_LEFT_TOP, m_strAppPath);
					}
					else if ((dAngle1 < (180 + iAverageAngle)) && (dAngle1 > (180 - iAverageAngle)))
					{
						SetPTZWndImage(CAMERA_PTZ_LEFT, m_strAppPath);
					}
					else if ((dAngle1 < (270 - iAverageAngle)) && (dAngle1 > (180 + iAverageAngle)))
					{
						SetPTZWndImage(CAMERA_PTZ_LEFT_BOTTOM, m_strAppPath);
					}
					else if ((dAngle1 < (270 + iAverageAngle)) && (dAngle1 > (270 - iAverageAngle)))
					{
						SetPTZWndImage(CAMERA_PTZ_BOTTOM, m_strAppPath);
					}
					else if ((dAngle1 < (360 - iAverageAngle)) && (dAngle1 > (270 + iAverageAngle)))
					{
						SetPTZWndImage(CAMERA_PTZ_RIGHT_BOTTOM, m_strAppPath);
					}
				}
				else
				{
					SetPTZWndImage(CAMERA_PTZ_NORMAL, m_strAppPath);
				}
			}
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CDlgPTZControlWnd::SetLayeredWnd(int iTransparent, CDC *pDC, Bitmap *pPTZImage)
{
	BLENDFUNCTION stcBlend;
	memset(&stcBlend, 0x0, sizeof(BLENDFUNCTION));

	if (pPTZImage == NULL)
	{
		return FALSE;
	}

	if (iTransparent < 0 || iTransparent > 100)
	{
		iTransparent = 100;
	}

	if (pDC == NULL)
	{
		pDC = GetDC();
	}

	stcBlend.BlendOp = AC_SRC_OVER;		//把源图片覆盖到目标之上
	stcBlend.BlendFlags = 0;
	stcBlend.AlphaFormat = AC_SRC_ALPHA;	//AC_SRC_ALPHA:		//0:
	stcBlend.SourceConstantAlpha = int(iTransparent*2.55);

	if (!UpdateLayeredWnd(pDC, pPTZImage->GetWidth(), pPTZImage->GetHeight(), stcBlend))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDlgPTZControlWnd::UpdateLayeredWnd(CDC *pDC, int iWidth, int iHeight, BLENDFUNCTION stcBlend)
{
	BOOL bRet = FALSE;

	CDC MemDC;
	HDC hWndDC;

	POINT ptSrcPos;
	POINT ptWndPos;
	CRect rcClient;
	CBitmap bitmap, *pOldBitmap;

	HWND  hWnd;
	SIZE  sizeWnd;
	
	do 
	{
		if (pDC == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (iWidth <= 0 || iHeight <= 0)
		{
			bRet = FALSE;
			break;
		}

		hWnd = GetSafeHwnd();
		if (hWnd == NULL)
		{
			bRet = FALSE;
			break;
		}

		hWndDC = ::GetDC(hWnd);
		if (hWndDC == NULL)
		{
			bRet = FALSE;
			break;
		}

		if (!MemDC.CreateCompatibleDC(pDC))
		{
			bRet = FALSE;
			break;
		}

// 		DWORD dwExStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
// 		if ((dwExStyle&GWL_EXSTYLE) != WS_EX_LAYERED)
// 		{
// 			SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwExStyle^WS_EX_LAYERED);
// 		}

		bitmap.CreateCompatibleBitmap(pDC, iWidth, iHeight);
		pOldBitmap = MemDC.SelectObject(&bitmap);

		Graphics graphics(MemDC.GetSafeHdc());
		graphics.DrawImage(m_pPTZImage, 0, 0, iWidth, iHeight);

		sizeWnd.cx = iWidth;
		sizeWnd.cy = iHeight;

		ptSrcPos.x = 0;
		ptSrcPos.y = 0;

		GetWindowRect(&rcClient);
		ptWndPos.x = rcClient.left;
		ptWndPos.y = rcClient.top;

		::g_pUpdateLayeredWindow(hWnd, hWndDC, &ptWndPos, &sizeWnd, MemDC.m_hDC, &ptSrcPos, 0, &stcBlend, ULW_ALPHA);
		//::UpdateLayeredWindow(hWnd, hWndDC, &ptWndPos, &sizeWnd, MemDC.m_hDC, &ptSrcPos, 0, &stcBlend, ULW_ALPHA);

		graphics.ReleaseHDC(MemDC.GetSafeHdc());
		MemDC.SelectObject(pOldBitmap);

		bRet = TRUE;
	} while (FALSE);

	if (hWndDC != NULL)
	{
		::ReleaseDC(hWnd, hWndDC);
		hWndDC = NULL;
	}
	
	if (MemDC != NULL && MemDC.GetSafeHdc())
	{
		MemDC.DeleteDC();
	}
	
	return bRet;
}