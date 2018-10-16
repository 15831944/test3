#include "stdafx.h"
#include "HttpClientTrans.h"

#pragma comment(lib,"Winhttp.lib")
CHttpClientTrans::CHttpClientTrans()
{
	m_hSession = NULL;
	m_hConnect = NULL;
	m_hRequest = NULL;
}

CHttpClientTrans::~CHttpClientTrans()
{
}

CHttpClientTrans& CHttpClientTrans::Instance()
{
	static CHttpClientTrans inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CHttpClientTrans::OpenHttpClient(LPCTSTR lpszUrl, DWORD dwTimeout)
{
	BOOL bRet = FALSE;
	
	do
	{
		if (lpszUrl == NULL || *lpszUrl == '\0')
		{
			bRet = FALSE;
			break;
		}
		
		m_hSession = WinHttpOpen(HttpAgent, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		if (m_hSession == NULL)
		{
			bRet = FALSE;
			break;
		}

		m_hConnect = WinHttpConnect(m_hSession, L"", 0, 0);
		if (m_hConnect == NULL)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while(FALSE);
	
	return bRet;
}

BOOL CHttpClientTrans::HttpGetDataSize()
{
	BOOL bRet = FALSE;

	DWORD dwSize = 0;
	LPVOID lpHeaderBuffer = NULL;

	do 
	{
		if (m_hConnect == NULL)
		{
			bRet = FALSE;
			break;
		}

		//创建Http请求连接
		m_hRequest = WinHttpOpenRequest(m_hConnect, L"Get", L"", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		if (m_hRequest == NULL)
		{
			bRet = FALSE;
			break;
		}

		//发送请求
		if (!WinHttpSendRequest(m_hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
		{
			bRet = FALSE;
			break;
		}

		//结束请求
		if (!WinHttpReceiveResponse(m_hRequest, NULL))
		{
			bRet = FALSE;
			break;
		}

		if (!WinHttpQueryDataAvailable(m_hRequest, &dwSize))
		{
			bRet = FALSE;
			break;
		}

		if (!WinHttpQueryHeaders(m_hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, NULL, &dwSize, WINHTTP_NO_HEADER_INDEX))
		{
			lpHeaderBuffer = new WCHAR[dwSize / sizeof(WCHAR)];
			if (lpHeaderBuffer == NULL)
			{
				bRet = FALSE;
				break;
			}
			memset(lpHeaderBuffer, 0x0, dwSize / sizeof(WCHAR));

			if (!WinHttpQueryHeaders(m_hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, lpHeaderBuffer, &dwSize, WINHTTP_NO_HEADER_INDEX))
			{
				bRet = FALSE;
				break;
			}
		}

		do 
		{

		} while (dwSize > 0);

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CHttpClientTrans::HttpPostData()
{
	BOOL bRet = FALSE;

	do 
	{
	} while (FALSE);

	return bRet;
}