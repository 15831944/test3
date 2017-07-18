#include "stdafx.h"
#include "CaptureVMR9Video.h"

#pragma comment( lib, "strmiids.lib" )
// #pragma comment( lib, "Quartz.lib" )
// #pragma comment( lib, "d3d9.lib" )
// #pragma comment( lib, "d3dx9.lib" )

CCaptureVMR9Video::CCaptureVMR9Video()
{
	CoInitialize(NULL);

	m_pGB		= NULL;
	m_pCapture	= NULL;
	m_pMC		= NULL;

	m_pVW		= NULL;
	m_pBV		= NULL;
}

CCaptureVMR9Video::~CCaptureVMR9Video()
{
	if (m_pMC)
	{
		m_pMC->Stop();
	}

	if (m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner(NULL);
	}

	SAFE_RELEASE(m_pCapture);
	SAFE_RELEASE(m_pMC);
	SAFE_RELEASE(m_pGB);
	SAFE_RELEASE(m_pBV);

	CoUninitialize( );	
}


HRESULT CCaptureVMR9Video::EnumDevices(std::vector<DeviceName> &vecDeviceList)
{
	HRESULT hr;
	ULONG cFetched = 0;
	
	ICreateDevEnum *pCreateDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
	if(FAILED(hr))
	{
		return hr;
	}
	
	IEnumMoniker *pEm = NULL;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	if(FAILED(hr))
	{
		return hr;
	}
	
	pEm->Reset();
	
	IMoniker *pM = NULL;
	while(pEm->Next(1, &pM, &cFetched) == S_OK)
	{
		DeviceName deviceName={0};
		
		IPropertyBag* pBag = NULL;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pBag);
		if(SUCCEEDED(hr))
		{
			VARIANT var;
			VariantInit(&var);
			
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if(SUCCEEDED(hr))
			{
				StringCchCopyW(deviceName.FriendlyName, MAX_FRIENDLY_NAME_LEN, var.bstrVal);
//				wcscpy(deviceName.FriendlyName, var.bstrVal);
				
				LPOLESTR pOleDisplayName = reinterpret_cast<LPOLESTR>(CoTaskMemAlloc(MAX_MONIKER_NAME_LEN * 2));
				if(pOleDisplayName != NULL)
				{
					hr = pM->GetDisplayName(NULL, NULL, &pOleDisplayName);
					if(SUCCEEDED(hr))
					{
						StringCchCopyW(deviceName.MonikerName, MAX_MONIKER_NAME_LEN, pOleDisplayName);
//						wcscpy(deviceName.MonikerName, pOleDisplayName);
						
						vecDeviceList.push_back(deviceName);
					}
					
					CoTaskMemFree(pOleDisplayName);
				}
			}
			
			VariantClear(&var);
			pBag->Release();
		}
		
		//注意：IEnumMoniker::Next增加IMoniker引用计数
		pM->Release();
	}
	
	pCreateDevEnum->Release();
	
	return hr;
}


HRESULT CCaptureVMR9Video::InitCaptureGraphBuilder()
{
	HRESULT hr;
	
	//创建IGraphBuilder接口
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGB);
	if(FAILED(hr))
	{
		return hr;
	}
	
	//创建 ICaptureGraphBuilder2接口
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&m_pCapture);
	if(FAILED(hr))
	{
		return hr;
	}
	
// 	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&m_pGrabber);
// 	if (FAILED(hr))
// 	{
// 		return hr;
// 	}

	hr = m_pCapture->SetFiltergraph(m_pGB);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = m_pGB->QueryInterface(IID_IMediaControl, (void**)&m_pMC);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = m_pGB->QueryInterface(IID_IVideoWindow,(void**)&m_pVW);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = m_pGB->QueryInterface(IID_IBasicVideo, (void**)&m_pBV);
	if (FAILED(hr))
	{
		return hr;
	}
	
	return hr;
}


HRESULT CCaptureVMR9Video::AddSourceFilter(WCHAR* FriendlyName, std::vector<DeviceName> &vecDeviceList, IBaseFilter* &pFilter)
{
	if((FriendlyName == NULL) || (m_pGB == NULL))
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	
	UINT  uIndex = 0;
	DWORD dwSize = 0;
	
	dwSize = vecDeviceList.size();
	for(uIndex=0; uIndex < dwSize; uIndex++)
	{
		if(wcscmp(FriendlyName, vecDeviceList[uIndex].FriendlyName) == 0)
		{
			break;
		}
	}
	
	if(uIndex >= dwSize)
	{
		return E_FAIL;
	}
	
	IBindCtx *pBindCtx; 
	hr = CreateBindCtx(0, &pBindCtx);	//创建一个绑定环境对象
	if(SUCCEEDED(hr))
	{
		IMoniker *pM = NULL;
		ULONG chEaten = 0;  
		
		hr = MkParseDisplayName(pBindCtx, vecDeviceList[uIndex].MonikerName, &chEaten, &pM);	//根据显示名进行解析的结果生成名字对象，并以IMoniker指针的方式返回给客户
		if(SUCCEEDED(hr))
		{
			hr = BindMoniker(pM, 0, IID_IBaseFilter, (void**)&pFilter);
			if (SUCCEEDED(hr))
			{
				hr = m_pGB->AddFilter(pFilter, FILTER_NAME_VCS);

				pFilter->Release();
			}
			
			pM->Release();
		}
		
		pBindCtx->Release();
	}
	
	return hr;
}


HRESULT CCaptureVMR9Video::AddSinkFilter(HWND hWnd, IBaseFilter* &pFilter)
{
	if((hWnd == NULL) || (m_pGB == NULL))
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	
	hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pFilter);
	if(SUCCEEDED(hr))
	{
		hr = m_pGB->AddFilter(pFilter, FILTER_NAME_VMR9);
		if(SUCCEEDED(hr))
		{
			IVMRFilterConfig9* pConfig9; 
			hr = pFilter->QueryInterface(IID_IVMRFilterConfig9, (void**)&pConfig9);
			if(SUCCEEDED(hr))
			{
//				pConfig9->SetNumberOfStreams(2);
				pConfig9->SetRenderingMode(VMR9Mode_Windowless);
//				pConfig9->SetRenderingPrefs(RenderPrefs_AllowOverlays);
				pConfig9->Release();
			}
			pConfig9 = NULL;
			
			IVMRWindowlessControl9* pVMRWindowlessControl = NULL;
			hr = pFilter->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pVMRWindowlessControl);
			if(SUCCEEDED(hr))
			{
				RECT rc;
				::GetClientRect(hWnd, &rc);

				pVMRWindowlessControl->SetVideoPosition(NULL, &rc);
				pVMRWindowlessControl->SetVideoClippingWindow(hWnd);
				
				pVMRWindowlessControl->Release();
			}
		}
		
		pFilter->Release();
	}
	
	return hr;
}

HRESULT CCaptureVMR9Video::AddSampleGrabber(IBaseFilter* &pFilter)
{
	if((m_pGB == NULL) || (m_pCapture == NULL) /*|| (m_pGrabber == NULL)*/)
	{
		return E_INVALIDARG;
	}

	HRESULT hr;
/*
	hr = m_pGB->AddFilter(m_pGrabber, FILTER_NAME_GRABBER);
	if (FAILED(hr))
	{
		return hr;
	}


	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));

	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;

	hr = m_pGrabber->SetMediaType(&mt);
	if (FAILED(hr))
	{
		return hr;
	}

	DeleteMediaType(mt);

	hr = m_pGrabber->SetOneShot(FALSE);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pGrabber->SetBufferSamples(TRUE);
	if (FAILED(hr))
	{
		return hr;
	}
*/
	return hr;
}


HRESULT CCaptureVMR9Video::PreviewVideo(WCHAR* FriendlyName, std::vector<DeviceName> &vecDeviceList, HWND hWnd)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	
	IBaseFilter *pSourceFilter = NULL; 
	hr = AddSourceFilter(FriendlyName, vecDeviceList, pSourceFilter);
	if(FAILED(hr))
	{
		return hr;
	}
	
	IBaseFilter *pSinkFilter = NULL;  
	hr = AddSinkFilter(hWnd, pSinkFilter);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pSourceFilter, NULL, pSinkFilter);
	if(FAILED(hr))
	{
		return hr;
	}
	
	return hr;
}


HRESULT CCaptureVMR9Video::SetMediaControl(FILTER_STATE uCommandType)
{
	if((m_pGB == NULL) || (m_pCapture == NULL) || (m_pMC == NULL))
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	
	switch(uCommandType)
	{
	case State_Running:
		hr = m_pMC->Run();
		break;
			
	case State_Stopped:
		hr = m_pMC->Stop();	
		break;
			
	case State_Paused:
		hr = m_pMC->Pause();
		break;
	}
	
	return hr;
}


HRESULT CCaptureVMR9Video::GetFilterGraphState(LONG msTimeout, FILTER_STATE &fs)
{
	if((m_pGB == NULL) || (m_pCapture == NULL) || (m_pMC == NULL))
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	
	hr = m_pMC->GetState(msTimeout, (OAFilterState*)&fs);
	
	return hr;
}


HRESULT CCaptureVMR9Video::SetFormat(std::vector<VideoFormat> &vecFormat, int nIndex)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return E_INVALIDARG;
	}

	if ((nIndex<0) || (nIndex >= vecFormat.size()))
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	int nCount = 0;
	int nSize  = 0;

	BOOL bOK = FALSE;

	IBaseFilter *pSourceFilter = NULL;
	hr = m_pGB->FindFilterByName(FILTER_NAME_VCS, &pSourceFilter);
	if (FAILED(hr))
	{
		return hr;
	}

	IAMStreamConfig *pIAMS;
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSourceFilter, IID_IAMStreamConfig, (LPVOID*)&pIAMS);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pIAMS->GetNumberOfCapabilities(&nCount, &nSize);
	if (FAILED(hr))
	{
		return hr;
	}

	if (nSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
	{
		VIDEO_STREAM_CONFIG_CAPS scc;

		for (int i = 0; i<nCount; i++)
		{
			AM_MEDIA_TYPE *pmt = NULL;
			hr = pIAMS->GetStreamCaps(i, &pmt, reinterpret_cast<LPBYTE>(&scc));
			if (SUCCEEDED(hr))
			{
				if (pmt->formattype == FORMAT_VideoInfo)
				{
					// 获取视频格式
					VIDEOINFOHEADER *pVIH = reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);
					LONG biWidth	= pVIH->bmiHeader.biWidth;  
					LONG biHeight	= pVIH->bmiHeader.biHeight;  
					WORD biBitCount = pVIH->bmiHeader.biBitCount;

					if ((biWidth  == vecFormat[nIndex].biWidth)  &&
						(biHeight == vecFormat[nIndex].biHeight) &&
						(biBitCount == vecFormat[nIndex].biBitCount))
					{
						hr = pIAMS->SetFormat(pmt);
						bOK = TRUE;
					}
				}

				DeleteMediaType(pmt);
			}

			if (bOK)
			{
				break;
			}
		}
	}

	if (!bOK)
		hr = E_FAIL;

	pIAMS->Release();

	return hr;
}


HRESULT CCaptureVMR9Video::UpdateFormat(std::vector<VideoFormat> &vecFormat, int nIndex, BOOL bState)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return E_INVALIDARG;
	}

	if ((nIndex<0) || (nIndex >= vecFormat.size()))
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	//判断Filter Graph的状态
	if (bState)
	{
		FILTER_STATE fs;
		hr = GetFilterGraphState(1000, fs);
		if (FAILED(hr) || (fs != State_Stopped))
		{
			return E_ACCESSDENIED;
		}
	}

	IBaseFilter *pSourceFilter = NULL; 
	IBaseFilter *pSinkFilter = NULL;

	m_pGB->FindFilterByName(FILTER_NAME_VCS, &pSourceFilter);
	m_pGB->FindFilterByName(FILTER_NAME_VMR9, &pSinkFilter);

	if ((pSourceFilter != NULL) && (pSinkFilter != NULL))
	{
		hr = DisconnectFilter(pSourceFilter, pSinkFilter);
		if (SUCCEEDED(hr))
		{
			hr = SetFormat(vecFormat, nIndex);
			if (SUCCEEDED(hr))
			{
				hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSourceFilter, NULL, pSinkFilter);
			}
		}
	}

	if (pSourceFilter != NULL)
	{
		pSourceFilter->Release();
	}

	if (pSinkFilter != NULL)
	{
		pSinkFilter->Release();
	}

	return hr;
}

HRESULT CCaptureVMR9Video::GetVideoFormat(std::vector<VideoFormat> &vecFormat)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return E_INVALIDARG;
	}
	
	HRESULT hr;
	
	int nIndex = 0;
	int nCount = 0;
	int nSize  = 0;
	
	vecFormat.clear();
	
	IBaseFilter *pSourceFilter = NULL;
	hr = m_pGB->FindFilterByName(FILTER_NAME_VCS, &pSourceFilter);
	if(FAILED(hr))
	{
		return hr;
	}
	
	IAMStreamConfig *pIAMS = NULL;
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSourceFilter, IID_IAMStreamConfig, (LPVOID*)&pIAMS);	
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = pIAMS->GetNumberOfCapabilities(&nCount, &nSize);
	if(FAILED(hr))
	{
		return hr;
	}
	else
	{
		if(nSize != sizeof(VIDEO_STREAM_CONFIG_CAPS))
		{
			return E_FAIL;
		}
		
		VIDEO_STREAM_CONFIG_CAPS scc;
		VideoFormat videoFormat;
		
		for(nIndex=0; nIndex<nCount; nIndex++)
		{
			AM_MEDIA_TYPE *pmt = NULL; 
			hr = pIAMS->GetStreamCaps(nIndex, &pmt, reinterpret_cast<LPBYTE>(&scc) ); 
			
			if(SUCCEEDED(hr))
			{
				if (pmt->formattype == FORMAT_VideoInfo)
				{
					VIDEOINFOHEADER *pVIH;
					
					pVIH = reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);
					videoFormat.biWidth = pVIH->bmiHeader.biWidth;  
                    videoFormat.biHeight = pVIH->bmiHeader.biHeight;  
                    videoFormat.biBitCount = pVIH->bmiHeader.biBitCount; 
					
                    vecFormat.push_back(videoFormat);
				}
				
				DeleteMediaType(pmt);
			}
		}
	}
	pIAMS->Release();
	
	return hr;
}


int CCaptureVMR9Video::GetCurFormatIndex(std::vector<VideoFormat> &vecFormat)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return -1;
	}
	
	HRESULT hr;
	int  nIndex  = -1;
	long nWidth  = 0;
	long nHeight = 0;
	WORD wBitCount = 0;
	
	IBaseFilter *pSourceFilter = NULL;
	hr = m_pGB->FindFilterByName(FILTER_NAME_VCS, &pSourceFilter);
	if(FAILED(hr))
	{
		return -1;
	}
	
	IAMStreamConfig *pIAMS = NULL;
	hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSourceFilter, IID_IAMStreamConfig, (LPVOID*)&pIAMS);
	if(FAILED(hr))
	{
		return -1;
	}
	
	AM_MEDIA_TYPE *pmt = NULL; 
	hr = pIAMS->GetFormat(&pmt); 
	if(FAILED(hr))
	{
		return -1;
	}	
	
	if(pmt->formattype == FORMAT_VideoInfo)
	{
		VIDEOINFOHEADER *pVIH = reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);
		nWidth  = pVIH->bmiHeader.biWidth;
		nHeight = pVIH->bmiHeader.biHeight;
		wBitCount = pVIH->bmiHeader.biBitCount;
		
		for(int i=0; i<vecFormat.size(); i++)
		{
			if((vecFormat[i].biWidth  == nWidth)   &&
			   (vecFormat[i].biHeight == nHeight)  &&
			   (vecFormat[i].biBitCount == wBitCount))
			{
				nIndex = i;
				break;
		    }
		}
	}
	
	DeleteMediaType(pmt); 
	pIAMS->Release();	
	 
	return nIndex;
}


void CCaptureVMR9Video::DeleteMediaType(AM_MEDIA_TYPE *pmt)
{
	if (pmt == NULL)
	{
		return;
	}

	if (pmt->cbFormat != 0)  
	{
		CoTaskMemFree( pmt->pbFormat );
		pmt->cbFormat = 0; 
		pmt->pbFormat = NULL;
	}

	if (pmt->pUnk != NULL)
	{
		pmt->pUnk->Release(); 
		pmt->pUnk = NULL;
	}
//	CoTaskMemFree( pmt );
}


HRESULT CCaptureVMR9Video::DisconnectFilter(IBaseFilter* pSourceFilter, IBaseFilter* pSinkFilter)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return E_INVALIDARG;
	}

	if ((pSourceFilter == NULL) && (pSinkFilter == NULL))
	{
		return E_POINTER;
	}

	HRESULT hr;

	// 获取视频输出Pin
	IPin *pPinOutput = NULL;
	hr = m_pCapture->FindPin( pSourceFilter, PINDIR_OUTPUT, &PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, FALSE, 0, &pPinOutput);
	if (FAILED(hr))
	{
		return hr;
	}

	m_pGB->Disconnect(pPinOutput);
	pPinOutput->Release();
	
	// 获取视频输入Pin
	IPin *pPinInput = NULL;
	hr = m_pCapture->FindPin(pSinkFilter, PINDIR_INPUT, NULL, NULL, FALSE, 0, &pPinInput);
	if (FAILED(hr))
	{
		return hr;
	}

	m_pGB->Disconnect(pPinInput);
	pPinInput->Release();


	return hr;
}

LRESULT CCaptureVMR9Video::GetCurrentDIBImage(LPBYTE &pDIB)
{
	if((m_pGB == NULL) || (m_pCapture == NULL))
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	IBaseFilter *pVMR9 = NULL;
	hr = m_pGB->FindFilterByName(FILTER_NAME_VMR9, &pVMR9);
	if (FAILED(hr))
	{
		return hr;
	}

	IVMRWindowlessControl9 *pWindowlessControl = NULL; 
	hr = pVMR9->QueryInterface(IID_IVMRWindowlessControl9, (LPVOID*)&pWindowlessControl);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = pWindowlessControl->GetCurrentImage(&pDIB);
	pWindowlessControl->Release();

	pVMR9->Release();

	return hr;
}

HBITMAP CCaptureVMR9Video::DIB2DDB(LPBYTE pDIB, HDC hDC)
{
	return CreateDIBitmap(hDC, (BITMAPINFOHEADER*)pDIB, CBM_INIT, pDIB + sizeof(BITMAPINFOHEADER), (BITMAPINFO*)pDIB, DIB_RGB_COLORS);
}

void CCaptureVMR9Video::FreeDIB(LPBYTE &pDIB)
{
	CoTaskMemFree(pDIB); 
	pDIB = NULL;
}

HBITMAP CCaptureVMR9Video::GetCurrentDDBImage(HDC hDC)
{
	HRESULT  hr;

	LPBYTE pDIB = NULL;
	hr = GetCurrentDIBImage(pDIB);
	if (FAILED(hr))
	{
		return NULL;
	}

	// 转换DIB图像到DDB图像
	HBITMAP hBitmap = DIB2DDB(pDIB, hDC);
	CoTaskMemFree( pDIB );

	return hBitmap;
}


HRESULT CCaptureVMR9Video::DDB2PNG(LPCTSTR pszFileName, HBITMAP hBitmap)
{
	if ((pszFileName == NULL) || (hBitmap == NULL))
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	CImage image;
	image.Attach( hBitmap );

	hr = image.Save(pszFileName, Gdiplus::ImageFormatPNG);
	image.Detach();

	return hr;
}


HRESULT CCaptureVMR9Video::DDB2JPEG(LPCTSTR pszFileName, HBITMAP hBitmap)
{
	if ((pszFileName == NULL) || (hBitmap == NULL))
	{
		return E_INVALIDARG;
	}

	HRESULT hr;

	CImage image;
	image.Attach( hBitmap );

	hr = image.Save( pszFileName, Gdiplus::ImageFormatJPEG );
	image.Detach();

	return hr;
}

void CCaptureVMR9Video::DisplayImage(HBITMAP hBitmap, HWND hWnd)
{
	if ((hBitmap == NULL) || (hWnd == NULL)) return;

	CImage image;
	RECT rc;

	image.Attach( hBitmap );
	HDC dc = ::GetDC( hWnd );

	::GetClientRect( hWnd, &rc );
	::SetStretchBltMode( dc, COLORONCOLOR );

	image.Draw( dc, rc );
	image.Detach();
}