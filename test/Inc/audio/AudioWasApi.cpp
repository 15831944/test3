#include "stdafx.h"
#include "AudioWasApi.h"

#include <avrt.h>
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>

#define SAFE_RELEASE(objectPtr) \
		if (objectPtr) { objectPtr->Release(); objectPtr = NULL;}

CAudioWasApi::CAudioWasApi()
	: m_bInitialized(false),
	m_pDevEnumerator(NULL)
{
	audio_init();
}

CAudioWasApi::~CAudioWasApi()
{
	audio_uninit();
}

int CAudioWasApi::audio_geterror()
{
	return m_iError;
}

bool CAudioWasApi::audio_init()
{
	bool bRet = false;

	HRESULT hr;

	do 
	{
		hr = CoInitialize(NULL);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}

		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
					CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
					(void**)&m_pDevEnumerator);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}

		m_bInitialized = true;

		bRet = true;
	} while (false);
	
	return bRet;
}

void CAudioWasApi::audio_uninit()
{
	SAFE_RELEASE(m_pDevEnumerator);
	if (m_bInitialized)
	{
		CoUninitialize();
		m_bInitialized = false;
	}
}

bool CAudioWasApi::audio_enumDevice(std::vector<CDevData> &vecAudioDev)
{
	if (m_pDevEnumerator == NULL)
	{
		return false;
	}

	audio_getDevInfo(EDataFlow::eAll, DEVICE_STATE_ACTIVE, vecAudioDev);
	return true;
}

bool CAudioWasApi::audio_openDevice()
{
	bool bRet = false;

	do
	{
		m_hHandle = CreateThread(NULL, 0, NULL, this, CREATE_SUSPENDED, NULL);
		if (m_hHandle == NULL || m_hHandle == INVALID_HANDLE_VALUE)
		{
			bRet = false;
			break;
		}


		bRet = true;
	} while (false);

	return bRet;
}

void CAudioWasApi::audio_closeDevice()
{

}

void CAudioWasApi::audio_startStream()
{

}

void CAudioWasApi::audio_closeStream()
{

}

void CAudioWasApi::audio_stopStream()
{

}

void CAudioWasApi::audio_abortStream()
{

}

//////////////////////////////////////////////////////////////////////////
//
DWORD WINAPI CAudioWasApi::audio_wasApiThread(void *pWasApiPtr)
{
	if (pWasApiPtr != NULL)
	{
		((CAudioWasApi*)pWasApiPtr)->audio_wasApiProc();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//
static UINT getDeviceState(IMMDevice* pDevPtr)
{
	DWORD dwState = 0;

	pDevPtr->GetState(&dwState);
	return dwState;
}

static std::string getDeviceId(IMMDevice* pDevPtr)
{
	std::string strDevId;
	LPWSTR lpszDevId = NULL;

	USES_CONVERSION;
	if (SUCCEEDED(pDevPtr->GetId(&lpszDevId)))
	{
		strDevId = W2A(lpszDevId);
	}

	return strDevId;
}

static std::string getPropString(IMMDevice* pDevPtr, REFPROPERTYKEY emKey)
{
	std::string strPropName;
	IPropertyStore* devPropStore = NULL;

	PROPVARIANT devNameProp;
	::PropVariantInit(&devNameProp);

	USES_CONVERSION;
	if (SUCCEEDED(pDevPtr->OpenPropertyStore(STGM_READ, &devPropStore)))
	{
		if (SUCCEEDED(devPropStore->GetValue(emKey, &devNameProp)))
		{
			strPropName = W2A(devNameProp.pwszVal);
		}
	}

	::PropVariantClear(&devNameProp);
	SAFE_RELEASE(devPropStore);

	return strPropName;
}

static WAVEFORMATEX* getDeviceFormat(IMMDevice* pDevPtr)
{
	IAudioClient* audioClient = NULL;
	WAVEFORMATEX* deviceFormat = NULL;

	if (SUCCEEDED(pDevPtr->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&audioClient)))
	{
		audioClient->GetMixFormat(&deviceFormat);
	}

	SAFE_RELEASE(audioClient);
	return deviceFormat;
}

bool CAudioWasApi::audio_getDevInfo(EDataFlow emdataFlow, UINT uiDevState, std::vector<CDevData> &vecAudioDev)
{
	bool bRet = false;

	UINT uiIndex = 0;
	UINT uiDevCount = 0;

	HRESULT hr;
	IMMDevice* pDevPtr = NULL;
	IMMDeviceCollection* pDevCollection = NULL;

	do 
	{
		//eCapture:ÒôÆµ²¶×½Á÷,Â¼ÒôÉè±¸;	//eRender:ÒôÆµäÖÈ¾Á÷,²¥·ÅÉè±¸;
		hr = m_pDevEnumerator->EnumAudioEndpoints(emdataFlow, uiDevState, &pDevCollection);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}

		hr = pDevCollection->GetCount(&uiDevCount);
		if (FAILED(hr))
		{
			bRet = false;
			break;
		}

		for (uiIndex=0; uiIndex<uiDevCount; ++uiIndex)
		{
			hr = pDevCollection->Item(uiIndex, &pDevPtr);
			if (FAILED(hr))
			{
				continue;
			}

			CDevData audioDev;
			audioDev.SetDevState(getDeviceState(pDevPtr));
			audioDev.SetDevId(getDeviceId(pDevPtr).c_str());
			
			WAVEFORMATEX* pDevFormat = NULL;
			audioDev.SetDevFormat(pDevFormat=getDeviceFormat(pDevPtr));
			audioDev.SetDevFriendName(getPropString(pDevPtr, PKEY_Device_FriendlyName).c_str());

			CoTaskMemFree(pDevFormat);
			vecAudioDev.push_back(audioDev);
		}

		bRet = true;
	} while (false);
	
	return bRet;
}

void CAudioWasApi::audio_wasApiProc()
{

}