//http://blog.csdn.net/xujiezhige/article/details/6206133
void test1()
{
#if 0
	CVideoWndThread* pVideoWndThread = (CVideoWndThread*)AfxBeginThread(RUNTIME_CLASS(CVideoWndThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	ASSERT_VALID(pVideoWndThread);

	pVideoWndThread->m_bAutoDelete = FALSE;	//不让线程delete this
	pVideoWndThread->ResumeThread();

	if (WaitForSingleObject(pVideoWndThread->m_hThread, INFINITE) == WAIT_OBJECT_0)
	{
		pVideoWndThread->m_bAutoDelete = TRUE;
		pVideoWndThread->Delete();
	}
#endif
}

void test2()
{
#if 0
	CString strDiskInfo;
	unsigned int nDrvIndex = 0;
	
	char szModelNo[MAX_PATH]  = {0};
	char szSerialNo[MAX_PATH] = {0};
	
	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return;
	}

	if (!pGlobal->GetDiskInfo(nDrvIndex, szModelNo, szSerialNo))
	{
		return;
	}

	strDiskInfo.Format(_T("硬盘型号:%s \n硬盘序列号:%s"), szModelNo, szSerialNo);
	AfxMessageBox(strDiskInfo);
	
#endif
}

void test3()
{
#if 0
	int nPos = 0;
	int nPrePos = 0;

	DWORD dwFlag = 0;
	DWORD dwOffset = 0;
	
	BOOL bRet = FALSE;
	BOOL bPreExist  = FALSE;
	BOOL bIsP2pCall = FALSE;
	BOOL bIsThreePartyCall   = FALSE;
	BOOL bIsRingGroupCall	 = FALSE;
	BOOL bIsPickupCall		 = FALSE;
	BOOL bIsUnPackCall		 = FALSE;
	BOOL bIsOrdinaryMeeting  = FALSE;
	BOOL bIsAnnouncementCall = FALSE;
	BOOL bIsScheduledMeeting = FALSE;
	
	char* p = NULL;
	char chTag[3] = {0};
	const char* pszCallTag = {"HTMLYDN"};
	string s1 = "1234567HY";

	p = (char*)pszCallTag;
	while(*p != '\0')
	{
		if (!bPreExist)
		{
			dwOffset = 0;
		}
		else
		{
			dwOffset = nPrePos;
		}

		nPos = s1.find(*p, dwOffset);
		if (nPos != string::npos)
		{
			if (!bPreExist)
			{
				if (*p == 'H')
				{//会议
					bIsOrdinaryMeeting = TRUE;
				}
				else if (*p == 'T')
				{//通播
					bIsAnnouncementCall = TRUE;
				}
				else if (*p == 'M')
				{//三方通话
					bIsThreePartyCall = TRUE;
				}
				else if (*p == 'L')
				{//振铃组
					bIsRingGroupCall = TRUE;
				}
				else if (*p == 'D')
				{//代接
					bIsPickupCall = TRUE;
				}
				else if (*p == 'N')
				{//强插
					bIsUnPackCall = TRUE;
				}

				bRet = TRUE;
				nPrePos = nPos;
				bPreExist = TRUE;
				
				memset(chTag, 0x0, 3);
				sprintf(chTag, _T("%c"), *p);

				dwFlag = dwFlag << 1;
				dwFlag |= 1;

				p++;
				continue;
			}
			else
			{
				dwFlag = dwFlag << 1;
				dwFlag |= 1;

				if (*p == 'Y' && strcmp(chTag, _T("H")) == 0)
				{
					bRet = TRUE;
					bIsScheduledMeeting = TRUE;
					sprintf(chTag, _T("HY"));
					break;
				}
				else
				{
					bRet = FALSE;
					break;
				}
			}
		}

		p++;
	}
#endif	
}

void test4()
{
#if 0
	CGlobalInfo* pGlobal = CGlobalInfo::CreateInstance();
	if (pGlobal == NULL)
	{
		return;
	}

	char s1[MAX_PATH] = {0};
	unsigned int n1 = MAX_PATH;

	pGlobal->DNSLookupInfo("192.168.2.222", "www.baidu.com", s1, &n1);
#endif	
}

void test5()
{
#if 0
	DWORD dwProcessID = 0;
	HT_ENUM_WNDINFO hWndInfo;

	HWND hFind = ::FindWindow("dd", NULL);
	if (hFind == NULL)
	{
		return;
	}

	::GetWindowThreadProcessId(hFind, &dwProcessID);

	//::BringWindowToTop(hFind);

	/*
	AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),TRUE);
	::ShowWindow(hFind, SW_SHOWNORMAL);
	::SetForegroundWindow(hFind); 
	::SetFocus(hFind); 
	AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),FALSE);
	*/

	/*
	typedef void (WINAPI *PROCSWITCHTOTHISWINDOW) (HWND, BOOL);
	PROCSWITCHTOTHISWINDOW SwitchToThisWindow; 

	HMODULE hUser32 = GetModuleHandle("user32");
	SwitchToThisWindow    =    (PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");
	SwitchToThisWindow(hFind, TRUE);
	*/

	hWndInfo.hWnd = hFind;
	hWndInfo.dwProcessID = dwProcessID;
	EnumWindows(EnumWindowsProc, (LPARAM)&hWndInfo);
#endif	
}

void test6()
{
}