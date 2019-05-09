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
}

BEGIN_MESSAGE_MAP(CDlgTest1Wnd, CDialog)
	ON_BN_CLICKED(IDC_BTN_TEST1,		OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_TEST2,		OnBnClickedButton2)
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

#if 0
	std::string str2 = _T("this is?#( a #@#test!");
	std::string regex_str2 = _T("((?=[\x21-\x7e]+)[^A-Za-z0-9])");	//(( )+|(\n)+)	//((?=[\x21-\x7e]+)[^A-Za-z0-9])
	std::tr1::regex pattern2(regex_str2, std::tr1::regex::icase);
	
	//ltrim(str2);
	//rtrim(str2);

	s1 = _T("");
	str = std::tr1::regex_replace(str2, pattern2, s1);
#endif
}

#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")

typedef struct {
	pthread_attr_t		ptAttr;
	pthread_cond_t		ptCond;
	pthread_mutex_t		ptMutex;
	pthread_condattr_t	ptCattr;
	void*				pParam;
}THREAD_MUTEX_T;

typedef enum {
	PRIORITY_IDLE = -1,
	PRIORITY_NORMAL = 0,
	PRIORITY_ABOVE_NORMAL = 1,
	PRIORITY_HIGH = 2,
}THREAD_PRIORITY_T;

void* ptThreadFunc1(void *pParam);
void CDlgTest1Wnd::OnBnClickedButton2()
{
	BOOL bRet = FALSE;

	pthread_t ptThreadId1;
	THREAD_PRIORITY_T priority = PRIORITY_NORMAL;

	struct sched_param param = {0};
	THREAD_MUTEX_T* pMutex_t = NULL;

	do 
	{
		pMutex_t = new THREAD_MUTEX_T;
		if (pMutex_t == NULL)
		{
			bRet = FALSE;
			break;
		}
		memset(pMutex_t, 0x0, sizeof(THREAD_MUTEX_T));

		pthread_attr_init(&pMutex_t->ptAttr);
		if (priority != PRIORITY_NORMAL)
		{
			if (priority != PRIORITY_IDLE)
			{
				pthread_attr_setschedpolicy(&pMutex_t->ptAttr, SCHED_RR);			//实时,轮转法
				if (pthread_attr_getschedparam(&pMutex_t->ptAttr, &param) == 0)		//查询优先级
				{
					if (priority == PRIORITY_HIGH)
					{
						param.sched_priority = 6;									//6:HIGH
					}
					else
					{
						param.sched_priority = 4;									//4:ABOVE_NORMAL
					}

					pthread_attr_setschedparam(&pMutex_t->ptAttr, &param);			//设置优先级
				}
			}
		}

		pMutex_t->pParam = this;
		pthread_mutex_init(&pMutex_t->ptMutex, NULL);
		pthread_condattr_init(&pMutex_t->ptCattr);
		pthread_cond_init(&pMutex_t->ptCond, &pMutex_t->ptCattr);

		pthread_create(&ptThreadId1, &pMutex_t->ptAttr, ptThreadFunc1, pMutex_t);

		//pthread_join(ptThreadId1, NULL);
		//pthread_detach(ptThreadId1);

		bRet = TRUE;
	} while (FALSE);
}

void* ptThreadFunc1(void *pParam)
{
	BOOL bRet = FALSE;

	int nRet = 0;
	struct timespec tv;
	THREAD_MUTEX_T *pMutex_t = NULL;

	do 
	{
		pthread_detach(pthread_self());
		pMutex_t = (THREAD_MUTEX_T *)pParam;
		if (pMutex_t == NULL)
		{
			bRet = FALSE;
			break;
		}

		while(TRUE)
		{
			tv.tv_sec = time(NULL);
			tv.tv_sec += 2;

			pthread_mutex_lock(&pMutex_t->ptMutex);
			nRet = pthread_cond_timedwait(&pMutex_t->ptCond, &pMutex_t->ptMutex, &tv);
			pthread_mutex_unlock(&pMutex_t->ptMutex);

			TRACE("wait:%d", nRet);
		}

		bRet = TRUE;
	} while (FALSE);

	if (pMutex_t != NULL)
	{
		delete pMutex_t;
		pMutex_t = NULL;
	}
	
	return NULL;
}