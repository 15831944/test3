#include "stdafx.h"
#include "JsonConvert.h"

CJsonConvert::CJsonConvert()
{
	
}


CJsonConvert::~CJsonConvert()
{
	
}

BOOL CJsonConvert::ReadJson(const char* pszJson)
{
	Json::Reader  reader;
	
	if(!reader.parse(pszJson, m_root))
	{
		return FALSE;
	}

	if (m_root.size() == 0)
	{
		return FALSE;
	}

	return TRUE;	
}

BOOL CJsonConvert::IsJsonArray()
{
	return m_root.isArray();
}

std::string CJsonConvert::SetJsonInfo(std::map<std::string, std::string> mapValue)
{
	std::string strKey;
	std::string strValue;

	if (mapValue.size() <= 0)
	{
		return _T("");
	}

	Json::Value root;
	Json::FastWriter writer;
	std::map<std::string, std::string>::iterator iter = mapValue.begin();

	for (iter; iter != mapValue.end(); iter++)
	{
		strKey = iter->first;
		strValue = iter->second;

		root[strKey] = strValue;
	}

	m_root.append(root);
	std::string strResult(writer.write(root));

	return strResult;
}

std::vector<std::vector<string>> CJsonConvert::GetJsonArrayInfo(char* pKey,...)
{
	Json::Value attr;
	Json::Value value;
	Json::Value def = Json::Value("null");
	
	int nIndex = 0;
	int nSize = m_root.size();
	
	char *pValue = NULL;
	std::string strResult;

	std::vector<std::vector<string>> vecResult;
	
	value = m_root;
	
	for(nIndex=0; nIndex<nSize; nIndex++)
	{
		std::vector<std::string> vecString;
		value = m_root.get(nIndex, def);
		
		pValue = pKey;
		
		va_list   pArgList;
		va_start(pArgList, pKey);
	
		while(1)
		{
			if(pValue != NULL)
			{
				attr = value.get(pValue, def);
				strResult = attr.asCString();
				
				if (strResult == _T("null"))
				{
					strResult = _T("");
				}

				vecString.push_back(strResult);
				
				pValue = NULL;
				continue;
			}
			else
			{
				pValue = va_arg(pArgList, char*);
				if (strcmp(pValue, _T("\0")) == 0)
				{
					break;
				}
			
				attr = value.get(pValue, def);
				strResult = attr.asCString();

				if (strResult == _T("null"))
				{
					strResult = _T("");
				}
			
				vecString.push_back(strResult);
			
				pValue = NULL;
				continue;
			}	
		}	
		
		va_end(pArgList);
		vecResult.push_back(vecString);
	}

	return vecResult;
}

std::vector<std::string> CJsonConvert::GetJsonObjectInfo(char* pKey,...)
{
	Json::Value attr;
	Json::Value value;
	Json::Value def = Json::Value("null");
	
	char *pValue = pKey;
	std::string strResult;
	std::vector<std::string> vecResult;
	
	value = m_root;

	va_list   pArgList;
	va_start(pArgList, pKey);
	
	while(1)
	{
		if(pValue != NULL)
		{			
			attr = value.get(pValue, def);
			strResult = attr.asCString();

			if (strResult == "null")
			{
				strResult = _T("");
			}

			vecResult.push_back(strResult);
			pValue = NULL;
		}
		else
		{
			pValue = va_arg(pArgList, char*);
			if (strcmp(pValue, _T("\0")) == 0)
			{
				pValue = NULL;
				break;
			}
			
			attr = value.get(pValue, def);
			strResult = attr.asCString();
			
			if (strResult == "null")
			{
				strResult = _T("");
			}
			
			vecResult.push_back(strResult);
			pValue = NULL;
		}		
	}
	
	va_end(pArgList);
	
	return vecResult;
}
//////////////////////////////////////////////////////////////////////////
//
BOOL GetJsonPubKey(LPCTSTR lpszJson, string &strPubKey)
{
	BOOL bRet = FALSE;

	std::vector<std::string> vecString;

	CJsonConvert JsonConvert;

	if (JsonConvert.ReadJson(lpszJson))
	{
		if (!JsonConvert.IsJsonArray())
		{
			vecString = JsonConvert.GetJsonObjectInfo("SM2PubKey", "\0");

			if (vecString.size() > 0)
			{
				strPubKey = vecString[0];
				bRet = TRUE;
			}
		}
	}

	return bRet;
}

BOOL GetJsonSeedSN(LPCTSTR lpszJson, string &strSeedSN)
{
	BOOL bRet = FALSE;

	std::vector<std::string> vecString;

	CJsonConvert JsonConvert;

	if (JsonConvert.ReadJson(lpszJson))
	{
		if (!JsonConvert.IsJsonArray())
		{
			vecString = JsonConvert.GetJsonObjectInfo("tokenSN", "\0");

			if (vecString.size() > 0)
			{
				strSeedSN = vecString[0];
			}

			bRet = TRUE;
		}
	}

	return bRet;
}

BOOL GetJsonSeedEnc(LPCTSTR lpszJson, string &strSeedEnc, string &strSeedSN)
{
	BOOL bRet = FALSE;

	std::vector<std::string> vecString;

	CJsonConvert JsonConvert;

	if (JsonConvert.ReadJson(lpszJson))
	{
		if (!JsonConvert.IsJsonArray())
		{
			vecString = JsonConvert.GetJsonObjectInfo("seedCipher","tokenSN", "\0");

			if (vecString.size() > 0)
			{
				strSeedEnc = vecString[0];
				strSeedSN  = vecString[1];
			}

			bRet = TRUE;
		}
	}

	return bRet;
}

BOOL GetJsonArraySeedSN(LPCTSTR lpszJson, std::vector<std::string> &vecSeedSN)
{
	BOOL bRet = FALSE;

	std::vector<std::vector<string>> vecResult;
	std::vector<std::vector<string>>::iterator iter;

	CJsonConvert JsonConvert;

	if (JsonConvert.ReadJson(lpszJson))
	{
		if (JsonConvert.IsJsonArray())
		{
			vecResult = JsonConvert.GetJsonArrayInfo("tokenSN", "\0");

			iter = vecResult.begin();
			while(iter != vecResult.end())
			{
				if ((*iter)[0] != _T(""))
				{
					vecSeedSN.push_back((*iter)[0]);
				}

				++iter;
			}

			if (vecSeedSN.size() > 0)
			{
				bRet = TRUE;
			}
		}
	}

	return bRet;
}

BOOL WriteJsonSeedEnc(std::map<string, string> mapSeedEnc, string &strSeedEnc)
{
	if (mapSeedEnc.size() <= 0)
	{
		return FALSE;
	}

	CJsonConvert JsonConvert;
	strSeedEnc = JsonConvert.SetJsonInfo(mapSeedEnc);

	return TRUE;
}