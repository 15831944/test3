#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#define	SKINUI_VERSION			1
#define GETSYSTEM_FONT			0x1000
#define MAX_EDITCHAR_SIZE		16

#define WM_TEST1WND_CTRL		WM_USER + 0x1000
#define WM_TEST2WND_CTRL		WM_USER + 0x1001
#define WM_TEST3WND_CTRL		WM_USER + 0x1002
#define WM_TEST4WND_CTRL		WM_USER + 0x1003

//////////////////////////////////////////////////////////////////////////
//
#if defined _WIN64 || __x86_64 || __LP64__
	#define _VER_64_
#endif

#if defined _MSC_VER
	#define _WIN_32_
#else
	#define _GCC_
	#if defined(__APPLE__)
		#define _APPLE_
	#endif	
#endif	

#define L_(x) L__(x)
#define L__(x) L ## x

#define countof(arr)			(sizeof(arr)/sizeof(arr[0]))
#define SIZEOF_ARRAY(array)		(sizeof(array) / sizeof(array[0]))

#define DIV_ROUND_UP(x, d)		(((x) + (d) - 1) / (d))
#define ROUND_UP(x, d)			(DIV_ROUND_UP(x, d) * (d))

#ifndef NOMINMAX
	#ifndef max
		#define max(a,b)		(((a) > (b)) ? (a) : (b))
	#endif

	#ifndef min
		#define min(a,b)		(((a) < (b)) ? (a) : (b))
	#endif
#endif

#if defined  _WIN_32_
	typedef signed   __int8		v_int8_t;
	typedef unsigned __int8		v_uint8_t;
	typedef	signed   __int16	v_int16_t;
	typedef unsigned __int16	v_uint16_t;
	typedef signed	 __int32	v_int32_t;
	typedef unsigned __int32	v_uint32_t;
	typedef signed   __int64	v_int64_t;
	typedef unsigned __int64	v_uint64_t;
#elif defined  _GCC_
	typedef int8_t				v_int8_t;
	typedef uint8_t				v_uint8_t;
	typedef int16_t				v_int16_t;
	typedef uint16_t			v_uint16_t;
	typedef int32_t				v_int32_t;
	typedef uint32_t			v_uint32_t;
	typedef int64_t				v_int64_t;
	typedef uint64_t			v_uint64_t;
#endif

#ifdef 	_VER_64_
	typedef v_int64_t			v_int;
	typedef v_int64_t			v_sint;
	typedef v_uint64_t			v_uint;
#else
	typedef v_int32_t			v_int;
	typedef v_int32_t			v_sint;
	typedef v_uint32_t			v_uint;
#endif

typedef	v_int64_t				v_pos_t;

#if defined _WIN_32_
	#include <intrin.h>
#elif defined _GCC_
	#include <x86intrin.h>
	#include <stdint.h>
	#include <stddef.h>
	#include <wchar.h>
	
	#define abstract
	#define __thiscall
	#define __forceinline inline
	
	#define	_I8_MIN				((v_int8_t)0x80)	
	#define _I8_MAX				((v_int8_t)0x7F)
	#define _UI8_MAX			((v_uint8_t)0xFF)
	
	#define _I16_MIN    		((vint16_t)0x8000)
	#define _I16_MAX    		((vint16_t)0x7FFF)
	#define _UI16_MAX   		((vuint16_t)0xFFFF)

	#define _I32_MIN    		((vint32_t)0x80000000)
	#define _I32_MAX    		((vint32_t)0x7FFFFFFF)
	#define _UI32_MAX   		((vuint32_t)0xFFFFFFFF)
	
	#define _I64_MIN    		((vint64_t)0x8000000000000000L)
	#define _I64_MAX    		((vint64_t)0x7FFFFFFFFFFFFFFFL)
	#define _UI64_MAX   		((vuint64_t)0xFFFFFFFFFFFFFFFFL)
#endif

#ifdef 	_VER_64_
	#define ITOA_S				_i64toa_s
	#define ITOW_S				_i64tow_s	
	#define I64TOA_S			_i64toa_s
	#define I64TOW_S			_i64tow_s
	#define UITOA_S				_ui64toa_s
	#define UITOW_S				_ui64tow_s
	#define UI64TOA_S			_ui64toa_s
	#define UI64TOW_S			_ui64tow_s
	
	#if defined _WIN_32_
		#define INCRC(x)		(_InterlockedIncrement64(x))
		#define DECRC(x)		(_InterlockedDecrement64(x))
	#elif defined _GCC_
		#define INCRC(x)		(__sync_add_and_fetch(x, 1))
		#define DECRC(x)		(__sync_sub_and_fetch(x, 1))
	#endif
#else
	#define ITOA_S				_itoa_s
	#define ITOW_S				_itow_s
	#define I64TOA_S			_i64toa_s
	#define I64TOW_S			_i64tow_s
	#define UITOA_S				_ui64toa_s
	#define UITOW_S				_ui64tow_s
	#define UI64TOA_S			_ui64toa_s
	#define UI64TOW_S			_ui64tow_s
	
	#if defined _WIN_32_
		#define INCRC(x)		(_InterlockedIncrement((volatile long*)(x)))
		#define DECRC(x)		(_InterlockedDecrement((volatile long*)(x)))
	#elif defined _GCC_
		#define INCRC(x)		(__sync_add_and_fetch(x, 1))
		#define DECRC(x)		(__sync_sub_and_fetch(x, 1))
	#endif
#endif	
//////////////////////////////////////////////////////////////////////////
//
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