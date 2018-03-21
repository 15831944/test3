
#include <Windows.h>

BOOL WINAPI DllMain(
					_In_ HINSTANCE hinstDLL, // 指向自身的句柄
					_In_ DWORD fdwReason, // 调用原因
					_In_ LPVOID lpvReserved // 隐式加载和显式加载
					)
{
	return TRUE;
}