#include "stdafx.h"
#include "CameraVideoValue.h"

CameraVideoValue::CameraVideoValue()
{
	m_strCameraTokenSN  = _T("");
	m_strCameraTokenLCD = _T("");
}

CameraVideoValue::~CameraVideoValue()
{
	while(!m_threadCameraQueue.IsEmpty())
	{
		CAMERA_DATA_BUFFER* pBuf = (CAMERA_DATA_BUFFER*)m_threadCameraQueue.Front();
		if (pBuf != NULL)
		{
			delete pBuf;
			pBuf = NULL;
		}

		m_threadCameraQueue.Pop();
	}
}

BOOL CameraVideoValue::SetTokenSNValue(LPCTSTR lpszValue)
{
	DWORD dwValueLen;

	CString strTokenSN;
	CString strTokenLCD;
	
	if (lpszValue==NULL)
	{
		return FALSE;
	}
	
	dwValueLen = strlen(lpszValue);
	if (dwValueLen>TOKENSN_MAX_LEN || dwValueLen<TOKENSN_MIN_LEN)
	{
		return FALSE;
	}

	if (m_strCameraTokenLCD == _T(""))			//´æÔÚÎÊÌâ, 
	{
		return FALSE;
	}

	m_strCameraTokenSN = lpszValue;

	strTokenSN  = m_strCameraTokenSN;
	strTokenLCD = m_strCameraTokenLCD;

	CAMERA_DATA_BUFFER* pBuf = new CAMERA_DATA_BUFFER;
	if (pBuf == NULL)
	{
		return FALSE;			 
	}
	memset(pBuf, 0x0, sizeof(CAMERA_DATA_BUFFER));

	memcpy(pBuf->szTokenSN,  strTokenSN.GetBuffer(0),  strTokenSN.GetLength());
	memcpy(pBuf->szTokenLCD, strTokenLCD.GetBuffer(0), strTokenLCD.GetLength());

	m_threadCameraQueue.Push(pBuf);

	return TRUE;
}

BOOL CameraVideoValue::SetTokenLCDValue(LPCTSTR lpszValue)
{
	BOOL bRet = FALSE;
	DWORD dwValueLen;

	CString strText;	

	if (lpszValue==NULL)
	{
		return FALSE;
	}

	dwValueLen = strlen(lpszValue);
	if (dwValueLen > TOKENLCD_MAX_LEN || dwValueLen < TOKENLCD_MAX_LEN)
	{
		return FALSE;
	}	

	if (strcmp(lpszValue, m_strCameraTokenLCD.GetBuffer(0)) == 0)
	{
		return FALSE;
	}
	
	m_strCameraTokenLCD = lpszValue;
	return bRet;
}

BOOL CameraVideoValue::GetTokenAutoValue(CString &strTokenSN, CString &strTokenLCD)
{
	if (m_threadCameraQueue.IsEmpty())
	{
		return FALSE;
	}

	CAMERA_DATA_BUFFER *pBuf = (CAMERA_DATA_BUFFER*)m_threadCameraQueue.Front();	//TRANSMIT_DATA_BUFFER
	if (pBuf == NULL)
	{
		return FALSE;
	}

	strTokenSN  = pBuf->szTokenSN;
	strTokenLCD = pBuf->szTokenLCD;

	delete pBuf;
	pBuf = NULL;

	m_threadCameraQueue.Pop();
	return TRUE;
}

BOOL CameraVideoValue::GetTokenManualValue(CString &strTokenSN, CString &strTokenLCD)
{
	if (m_threadCameraQueue.IsEmpty())
	{
		m_strCameraTokenSN  = _T("");
		m_strCameraTokenLCD = _T("");

		Sleep(500);

		if (m_threadCameraQueue.IsEmpty())
		{
			return FALSE;
		}
	}

	CAMERA_DATA_BUFFER *pBuf = (CAMERA_DATA_BUFFER*)m_threadCameraQueue.Front();	//TRANSMIT_DATA_BUFFER
	if (pBuf == NULL)
	{
		return FALSE;
	}

	strTokenSN  = pBuf->szTokenSN;
	strTokenLCD = pBuf->szTokenLCD;

	delete pBuf;
	pBuf = NULL;

	m_threadCameraQueue.Pop();

	return TRUE;
}