#ifndef __HTTP_CLIENT_TRANS_H__
#define __HTTP_CLIENT_TRANS_H__

#include <Winhttp.h>
#define HttpAgent L""

class CHttpClientTrans
{
public:
	CHttpClientTrans();
	~CHttpClientTrans();

public:
	static CHttpClientTrans&	Instance();
	
public:
	BOOL						OpenHttpClient(LPCTSTR lpszUrl, DWORD dwTimeout);

	BOOL						HttpGetDataSize();

protected:
	HINTERNET 					m_hSession;
	HINTERNET					m_hConnect;
	HINTERNET					m_hRequest;
};

#endif