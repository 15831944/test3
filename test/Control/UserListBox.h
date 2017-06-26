#ifndef __USERLISTBOX_H__
#define __USERLISTBOX_H__

class CUserListBox : public CListBox
{
public:
	CUserListBox();
	virtual ~CUserListBox();
	
public:
	void	InsertItem(int nIndex, LPCTSTR lpszItem, LPCTSTR lpszAppend, UNIT uIcon, BOOL state);	
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();	

protected:
	typedef struct _LISTBOX_DATA{
		CString	strAppend;
		LPTSTR	szIcon;
		CRect	rcRect;
		BOOL	bState;
		_LISTBOX_DATA()
		{
			bState = TRUE;
			rcRect.left  = 0;
			rcRect.top   = 0;
			rcRect.right = 0;
			rcRect.bottom= 0;
			strAppend = _T("");
			szIcon    = NULL;
		}
	}LISTBOX_DATA;
	
private:
	BOOL	m_bDown;	
};
#endif