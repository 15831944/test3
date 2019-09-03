#ifndef __UKEYENUM_THREAD_H__
#define __UKEYENUM_THREAD_H__

void	UKeyEnumInitialize(CK_UKEYHANDLE *pUKeyHandle);
void	UKeyEnumFinalize(CK_UKEYHANDLE *pUKeyHandle);

bool 	openUKeyEnumProc(CK_UKEYHANDLE *pUKeyHandle);
bool	closeUKeyEnumProc(CK_UKEYHANDLE *pUKeyHandle);

#endif