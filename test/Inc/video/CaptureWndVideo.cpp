#include "stdafx.h"
#include "CaptureWndVideo.h"

#pragma comment(lib, "strmiids.lib")
CCaptureWndVideo::CCaptureWndVideo()
{
		CoInitialize(NULL);

		m_nWidth		= 0;
		m_nHeight		= 0;
		m_nChannel		= 0;

		m_nBufferSize	= 0;

		m_hWnd			= NULL;
		m_pFrameData	= NULL;
		
		
		m_pGraphBuilder  = NULL;
		m_pCaptureGB	 = NULL;

		m_pMediaControl	 = NULL;
		m_pVideoWindow   = NULL;

		m_pSampleGrabber = NULL;
}

CCaptureWndVideo::~CCaptureWndVideo()
{
	CoUninitialize( );	
}

//////////////////////////////////////////////////////////////////////////
//
BOOL CCaptureWndVideo::EnumDevices(std::list<DeviceName> &listDevice)
{
	HRESULT hr;

	int		nID = 0;
	int		nIndex = 0;

	ULONG	cFetched  = 0;

	CHAR	pFriendlyName[MAX_PATH] = {0};
	CHAR    pMonikerName[MAX_PATH]  = {0};

	ICreateDevEnum *pCreateDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if(hr != S_OK)
	{
		return FALSE;
	}

	IEnumMoniker *pEm = NULL;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if (hr != S_OK)
	{
		pCreateDevEnum->Release();
		pCreateDevEnum = NULL;

		return FALSE;
	}

	pEm->Reset();

	IMoniker *pM = NULL;
	while(pEm->Next(1, &pM, &cFetched) == S_OK)
	{
		memset(pFriendlyName, 0x0, MAX_PATH);
		memset(pMonikerName,  0x0, MAX_PATH);

		IPropertyBag* pBag = NULL;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pBag);
		if(hr != S_OK)
		{
			pM->Release();
			pM = NULL;

			continue;
		}

		VARIANT var;
		VariantInit(&var);

		hr = pBag->Read(L"FriendlyName", &var, NULL);
		if(hr != S_OK)
		{
			pBag->Release();
			pBag = NULL;

			VariantClear(&var);
			continue;
		}

		LPOLESTR pOleDisplayName = reinterpret_cast<LPOLESTR>(CoTaskMemAlloc(MAX_MONIKER_NAME_LEN * 2));
		if(pOleDisplayName == NULL)
		{
			if (pM)
			{
				pM->Release();
				pM = NULL;
			}

			if (pBag)
			{
				pBag->Release();
				pBag = NULL;
			}

			VariantClear(&var);
			continue;
		}

		hr = pM->GetDisplayName(NULL, NULL, &pOleDisplayName);
		if(FAILED(hr))
		{
			if (pM)
			{
				pM->Release();
				pM = NULL;
			}

			if (pBag)
			{
				pBag->Release();
				pBag = NULL;
			}

			VariantClear(&var);
			continue;
		}

		StringCchCopy(pFriendlyName, MAX_FRIENDLY_NAME_LEN, COLE2T(var.bstrVal));		//wcscpy
		StringCchCopy(pMonikerName,  MAX_MONIKER_NAME_LEN,  COLE2T(pOleDisplayName));	//wcscpy

		std::list<DeviceName>::iterator iter = find_if(listDevice.begin(), listDevice.end(), DeviceFinder(pFriendlyName, pMonikerName));
		if (iter != listDevice.end())
		{
			sprintf(pFriendlyName, _T("%s - #%d"), pFriendlyName, ++nIndex);
		}

		DeviceName deviceName = {0};

		strcpy(deviceName.FriendlyName, pFriendlyName);
		strcpy(deviceName.MonikerName, pMonikerName);
		deviceName.nDeviceID = nID;

		listDevice.push_back(deviceName);

		nID++;
		CoTaskMemFree(pOleDisplayName);

		VariantClear(&var);

		pBag->Release();
		pM->Release();
	}

	pEm->Release();
	pCreateDevEnum->Release();

	return TRUE;
}

BOOL CCaptureWndVideo::OpenCamera(UINT ulDeviceID, HWND hWnd)
{
	HRESULT hr;

	hr = InitCaptureGraphBuilder();
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = PreviewVideo(ulDeviceID, hWnd);
	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCaptureWndVideo::CloseCamera()
{
	HRESULT hr;

	hr = DestroyGraphBuilder();
	if (FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
HRESULT CCaptureWndVideo::PreviewVideo(UINT ulDeviceID, HWND hWnd)
{	
	HRESULT hr;

	AM_MEDIA_TYPE mediaType;
	ZeroMemory(&mediaType, sizeof(AM_MEDIA_TYPE));

	IBaseFilter *pSourceFilter = NULL;
	hr = AddSourceFilter(ulDeviceID, pSourceFilter);
	if(FAILED(hr))
	{
		return hr;
	}

	IBaseFilter *pGrabberFilter = NULL;
	hr = AddSampleGrabber(pGrabberFilter);
	if(FAILED(hr))
	{
		return hr;
	}

	IBaseFilter *pNullFilter = NULL;
	hr = AddNullRenderer(pNullFilter);
	if(FAILED(hr))
	{
		return hr; 		
	}

	hr = SetMediaType(&mediaType);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pCaptureGB->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pSourceFilter, pGrabberFilter, NULL);
	if(FAILED(hr))
	{
		hr = m_pCaptureGB->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSourceFilter, pGrabberFilter, NULL);
		if (FAILED(hr))
		{
			return hr;
		}	
	}

	hr = GetMediaType(&mediaType);
	if (FAILED(hr))
	{
		return hr;
	}

	pSourceFilter->Release();
	pGrabberFilter->Release();
	pNullFilter->Release();

	hr = m_pSampleGrabber->SetOneShot(FALSE);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pSampleGrabber->SetBufferSamples(TRUE);
	if (FAILED(hr))
	{
		return hr;
	}

	m_hWnd = hWnd;
	SetupVideoWindow();

	hr = m_pMediaControl->Run();
	if(FAILED(hr))
	{
		return hr;
	}

	hr = FreeMediaType(&mediaType);
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CCaptureWndVideo::InitCaptureGraphBuilder()
{
	HRESULT hr;
	
	//创建IGraphBuilder接口
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder);
	if(FAILED(hr))
	{
		return hr;
	}
	
	//创建 ICaptureGraphBuilder2接口
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&m_pCaptureGB);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl); 
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow,(void**)&m_pVideoWindow);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,(LPVOID*)&m_pMediaEvent);
	if (FAILED(hr))
	{
		return hr;
	}

//	hr = m_pMediaEvent->SetNotifyWindow((OAHWND)m_App,WM_GRAPHNOTIFY,0);

// 	hr = m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void**)&m_pBV);
// 	if (FAILED(hr))
// 	{
// 		return hr;
// 	}

	hr = m_pCaptureGB->SetFiltergraph(m_pGraphBuilder);
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CCaptureWndVideo::DestroyGraphBuilder()
{
	if (m_pMediaControl)
	{
		m_pMediaControl->Stop();
	}

	if (m_pVideoWindow)
	{
		m_pVideoWindow->put_Owner(NULL);
		m_pVideoWindow->put_Visible(OAFALSE);
	}

	if (m_pFrameData)
	{
		delete m_pFrameData;
		m_pFrameData = NULL;
	}

	m_hWnd = NULL;

	SAFE_RELEASE(m_pCaptureGB);
	SAFE_RELEASE(m_pGraphBuilder);
	SAFE_RELEASE(m_pVideoWindow);
	SAFE_RELEASE(m_pMediaControl);

	return S_OK;
}

HRESULT CCaptureWndVideo::AddSourceFilter(UINT ulDeviceID, IBaseFilter* &pFilter)
{
	HRESULT hr;
	UINT  ulIndex  = 0;
	ULONG cFetched = 0;

	ICreateDevEnum *pCreateDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if (FAILED(hr))
	{
		return hr;
	}

	IEnumMoniker *pEnumMon = NULL;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMon, 0);
	if (FAILED(hr))
	{
		return hr;
	}

	pEnumMon->Reset();

	IMoniker *pMoniker = NULL;
	while(pEnumMon->Next(1, &pMoniker, &cFetched) == S_OK)
	{
		IPropertyBag* pBag = NULL;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pBag);
		if(SUCCEEDED(hr))
		{
			if (ulIndex == ulDeviceID)
			{
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pFilter);
				if (SUCCEEDED(hr))
				{
					hr = m_pGraphBuilder->AddFilter(pFilter, FILTER_NAME_VCS);

					pFilter->Release();
				}

				pMoniker->Release();
				break;
			}
		}

		ulIndex++;
		pMoniker->Release();		
	}

	pEnumMon->Release();

// 	IBindCtx *pBindCtx = NULL; 
// 	hr = CreateBindCtx(0, &pBindCtx);	//创建一个绑定环境对象
// 	if(SUCCEEDED(hr))
// 	{
// 		IMoniker *pM = NULL;
// 		ULONG chEaten = 0;  
// 		
// 		hr = MkParseDisplayName(pBindCtx, vecDeviceList[uIndex].MonikerName, &chEaten, &pM);	//根据显示名进行解析的结果生成名字对象，并以IMoniker指针的方式返回给客户
// 		if(SUCCEEDED(hr))
// 		{
// 			hr = BindMoniker(pM, 0, IID_IBaseFilter, (void**)&pFilter);
// 			if (SUCCEEDED(hr))
// 			{
// 				hr = m_pGB->AddFilter(pFilter, FILTER_NAME_VCS);
// 
// 				pFilter->Release();
// 			}
// 			
// 			pM->Release();
// 		}
// 		
// 		pBindCtx->Release();
// 	}
	
	return hr;
}

HRESULT CCaptureWndVideo::AddSampleGrabber(IBaseFilter* &pFilter)
{
	HRESULT hr;

	//
	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pFilter);
	if (FAILED(hr))
	{
		return hr;
	}

	//
	hr = pFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabber);
	if (FAILED(hr))
	{
		return hr;
	}

	//
	hr = m_pGraphBuilder->AddFilter(pFilter, FILTER_NAME_GRABBER);
	if(FAILED(hr))
	{
		return  hr;
	}

	return hr;
}

HRESULT CCaptureWndVideo::AddNullRenderer(IBaseFilter* &pFilter)
{
	HRESULT hr;
	
	hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pFilter);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = m_pGraphBuilder->AddFilter(pFilter, FILTER_NAME_NULL);
	if(FAILED(hr))
	{
		return hr;
	}

	return  hr;
}

HRESULT CCaptureWndVideo::SetMediaType(AM_MEDIA_TYPE* pMediaType)
{
	HRESULT hr;

	if (pMediaType == NULL)
	{
		return E_FAIL;
	}

	HDC hDC = GetDC(NULL);
	int nBitDepth = GetDeviceCaps(hDC, BITSPIXEL);
	ReleaseDC(NULL, hDC);

	pMediaType->majortype  = MEDIATYPE_Video;
	pMediaType->formattype = FORMAT_VideoInfo;

	switch(nBitDepth)
	{
	case 8:
		pMediaType->subtype=MEDIASUBTYPE_RGB8;
		break;

	case 16:
		pMediaType->subtype=MEDIASUBTYPE_RGB555; 
		break;

	case 24:
		pMediaType->subtype=MEDIASUBTYPE_RGB24;
		break;

	case 32:
		pMediaType->subtype=MEDIASUBTYPE_RGB32;
		break;

	default:
		return E_FAIL;
	}

	hr = m_pSampleGrabber->SetMediaType(pMediaType);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

HRESULT CCaptureWndVideo::GetMediaType(AM_MEDIA_TYPE* pMediaType)
{
	HRESULT hr;

	if (pMediaType == NULL)
	{
		return E_FAIL;
	}

	hr = m_pSampleGrabber->GetConnectedMediaType(pMediaType);
	if(FAILED(hr))
	{
		return hr;
	}

	VIDEOINFOHEADER *pVih = reinterpret_cast<VIDEOINFOHEADER*>(pMediaType->pbFormat);
	m_nChannel = pVih->bmiHeader.biBitCount / 8;
	m_nWidth   = pVih->bmiHeader.biWidth;
	m_nHeight  = pVih->bmiHeader.biHeight;
	
	return S_OK;
}

HRESULT CCaptureWndVideo::FreeMediaType(AM_MEDIA_TYPE* pMediaType)
{
	if (pMediaType == NULL)
	{
		return E_FAIL;
	}

	if (pMediaType->cbFormat != 0)  
	{
		CoTaskMemFree(pMediaType->pbFormat);
		pMediaType->cbFormat = 0; 
		pMediaType->pbFormat = NULL;
	}	

	if (pMediaType->pUnk != NULL)
	{
		pMediaType->pUnk->Release(); 
		pMediaType->pUnk = NULL;
	}

	return S_OK;
}

HRESULT CCaptureWndVideo::SetupVideoWindow()
{
	HRESULT hr;

	hr = m_pVideoWindow->put_Visible(OAFALSE);				
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pVideoWindow->put_Owner((OAHWND)m_hWnd);					//设置视频窗口
	if(FAILED(hr))
	{
		return hr;
	}

	hr = m_pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN);
	if(FAILED(hr))
	{
		return hr;
	}

	ResizeVideoWindow();

	hr = m_pVideoWindow->put_Visible(OATRUE);				
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void CCaptureWndVideo::ResizeVideoWindow()
{
	CRect rcWnd;

	if(m_pVideoWindow)
	{
		::GetClientRect(m_hWnd, &rcWnd);
		m_pVideoWindow->SetWindowPosition(0, 0, rcWnd.right, rcWnd.bottom);

//		long m =0, n=0;
//		m_pBV->GetVideoSize(&m, &n);		
	}
}

const unsigned char* CCaptureWndVideo::GrabFrame()
{
	HRESULT hr;

	hr = m_pSampleGrabber->SetBufferSamples(TRUE);
	if (FAILED(hr))
	{
		return NULL;
	}

	LONG nSize = 0;
	hr = m_pSampleGrabber->GetCurrentBuffer(&nSize, NULL);
	if(FAILED(hr))
	{
		return NULL;
	}
	
	if(nSize != m_nBufferSize)
	{
		m_nBufferSize = nSize;
		if(m_pFrameData != NULL)
		{
			delete[] m_pFrameData;
			m_pFrameData = NULL;
		}
		
		m_pFrameData = new unsigned char[m_nBufferSize];
	}
	
	memset(m_pFrameData, 0x0, m_nBufferSize);
	hr = m_pSampleGrabber->GetCurrentBuffer(&nSize, reinterpret_cast<LONG*>(m_pFrameData));
	if(FAILED(hr))
	{
		return NULL;
	}

	m_pSampleGrabber->SetOneShot(FALSE);	//FALSE:截图后继续运行graph; TRUE:STOP RUN GRAPH
	m_pSampleGrabber->SetBufferSamples(TRUE);
	
	return m_pFrameData;
}

BOOL CCaptureWndVideo::CaptureImages(LPCTSTR lpszFileName)
{
	HRESULT hr;
	BOOL bRet = FALSE;

	BITMAPINFO BitmapInfo;  
	ZeroMemory(&BitmapInfo, sizeof(BitmapInfo)); 

	AM_MEDIA_TYPE mediaType;  
	ZeroMemory(&mediaType, sizeof(AM_MEDIA_TYPE));

	HBITMAP hBitmap = NULL;
	VIDEOINFOHEADER* pVideoHeader = NULL;

	if (lpszFileName==NULL || strlen(lpszFileName)<=0)
	{
		return FALSE;
	}

	hr = m_pSampleGrabber->GetConnectedMediaType(&mediaType);
	if(FAILED(hr))
	{
		return FALSE;
	}

	if (mediaType.formattype == FORMAT_VideoInfo &&
		mediaType.cbFormat >= sizeof(VIDEOINFOHEADER) &&
		mediaType.pbFormat != NULL)
	{
		pVideoHeader = (VIDEOINFOHEADER*)mediaType.pbFormat;
	}
	else
	{
		return FALSE;
	}

	CopyMemory(&BitmapInfo.bmiHeader, &(pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));

	unsigned char* pBuffer = (unsigned char*)GrabFrame();
	if (pBuffer == NULL)
	{
		return FALSE;
	}

	hBitmap = ::CreateDIBitmap(::GetDC(NULL), &(pVideoHeader->bmiHeader), CBM_INIT, pBuffer, &BitmapInfo, DIB_RGB_COLORS);
	if (hBitmap != NULL)
	{
		bRet = SaveBitmapToFile(hBitmap, lpszFileName);

		DeleteObject(hBitmap);
	}

	FreeMediaType(&mediaType);
	return bRet;
}

void CCaptureWndVideo::ConfigCameraPin(HWND hwndParent)
{
// 	HRESULT hr;
// 	IAMStreamConfig *pSC ;
// 	ISpecifyPropertyPages *pSpec ;	
// 	CAUUID cauuid ;
// 	
// 	
// 	hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,m_pBF,IID_IAMStreamConfig,(void**)&pSC);
// 	if (hr != NOERROR)
// 	{
// 		return;
// 	}
// 	
// 	hr = pSC->QueryInterface(IID_ISpecifyPropertyPages,(void**)&pSpec);
// 	if(hr == S_OK)
// 	{
// 		hr = pSpec->GetPages(&cauuid);
// 		
// 		hr = OleCreatePropertyFrame(hwndParent,30,30,NULL,1,(IUnknown**)&pSC,cauuid.cElems,(GUID*)cauuid.pElems,0,0,NULL);
// 		
// 		CoTaskMemFree(cauuid.pElems);
// 		pSpec->Release();
// 		pSC->Release();
// 	}
// 
}

BOOL CCaptureWndVideo::SaveBitmapToFile(HBITMAP hBitmap,LPCSTR lpFileName) 
{   
	HDC            hDC; //设备描述表
	int            iBits;//当前显示分辨率下每个像素所占字节数
	WORD           wBitCount;//位图中每个像素所占字节数    
	DWORD          dwPaletteSize=0;//定义调色板大小
	DWORD          dwBmBitsSize;//位图中像素字节大小
	DWORD          dwDIBSize;// 位图文件大小
	DWORD          dwWritten;//写入文件字节数
	BITMAP         Bitmap;//位图结构
	BITMAPFILEHEADER   bmfHdr;   //位图属性结构    
	BITMAPINFOHEADER   bi;       //位图文件头结构
	LPBITMAPINFOHEADER lpbi;     //位图信息头结构     指向位图信息头结构
	HANDLE          fh;//定义文件句柄
	HANDLE            hDib;//分配内存句柄
	HANDLE            hPal;//分配内存句柄
	HANDLE          hOldPal=NULL;//调色板句柄  

	//计算位图文件每个像素所占字节数    
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);    
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);    
	DeleteDC(hDC);

	if (iBits <= 1)        
		wBitCount = 1;    
	else if (iBits <= 4)        
		wBitCount = 4;    
	else if (iBits <= 8)        
		wBitCount = 8;    
	else if (iBits <= 24)
		wBitCount = 24;
	else if (iBits<=32)
		wBitCount = 24;

	//计算调色板大小    
	if (wBitCount <= 8)        
		dwPaletteSize = (1 << wBitCount) *sizeof(RGBQUAD);

	//设置位图信息头结构    
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);    
	bi.biSize            = sizeof(BITMAPINFOHEADER);    
	bi.biWidth           = Bitmap.bmWidth;    
	bi.biHeight          = Bitmap.bmHeight;    
	bi.biPlanes          = 1;    
	bi.biBitCount         = wBitCount;        
	bi.biCompression      = BI_RGB;    
	bi.biSizeImage        = 0;    
	bi.biXPelsPerMeter     = 0;    
	bi.biYPelsPerMeter     = 0;    
	bi.biClrUsed         = 0;    
	bi.biClrImportant      = 0;    
	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;

	//为位图内容分配内存    
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));    
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	if (lpbi==NULL)
	{
		return FALSE;
	}

	*lpbi = bi;    
	// 处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);    
	if (hPal)
	{        
		hDC  = GetDC(NULL);        
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);        
		RealizePalette(hDC);        
	}   

	// 获取该调色板下新的像素值    
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,        
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);  

	//恢复调色板       
	if (hOldPal)        
	{        
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);        
		RealizePalette(hDC);        
		ReleaseDC(NULL, hDC);        
	}    

	//创建位图文件  
	fh = CreateFile(lpFileName, GENERIC_WRITE,         
		0, NULL, CREATE_ALWAYS,        
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);    

	if (fh == INVALID_HANDLE_VALUE)        
		return FALSE;

	// 设置位图文件头    
	bmfHdr.bfType = 0x4D42;  // "BM"    
	dwDIBSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ dwPaletteSize + dwBmBitsSize;      
	bmfHdr.bfSize = dwDIBSize;    
	bmfHdr.bfReserved1 = 0;    
	bmfHdr.bfReserved2 = 0;    
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER)+ dwPaletteSize;

	// 写入位图文件头    
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// 写入位图文件其余内容    
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	//清除       
	GlobalUnlock(hDib);    
	GlobalFree(hDib);    
	CloseHandle(fh);

	return TRUE;
}