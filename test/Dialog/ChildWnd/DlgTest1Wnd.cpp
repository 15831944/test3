#include  "stdafx.h"
#include  "DlgTest1Wnd.h"

IMPLEMENT_DYNAMIC(CDlgTest1Wnd, CDialog)
CDlgTest1Wnd::CDlgTest1Wnd(CWnd* pParent)
	: CDialog(CDlgTest1Wnd::IDD, pParent)
{
}

CDlgTest1Wnd::~CDlgTest1Wnd()
{
}

void CDlgTest1Wnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1_SHOWINFO,	m_ListBox);
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_BN_CLICKED(IDC_BTN2_OPENFLODER,		OnBnClickedButton1)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::OnInitDialog()
{
	BOOL bRet = FALSE;
	CDialog::OnInitDialog();

	do 
	{
		if (!InitCtrl())
		{
			bRet = FALSE;
			break;
		}

		if (!InitInfo())
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);
	
	if (!bRet)
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
	}

	return bRet; 
}

BOOL CDlgTest1Wnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CDlgTest1Wnd::InitCtrl()
{
	return TRUE;
}

BOOL CDlgTest1Wnd::InitInfo()
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
CString InterceptSubText(LPCTSTR lpszUserName, UINT uiLimitLen)
{
	BOOL bRet = FALSE;

	UINT uiBit = 0;
	UINT uiPos = 0;
	UINT uiNameLen = 0;

	UINT uiIndex = 0;
	UINT uiOffset = 0;
	UINT uiSpecIndex = 0;

	char *p = NULL;
	CString strUserName;
	char szNewUserName[256] = { 0 };

	do
	{
		if (lpszUserName == NULL || *lpszUserName == '\0')
		{
			bRet = FALSE;
			break;
		}

		p = (char*)lpszUserName;
		uiNameLen = strlen(lpszUserName);
		if (uiLimitLen > uiNameLen)
		{
			uiSpecIndex = uiNameLen;
		}
		else
		{
			uiSpecIndex = uiLimitLen;
		}

		while (*p != '\0')
		{
			if ((*p & 0x80) && (*(p + 1) & 0x80))
			{
				uiBit = 2;
			}
			else
			{
				uiBit = 1;
			}

			if (*(p + uiBit) != '\0' && uiIndex != uiSpecIndex)
			{
				memcpy(szNewUserName + uiOffset, p, uiBit);
				uiOffset += uiBit;

				p += uiBit;
				uiPos += uiBit;
			}
			else
			{
				if (uiIndex == uiSpecIndex)
				{
					if (uiPos == uiNameLen)
					{
						strUserName.Format(_T("%s"), szNewUserName);
					}
					else
					{
						strUserName.Format(_T("%s..."), szNewUserName);
					}
				}
				else
				{
					memcpy(szNewUserName + uiOffset, p, uiBit);
					uiOffset += uiBit;

					p += uiBit;
					uiPos += uiBit;

					strUserName.Format(_T("%s"), szNewUserName);
				}

				bRet = TRUE;
				break;
			}

			uiIndex++;
		}
	} while (FALSE);

	return strUserName;
}

#include<regex>
#include <array>
void CDlgTest1Wnd::OnBnClickedButton1()
{
#if 0
	std::tr1::array<int, 5> arry = {1, 2, 3, 4, 5};
	string str("1994 is my birth year");

	string regex_str("\\d{4}");
	std::tr1::regex pattern(regex_str, std::tr1::regex::icase);

	string temp;
	std::tr1::match_results<string::const_iterator> result;

	string::const_iterator iter = str.begin();
	string::const_iterator iterEnd= str.end();

	while (std::tr1::regex_search(iter, iterEnd, result, pattern))
	{
		temp=result[0];

		cout<<temp<<endl;
		iter = result[0].second; //更新搜索起始位置
	}
#endif
}