#ifndef __DLG_FILENAME_DEL_H__
#define __DLG_FILENAME_DEL_H__

class CDlgFileNameDel : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileNameDel)
public:
	CDlgFileNameDel(CWnd* pParent = NULL);
	virtual ~CDlgFileNameDel();
	
	BOOL					GetWndAddData(UPDATE_FILEDATA *pUpdateData);

public:
	enum { IDD = IDD_FILENAMEDEL_DIALOG };
	
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

	BOOL					SetConfigData();

private:
	BOOL					m_bInited;
	BOOL					m_bShowing;	

	UPDATE_FILEDATA			m_stConfigData;
};

#endif