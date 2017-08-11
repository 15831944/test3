#ifndef __UPDATE_FILE_NAME_H__
#define __UPDATE_FILE_NAME_H__

class update_file_name
{
public:
	update_file_name();
	~update_file_name();
	
public:
	BOOL							CreateUpdateProc();
	BOOL							CloseUpdateProc();
	
	static 	update_file_name&		Instance();
	
protected:
	static  DWORD WINAPI			UpdateFileThreadProc(LPVOID lpParam);
	
protected:
	void							UpdateFileInfo();

protected:
	HANDLE							m_hThread;

	HANDLE							m_hStartEvent;
	HANDLE							m_hEndEvent;
	
private:
	BOOL							m_bExit;
	
	DWORD							m_dwThreadID;
	DWORD							m_dwWaitTime;
};

#endif