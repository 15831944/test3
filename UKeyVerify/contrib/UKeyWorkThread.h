#ifndef __UKEYWORK_THREAD_H__
#define __UKEYWORK_THREAD_H__

void	UKeyWorkInitialize(CK_UKEYHANDLE *pUKeyHandle);
void	UKeyWorkFinalize(CK_UKEYHANDLE *pUKeyHandle);

bool	openUKeyWorkProc(CK_UKEYHANDLE *pUKeyHandle);
bool	closeUKeyWorkProc(CK_UKEYHANDLE *pUKeyHandle);

bool	enumUKeyDevice(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex, CK_ULONG *pulSlotId, bool *pbIsVerify, HANDLE *pUKeyReadEvent, HANDLE *pUKeyWriteEvent);
bool	verifyUKeyDevice(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId);

bool	readUKeyDeviceData(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, HANDLE hUKeyReadEvent);
bool	writeUKeyDeviceData(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, HANDLE hUKeyWriteEvent);
#endif
