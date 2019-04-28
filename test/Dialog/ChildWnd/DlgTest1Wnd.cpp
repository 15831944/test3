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
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>

static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

void CDlgTest1Wnd::OnBnClickedButton1()
{
	std::string str;
	std::string s1;

#if 0
	std::tr1::array<int, 5> arry = {1, 2, 3, 4, 5};
	string str1("1994 is my birth year");

	string regex_str1("\\d{4}");
	std::tr1::regex pattern1(regex_str1, std::tr1::regex::icase);

	string s1;
	std::tr1::match_results<string::const_iterator> result;

	string::const_iterator iter = str1.begin();
	string::const_iterator iterEnd= str1.end();

	while (std::tr1::regex_search(iter, iterEnd, result, pattern))
	{
		s1=result[0];

		cout<<s1<<endl;
		iter = result[0].second; //更新搜索起始位置
	}
#endif

#if 1
	std::string str2 = _T("this is?#( a #@#test!");
	std::string regex_str2 = _T("((?=[\x21-\x7e]+)[^A-Za-z0-9])");	//(( )+|(\n)+)	//((?=[\x21-\x7e]+)[^A-Za-z0-9])
	std::tr1::regex pattern2(regex_str2, std::tr1::regex::icase);
	
	//ltrim(str2);
	//rtrim(str2);

	s1 = _T("");
	str = std::tr1::regex_replace(str2, pattern2, s1);
#endif
}