#include "stdafx.h"
#include "usb_io_test.h"

#include <WinIoCtl.h>
#include <Setupapi.h>
#pragma comment(lib, "Setupapi.lib")

//////////////////////////////////////////////////////////////////////////
//IDE
#define  DFP_RECEIVE_DRIVE_DATA   CTL_CODE(IOCTL_DISK_BASE, 0x0022, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define  FILE_DEVICE_SCSI           0x0000001B
#define  IOCTL_SCSI_MINIPORT_IDENTIFY      ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT        0x0004D008          //  see NTDDSCSI.H for definition
#define  IDE_ATA_IDENTIFY           0xEC     // ATA��IDָ��(IDENTIFY DEVICE)

typedef struct _IDINFO
{
	USHORT  wGenConfig;                 // WORD 0: ������Ϣ��
	USHORT  wNumCyls;                   // WORD 1: ������
	USHORT  wReserved2;                 // WORD 2: ����
	USHORT  wNumHeads;                  // WORD 3: ��ͷ��
	USHORT  wReserved4;                 // WORD 4: ����
	USHORT  wReserved5;                 // WORD 5: ����
	USHORT  wNumSectorsPerTrack;        // WORD 6: ÿ�ŵ�������
	USHORT  wVendorUnique[3];           // WORD 7-9: �����趨ֵ
	CHAR    sSerialNumber[20];          // WORD 10-19:���к�
	USHORT  wBufferType;                // WORD 20: ��������
	USHORT  wBufferSize;                // WORD 21: �����С
	USHORT  wECCSize;                   // WORD 22: ECCУ���С
	CHAR    sFirmwareRev[8];            // WORD 23-26: �̼��汾
	CHAR    sModelNumber[40];           // WORD 27-46: �ڲ��ͺ�
	USHORT  wMoreVendorUnique;          // WORD 47: �����趨ֵ
	USHORT  wReserved48;                // WORD 48: ����
	struct {
		USHORT  reserved1:8;
		USHORT  DMA:1;                  // 1=֧��DMA
		USHORT  LBA:1;                  // 1=֧��LBA
		USHORT  DisIORDY:1;             // 1=�ɲ�ʹ��IORDY
		USHORT  IORDY:1;                // 1=֧��IORDY
		USHORT  SoftReset:1;            // 1=��ҪATA������
		USHORT  Overlap:1;              // 1=֧���ص�����
		USHORT  Queue:1;                // 1=֧���������
		USHORT  InlDMA:1;               // 1=֧�ֽ����ȡDMA
	} wCapabilities;                    // WORD 49: һ������
	USHORT  wReserved1;                 // WORD 50: ����
	USHORT  wPIOTiming;                 // WORD 51: PIOʱ��
	USHORT  wDMATiming;                 // WORD 52: DMAʱ��
	struct {
		USHORT  CHSNumber:1;            // 1=WORD 54-58��Ч
		USHORT  CycleNumber:1;          // 1=WORD 64-70��Ч
		USHORT  UnltraDMA:1;            // 1=WORD 88��Ч
		USHORT  reserved:13;
	} wFieldValidity;                   // WORD 53: �����ֶ���Ч�Ա�־
	USHORT  wNumCurCyls;                // WORD 54: CHS��Ѱַ��������
	USHORT  wNumCurHeads;               // WORD 55: CHS��Ѱַ�Ĵ�ͷ��
	USHORT  wNumCurSectorsPerTrack;     // WORD 56: CHS��Ѱַÿ�ŵ�������
	USHORT  wCurSectorsLow;             // WORD 57: CHS��Ѱַ����������λ��
	USHORT  wCurSectorsHigh;            // WORD 58: CHS��Ѱַ����������λ��
	struct {
		USHORT  CurNumber:8;            // ��ǰһ���Կɶ�д������
		USHORT  Multi:1;                // 1=��ѡ���������д
		USHORT  reserved1:7;
	} wMultSectorStuff;                 // WORD 59: ��������д�趨
	ULONG  dwTotalSectors;              // WORD 60-61: LBA��Ѱַ��������
	USHORT  wSingleWordDMA;             // WORD 62: ���ֽ�DMA֧������
	struct {
		USHORT  Mode0:1;                // 1=֧��ģʽ0 (4.17Mb/s)
		USHORT  Mode1:1;                // 1=֧��ģʽ1 (13.3Mb/s)
		USHORT  Mode2:1;                // 1=֧��ģʽ2 (16.7Mb/s)
		USHORT  Reserved1:5;
		USHORT  Mode0Sel:1;             // 1=��ѡ��ģʽ0
		USHORT  Mode1Sel:1;             // 1=��ѡ��ģʽ1
		USHORT  Mode2Sel:1;             // 1=��ѡ��ģʽ2
		USHORT  Reserved2:5;
	} wMultiWordDMA;                    // WORD 63: ���ֽ�DMA֧������
	struct {
		USHORT  AdvPOIModes:8;          // ֧�ָ߼�POIģʽ��
		USHORT  reserved:8;
	} wPIOCapacity;                     // WORD 64: �߼�PIO֧������
	USHORT  wMinMultiWordDMACycle;      // WORD 65: ���ֽ�DMA�������ڵ���Сֵ
	USHORT  wRecMultiWordDMACycle;      // WORD 66: ���ֽ�DMA�������ڵĽ���ֵ
	USHORT  wMinPIONoFlowCycle;         // WORD 67: ��������ʱPIO�������ڵ���Сֵ
	USHORT  wMinPOIFlowCycle;           // WORD 68: ��������ʱPIO�������ڵ���Сֵ
	USHORT  wReserved69[11];            // WORD 69-79: ����
	struct {
		USHORT  Reserved1:1;
		USHORT  ATA1:1;                 // 1=֧��ATA-1
		USHORT  ATA2:1;                 // 1=֧��ATA-2
		USHORT  ATA3:1;                 // 1=֧��ATA-3
		USHORT  ATA4:1;                 // 1=֧��ATA/ATAPI-4
		USHORT  ATA5:1;                 // 1=֧��ATA/ATAPI-5
		USHORT  ATA6:1;                 // 1=֧��ATA/ATAPI-6
		USHORT  ATA7:1;                 // 1=֧��ATA/ATAPI-7
		USHORT  ATA8:1;                 // 1=֧��ATA/ATAPI-8
		USHORT  ATA9:1;                 // 1=֧��ATA/ATAPI-9
		USHORT  ATA10:1;                // 1=֧��ATA/ATAPI-10
		USHORT  ATA11:1;                // 1=֧��ATA/ATAPI-11
		USHORT  ATA12:1;                // 1=֧��ATA/ATAPI-12
		USHORT  ATA13:1;                // 1=֧��ATA/ATAPI-13
		USHORT  ATA14:1;                // 1=֧��ATA/ATAPI-14
		USHORT  Reserved2:1;
	} wMajorVersion;                    // WORD 80: ���汾
	USHORT  wMinorVersion;              // WORD 81: ���汾
	USHORT  wReserved82[6];             // WORD 82-87: ����
	struct {
		USHORT  Mode0:1;                // 1=֧��ģʽ0 (16.7Mb/s)
		USHORT  Mode1:1;                // 1=֧��ģʽ1 (25Mb/s)
		USHORT  Mode2:1;                // 1=֧��ģʽ2 (33Mb/s)
		USHORT  Mode3:1;                // 1=֧��ģʽ3 (44Mb/s)
		USHORT  Mode4:1;                // 1=֧��ģʽ4 (66Mb/s)
		USHORT  Mode5:1;                // 1=֧��ģʽ5 (100Mb/s)
		USHORT  Mode6:1;                // 1=֧��ģʽ6 (133Mb/s)
		USHORT  Mode7:1;                // 1=֧��ģʽ7 (166Mb/s) ???
		USHORT  Mode0Sel:1;             // 1=��ѡ��ģʽ0
		USHORT  Mode1Sel:1;             // 1=��ѡ��ģʽ1
		USHORT  Mode2Sel:1;             // 1=��ѡ��ģʽ2
		USHORT  Mode3Sel:1;             // 1=��ѡ��ģʽ3
		USHORT  Mode4Sel:1;             // 1=��ѡ��ģʽ4
		USHORT  Mode5Sel:1;             // 1=��ѡ��ģʽ5
		USHORT  Mode6Sel:1;             // 1=��ѡ��ģʽ6
		USHORT  Mode7Sel:1;             // 1=��ѡ��ģʽ7
	} wUltraDMA;                        // WORD 88:  Ultra DMA֧������
	USHORT    wReserved89[167];         // WORD 89-255
} IDINFO, *PIDINFO;

void AdjustString(char* str, int len)
{
	char ch;
	int i;

	// �����ߵ�
	for (i = 0; i < len; i += 2)
	{
		ch = str[i];
		str[i] = str[i + 1];
		str[i + 1] = ch;
	}

	// �����Ҷ���ģ�����Ϊ����� (ȥ����ߵĿո�)
	i = 0;
	while ((i < len) && (str[i] == ' ')) i++;

	::memmove(str, &str[i], len - i);

	// ȥ���ұߵĿո�
	i = len - 1;
	while ((i >= 0) && (str[i] == ' '))
	{
		str[i] = '/0';
		i--;
	}
}

BOOL IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo)
{
	PSENDCMDINPARAMS pSCIP;      // �������ݽṹָ��
	PSENDCMDOUTPARAMS pSCOP;     // ������ݽṹָ��
	DWORD dwOutBytes;            // IOCTL������ݳ���
	BOOL bResult;                // IOCTL����ֵ

	// ��������/������ݽṹ�ռ�
	pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDINPARAMS) - 1);
	pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);

	// ָ��ATA/ATAPI����ļĴ���ֵ
	//    pSCIP->irDriveRegs.bFeaturesReg = 0;
	//    pSCIP->irDriveRegs.bSectorCountReg = 0;
	//    pSCIP->irDriveRegs.bSectorNumberReg = 0;
	//    pSCIP->irDriveRegs.bCylLowReg = 0;
	//    pSCIP->irDriveRegs.bCylHighReg = 0;
	//    pSCIP->irDriveRegs.bDriveHeadReg = 0;
	pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;

	// ָ������/������ݻ�������С
	pSCIP->cBufferSize = 0;
	pSCOP->cBufferSize = sizeof(IDINFO);

	// IDENTIFY DEVICE
	bResult = ::DeviceIoControl(hDevice,        // �豸���
		DFP_RECEIVE_DRIVE_DATA,                 // ָ��IOCTL
		pSCIP, sizeof(SENDCMDINPARAMS) - 1,     // �������ݻ�����
		pSCOP, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1,    // ������ݻ�����
		&dwOutBytes,                // ������ݳ���
		(LPOVERLAPPED)NULL);        // ��ͬ��I/O

	// �����豸�����ṹ
	::memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));

	// �ͷ�����/������ݿռ�
	::GlobalFree(pSCOP);
	::GlobalFree(pSCIP);

	return bResult;
}

HANDLE OpenDevice(LPCTSTR filename)
{
	HANDLE hDevice;

	// ���豸
	hDevice = ::CreateFile(filename,            // �ļ���
		GENERIC_READ | GENERIC_WRITE,          // ��д��ʽ
		FILE_SHARE_READ | FILE_SHARE_WRITE,    // ����ʽ
		NULL,                    // Ĭ�ϵİ�ȫ������
		OPEN_EXISTING,           // ������ʽ
		0,                       // ���������ļ�����
		NULL);                   // �������ģ���ļ�

	return hDevice;
}

BOOL GetPhysicalDriveIDEInfo(const char* szDrive, PIDINFO pIdInfo)
{
	HANDLE hDevice;         // �豸���
	BOOL bResult;           // ���ؽ��
	char szFileName[20];    // �ļ���

	::sprintf(szFileName,"\\\\.\\%s", szDrive);
	hDevice = ::OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// IDENTIFY DEVICE
	bResult = ::IdentifyDevice(hDevice, pIdInfo);

	if (bResult)
	{
		// �����ַ���
		::AdjustString(pIdInfo->sSerialNumber, 20);
		::AdjustString(pIdInfo->sModelNumber, 40);
		::AdjustString(pIdInfo->sFirmwareRev, 8);
	}

	::CloseHandle (hDevice);

	return bResult;
}
//////////////////////////////////////////////////////////////////////////
//
#include <initguid.h> 
#include <cfgmgr32.h>

DEVINST GetDrivesDevInstByDeviceNumber(long DeviceNumber, UINT DriveType, char* szDosDeviceName)
{
	bool IsFloppy = (strstr(szDosDeviceName, "\\Floppy") != NULL); // who knows a better way?

	GUID* guid;

	switch (DriveType) {
	case DRIVE_REMOVABLE:
		if ( IsFloppy ) {
			guid = (GUID*)&GUID_DEVINTERFACE_FLOPPY;
		} else {
			guid = (GUID*)&GUID_DEVINTERFACE_DISK;
		}
		break;
	case DRIVE_FIXED:
		guid = (GUID*)&GUID_DEVINTERFACE_DISK;
		break;
	case DRIVE_CDROM:
		guid = (GUID*)&GUID_DEVINTERFACE_CDROM;
		break;
	default:
		return 0;
	}

	// Get device interface info set handle for all devices attached to system
	HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)	{
		return 0;
	}

	// Retrieve a context structure for a device interface of a device information set
	DWORD dwIndex = 0;
	long res;

	BYTE Buf[1024];
	PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)Buf;
	SP_DEVICE_INTERFACE_DATA         spdid;
	SP_DEVINFO_DATA                  spdd;
	DWORD                            dwSize;
	
	spdid.cbSize = sizeof(spdid);

	while ( true )	{
		res = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwIndex, &spdid);
		if ( !res ) {
			break;
		}

		dwSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL); // check the buffer size

		if ( dwSize!=0 && dwSize<=sizeof(Buf) ) {

			pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!

			ZeroMemory(&spdd, sizeof(spdd));
			spdd.cbSize = sizeof(spdd);

			long res = SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd, dwSize, &dwSize, &spdd);
			if ( res ) {

				// in case you are interested in the USB serial number:
				// the device id string contains the serial number if the device has one,
				// otherwise a generated id that contains the '&' char...
				/*
				DEVINST DevInstParent = 0;
				CM_Get_Parent(&DevInstParent, spdd.DevInst, 0); 
				char szDeviceIdString[MAX_PATH];
				CM_Get_Device_ID(DevInstParent, szDeviceIdString, MAX_PATH, 0);
				printf("DeviceId=%s\n", szDeviceIdString);
				*/

				// open the disk or cdrom or floppy
				HANDLE hDrive = CreateFile(pspdidd->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				if ( hDrive != INVALID_HANDLE_VALUE ) {
					// get its device number
					STORAGE_DEVICE_NUMBER sdn;
					DWORD dwBytesReturned = 0;
					res = DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
					if ( res ) {
						if ( DeviceNumber == (long)sdn.DeviceNumber ) {  // match the given device number with the one of the current device
							CloseHandle(hDrive);
							SetupDiDestroyDeviceInfoList(hDevInfo);
							return spdd.DevInst;
						}
					}
					CloseHandle(hDrive);
				}
			}
		}
		dwIndex++;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);

	return 0;
}

BOOL GetPhysicalDriveUSBInfo1(const char* szDrive)
{
	HANDLE hDevice;
	BOOL bResult;

	char szFileName[20];    // �ļ���
	char szDosDeviceName[MAX_PATH];

	DWORD dwBytesReturned = 0;
	long DeviceNumber = -1;
	long res = -1;
	UINT DriveType = 0;

	HDEVINFO    deviceInfo;
	STORAGE_DEVICE_NUMBER sdn;

	::sprintf(szFileName,"\\\\.\\%s", szDrive);
	hDevice = ::OpenDevice(szFileName);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (!DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL))
	{
		return FALSE;
	}

	DeviceNumber = sdn.DeviceNumber;

	CloseHandle(hDevice);

	if ( DeviceNumber == -1 ) {
		return FALSE;
	}

	DriveType = GetDriveType(szDrive);
	if (!QueryDosDevice(szDrive, szDosDeviceName, MAX_PATH))
	{
		return FALSE;
	}

	DEVINST DevInst = GetDrivesDevInstByDeviceNumber(DeviceNumber, DriveType, szDosDeviceName);
	if (DevInst == 0)
	{
		return FALSE;
	}

	PNP_VETO_TYPE VetoType = PNP_VetoTypeUnknown; 
	WCHAR VetoNameW[MAX_PATH];
	VetoNameW[0] = 0;
	bool bSuccess = false;

	DEVINST DevInstParent = 0;
	CM_Get_Parent(&DevInstParent, DevInst, 0); 
// 
// 	for ( long tries=1; tries<=3; tries++ ) 
// 	{
// 		VetoNameW[0] = 0;
// 		res =CM_Request_Device_EjectW(DevInstParent, &VetoType, VetoNameW, MAX_PATH, 0);
// 
// 		bSuccess = (res==CR_SUCCESS && VetoType==PNP_VetoTypeUnknown);
// 		if ( bSuccess )  { 
// 			break;
// 		}
// 
// 		Sleep(500);
// 	}
// 
// 	if ( bSuccess ) {
// 		printf("Success\n\n");
// 		return 0;
// 	}
// 
// 	printf("Result=0x%2X\n", res);
// 
// 	if ( VetoNameW[0] ) {
// 		printf("VetoName=%ws)\n\n", VetoNameW);
// 	}	
}

BOOL GetPhysicalDriveUSBInfo2(const char* szDrive)
{
	BOOL bRet = FALSE;

	DWORD dwRet = 0;
	HANDLE hDevice;

	CString strDevName;
	strDevName.Format("\\\\.\\%s", szDrive);

	hDevice = OpenDevice(strDevName);
	if(hDevice == NULL || hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
}
void usb_io_test1()
{
	//	IDINFO IDINFO;
//	GetPhysicalDriveIDEInfo("C:", &IDINFO);

//	GetPhysicalDriveUSBInfo1("H:");	
	GetPhysicalDriveUSBInfo2("H:");
}