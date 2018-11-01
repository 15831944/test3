#include "stdafx.h"
#include "HttpClientTrans.h"

#include <Winhttp.h>
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

	DWORD dwError = 0;
	int timeout_in_ms = 0;
	URL_COMPONENTS url_components = {0};
	
	USES_CONVERSION;
	do
	{
		if (lpszUrl == NULL || *lpszUrl == '\0')
		{
			bRet = FALSE;
			break;
		}

		url_components.dwStructSize = sizeof(URL_COMPONENTS);
		url_components.dwSchemeLength = (DWORD)-1;
		url_components.dwHostNameLength = (DWORD)-1;
		url_components.dwUrlPathLength = (DWORD)-1;
		url_components.dwExtraInfoLength = (DWORD)-1;

		if (!WinHttpCrackUrl(A2CW(lpszUrl), NULL, ICU_REJECT_USERPWD, &url_components))	//ICU_REJECT_USERPWD //ICU_ESCAPE
		{
			bRet = FALSE;
			break;
		}
		
		m_hSession = WinHttpOpen(NULL, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		if (m_hSession == NULL)
		{
			bRet = FALSE;
			break;
		}

		timeout_in_ms = static_cast<int>(dwTimeout*1000);
		if (!WinHttpSetTimeouts(m_hSession, timeout_in_ms, timeout_in_ms, timeout_in_ms, timeout_in_ms))
		{
			bRet = FALSE;
			break;
		}

		std::wstring host_name(url_components.lpszHostName, url_components.dwHostNameLength);
		std::wstring url_path(url_components.lpszUrlPath, url_components.dwUrlPathLength);
		std::wstring extra_info(url_components.lpszExtraInfo, url_components.dwExtraInfoLength);

		m_hConnect = WinHttpConnect(m_hSession, host_name.c_str(), url_components.nPort, 0);
		if (m_hConnect == NULL)
		{
			bRet = FALSE;
			break;
		}

		m_strHostName = host_name.c_str();
		m_strUrlPath = url_path.c_str();
		m_strExtraInfo = extra_info.c_str();

		bRet = TRUE;
	} while(FALSE);
	
	return bRet;
}

void CHttpClientTrans::CloseHttpClient()
{
	BOOL bRet = FALSE;
	
	do 
	{
		if (m_hRequest != NULL)
		{
			WinHttpCloseHandle(m_hRequest);
			m_hRequest = NULL;
		}

		if (m_hConnect != NULL)
		{
			WinHttpCloseHandle(m_hConnect);
			m_hConnect = NULL;
		}

		if (m_hSession != NULL)
		{
			WinHttpCloseHandle(m_hSession);
			m_hSession = NULL;
		}

		bRet = TRUE;
	} while (FALSE);
}

BOOL CHttpClientTrans::HttpGetData(char *pszData, DWORD &dwSize)
{
	BOOL bRet = FALSE;
	DWORD dwWritePos = 0;

	DWORD dwDataBytes = 0;
	DWORD dwReadBytes = 0;

	DWORD dwHeaderLen = 0;
	WCHAR *szHeaderBuf = NULL;

	DWORD dwReadLen = 0;
	char *szReadBuf = NULL;

	do 
	{
		if (m_hConnect == NULL)
		{
			bRet = FALSE;
			break; 
		}

		if (m_hRequest == NULL)
		{
			m_hRequest = WinHttpOpenRequest(m_hConnect, L"GET", m_strUrlPath.GetBuffer(0), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
			if (m_hRequest == NULL)
			{
				bRet = FALSE;
				break;
			}

			if (!WinHttpSendRequest(m_hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
			{
				bRet = FALSE;
				break;
			}

			if (!WinHttpReceiveResponse(m_hRequest, NULL))
			{
				bRet = FALSE;
				break;
			}
		}
		
		if (pszData == NULL)
		{
			if (!WinHttpQueryHeaders(m_hRequest, WINHTTP_QUERY_CONTENT_LENGTH, WINHTTP_HEADER_NAME_BY_INDEX, WINHTTP_NO_OUTPUT_BUFFER, &dwHeaderLen, WINHTTP_NO_HEADER_INDEX))
			{
				if (GetLastError()  == ERROR_INSUFFICIENT_BUFFER)
				{
					szHeaderBuf = new WCHAR[dwHeaderLen/sizeof(WCHAR)];
					if (szHeaderBuf == NULL)
					{
						bRet = FALSE;
						break;
					}
					memset(szHeaderBuf, 0x0, dwHeaderLen/sizeof(WCHAR));

					if (!WinHttpQueryHeaders(m_hRequest, WINHTTP_QUERY_CONTENT_LENGTH, WINHTTP_HEADER_NAME_BY_INDEX, szHeaderBuf, &dwHeaderLen, WINHTTP_NO_HEADER_INDEX))
					{
						bRet = FALSE;
						break;
					}

					dwSize = _wtoi(szHeaderBuf);
				}
			}
		}
		else
		{
			dwReadLen = 4*1024;
			dwDataBytes = dwSize;

			szReadBuf = new char[dwReadLen/sizeof(char)];
			if (szReadBuf == NULL)
			{
				bRet = FALSE;
				break;
			}
			
			while (dwDataBytes > 0)
			{
				memset(szReadBuf, 0x0, dwReadLen/sizeof(char));
				if (!WinHttpReadData(m_hRequest, szReadBuf, dwReadLen, &dwReadBytes))
				{
					bRet = FALSE;
					break;
				}

				memcpy(pszData+dwWritePos, szReadBuf, dwReadBytes);
				dwWritePos += dwReadBytes;

				dwDataBytes -= dwReadBytes;
				bRet = TRUE;
			}

			if (!bRet)
			{
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);

	if (szHeaderBuf != NULL)
	{
		delete[] szHeaderBuf;
		szHeaderBuf = NULL;
	}

	if (szReadBuf != NULL)
	{
		delete[] szReadBuf;
		szReadBuf = NULL;
	}

	return bRet;
}