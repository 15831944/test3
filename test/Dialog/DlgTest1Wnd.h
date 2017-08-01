#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "../Control/DrawButton.h"

typedef int (_stdcall *CALLRING_CALLBACK_FUNC)(const char* pszFilePath, int nAudioCard);

using namespace std;
class CDlgTest1Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest1Wnd)
public:
	CDlgTest1Wnd(CWnd* pParent = NULL);					
	virtual ~CDlgTest1Wnd();

public:
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);   
	virtual BOOL					OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void					OnBnClickedButton1();
	afx_msg void					OnBnClickedButton2();
	afx_msg void					OnBnClickedButton3();
	afx_msg void					OnBnClickedButton4();
	afx_msg void					OnBnClickedButton5();
	afx_msg void					OnBnClickedButton6();
	afx_msg void					OnBnClickedButton7();

protected:
	static LRESULT 					EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	void							InitialLayout();
	
protected:
	CALLRING_CALLBACK_FUNC			m_pfCallRingFunc;
	WNDPROC			 				m_OldEditProc;	//FARPROC,WNDPROC
	HWND							m_hEditWnd;

private:
	CString							m_strDefaultPath;
	CString							m_strAppPath;
	std::map<std::string, int*>		m_mapInt;
};
