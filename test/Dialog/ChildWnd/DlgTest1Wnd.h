#pragma once
#include "afxcmn.h"
#include "afxwin.h"

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
	afx_msg void					OnBnClickedBtnOpenFloder();
	void							OnBnClickedBtnRunModify();
	afx_msg void					OnCbnSelchangeComboEvalname();

protected:
	static BOOL						GetShellTreePath(char* pszShellPath, void* pParam);
	static LRESULT 					EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

protected:
	BOOL							Init();

	BOOL							InitCtrl();
	BOOL							InitInfo();
	
protected:
	CComboBox						m_hComboEval;
	CShellTreeCtrl					m_hSysDirTree;
	CShellListCtrl					m_hSysDirList;

private:
	HWND							m_hEditWnd;

	CString							m_strAppPath;
	CString							m_strShellPath;
	CString							m_strDefaultPath;

	//UPDATE_EVALTYPE					m_emEvalType;
	WNDPROC			 				m_OldEditProc;	//FARPROC,WNDPROC
	CALLRING_CALLBACK_FUNC			m_pfCallRingFunc;
};
