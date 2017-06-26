#ifndef __DBCONNECT_H__
#define __DBCONNECT_H__

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#define MAX_BUFFER 256

class CDBConnect
{
public:
	CDBConnect();
	~CDBConnect();
	
public:
	enum drvCompletion
	{
		sqlNoPrompt = SQL_DRIVER_NOPROMPT,
		sqlPrompt   = SQL_DRIVER_PROMPT
	};

	struct TableInfo
	{
		SQLVARCHAR	catalogName[1024];
		SQLVARCHAR	schemaName[1024];
		SQLVARCHAR	tableName[1024];
		SQLVARCHAR	tableType[1024];
		SQLVARCHAR	tableRemarks[1024];
	};

public:
	BOOL								Connect(LPCTSTR lpszDSN, LPCTSTR lpszUser, LPCTSTR lpszPasswd);
	BOOL								DriverConnect(LPCTSTR lpszConnStr, CHAR *pszConnStrOut, HWND hWnd, enum drvCompletion drvConn);
	BOOL								BrowseConnect(LPCTSTR lpszConnStr, CHAR *pszConnStrOut);

	void								Close();
	void								SetConnectionTimeout(DWORD dwTime);
	void								SetAccessMode(BOOL bMode);

	DWORD								GetConnectionTimeout();
	BOOL								GetTableInfo(TableInfo *pTableInfo, int &nSize);
	BOOL								ExecuteSQL(LPCTSTR lpszSQL);
	

private:
	SQLHENV								m_henv;
	SQLHDBC								m_hdbc;
	SQLHSTMT							m_hstmt;
	
	BOOL								m_bConnected;

	DWORD								m_dwConnTimeOut;
	DWORD								m_dwLoginTimeOut;		
};

#endif