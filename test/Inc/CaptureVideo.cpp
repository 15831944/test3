#include "stdafx.h"
#include "CaptureVideo.h"

CCaptureVideo::CCaptureVideo()
{
	
}

CCaptureVideo::~CCaptureVideo()
{
	
}

HRESULT CCaptureVideo::EnumDevices(std::vector<DeviceName> &vecDevice)
{
	HRESULT hr;
	
	WCHAR	pFriendlyName[MAX_FRIENDLY_NAME_LEN] = {0};
	WCHAR   pMonikerName[MAX_MONIKER_NAME_LEN]   = {0};
	
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
		memset(pFriendlyName, 0x0, MAX_PATH);
		memset(pMonikerName,  0x0, MAX_PATH);
		
		IPropertyBag* pBag = NULL;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pBag);
		if(FAILED(hr))
		{
			continue;
		}
		
		VARIANT var;
		VariantInit(&var);
		
		hr = pBag->Read(L"FriendlyName", &var, NULL);
		if(FAILED(hr))
		{
			continue;
		}
		
		LPOLESTR pOleDisplayName = reinterpret_cast<LPOLESTR>(CoTaskMemAlloc(MAX_MONIKER_NAME_LEN * 2));
		if(pOleDisplayName == NULL)
		{
			continue;
		}	
		
		hr = pM->GetDisplayName(NULL, NULL, &pOleDisplayName);
		if(FAILED(hr))
		{
			continue;
		}
		
		StringCchCopyW(pFriendlyName, MAX_FRIENDLY_NAME_LEN, var.bstrVal);		//wcscpy
		StringCchCopyW(pMonikerName,  MAX_MONIKER_NAME_LEN, pOleDisplayName);	//wcscpy
		
		std::vector<DeviceName>::iterator iter = find_if(vecDevice.begin(), vecDevice.end(), DeviceFinder(pFriendlyName, pMonikerName));
		if (iter != vecDevice.end())
		{
			wsprintfW(pFriendlyName, L"%s - #%d", pFriendlyName, ++nIndex);
		}
		
		DeviceName deviceName = {0};
		
		wcscpy(deviceName.FriendlyName, pFriendlyName);
		wcscpy(deviceName.MonikerName, pMonikerName);
		deviceName.nDeviceID = nID;
		
		vecDevice.push_back(deviceName);
		
		nID++;
		CoTaskMemFree(pOleDisplayName);
		
		VariantClear(&var);
		pBag->Release();
		pM->Release();
	}
	
	pCreateDevEnum->Release();
	
	return S_OK;
}

HRESULT CCaptureVideo::InitializeGraph()
{
	HRESULT hr;
	
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGB);
	if(FAILED(hr))
	{
		return hr;
	}	
	
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&m_pCapture);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = m_pCapture->SetFiltergraph(m_pGB);
	if(FAILED(hr))
	{
		return hr;
	}
	
	return S_OK;
}