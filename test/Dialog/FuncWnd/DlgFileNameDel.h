#ifndef __DLG_FILENAME_DEL_H__
#define __DLG_FILENAME_DEL_H__

class CDlgFileNameDel : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileNameDel)
public:
	CDlgFileNameDel(CWnd* pParent = NULL);
	virtual ~CDlgFileNameDel();
	
public:
	enum { IDD = IDD_FILENAMEDEL_DIALOG };
	
protected:
	virtual void					DoDataExchange(CDataExchange* pDX);   
	virtual BOOL					PreTranslateMessage(MSG* pMsg);
	virtual BOOL					OnInitDialog();	
	
	afx_msg void					OnPaint();
	DECLARE_MESSAGE_MAP()
	
protected:
	BOOL							InitCtrl();
	BOOL							InitInfo();

	BOOL							InitWndSkin();
	BOOL							CreateChildWnd();
	
private:
	BOOL							m_bInited;	
public:
	afx_msg void OnEnChangeEdit1();
};

#endif