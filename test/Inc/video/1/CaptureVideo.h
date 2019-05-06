#ifndef __CAPTUREVIDEO_H__
#define __CAPTUREVIDEO_H__

#define MAX_FRIENDLY_NAME_LEN			128
#define MAX_MONIKER_NAME_LEN			128

typedef struct tagDeviceName{
	UINT	nDeviceID;
	WCHAR	FriendlyName[MAX_FRIENDLY_NAME_LEN];
	WCHAR	MonikerName[MAX_MONIKER_NAME_LEN];
}DeviceName;

using namespace std;
class DeviceFinder
{
public:
	DeviceFinder(const WCHAR *pFriendlyName, const WCHAR *pMonikerName) : strFriendlyName(pFriendlyName), strMonikerName(pMonikerName) {}
	bool operator () (std::vector<DeviceName>::value_type &value)
	{
		bool bRet = false;
		if ( wcscmp(value.FriendlyName, strFriendlyName.c_str())==0  &&  wcscmp(value.MonikerName,  strMonikerName.c_str())!=0)
		{
			bRet = true;
		}
		return bRet;
	}
private:
	wstring	strFriendlyName;
	wstring	strMonikerName;
};

class CCaptureVideo
{
public:
	CCaptureVideo();
	~CCaptureVideo();
	
public:
	HRESULT							EnumDevices(std::vector<DeviceName> &vecDevice);
	HRESULT							InitializeGraph();
	
protected:
	IGraphBuilder*					m_pGB;
	ICaptureGraphBuilder2*			m_pCapture;
};

#endif