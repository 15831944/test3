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

	BOOL					UpdateConfigInfo(BOOL bFlag);
	BOOL					GetCurConfigData(UPDATE_FILEDATA *pUpdateData);

public:
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();

	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void			OnPaint();
	afx_msg BOOL			OnEraseBkgnd(CDC* pDC);

	afx_msg void			OnSize(UINT nType, int cx, int cy);
	afx_msg void			OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void			OnBnClickedButtonRun();
	afx_msg void			OnBnClickedBtnOpenFloder();

	afx_msg void			OnCbnSelchangeComboEvalname();
	afx_msg void			OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

protected:
	static BOOL				GetUpdateFileData(void *pUpdateData, void *pParentObject);
	static BOOL				GetShellTreePath(char* pszShellPath, void *pParentObject);
	
	static LRESULT 			EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

protected:
	BOOL					Init();
	BOOL					InitCtrl();
	BOOL					InitInfo();

	BOOL					CreateChildWnd();
	BOOL					InitWndSkin();
	BOOL					InitWndInfo();

	BOOL					UpdateWndCtrl();
	BOOL					UpdateWndInfo();

	void					SetWndControlLayout();
	BOOL					DrawWndImage(CDC *pDC);

protected:
	BOOL					SetChildWnd(BOOL bFlag);
	BOOL					SetCtrlInfo(HTREEITEM hItem);
	
protected:
	CComboBox				m_hComboEval;
	CMFCShellTreeCtrl 		m_hSysDirTree;		//CMFCShellTreeCtrl
	CReportCtrl 			m_hSysDirList;		//CMFCShellListCtrl

	CDlgShowLogoWnd			m_dlgShowLogoWnd;
	CDlgFileNameAdd			m_dlgFileNameAdd;
	CDlgFileNameDate		m_dlgFileNameDate;
	CDlgFileNameDel			m_dlgFileNameDel;
	CDlgFileNameExt			m_dlgFileNameExt;
	CDlgFileNameIndex		m_dlgFileNameIndex;
	CDlgFileNameReplace		m_dlgFileNameReplace;

private:
	BOOL					m_bInited;
	BOOL					m_bShowing;
	
	int						m_nPrePage;
	int						m_nDefaultSel;
	int						m_nCurSelIndex;

	HWND					m_hEditWnd;
	CPtrArray				m_pArPage;

	CString					m_strAppPath;
	CString					m_strShellPath;
	CString					m_strDefaultPath;

	WNDPROC			 		m_OldEditProc;	//FARPROC,WNDPROC
	std::vector<UPDATE_FILEINFO*> m_vecFileInfo;
};
