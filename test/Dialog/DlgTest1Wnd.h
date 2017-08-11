#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "afxshelltreeCtrl.h"
#include "../Control/DrawButton.h"
#include "../Control/ShellCtrlClass.h"

typedef int (_stdcall *CALLRING_CALLBACK_FUNC)(const char* pszFilePath, int nAudioCard);

using namespace std;
class CDlgTest1Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest1Wnd)
public:
	CDlgTest1Wnd(CWnd* pParent = NULL);					
	virtual ~CDlgTest1Wnd();

	static CDlgTest1Wnd& Instance();

public:
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);   
	virtual BOOL					OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void					OnBnClickedButton1();
	afx_msg void					OnBnClickedButton2();
	afx_msg void					OnBnClickedButton3();

protected:
	static BOOL						GetShellTreePath(char* pszShellPath);
	static LRESULT 					EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	void							InitialLayout();
	void							SetShellTreePath(const char* pszShellPath);
	
protected:
	CALLRING_CALLBACK_FUNC			m_pfCallRingFunc;
	WNDPROC			 				m_OldEditProc;	//FARPROC,WNDPROC
	HWND							m_hEditWnd;

	CShellTreeCtrl					m_hSysDirTree;
	CShellListCtrl					m_hSysDirList;

private:
	CString							m_strDefaultPath;
	CString							m_strShellPath;
	CString							m_strAppPath;
	
	std::map<std::string, int*>		m_mapInt;
};
