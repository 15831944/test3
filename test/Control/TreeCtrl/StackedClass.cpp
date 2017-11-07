#include "stdafx.h"
#include "StackedClass.h"

CStackedClass::CStackedClass()
{
}

CStackedClass::~CStackedClass()
{
}

BOOL CStackedClass::AddPane(CWnd* pWndItem, CWnd* pParentWnd)
{
	if(pWndItem == NULL)
	{
		return FALSE;
	}
	
	if(pParentWnd == NULL)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CStackedClass::ShowPane()
{
	return TRUE;
}