#ifndef __UKEYWORK_THREAD_H__
#define __UKEYWORK_THREAD_H__

void	UKeyWorkInitialize();
void	UKeyWorkFinalize();

bool	openUKeyWorkProc(CK_UKEYVERIFY_CALLBACK_FUNC pfUkeyVerify, CK_UKEYREAD_CALLBACK_FUNC pfUkeyReadData, CK_UKEYWRITE_CALLBACK_FUNC pfUKeyWriteData, HANDLE &hWorkHandle);
bool	closeUKeyWorkProc();

#endif
