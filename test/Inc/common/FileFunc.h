#ifndef __FILE_FUNC_H__
#define __FILE_FUNC_H__

class CFileFunc
{
public:
	CFileFunc();
	~CFileFunc();
	
public:
	static 	CFileFunc& Instance();
	
public:
	CString		GetAppPath();

	bool		IsFileExists(LPCTSTR lpszFilePath);
	bool		DeleteSelfFile();
};

#endif