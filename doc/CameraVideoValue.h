#ifndef __CAMERAVIDEOVALUE_H__
#define __CAMERAVIDEOVALUE_H__

#include "thread_queue.h"

class CameraVideoValue
{
public:
	CameraVideoValue();
	~CameraVideoValue();

public:
	BOOL							    SetTokenSNValue(LPCTSTR lpszValue);
	BOOL								SetTokenLCDValue(LPCTSTR lpszValue);

	BOOL								GetTokenAutoValue(CString &strTokenSN, CString &strTokenLCD);
	BOOL								GetTokenManualValue(CString &strTokenSN, CString &strTokenLCD);

private:
	CString								m_strCameraTokenSN;
	CString								m_strCameraTokenLCD;

	thread_queue<CAMERA_DATA_BUFFER*>   m_threadCameraQueue;
};

#endif