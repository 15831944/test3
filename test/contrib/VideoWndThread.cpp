#include "stdafx.h"
#include "VideoWndThread.h"

IMPLEMENT_DYNCREATE(CVideoWndThread, CWinThread)

CVideoWndThread::CVideoWndThread()
{

}

CVideoWndThread::~CVideoWndThread()
{

}

BEGIN_MESSAGE_MAP(CVideoWndThread, CWinThread)
	//{{AFX_MSG_MAP(CVideoWndThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CVideoWndThread::InitInstance()
{
	return TRUE;
}

int CVideoWndThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

int CVideoWndThread::Run()
{
	return CWinThread::Run();
}