#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#define	SKINUI_VERSION							1
#define GETSYSTEM_FONT							0x1000

#define WM_TEST1WND_CTRL						WM_USER + 0x1000
#define WM_TEST2WND_CTRL						WM_USER + 0x1001
#define WM_TEST3WND_CTRL						WM_USER + 0x1002
#define WM_TEST4WND_CTRL						WM_USER + 0x1003

//////////////////////////////////////////////////////////////////////////
//
#define countof(arr)						(sizeof(arr)/sizeof(arr[0]))
#define SIZEOF_ARRAY(array)					(sizeof(array) / sizeof(array[0]))

#define DIV_ROUND_UP(x, d)					(((x) + (d) - 1) / (d))
#define ROUND_UP(x, d)						(DIV_ROUND_UP(x, d) * (d))

#ifndef NOMINMAX
	#ifndef max
		#define max(a,b)					(((a) > (b)) ? (a) : (b))
	#endif

	#ifndef min
		#define min(a,b)					(((a) < (b)) ? (a) : (b))
	#endif
#endif

#define MAX_EDITCHAR_SIZE					16
//////////////////////////////////////////////////////////////////////////
//
typedef struct{
	BOOL			bFlag;
	HWND			hWnd;
	DWORD			dwParam;
	char			szValue[MAX_PATH];
	char			szAddInfo[MAX_PATH];
	LOGFONT			hLogFont;
}HT_TESTWND_MSG;

typedef struct{
	HWND			hWnd;
	DWORD			dwProcessID;
}HT_ENUM_WNDINFO;

typedef struct{
	USHORT			usTransId;
	USHORT			usFlag;
	USHORT			usQuestionCount;
	USHORT			usAnswerCount;
	USHORT			usAuthorityCount;
	USHORT			usAdditionalCount;
}HT_DNS_HEADER;

typedef int (_stdcall *CALLRING_CALLBACK_FUNC)(const char* pszFilePath, int nAudioCard);
#endif