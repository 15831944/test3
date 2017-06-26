#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "../Inc/ETSLayout.h"
#include "../Control/DrawButton.h"

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

	void	Test1(void* pParam);
protected:
	static LRESULT 					EditWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	void							InitialLayout();
	
protected:
	WNDPROC			 				m_OldEditProc;	//FARPROC,WNDPROC
	HWND							m_hEditWnd;


private:
	CString							m_strDefaultPath;
	CString							m_strAppPath;
	std::vector<void*> m_vecList;
	
};
