#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include "afxshelltreeCtrl.h"
#include "../Control/Button/DrawButton.h"
#include "../Control/TreeCtrl/ShellCtrlClass.h"
#include "../contrib/update_file_name.h"

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
	afx_msg void					OnCbnSelchangeComboEvalname();
	afx_msg void					OnBnClickedButton1();
	afx_msg void					OnBnClickedButton2();
	afx_msg void					OnBnClickedButton3();

	afx_msg LRESULT					OnUpdateFileName(WPARAM wParam, LPARAM lParam);
protected:
	static BOOL						GetShellTreePath(char* pszShellPath, void* pParam);
	static LRESULT 					EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	void							Init();
	void							InitCtrl();
	void							InitInfo();

	void							InitialLayout();
	
protected:
	CALLRING_CALLBACK_FUNC			m_pfCallRingFunc;
	WNDPROC			 				m_OldEditProc;	//FARPROC,WNDPROC
	HWND							m_hEditWnd;

	CShellTreeCtrl					m_hSysDirTree;
	CShellListCtrl					m_hSysDirList;
	CComboBox						m_hComboEval;

	update_file_name				m_hUpdateFile;
	UPDATE_EVALTYPE					m_hEvalType;

private:
	CString							m_strDefaultPath;
	CString							m_strShellPath;
	CString							m_strAppPath;	
};
