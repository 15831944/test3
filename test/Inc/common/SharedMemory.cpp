// SharedMemory.cpp: implementation of the CSharedMemory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SharedMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharedMemory::CSharedMemory()
{
	m_strName.Empty();
	m_nSize = -1;
	m_hShmFile = NULL;
	m_hMutex   = NULL;
	m_pData    = NULL;
}

CSharedMemory::~CSharedMemory()
{
}

BOOL CSharedMemory::Create(LPCSTR lpName, int nSize, BOOL* pbExist/* =NULL */)
{
	BOOL bRet = FALSE;
	HANDLE hShareFile, hMutex;
	CString szShareName, szMutexName;
	void*   pData;  

//	assert(m_hShmFile == NULL);
//	assert(m_pData == NULL);

	szMutexName.Format(_T("MTX_%s"), lpName);
	hMutex = CreateMutex(NULL, FALSE, szMutexName);
	if (hMutex != NULL)
	{
		szShareName.Format(_T("SHM_%s"), lpName);
		hShareFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, szShareName);
		if (hShareFile != NULL && pbExist != NULL)
		{
			*pbExist = (GetLastError() == ERROR_ALREADY_EXISTS) ? TRUE : FALSE;

			pData = MapViewOfFile(hShareFile, FILE_MAP_WRITE, 0, 0, nSize);
			if (pData != NULL)
			{
				m_strName = lpName;
				m_nSize   = nSize;
				m_hShmFile = hShareFile;
				m_hMutex  = hMutex;
				m_pData   = pData;

				bRet = TRUE;
			}
		}
		else
		{
			CloseHandle(hShareFile);
			hShareFile = NULL;
		}
	}
	else
	{
		CloseHandle(hMutex);
		hMutex = NULL;
	}
	return bRet;
}

void CSharedMemory::Destroy()
{
//	assert(m_hShmFile != NULL);  
//	assert(m_pData != NULL); 
	
	if (UnmapViewOfFile(m_pData))  
	{
		if (CloseHandle(m_hShmFile) && CloseHandle(m_hMutex))
		{
			m_strName.Empty();
			m_nSize = -1;
			m_hShmFile = NULL;
			m_hMutex   = NULL;
			m_pData    = NULL;
		}
	}
}

BOOL CSharedMemory::IsExist(LPCSTR lpName)
{
	BOOL bRet = FALSE;
	HANDLE  hShmFile;  
	CString strShmName;  
     
	strShmName.Format("SHM_%s", lpName);  
	hShmFile = OpenFileMapping(PAGE_NOACCESS, 0, strShmName);  
	if (hShmFile != NULL)  
	{
		CloseHandle(hShmFile);
		bRet = TRUE;
	}

	return bRet;
}

PBYTE CSharedMemory::GetData(LPCSTR lpName, int nSize)
{
	HANDLE  hShmFile;
	CString strShmName;

	strShmName.Format(_T("SHM_%s"), lpName);
	hShmFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, strShmName);
	if (hShmFile != NULL)
	{
		PBYTE pData = (PBYTE)MapViewOfFile(hShmFile, FILE_MAP_ALL_ACCESS, 0, 0, nSize);  
		if (pData != NULL)
		{
			CloseHandle(hShmFile);
			return pData;
		}
	}
	return NULL;
}

PBYTE CSharedMemory::GetData(int nOffset /* = 0 */)
{
	return (PBYTE)m_pData + nOffset;;
}

BOOL CSharedMemory::SetData(PBYTE pData, int nOffset /* = 0 */, int nSize /* = -1 */)
{
	BOOL bRet = FALSE;

	if (nOffset < 0 || nOffset >= m_nSize)
	{
		return FALSE;
	}

	if (nSize == -1) 
	{
		nSize = m_nSize - nOffset;
	}

	if (nSize < 0)  
	{
		return FALSE;
	}

	if (nOffset + nSize > m_nSize)
	{
		return FALSE;
	}

	Lock();  
	memcpy((PBYTE)m_pData + nOffset, pData, nSize);  
	Unlock();
	return TRUE;
}

BOOL CSharedMemory::SetLong(DWORD dwData, int nOffset /* = 0 */)
{
	return SetData((PBYTE)&dwData, nOffset, sizeof(dwData));;
}

DWORD CSharedMemory::GetLong(int nOffset /* = 0 */)
{
	DWORD dwData;
	Lock();  
	dwData = *(DWORD*)GetData(nOffset);  
	Unlock();  

	return dwData;
}

BOOL CSharedMemory::SetString(CString strData, int nOffset /* = 0 */, int nSize /* = -1 */)
{
	int nLen = 0; 
	if (nSize == -1) 
	{
		nLen = strData.GetLength() + 1;  
	}
	else
	{
		nLen = min(strData.GetLength() + 1, nSize);  
	}

	if (nLen < 0)  
	{
		return FALSE;
	}
	return  SetData((PBYTE)(LPCSTR)strData, nOffset, nLen);    
}

BOOL CSharedMemory::GetString(CString &strData, int nOffset /* = 0 */)
{
	int nLen = 0;  
	PBYTE pData = NULL;  
	
	Lock();  
	pData = GetData(nOffset);   
	while (*pData++ != '\0') nLen++;  
	Unlock();  
  
	if (nOffset + nLen + 1 > m_nSize)  
	return FALSE;  
    
	Lock();  
	strData.Format("%s", (LPCSTR)GetData(nOffset));  
	Unlock();  	
    return TRUE;  
}

BOOL CSharedMemory::Lock()
{
	if (WaitForSingleObject(m_hMutex, INFINITE) == WAIT_OBJECT_0)  
		return TRUE;  
 
    return FALSE;
}

BOOL CSharedMemory::Unlock()
{
	return ReleaseMutex(m_hMutex);
}

