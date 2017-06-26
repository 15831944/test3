#pragma once


// CUseListBox

class CUseListBox : public CListBox
{
	DECLARE_DYNAMIC(CUseListBox)

public:
	CUseListBox();
	virtual ~CUseListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bDown;
	typedef struct _ListBox_Data 
	{
		CString strAppend;	//第二排附加数据
		LPTSTR	hIcon;		//图标
		CRect	buttomRC;	//按钮矩形
		BOOL	bState;		//按钮状态
		_ListBox_Data()
		{
			bState = TRUE;
			buttomRC.left = 0;
			buttomRC.top = 0;
			buttomRC.right = 0;
			buttomRC.bottom = 0;
			strAppend = _T("");
			hIcon = NULL;
		}
	} List_AppendData;	//List_AppendData为_ListBox_Data结构体的别名
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	void InsertStr(CString str,int iIndex,CString strAppend,UINT uIcon,BOOL state);//UINT uIcon为图标ID
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


