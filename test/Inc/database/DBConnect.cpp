#include "stdafx.h"
#include "DBConnect.h"

#pragma comment(lib, "odbc32.lib")
CDBConnect::CDBConnect()
{
	m_dwConnTimeOut  = 0;
	m_dwLoginTimeOut = 0;

	m_bConnected = FALSE;

	//分配环境句柄
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	//设置环境属性
	SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	//分配连接句柄
	SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
}

CDBConnect::~CDBConnect()
{

}

BOOL CDBConnect::Connect(LPCTSTR lpszDSN, LPCTSTR lpszUser, LPCTSTR lpszPasswd)
{
	SQLRETURN ret;

	if (m_dwConnTimeOut > 0)
	{
		//设置数据库连接等待超时
		SQLSetConnectAttr(m_hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)m_dwConnTimeOut, 0);
	}

	//设置数据库登录等待超时
	SQLSetConnectAttr(m_hdbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)m_dwLoginTimeOut, 0);

	//连接数据库
	ret = SQLConnect(m_hdbc, (SQLCHAR*)lpszDSN, sizeof(lpszDSN), (SQLCHAR*)lpszUser, sizeof(lpszUser), (SQLCHAR*)lpszPasswd, sizeof(lpszPasswd));
	if(!SQL_SUCCEEDED(ret))
	{
		m_bConnected = FALSE;
		return FALSE;
	}	
	
	m_bConnected = ((ret==SQL_SUCCESS) || (ret==SQL_SUCCESS_WITH_INFO));
	return m_bConnected;
}

BOOL CDBConnect::DriverConnect(LPCTSTR lpszConnStr, CHAR *pszConnStrOut, HWND hWnd, enum drvCompletion drvConn)
{
	SQLRETURN ret;
	SQLSMALLINT pcbConnStrOut;

	if (m_dwConnTimeOut > 0)
	{
		//设置数据库连接等待超时
		SQLSetConnectAttr(m_hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)m_dwConnTimeOut, 0);
	}

	//设置数据库登录等待超时
	SQLSetConnectAttr(m_hdbc, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER)m_dwLoginTimeOut, 0);

	ret = SQLDriverConnect(m_hdbc, hWnd, (SQLCHAR*)lpszConnStr, SQL_NTS, (SQLCHAR*)pszConnStrOut, sizeof(pszConnStrOut), &pcbConnStrOut, (SQLUSMALLINT)drvConn);
	if(!SQL_SUCCEEDED(ret))	
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	m_bConnected = ((ret==SQL_SUCCESS) || (ret==SQL_SUCCESS_WITH_INFO));
	return m_bConnected;
}

BOOL CDBConnect::BrowseConnect(LPCTSTR lpszConnStr, CHAR *pszConnStrOut)
{
	SQLRETURN ret;
	SWORD  nLenOut = 0;

	ret = SQLBrowseConnect(m_hdbc, (SQLCHAR*)lpszConnStr, sizeof(lpszConnStr), (SQLCHAR*)pszConnStrOut, MAX_BUFFER, &nLenOut);
	if(!SQL_SUCCEEDED(ret))
	{
		m_bConnected = FALSE;
		return FALSE;
	}

	m_bConnected = ((ret==SQL_SUCCESS) || (ret==SQL_SUCCESS_WITH_INFO));
	return m_bConnected;
}

void CDBConnect::Close()
{
	m_bConnected = FALSE;
	if (m_hdbc == NULL)
	{
		return;
	}

	SQLDisconnect(m_hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC,  m_hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV,  m_henv);
	m_hdbc = NULL;
	m_henv = NULL;
}

void CDBConnect::SetConnectionTimeout(DWORD dwTime)
{
	if (m_hdbc)
	{
		SQLSetConnectAttr(m_hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)dwTime, 0);
	}

	m_dwConnTimeOut = dwTime;
}

void CDBConnect::SetAccessMode(BOOL bMode)
{
	SQLSetConnectAttr(m_hdbc, SQL_ATTR_ACCESS_MODE, (SQLPOINTER)(bMode ? SQL_MODE_READ_ONLY : SQL_MODE_READ_WRITE), 0);
}

DWORD CDBConnect::GetConnectionTimeout()
{
	DWORD dwConnTimeOut;

	SQLGetConnectAttr(m_hdbc, SQL_ATTR_CONNECTION_TIMEOUT, &dwConnTimeOut, NULL, 0);
	return dwConnTimeOut;
}

BOOL CDBConnect::GetTableInfo(TableInfo *pTableInfo, int &nSize)
{
	if (pTableInfo == NULL)
	{
		return FALSE;
	}

	SQLRETURN ret;

	int nIndex = 0;
	SQLINTEGER len;
	SQLVARCHAR ctlName[1024];
	SQLVARCHAR schName[1024];
	SQLVARCHAR tblName[1024];
	SQLVARCHAR tblType[1024];
	SQLVARCHAR tblRmks[1024];

	ret = SQLTables(m_hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
	if (ret != SQL_SUCCESS || ret != SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}


	while(ret = SQLFetch(m_hstmt),  ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		SQLGetData(m_hstmt, 1, SQL_C_CHAR, ctlName, 1024, &len);
		SQLGetData(m_hstmt, 2, SQL_C_CHAR, schName, 1024, &len);
		SQLGetData(m_hstmt, 3, SQL_C_CHAR, tblName, 1024, &len);
		SQLGetData(m_hstmt, 4, SQL_C_CHAR, tblType, 1024, &len);
		SQLGetData(m_hstmt, 5, SQL_C_CHAR, tblRmks, 1024, &len);

		if (_tcscmp((_TCHAR*)tblType, _T("TABLE")) == 0)
		{
			_tcscpy((LPTSTR)pTableInfo[nIndex].catalogName,  (LPTSTR)ctlName);
			_tcscpy((LPTSTR)pTableInfo[nIndex].schemaName,   (LPTSTR)schName);
			_tcscpy((LPTSTR)pTableInfo[nIndex].tableName,    (LPTSTR)tblName);
			_tcscpy((LPTSTR)pTableInfo[nIndex].tableType,    (LPTSTR)tblType);
			_tcscpy((LPTSTR)pTableInfo[nIndex].tableRemarks, (LPTSTR)tblRmks);
		
			nIndex++;
		}

	}

	nSize = nIndex;
	return TRUE;
}

BOOL CDBConnect::ExecuteSQL(LPCTSTR lpszSQL)
{
	SQLRETURN ret;
	SQLSMALLINT numColumns;

	if (m_hstmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT,  m_hstmt);
		SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	}

	ret = SQLPrepare(m_hstmt, (SQLCHAR*)lpszSQL, SQL_NTS);
	if (ret != SQL_SUCCESS || ret != SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}

	ret = SQLNumResultCols(m_hstmt, &numColumns);
	if (ret != SQL_SUCCESS || ret != SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}

	if (numColumns != 0)
	{
		return FALSE;
	}

	ret = SQLExecute(m_hstmt);
	if (ret != SQL_SUCCESS || ret != SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}

	return TRUE;
}