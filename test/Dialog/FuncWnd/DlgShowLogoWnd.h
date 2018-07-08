#ifndef __DLG_SHOWLOGO_WND_H__
#define __DLG_SHOWLOGO_WND_H__

class CDlgShowLogoWnd : public CDialog
{
	DECLARE_DYNAMIC(CDlgShowLogoWnd)
public:
	CDlgShowLogoWnd(CWnd* pParent = NULL);
	virtual ~CDlgShowLogoWnd();
	
public:
	enum { IDD = IDD_FILENAMEADD_DIALOG };
	
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
};

#endif