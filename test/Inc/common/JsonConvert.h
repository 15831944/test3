#ifndef __JSONCONVERT_H__
#define __JSONCONVERT_H__

#include <json/json.h>

using namespace std;
class CJsonConvert
{
public:
	CJsonConvert();
	~CJsonConvert();
	
public:	
	BOOL								ReadJson(const char* pszJson);
	BOOL								IsJsonArray();
	
	std::string							SetJsonInfo(std::map<std::string, std::string> mapValue);
	
	std::vector<std::vector<string>>	GetJsonArrayInfo(char* pKey,...);
	std::vector<std::string>			GetJsonObjectInfo(char* pKey,...);	
	
private:
	Json::Value							m_root;
};

extern	BOOL	GetJsonPubKey(LPCTSTR lpszJson, string &strPubKey);
extern  BOOL	GetJsonSeedSN(LPCTSTR lpszJson, string &strSeedSN);
extern  BOOL    GetJsonSeedEnc(LPCTSTR lpszJson, string &strSeedEnc, string &strSeedSN);
extern  BOOL    GetJsonArraySeedSN(LPCTSTR lpszJson, std::vector<std::string> &vecSeedSN);

extern	BOOL	WriteJsonSeedEnc(std::map<string, string> mapSeedEnc, string &strSeedEnc);

#endif