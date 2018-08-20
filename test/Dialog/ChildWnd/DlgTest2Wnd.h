#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "../FuncWnd/DlgShowLogoWnd.h"
#include "../FuncWnd/DlgFileNameAdd.h"
#include "../FuncWnd/DlgFileNameDate.h"
#include "../FuncWnd/DlgFileNameDel.h"
#include "../FuncWnd/DlgFileNameExt.h"
#include "../FuncWnd/DlgFileNameIndex.h"
#include "../FuncWnd/DlgFileNameReplace.h"

using namespace std;
class CDlgTest2Wnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest2Wnd)
public:
	CDlgTest2Wnd(CWnd* pParent = NULL);					
	virtual ~CDlgTest2Wnd();

public:
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void					DoDataExchange(CDataExchange* pDX);   
	virtual BOOL					OnInitDialog();

	afx_msg void					OnSize(UINT nType, int cx, int cy);
	afx_msg void					OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void					OnBnClickedBtnOpenFloder();
	afx_msg void					OnBnClickedBtnRunModify();
	afx_msg void					OnCbnSelchangeComboEvalname();
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL						GetShellTreePath(char* pszShellPath, void* pParam);
	static LRESULT 					EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

protected:
	BOOL							Init();

	BOOL							InitCtrl();
	BOOL							InitInfo();

	BOOL							CreateChildWnd();
	BOOL							InitWndSkin();

	BOOL							SetWndCtrlInfo();
	void							SetWndControlLayout();
	
protected:
	CComboBox						m_hComboEval;
	CShellTreeCtrl					m_hSysDirTree;
	CShellListCtrl					m_hSysDirList;

	CDlgShowLogoWnd					m_dlgShowLogoWnd;
	CDlgFileNameAdd					m_dlgFileNameAdd;
	CDlgFileNameDate				m_dlgFileNameDate;
	CDlgFileNameDel					m_dlgFileNameDel;
	CDlgFileNameExt					m_dlgFileNameExt;
	CDlgFileNameIndex				m_dlgFileNameIndex;
	CDlgFileNameReplace				m_dlgFileNameReplace;

private:
	BOOL							m_bInited;
	BOOL							m_bShowing;
	
	int								m_nIndex;
	int								m_nPrePage;
	int								m_nDefaultSel;

	HWND							m_hEditWnd;
	CPtrArray						m_pArPage;

	CString							m_strAppPath;
	CString							m_strShellPath;
	CString							m_strDefaultPath;

	WNDPROC			 				m_OldEditProc;	//FARPROC,WNDPROC
	CALLRING_CALLBACK_FUNC			m_pfCallRingFunc;
};
