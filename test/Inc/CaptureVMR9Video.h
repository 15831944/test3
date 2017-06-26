#ifndef __CAPTUREVMR9VIDEO_H__
#define __CAPTUREVMR9VIDEO_H__

#include <atlimage.h>

#include <DShow.h>
#include <d3d9.h>
#include <vmr9.h>

//#include <d3dx9tex.h>



#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#endif

#define FILTER_NAME_VMR9 				L"Video Mixing Renderer 9"
#define FILTER_NAME_VCS					L"Video Capture Source"
#define FILTER_NAME_GRABBER				L"Sample Grabber"
#define MAX_FRIENDLY_NAME_LEN			128
#define MAX_MONIKER_NAME_LEN			128


typedef struct tagDeviceName{
	WCHAR	FriendlyName[MAX_FRIENDLY_NAME_LEN];
	WCHAR	MonikerName[MAX_MONIKER_NAME_LEN];
}DeviceName;

typedef struct tagVideoFormat{
	LONG	biWidth; 
	LONG	biHeight;
	WORD	biBitCount;
}VideoFormat;

class CCaptureVMR9Video
{
public:
	CCaptureVMR9Video();
	virtual ~CCaptureVMR9Video();
	
public:
	HRESULT 					EnumDevices(std::vector<DeviceName> &vecDeviceList);
	HRESULT 					InitCaptureGraphBuilder();	
	HRESULT 					PreviewVideo(WCHAR* FriendlyName, std::vector<DeviceName> &vecDeviceList, HWND hWnd);
	HRESULT 					SetMediaControl(FILTER_STATE uCommandType);
	HRESULT 					GetFilterGraphState(LONG msTimeout, FILTER_STATE &fs);
	
	HRESULT						SetFormat(std::vector<VideoFormat> &vecFormat,    int nIndex);
	HRESULT						UpdateFormat(std::vector<VideoFormat> &vecFormat, int nIndex, BOOL bState);
	HRESULT						GetVideoFormat(std::vector<VideoFormat> &vecFormat);
	
	int 						GetCurFormatIndex(std::vector<VideoFormat> &vecFormat);
	void						DeleteMediaType(AM_MEDIA_TYPE *pmt);

	HRESULT						DisconnectFilter(IBaseFilter* pSourceFilter, IBaseFilter* pSinkFilter);

	HBITMAP						GetCurrentDDBImage(HDC hDC);
	HRESULT						DDB2PNG(LPCTSTR pszFileName, HBITMAP hBitmap);
	HRESULT						DDB2JPEG(LPCTSTR pszFileName, HBITMAP hBitmap);

	void						DisplayImage(HBITMAP hBitmap, HWND hWnd);
protected:
	HRESULT 					AddSourceFilter(WCHAR* FriendlyName, std::vector<DeviceName> &vecDeviceList, IBaseFilter* &pFilter);
	HRESULT 					AddSinkFilter(HWND hWnd, IBaseFilter* &pFilter);
	HRESULT						AddSampleGrabber(IBaseFilter* &pFilter);

	HRESULT						GetCurrentDIBImage(LPBYTE &pDIB);
	HBITMAP						DIB2DDB(LPBYTE pDIB, HDC hDC);
	void						FreeDIB(LPBYTE &pDIB);
	
private:
	IGraphBuilder 				*m_pGB;	
	ICaptureGraphBuilder2 		*m_pCapture;
//	ISampleGrabber				*m_pGrabber;

	IMediaControl				*m_pMC;

	IVideoWindow				*m_pVW;
	IBasicVideo					*m_pBV;
};

#endif