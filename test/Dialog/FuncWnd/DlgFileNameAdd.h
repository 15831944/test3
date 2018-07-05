#ifndef __DLG_FILENAME_ADD_H__
#define __DLG_FILENAME_ADD_H__

class CDlgFileNameAdd : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileNameAdd)
public:
	CDlgFileNameAdd(CWnd* pParent = NULL);
	virtual ~CDlgFileNameAdd();
	
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