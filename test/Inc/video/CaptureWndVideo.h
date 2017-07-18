#ifndef __CAPTUREVIDEO_H__
#define __CAPTUREVIDEO_H__

#include <atlbase.h>
#include <windows.h>
#include <DShow.h>
#include <qedit.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#endif

#define FILTER_NAME_VCS					L"Video Capture Source"
#define FILTER_NAME_GRABBER				L"Sample Grabber"
#define FILTER_NAME_NULL				L"Null Renderer"

#define MAX_FRIENDLY_NAME_LEN			128
#define MAX_MONIKER_NAME_LEN			128

typedef struct tagDeviceName{
	UINT	nDeviceID;
	CHAR	FriendlyName[MAX_FRIENDLY_NAME_LEN];
	CHAR	MonikerName[MAX_MONIKER_NAME_LEN];
}DeviceName;


using namespace std;
class DeviceFinder
{
public:
	DeviceFinder(const CHAR* pFriendlyName, const CHAR* pMonikerName) : strFriendlyName(pFriendlyName), strMonikerName(pMonikerName) {}
	bool operator () (std::list<DeviceName>::value_type &value)
	{
		bool bRet = false;
		if ( strcmp(value.FriendlyName, strFriendlyName.c_str())==0  &&  strcmp(value.MonikerName,  strMonikerName.c_str())!=0)
		{
			bRet = true;
		}
		return bRet;
	}
private:
	string	strFriendlyName;
	string	strMonikerName;
};


class CCaptureWndVideo
{
public:
	CCaptureWndVideo();
	virtual ~CCaptureWndVideo();
	
	static BOOL				EnumDevices(std::list<DeviceName> &listDevice);

public:	
	BOOL					OpenCamera(UINT ulDeviceID, HWND hWnd);
	BOOL					CloseCamera();

	BOOL					CaptureImages(LPCTSTR lpszFileName);
	const unsigned char*	GrabFrame();

protected:
	HRESULT 				PreviewVideo(UINT ulDeviceID, HWND hWnd);

	HRESULT					InitCaptureGraphBuilder();
	HRESULT					DestroyGraphBuilder();
		
	HRESULT 				AddSourceFilter(UINT ulDeviceID, IBaseFilter* &pFilter);
	HRESULT					AddSampleGrabber(IBaseFilter* &pFilter);
	HRESULT 				AddNullRenderer(IBaseFilter* &pFilter);
	
	HRESULT					SetMediaType(AM_MEDIA_TYPE *pMediaType);
	HRESULT					GetMediaType(AM_MEDIA_TYPE *pMediaType);
	HRESULT					FreeMediaType(AM_MEDIA_TYPE *pMediaType);

	void					ConfigCameraPin(HWND hwndParent);

protected:
	BOOL					SaveBitmapToFile(HBITMAP hBitmap,LPCSTR lpFileName);

	HRESULT					SetupVideoWindow();
	void					ResizeVideoWindow();	

protected:
	ICaptureGraphBuilder2*	m_pCaptureGB;
	IGraphBuilder*			m_pGraphBuilder;

	IMediaControl*			m_pMediaControl;
	IVideoWindow*			m_pVideoWindow;
	IMediaEventEx*			m_pMediaEvent; 

	ISampleGrabber			*m_pSampleGrabber;

private:
	HWND					m_hWnd;

	UINT					m_nWidth;
	UINT					m_nHeight;
	UINT					m_nChannel;	
	

	LONG					m_nBufferSize;
	BYTE*					m_pFrameData;

//	IBasicVideo				*m_pBV;
	
//	IBaseFilter				*m_pBF;
//	IBaseFilter				*m_pMux;
};

#endif