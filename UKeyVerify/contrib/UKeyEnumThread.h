#ifndef __UKEYENUM_THREAD_H__
#define __UKEYENUM_THREAD_H__

void	UKeyEnumInitialize();
void	UKeyEnumFinalize();

bool 	openUKeyEnumProc(CK_UKEYENUM_CALLBACK_FUNC pfUKeyEnum, HANDLE &hEnumHandle);
bool	closeUKeyEnumProc();

#endif