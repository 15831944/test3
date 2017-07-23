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
#define  IDE_ATA_IDENTIFY           0xEC     // ATA的ID指令(IDENTIFY DEVICE)

typedef struct _IDINFO
{
	USHORT  wGenConfig;                 // WORD 0: 基本信息字
	USHORT  wNumCyls;                   // WORD 1: 柱面数
	USHORT  wReserved2;                 // WORD 2: 保留
	USHORT  wNumHeads;                  // WORD 3: 磁头数
	USHORT  wReserved4;                 // WORD 4: 保留
	USHORT  wReserved5;                 // WORD 5: 保留
	USHORT  wNumSectorsPerTrack;        // WORD 6: 每磁道扇区数
	USHORT  wVendorUnique[3];           // WORD 7-9: 厂家设定值
	CHAR    sSerialNumber[20];          // WORD 10-19:序列号
	USHORT  wBufferType;                // WORD 20: 缓冲类型
	USHORT  wBufferSize;                // WORD 21: 缓冲大小
	USHORT  wECCSize;                   // WORD 22: ECC校验大小
	CHAR    sFirmwareRev[8];            // WORD 23-26: 固件版本
	CHAR    sModelNumber[40];           // WORD 27-46: 内部型号
	USHORT  wMoreVendorUnique;          // WORD 47: 厂家设定值
	USHORT  wReserved48;                // WORD 48: 保留
	struct {
		USHORT  reserved1:8;
		USHORT  DMA:1;                  // 1=支持DMA
		USHORT  LBA:1;                  // 1=支持LBA
		USHORT  DisIORDY:1;             // 1=可不使用IORDY
		USHORT  IORDY:1;                // 1=支持IORDY
		USHORT  SoftReset:1;            // 1=需要ATA软启动
		USHORT  Overlap:1;              // 1=支持重叠操作
		USHORT  Queue:1;                // 1=支持命令队列
		USHORT  InlDMA:1;               // 1=支持交叉存取DMA
	} wCapabilities;                    // WORD 49: 一般能力
	USHORT  wReserved1;                 // WORD 50: 保留
	USHORT  wPIOTiming;                 // WORD 51: PIO时序
	USHORT  wDMATiming;                 // WORD 52: DMA时序
	struct {
		USHORT  CHSNumber:1;            // 1=WORD 54-58有效
		USHORT  CycleNumber:1;          // 1=WORD 64-70有效
		USHORT  UnltraDMA:1;            // 1=WORD 88有效
		USHORT  reserved:13;
	} wFieldValidity;                   // WORD 53: 后续字段有效性标志
	USHORT  wNumCurCyls;                // WORD 54: CHS可寻址的柱面数
	USHORT  wNumCurHeads;               // WORD 55: CHS可寻址的磁头数
	USHORT  wNumCurSectorsPerTrack;     // WORD 56: CHS可寻址每磁道扇区数
	USHORT  wCurSectorsLow;             // WORD 57: CHS可寻址的扇区数低位字
	USHORT  wCurSectorsHigh;            // WORD 58: CHS可寻址的扇区数高位字
	struct {
		USHORT  CurNumber:8;            // 当前一次性可读写扇区数
		USHORT  Multi:1;                // 1=已选择多扇区读写
		USHORT  reserved1:7;
	} wMultSectorStuff;                 // WORD 59: 多扇区读写设定
	ULONG  dwTotalSectors;              // WORD 60-61: LBA可寻址的扇区数
	USHORT  wSingleWordDMA;             // WORD 62: 单字节DMA支持能力
	struct {
		USHORT  Mode0:1;                // 1=支持模式0 (4.17Mb/s)
		USHORT  Mode1:1;                // 1=支持模式1 (13.3Mb/s)
		USHORT  Mode2:1;                // 1=支持模式2 (16.7Mb/s)
		USHORT  Reserved1:5;
		USHORT  Mode0Sel:1;             // 1=已选择模式0
		USHORT  Mode1Sel:1;             // 1=已选择模式1
		USHORT  Mode2Sel:1;             // 1=已选择模式2
		USHORT  Reserved2:5;
	} wMultiWordDMA;                    // WORD 63: 多字节DMA支持能力
	struct {
		USHORT  AdvPOIModes:8;          // 支持高级POI模式数
		USHORT  reserved:8;
	} wPIOCapacity;                     // WORD 64: 高级PIO支持能力
	USHORT  wMinMultiWordDMACycle;      // WORD 65: 多字节DMA传输周期的最小值
	USHORT  wRecMultiWordDMACycle;      // WORD 66: 多字节DMA传输周期的建议值
	USHORT  wMinPIONoFlowCycle;         // WORD 67: 无流控制时PIO传输周期的最小值
	USHORT  wMinPOIFlowCycle;           // WORD 68: 有流控制时PIO传输周期的最小值
	USHORT  wReserved69[11];            // WORD 69-79: 保留
	struct {
		USHORT  Reserved1:1;
		USHORT  ATA1:1;                 // 1=支持ATA-1
		USHORT  ATA2:1;                 // 1=支持ATA-2
		USHORT  ATA3:1;                 // 1=支持ATA-3
		USHORT  ATA4:1;                 // 1=支持ATA/ATAPI-4
		USHORT  ATA5:1;                 // 1=支持ATA/ATAPI-5
		USHORT  ATA6:1;                 // 1=支持ATA/ATAPI-6
		USHORT  ATA7:1;                 // 1=支持ATA/ATAPI-7
		USHORT  ATA8:1;                 // 1=支持ATA/ATAPI-8
		USHORT  ATA9:1;                 // 1=支持ATA/ATAPI-9
		USHORT  ATA10:1;                // 1=支持ATA/ATAPI-10
		USHORT  ATA11:1;                // 1=支持ATA/ATAPI-11
		USHORT  ATA12:1;                // 1=支持ATA/ATAPI-12
		USHORT  ATA13:1;                // 1=支持ATA/ATAPI-13
		USHORT  ATA14:1;                // 1=支持ATA/ATAPI-14
		USHORT  Reserved2:1;
	} wMajorVersion;                    // WORD 80: 主版本
	USHORT  wMinorVersion;              // WORD 81: 副版本
	USHORT  wReserved82[6];             // WORD 82-87: 保留
	struct {
		USHORT  Mode0:1;                // 1=支持模式0 (16.7Mb/s)
		USHORT  Mode1:1;                // 1=支持模式1 (25Mb/s)
		USHORT  Mode2:1;                // 1=支持模式2 (33Mb/s)
		USHORT  Mode3:1;                // 1=支持模式3 (44Mb/s)
		USHORT  Mode4:1;                // 1=支持模式4 (66Mb/s)
		USHORT  Mode5:1;                // 1=支持模式5 (100Mb/s)
		USHORT  Mode6:1;                // 1=支持模式6 (133Mb/s)
		USHORT  Mode7:1;                // 1=支持模式7 (166Mb/s) ???
		USHORT  Mode0Sel:1;             // 1=已选择模式0
		USHORT  Mode1Sel:1;             // 1=已选择模式1
		USHORT  Mode2Sel:1;             // 1=已选择模式2
		USHORT  Mode3Sel:1;             // 1=已选择模式3
		USHORT  Mode4Sel:1;             // 1=已选择模式4
		USHORT  Mode5Sel:1;             // 1=已选择模式5
		USHORT  Mode6Sel:1;             // 1=已选择模式6
		USHORT  Mode7Sel:1;             // 1=已选择模式7
	} wUltraDMA;                        // WORD 88:  Ultra DMA支持能力
	USHORT    wReserved89[167];         // WORD 89-255
} IDINFO, *PIDINFO;

void AdjustString(char* str, int len)
{
	char ch;
	int i;

	// 两两颠倒
	for (i = 0; i < len; i += 2)
	{
		ch = str[i];
		str[i] = str[i + 1];
		str[i + 1] = ch;
	}

	// 若是右对齐的，调整为左对齐 (去掉左边的空格)
	i = 0;
	while ((i < len) && (str[i] == ' ')) i++;

	::memmove(str, &str[i], len - i);

	// 去掉右边的空格
	i = len - 1;
	while ((i >= 0) && (str[i] == ' '))
	{
		str[i] = '/0';
		i--;
	}
}

BOOL IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo)
{
	PSENDCMDINPARAMS pSCIP;      // 输入数据结构指针
	PSENDCMDOUTPARAMS pSCOP;     // 输出数据结构指针
	DWORD dwOutBytes;            // IOCTL输出数据长度
	BOOL bResult;                // IOCTL返回值

	// 申请输入/输出数据结构空间
	pSCIP = (PSENDCMDINPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDINPARAMS) - 1);
	pSCOP = (PSENDCMDOUTPARAMS)::GlobalAlloc(LMEM_ZEROINIT, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1);

	// 指定ATA/ATAPI命令的寄存器值
	//    pSCIP->irDriveRegs.bFeaturesReg = 0;
	//    pSCIP->irDriveRegs.bSectorCountReg = 0;
	//    pSCIP->irDriveRegs.bSectorNumberReg = 0;
	//    pSCIP->irDriveRegs.bCylLowReg = 0;
	//    pSCIP->irDriveRegs.bCylHighReg = 0;
	//    pSCIP->irDriveRegs.bDriveHeadReg = 0;
	pSCIP->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;

	// 指定输入/输出数据缓冲区大小
	pSCIP->cBufferSize = 0;
	pSCOP->cBufferSize = sizeof(IDINFO);

	// IDENTIFY DEVICE
	bResult = ::DeviceIoControl(hDevice,        // 设备句柄
		DFP_RECEIVE_DRIVE_DATA,                 // 指定IOCTL
		pSCIP, sizeof(SENDCMDINPARAMS) - 1,     // 输入数据缓冲区
		pSCOP, sizeof(SENDCMDOUTPARAMS) + sizeof(IDINFO) - 1,    // 输出数据缓冲区
		&dwOutBytes,                // 输出数据长度
		(LPOVERLAPPED)NULL);        // 用同步I/O

	// 复制设备参数结构
	::memcpy(pIdInfo, pSCOP->bBuffer, sizeof(IDINFO));

	// 释放输入/输出数据空间
	::GlobalFree(pSCOP);
	::GlobalFree(pSCIP);

	return bResult;
}

HANDLE OpenDevice(LPCTSTR filename)
{
	HANDLE hDevice;

	// 打开设备
	hDevice = ::CreateFile(filename,            // 文件名
		GENERIC_READ | GENERIC_WRITE,          // 读写方式
		FILE_SHARE_READ | FILE_SHARE_WRITE,    // 共享方式
		NULL,                    // 默认的安全描述符
		OPEN_EXISTING,           // 创建方式
		0,                       // 不需设置文件属性
		NULL);                   // 不需参照模板文件

	return hDevice;
}

BOOL GetPhysicalDriveIDEInfo(const char* szDrive, PIDINFO pIdInfo)
{
	HANDLE hDevice;         // 设备句柄
	BOOL bResult;           // 返回结果
	char szFileName[20];    // 文件名

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
		// 调整字符串
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

	char szFileName[20];    // 文件名
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