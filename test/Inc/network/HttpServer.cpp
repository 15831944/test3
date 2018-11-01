#include "stdafx.h"
#include "HttpServer.h"

#define MAX_BUFFER_SIZE		4048

#pragma comment(lib, "httpapi.lib")
CHttpServer::CHttpServer()
{
	m_iPort = 0;

	m_strHost = _T("");
	m_strUrlPath = _T("");
}

CHttpServer::~CHttpServer()
{
}

CHttpServer& CHttpServer::Instance()
{
	static CHttpServer inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CHttpServer::GetHandler(const HTTPREQUEST *pHttpReq, URLHANDLER **pUrlHandle)
{
	BOOL bRet = FALSE;

	int iRet = 0;
	std::map<CString, URLHANDLER*>::iterator iterUrlHandler;

	do 
	{
		if (pHttpReq == NULL)
		{
			bRet = FALSE;
			break;
		}

		iterUrlHandler = m_mapUrlHandlers.find(pHttpReq->szPath);
		if (iterUrlHandler == m_mapUrlHandlers.end())
		{
			bRet = FALSE;
			break;
		}

		*pUrlHandle = iterUrlHandler->second;
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CHttpServer::ReadRequest(HTTPREQUEST *pHttpReq)
{
	BOOL bRet = FALSE;

	int iRet = 0;
	int iPos = 0;

	CString strPath;
	CString strUrlPath;
	CString strQueryInfo;
	CString strContext;

	DWORD dwReceivedBytes = 0;
	TCHAR szOutBuffer[MAX_BUFFER_SIZE] = {0};

	HTTP_REQUEST* pReq = NULL;
	TCHAR szBuffer[MAX_BUFFER_SIZE] = {0};

	do 
	{
		if (pHttpReq == NULL)
		{
			bRet = FALSE;
			break;
		}

		pReq = reinterpret_cast<HTTP_REQUEST*>(szBuffer);
		if (pReq == NULL)
		{
			bRet = FALSE;
			break;
		}

		//
		iRet = ::HttpReceiveHttpRequest(m_hRequestHandle, HTTP_NULL_ID, HTTP_RECEIVE_REQUEST_FLAG_COPY_BODY, pReq, MAX_BUFFER_SIZE, NULL, NULL);
		if (iRet != NO_ERROR)
		{
			bRet = FALSE;
			break;
		}

		strUrlPath = pReq->pRawUrl;
		if (strUrlPath == _T(""))
		{
			bRet = FALSE;
			break;
		}

		iPos = strUrlPath.ReverseFind(_T('?'));
		if (iPos != -1)
		{
			strPath = strUrlPath.Left(iPos);
			strQueryInfo = strUrlPath.Right(strUrlPath.GetLength() - iPos -1);
		}
		else
		{
			strPath = strUrlPath;
		}

		pHttpReq->emVerb = pReq->Verb;
		sprintf(pHttpReq->szPath, _T("%s"), strPath);
		sprintf(pHttpReq->szQueryInfo, _T("%s"), strQueryInfo);
		memcpy(&pHttpReq->stcHttpRequest, pReq, sizeof(HTTP_REQUEST));

		if (pReq->Verb == HttpVerbPOST)
		{
			iRet = ::HttpReceiveRequestEntityBody(m_hRequestHandle, pReq->RequestId, 0, szOutBuffer, MAX_BUFFER_SIZE, &dwReceivedBytes, NULL);
			if (iRet != NO_ERROR)
			{
				bRet = FALSE;
				break;
			}

			sprintf(pHttpReq->szContext, _T("%s"), strContext);
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CHttpServer::SendResponse(HTTPRESPONSE *pHttpResp)
{
	BOOL bRet = FALSE;

	int iRet = 0;
	int iHttpRespFlag = 0;
	DWORD dwSendBytes = 0;
	
	HTTP_DATA_CHUNK chunk;
	HTTP_RESPONSE stcHttpRespOnse;
	HTTP_RESPONSE_HEADERS stcHeaders;

	HTTPREQUEST *pHttpReq = NULL;
	const char* const pOk = _T("OK");

	do 
	{
		if (pHttpResp == NULL)
		{
			bRet = FALSE;
			break;
		}

		pHttpReq = &pHttpResp->stcRequest;
		if (pHttpReq == NULL)
		{
			bRet = FALSE;
			break;
		}

		memset(&chunk, 0x0, sizeof(HTTP_DATA_CHUNK));
		memset(&stcHttpRespOnse, 0x0, sizeof(HTTP_RESPONSE));
		memset(&stcHeaders, 0x0, sizeof(HTTP_RESPONSE_HEADERS));

		//
		stcHeaders.pUnknownHeaders = 0;
		stcHeaders.TrailerCount = 0;

		//
		stcHttpRespOnse.pReason = pOk;
		stcHttpRespOnse.StatusCode = 200;
		stcHttpRespOnse.ReasonLength = static_cast<USHORT>(strlen(pOk));
		stcHttpRespOnse.Headers = stcHeaders;
		
		if (pHttpReq->emVerb == HttpVerbPOST)
		{
			SetHeader(&stcHttpRespOnse, HttpHeaderContentType, _T("text/xml; charset=UTF-8"));

			chunk.DataChunkType = HttpDataChunkFromMemory;

			stcHttpRespOnse.EntityChunkCount = 1;
			stcHttpRespOnse.pEntityChunks = &chunk;
		}
		else if (pHttpReq->emVerb == HttpVerbHEAD)
		{
			SetHeader(&stcHttpRespOnse, HttpHeaderContentType, _T("application/octet-stream"));
		}
		else if (pHttpReq->emVerb == HttpVerbGET)
		{
			SetHeader(&stcHttpRespOnse, HttpHeaderContentType, _T("application/octet-stream"));
		}

		//
		chunk.DataChunkType = HttpDataChunkFromMemory;

		iRet = ::HttpSendHttpResponse(m_hRequestHandle, pHttpReq->stcHttpRequest.RequestId, iHttpRespFlag, &stcHttpRespOnse, NULL, &dwSendBytes, NULL, 0, NULL, NULL);
		if (iRet != NO_ERROR)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CHttpServer::SetHeader(HTTP_RESPONSE *pHttpResponse, HTTP_HEADER_ID emHeaderId, LPCTSTR lpszValue)
{
	BOOL bRet = FALSE;

	do 
	{
		if (pHttpResponse == NULL || lpszValue == NULL)
		{
			bRet = FALSE;
			break;
		}

		pHttpResponse->Headers.KnownHeaders[emHeaderId].pRawValue = lpszValue;
		pHttpResponse->Headers.KnownHeaders[emHeaderId].RawValueLength = static_cast<USHORT>(strlen(lpszValue));

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CHttpServer::Initialize()
{
	BOOL bRet = FALSE;

	int iRet = 0;
	HTTPAPI_VERSION version = HTTPAPI_VERSION_1;

	do 
	{
		//初始化HTTP Server APIs
		iRet = ::HttpInitialize(version, HTTP_INITIALIZE_SERVER, NULL);
		if (iRet != NO_ERROR)
		{
			bRet = FALSE;
			break;
		}

		//创建请求队列句柄
		iRet = ::HttpCreateHttpHandle(&m_hRequestHandle, NULL);
		if (iRet != NO_ERROR)
		{
			bRet = FALSE;
			break;
		}

		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CHttpServer::AddUrlHandler(URLHANDLER *pUrlHandle)
{
	BOOL bRet = FALSE;

	int iRet = 0;
	CString strFullUrlPath;

	USES_CONVERSION;
	do 
	{
		if (pUrlHandle == NULL)
		{
			bRet = FALSE;
			break;
		}

		strFullUrlPath.Format(_T("%s/%s"), m_strUrlPath, pUrlHandle->szUrlPath);

		//指定要监听的URI,为每个URI调用HttpAddUrl 
		iRet = ::HttpAddUrl(m_hRequestHandle, A2CW(strFullUrlPath.GetBuffer(0)), NULL);
		if (iRet != NO_ERROR)
		{
			bRet = FALSE;
			break;
		}

		m_mapUrlHandlers.insert(make_pair(pUrlHandle->szUrlPath, pUrlHandle));
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}

BOOL CHttpServer::Start()
{
	BOOL bRet = FALSE;

	HTTPREQUEST stcHttpReq;
	HTTPRESPONSE stcHttpResp;

	URLHANDLER *pUrlHandler = NULL;

	do 
	{
		while (TRUE)
		{
			memset(&stcHttpReq, 0x0, sizeof(HTTPREQUEST));
			memset(&stcHttpResp, 0x0, sizeof(HTTPRESPONSE));

			if (!ReadRequest(&stcHttpReq))
			{
				continue;
			}

			if (!GetHandler(&stcHttpReq, &pUrlHandler))
			{
				continue;
			}

			if (!SendResponse(&stcHttpResp))
			{
				continue;
			}
		}
		
		bRet = TRUE;
	} while (FALSE);

	return bRet;
}