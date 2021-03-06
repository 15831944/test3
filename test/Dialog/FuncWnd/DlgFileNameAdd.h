#ifndef __DLG_FILENAME_ADD_H__
#define __DLG_FILENAME_ADD_H__

class CDlgFileNameAdd : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileNameAdd)
public:
	CDlgFileNameAdd(CWnd* pParent = NULL);
	virtual ~CDlgFileNameAdd();

	BOOL					GetWndAddData(UPDATE_FILEDATA *pUpdateData);
	
public:
	enum { IDD = IDD_FILENAMEADD_DIALOG };
	
protected:
	virtual void			DoDataExchange(CDataExchange* pDX);   
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual BOOL			OnInitDialog();	
	
	afx_msg void			OnPaint();
	afx_msg BOOL			OnEraseBkgnd(CDC* pDC);

	afx_msg void			OnSize(UINT nType, int cx, int cy);
	afx_msg void			OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void			OnBnClickedCheckReverse();
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

	BOOL					SetConfigData();

private:
	BOOL					m_bInited;
	BOOL					m_bShowing;

	UPDATE_FILEDATA			m_stConfigData;
};

#endif