#include "stdafx.h"
#include "com232.h"

BYTE  XwCom=0x40;
BYTE  sCom1[5],sCom2[MAXBLOCKLENGTH+12],sCom3[MAXBLOCKLENGTH+12];
BYTE  opation;
short ComNum;


CSerial::CSerial()
{
	m_hCom = NULL;
	m_bComId = (char)ComNum;	//COM1

	m_bByteSize=8;
	m_bStopBits=ONESTOPBIT;
	m_bParity=NOPARITY;
	m_dwBaudRate=9600;
	m_bEvtChar=EVENTCHAR;
	m_fBinary=1;

	m_bConnected = FALSE;
	m_bFlowCtrl = FC_XONXOFF ;
	m_fXonXoff = FALSE;
}
CSerial::CSerial(BYTE bComId,BYTE bByteSize,BYTE bStopBits,BYTE bParity,
		DWORD dwBaudRate,char bEvtChar,DWORD fBinary)
{
	m_hCom = NULL;
	m_bComId = bComId;

	m_bByteSize=bByteSize;
	m_bStopBits=bStopBits;
	m_bParity=bParity;
	m_dwBaudRate=dwBaudRate;
	m_bEvtChar=bEvtChar;
	m_fBinary=fBinary;

	m_bConnected = FALSE;

	m_bFlowCtrl = FC_XONXOFF ;
	m_fXonXoff = FALSE;

}

BOOL CSerial::OpenConnection()
{
	char csCom[10];
    COMMTIMEOUTS  CommTimeOuts ;

    if((m_bComId < 0) || (m_bComId > 4))
		return FALSE; //从COM1到COM4
	if(m_hCom)        //串口已经打开
		return FALSE;
    //OVERLAPPED包含异步I/O信息
	m_rdos.Offset = 0;
	m_rdos.OffsetHigh = 0;
	m_rdos.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_rdos.hEvent == NULL)
		return FALSE;
	m_wtos.Offset = 0;
	m_wtos.OffsetHigh = 0;
	m_wtos.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_wtos.hEvent == NULL)
	{
		CloseHandle(m_rdos.hEvent);
		return FALSE;
	}
	
	wsprintf(csCom,"COM%d",m_bComId);
	m_hCom = CreateFile(csCom,GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
    NULL
	);

	if(m_hCom == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	else
	{
       SetCommMask( m_hCom, EV_RXCHAR ) ;
      // 分配设备缓冲区
      SetupComm( m_hCom, 4096, 4096 ) ;

      // 初始化缓冲区中的信息
      PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT |
                         PURGE_TXCLEAR | PURGE_RXCLEAR) ;

      // set up for overlapped I/O
	  DWORD dwTemp = 1000 / (this->m_dwBaudRate / 8);
      CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
      CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
      CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
     
      CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/this->m_dwBaudRate;//( npTTYInfo ) ;
      CommTimeOuts.WriteTotalTimeoutConstant = 0;//1000 ;
      SetCommTimeouts( m_hCom, &CommTimeOuts ) ;
	}
	if(!SetupConnection())
	{
		CloseConnection();
		return FALSE;
	}
	EscapeCommFunction( m_hCom, SETDTR );
	m_bConnected = TRUE;
	return TRUE;
}

BOOL CSerial::CloseConnection()
{
   if (NULL == m_hCom)
      return ( TRUE ) ;
   // 设置连接属性为FALSE
	m_bConnected = FALSE;
   // 使事件消息无效并等待线程停止
   SetCommMask( m_hCom, 0 ) ;

   // 阻塞至线程停止
   EscapeCommFunction( m_hCom, CLRDTR ) ;

   PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT |
                      PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
   CloseHandle( m_hCom ) ;
	m_hCom = NULL;
	CloseHandle(m_rdos.hEvent);
	CloseHandle(m_wtos.hEvent);

   return ( TRUE ) ;
}
BOOL CSerial::SetupConnection()
{
   BOOL       fRetVal ;
   BYTE       bSet ;
   DCB        dcb ;
	if(m_hCom == NULL)
		return FALSE; 
   dcb.DCBlength = sizeof( DCB ) ;

   GetCommState( m_hCom, &dcb ) ;

   dcb.BaudRate = this->m_dwBaudRate;
   dcb.ByteSize = this->m_bByteSize;
   dcb.Parity =  this->m_bParity;
   dcb.StopBits = this->m_bStopBits ;
	dcb.EvtChar = this->m_bEvtChar ;
   // 设置硬件流控制
   bSet = (BYTE) ((m_bFlowCtrl & FC_DTRDSR) != 0) ;
   dcb.fOutxDsrFlow = bSet ;
   if (bSet)
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;
   else
      dcb.fDtrControl = DTR_CONTROL_ENABLE ;

   bSet = (BYTE) ((m_bFlowCtrl & FC_RTSCTS) != 0) ;
	dcb.fOutxCtsFlow = bSet ;
   if (bSet)
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
   else
      dcb.fRtsControl = RTS_CONTROL_ENABLE ;
   // 设置软件流控制
   bSet = (BYTE) ((m_bFlowCtrl & FC_XONXOFF) != 0) ;
   dcb.fInX = dcb.fOutX = bSet ;
   dcb.XonChar = ASCII_XON ;
   char xon = ASCII_XON ;
   dcb.XoffChar = ASCII_XOFF ;
    char xoff = ASCII_XOFF ;
    dcb.XonLim = 100 ;
   dcb.XoffLim = 100 ;
   // 其他设置

   dcb.fBinary = TRUE ;
   dcb.fParity = TRUE ;

   fRetVal = SetCommState( m_hCom, &dcb ) ;

   return ( fRetVal ) ;

} 

BOOL CSerial::IsConnected()
{
	return m_bConnected;
}
// 全局函数


int ReadCommBlock(CSerial& comDev,LPSTR lpszBlock, int nMaxLength )
{
   BOOL       fReadStat ;

   COMSTAT    ComStat ;
   DWORD      dwErrorFlags;
   DWORD      dwLength;
   DWORD      dwError;
   char       szError[ 10 ] ;
   
   ClearCommError( comDev.m_hCom, &dwErrorFlags, &ComStat ) ;
   dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

   if (dwLength > 0)
   {
      fReadStat = ReadFile( comDev.m_hCom, lpszBlock,
		                    dwLength, &dwLength, &(comDev.m_rdos) ) ;
      if (!fReadStat)
      {
         if (GetLastError() == ERROR_IO_PENDING)
         {
            OutputDebugString("\n\rIO Pending");
            // 必须等到数据读取完成，由于设置了
            // CommTimeOuts.ReadTotalTimeoutConstant变量
			// 这个函数受超时限制
            while(!GetOverlappedResult( comDev.m_hCom ,
               &(comDev.m_rdos), &dwLength, TRUE ))
            {
               dwError = GetLastError();
               if(dwError == ERROR_IO_INCOMPLETE)
                  continue;
               else
               {
                  wsprintf( szError, "<CE-%u>", dwError ) ;
                  ClearCommError( comDev.m_hCom , &dwErrorFlags, &ComStat ) ;
                  break;
               }

            }

	      }
         else
         {
            // 发生其他错误
			 dwLength = 0 ;
            ClearCommError( comDev.m_hCom , &dwErrorFlags, &ComStat ) ;
         }
      }
   }

   return (dwLength) ;
} 


BOOL WriteCommBlock( CSerial& comDev, LPSTR lpByte , DWORD dwBytesToWrite)
{

   BOOL        fWriteStat ;
   DWORD       dwBytesWritten ;
   DWORD       dwErrorFlags;
   DWORD   	dwError;
   DWORD       dwBytesSent=0;
   COMSTAT     ComStat;
   char        szError[ 128 ] ;

   fWriteStat = WriteFile( comDev.m_hCom , lpByte, dwBytesToWrite,
                           &dwBytesWritten, &( comDev.m_wtos) ) ;

   if (!fWriteStat)
   {
      if(GetLastError() == ERROR_IO_PENDING)
      {
         while(!GetOverlappedResult( comDev.m_hCom,
            &(comDev.m_wtos), &dwBytesWritten, TRUE ))
         {
            dwError = GetLastError();
            if(dwError == ERROR_IO_INCOMPLETE)
            {
               dwBytesSent += dwBytesWritten;
               continue;
            }
            else
            {
               wsprintf( szError, "<CE-%u>", dwError ) ;
               ClearCommError( comDev.m_hCom, &dwErrorFlags, &ComStat ) ;
               break;
            }
         }

         dwBytesSent += dwBytesWritten;

         if( dwBytesSent != dwBytesToWrite )
             wsprintf(szError,"\nProbable Write Timeout: Total of %ld bytes sent", dwBytesSent);
         else
             wsprintf(szError,"\n%ld bytes written", dwBytesSent);

         OutputDebugString(szError);

      }
      else
      {
         ClearCommError( comDev.m_hCom, &dwErrorFlags, &ComStat ) ;
         return ( FALSE );
      }
   }
   return ( TRUE );
} 