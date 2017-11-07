#ifndef __STACKEDCLASS_H__
#define __STACKEDCLASS_H__

class CStackedClass
{
public:
	CStackedClass();
	~CStackedClass();
	
public:
	BOOL					AddPane(CWnd* pWndItem, CWnd* pParentWnd);
	BOOL					ShowPane();
};

#endif