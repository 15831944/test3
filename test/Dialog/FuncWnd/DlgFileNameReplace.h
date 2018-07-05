#ifndef __DLG_FILENAME_REPLACE_H__
#define __DLG_FILENAME_REPLACE_H__

class CDlgFileNameReplace : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileNameReplace)
public:
	CDlgFileNameReplace(CWnd* pParent = NULL);
	virtual ~CDlgFileNameReplace();
	
public:
	enum { IDD = IDD_FILENAMEREPLACE_DIALOG };
	
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