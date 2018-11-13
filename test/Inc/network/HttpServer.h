#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <http.h>
#include <winhttp.h>

//
typedef struct tagHttpRequest{
	HTTP_VERB	emVerb;
	HTTP_REQUEST stcHttpRequest;
	char		szPath[MAX_PATH];
	char		szQueryInfo[MAX_PATH];
	int			iSize;
	char		*pContext;
}HTTPREQUEST;

//
typedef struct tagHttpResponse{
	int			iSize;
	char		szFileName[MAX_PATH];
	char		szRespInfo[MAX_PATH];
	HTTPREQUEST stcRequest;
}HTTPRESPONSE;

//
typedef struct tagUrlHandler{
	char		szUrlPath[MAX_PATH];
}URLHANDLER;

class CHttpServer
{
public:
	CHttpServer();
	~CHttpServer();

public:
	static CHttpServer&		Instance();
	
public:
	BOOL					Initialize();
	BOOL					AddUrlHandler(LPCTSTR lpszUrlPath, int iPort, URLHANDLER *pUrlHandle);
	BOOL					Start();

protected:
	BOOL					GetHandler(const HTTPREQUEST *pHttpReq, URLHANDLER **pUrlHandle);
	
	BOOL					ReadRequest(HTTPREQUEST *pHttpReq);
	BOOL					SendResponse(HTTPRESPONSE *pHttpResp);
	
	BOOL					SetHeader(HTTP_RESPONSE *pHttpResponse, HTTP_HEADER_ID emHeaderId, LPCTSTR lpszValue);

private:
	HANDLE					m_hRequestHandle;
	HTTP_REQUEST			m_hHttpRequest;

	std::map<CString, URLHANDLER*> m_mapUrlHandlers;
};

#endif