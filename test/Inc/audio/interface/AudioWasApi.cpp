#include "stdafx.h"
#include "AudioWasApi.h"

#define SAFE_RELEASE(objectPtr) \
		if (objectPtr) { \
			objectPtr->Release(); \
			objectPtr = NULL;}

CAudioWasApi::CAudioWasApi()
	: m_bInitialized(false),
	m_pDevEnumerator(NULL)
{
	HRESULT hr = CoInitialize( NULL );
	if (!FAILED(hr))
		m_bInitialized = true;
	
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
						  CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
						  (void**)&m_pDevEnumerator);
	if (FAILED(hr))
		m_pDevEnumerator = NULL;	
}

CAudioWasApi::~CAudioWasApi()
{
	SAFE_RELEASE(m_pDevEnumerator);
	if (m_bInitialized)
		CoUninitialize();
}

int CAudioWasApi::audio_geterror()
{
	return m_iError;
}

bool CAudioWasApi::audio_enumDevice()
{
	bool bRet = false;
	
	UINT uiCaptureDevCount = 0;
	UINT uiRenderDevCount = 0;
	
	HRESULT hr;
	IMMDeviceCollection* pCaptureDev = NULL;
	IMMDeviceCollection* pRenderDev = NULL;
	
	do
	{
		if (m_pDevEnumerator == NULL)
		{
			return false;
		}
		
		//eCapture:ÒôÆµ²¶×½Á÷,Â¼ÒôÉè±¸;
		hr = m_pDevEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &pCaptureDev);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}
		
		hr = pCaptureDev->GetCount(&uiCaptureDevCount);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}
		
		//eRender:ÒôÆµäÖÈ¾Á÷,²¥·ÅÉè±¸;
		hr = m_pDevEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pRenderDev);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}
		
		hr = pRenderDev->GetCount(&uiRenderDevCount);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}
		
		bRet = true;
	} while (false);
	
	if (!bRet)
	{
		SAFE_RELEASE(pCaptureDev);
		SAFE_RELEASE(pRenderDev);
	}

	return bRet;
}

bool CAudioWasApi::audio_getDeviceInfo()
{
	bool bRet = false;
	
	do
	{
		bRet = true;
	} while (false);
	
	if (!bRet)
	{
	}

	return bRet;
}

bool CAudioWasApi::audio_openDevice()
{
	bool bRet = false;

	do
	{
		bRet = true;
	} while (false);

	if (!bRet)
	{
	}

	return bRet;
}