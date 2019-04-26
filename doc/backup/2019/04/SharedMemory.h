// SharedMemory.h: interface for the CSharedMemory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDMEMORY_H__065482CD_06BD_4753_A5AA_81A7743E49BD__INCLUDED_)
#define AFX_SHAREDMEMORY_H__065482CD_06BD_4753_A5AA_81A7743E49BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSharedMemory  
{
public:
	CSharedMemory();
	virtual ~CSharedMemory();

	BOOL			Create(LPCSTR lpName, int nSize, BOOL* pbExist=NULL);
	void			Destroy();

	static BOOL		IsExist(LPCSTR lpName);
	static PBYTE	GetData(LPCSTR lpName, int nSize);

	BOOL			SetLong(DWORD dwData, int nOffset = 0);
	DWORD			GetLong(int nOffset = 0);

	BOOL			SetString(CString strData,  int nOffset = 0, int nSize = -1);  
	BOOL			GetString(CString &strData, int nOffset = 0);  

	inline int		GetSize() {return m_nSize;};  

	BOOL			Lock( );  
	BOOL			Unlock( );  

	BOOL			SetData(PBYTE pData, int nOffset = 0, int nSize = -1);  
	PBYTE			GetData(int nOffset = 0);  

protected:
	CString			m_strName;
	int				m_nSize;
	HANDLE			m_hShmFile;
	HANDLE			m_hMutex;
	PVOID			m_pData; 
};

#endif // !defined(AFX_SHAREDMEMORY_H__065482CD_06BD_4753_A5AA_81A7743E49BD__INCLUDED_)
