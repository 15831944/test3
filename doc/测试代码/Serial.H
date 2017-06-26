#if !defined(_COMM_ACCESS_FUNCTIONS_AND_DATA)
#define _COMM_ACCESS_FUNCTIONS_AND_DATA

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define EVENTCHAR               0x0d
#define MAXBLOCKLENGTH          59//130

extern BYTE XwCom;
extern BYTE  sCom1[5],sCom2[MAXBLOCKLENGTH+12],sCom3[MAXBLOCKLENGTH+12];
extern BYTE opation;
extern short ComNum;
//COMSTAT ComStat;
// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04


// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

class CSerial
{
public:
	HANDLE m_hCom;
	BYTE m_bComId;

	BYTE m_bByteSize;
	BYTE m_bStopBits;
	BYTE m_bParity;
	DWORD m_dwBaudRate;
	//WORD m_fChEvt;
	char m_bEvtChar;
	DWORD m_fBinary;

	BOOL m_bConnected;
	BOOL m_fXonXoff;
	BOOL m_bFlowCtrl;

	OVERLAPPED	m_rdos;
	OVERLAPPED  m_wtos;
	//functions
	CSerial();
	CSerial(BYTE bComId,BYTE bByteSize,BYTE bStopBits,BYTE bParity,
		DWORD dwBaudRate,/*WORD fChEvt,*/char bEvtChar,DWORD fBinary);
	BOOL OpenConnection();
	BOOL CloseConnection();
	BOOL SetupConnection();
	BOOL IsConnected();
  }; 
/******************************************************
GLOBAL FUNCTIONS
******************************************************/
BOOL WriteCommBlock( CSerial& comDev, LPSTR lpByte , DWORD dwBytesToWrite);
int ReadCommBlock(CSerial& comDev,LPSTR lpszBlock, int nMaxLength );

#endif