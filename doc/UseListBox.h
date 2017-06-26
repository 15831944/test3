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
		CString strAppend;	//�ڶ��Ÿ�������
		LPTSTR	hIcon;		//ͼ��
		CRect	buttomRC;	//��ť����
		BOOL	bState;		//��ť״̬
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
	} List_AppendData;	//List_AppendDataΪ_ListBox_Data�ṹ��ı���
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	void InsertStr(CString str,int iIndex,CString strAppend,UINT uIcon,BOOL state);//UINT uIconΪͼ��ID
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


