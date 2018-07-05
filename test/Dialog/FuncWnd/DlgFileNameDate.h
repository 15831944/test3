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