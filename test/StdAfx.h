#if !defined(AFX_STDAFX_H__DE93B492_AA7F_40D9_866C_4E02AF75EB6D__INCLUDED_)
#define AFX_STDAFX_H__DE93B492_AA7F_40D9_866C_4E02AF75EB6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN				// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>					// MFC core and standard components
#include <afxext.h>					// MFC extensions
#include <afxdisp.h>				// MFC Automation classes
#include <afxdtctl.h>				// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>					// MFC support for Windows Common Controls
#endif								// _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
//
#include <windows.h>

#pragma comment(lib, "version.lib")
#pragma warning(disable : 4275,4786,4996)


#include "Inc/PubClass.h"

//////////////////////////////////////////////////////////////////////////
//
#ifndef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#endif // !defined(AFX_STDAFX_H__DE93B492_AA7F_40D9_866C_4E02AF75EB6D__INCLUDED_)
