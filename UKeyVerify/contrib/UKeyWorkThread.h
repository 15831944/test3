#ifndef __UKEYWORK_THREAD_H__
#define __UKEYWORK_THREAD_H__

void	UKeyWorkInitialize(CK_UKEYHANDLE *pUKeyHandle);
void	UKeyWorkFinalize(CK_UKEYHANDLE *pUKeyHandle);

bool	openUKeyWorkProc(CK_UKEYHANDLE *pUKeyHandle);
bool	closeUKeyWorkProc(CK_UKEYHANDLE *pUKeyHandle);

bool	enumUKeyDevice(CK_UKEYHANDLE *pUKeyHandle, int iSlotIndex, CK_ULONG *pulSlotId, CK_UKEYDEVICETYPE &emUKeyType, HANDLE *pUKeyReadEvent, HANDLE *pUKeyWriteEvent);
bool	verifyUKeyDevice(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYDEVICETYPE emUKeyType);
bool	closeUKeySession(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId);

bool	readUKeyDeviceData(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYDEVICETYPE emUKeyType, HANDLE hUKeyReadEvent);
bool	writeUKeyDeviceData(CK_UKEYHANDLE *pUKeyHandle, CK_ULONG ulSlotId, CK_UKEYDEVICETYPE emUKeyType, HANDLE hUKeyWriteEvent);
#endif
