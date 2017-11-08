#if !defined(AFX_REDRAWTABCTRL_H__8D204DC8_DF94_4221_9D7E_723EEF80BC92__INCLUDED_)
#define AFX_REDRAWTABCTRL_H__8D204DC8_DF94_4221_9D7E_723EEF80BC92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RedrawTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRedrawTabCtrl window

class CRedrawTabCtrl : public CTabCtrl
{
// Construction
public:
	CRedrawTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRedrawTabCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRedrawTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRedrawTabCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDRAWTABCTRL_H__8D204DC8_DF94_4221_9D7E_723EEF80BC92__INCLUDED_)
