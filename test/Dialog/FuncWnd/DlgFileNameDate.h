#ifndef __DLG_FILENAME_DATE_H__
#define __DLG_FILENAME_DATE_H__

class CDlgFileNameDate : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileNameDate)
public:
	CDlgFileNameDate(CWnd* pParent = NULL);
	virtual ~CDlgFileNameDate();
	
public:
	enum { IDD = IDD_FILENAMEDATE_DIALOG };
	
protected:
	virtual void			DoDataExchange(CDataExchange* pDX);   
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();	
	
	afx_msg void			OnPaint();
	afx_msg BOOL			OnEraseBkgnd(CDC* pDC);

	afx_msg void			OnSize(UINT nType, int cx, int cy);
	afx_msg void			OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
	
protected:
	BOOL					InitCtrl();
	BOOL					InitInfo();

	BOOL					InitWndSkin();
	BOOL					InitWndInfo();

	BOOL					UpdateWndCtrl();
	BOOL					UpdateWndInfo();

	void					SetWndControlLayout();
	BOOL					DrawWndImage(CDC *pDC);
	
private:
	BOOL					m_bInited;
	BOOL					m_bShowing;
};

#endif