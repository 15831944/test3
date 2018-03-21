#ifndef __USERLISTBOX_H__
#define __USERLISTBOX_H__

class CUserListBox : public CListBox
{
public:
	CUserListBox();
	virtual ~CUserListBox();
	
public:
	int						AddString(LPCTSTR lpstString);
	int						InsertItem(int nIndex, LPCTSTR lpszString);	
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual void 			DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void			MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);

	afx_msg void			OnDestroy();	

protected:
#pragma pack(1)
	typedef struct _LISTBOX_DATA{
		DWORD		dwItemData;
		LPVOID		pData;
		int			nImage;
		UINT		nFormat;
		DWORD		dwFlags;
		_LISTBOX_DATA::_LISTBOX_DATA()
		{
			::ZeroMemory(this, sizeof(_LISTBOX_DATA));
		}
	}LISTBOX_DATA;
#pragma pack()

protected:
	int						SetListItemData(int nIndex, DWORD dwItemData, LPVOID lpData, int nImage, DWORD dwFlag, BYTE byMask);
};
#endif