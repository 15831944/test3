#include "stdafx.h"
#include "multimediastream.hpp"

//////////////////////////////////////////////////////////////////////////
//
MAudioStream::MAudioStream()
{
	memset(&_wfmt, 0, sizeof(_wfmt));
}

MAudioStream::~MAudioStream()
{
}

const WAVEFORMATEX& MAudioStream::GetWaveFormat() const
{
	return _wfmt;
}

HRESULT MAudioStream::SetMediaStream(IMediaStream* pMediaStream)
{
	HRESULT hr = E_HANDLE;
	if ( pMediaStream )
	{
		hr = pMediaStream->QueryInterface(IID_IAudioMediaStream, (void **)&_pAudioStream);
        if ( SUCCEEDED(hr) )
		{
			hr = _pAudioData.CoCreateInstance(CLSID_AMAudioData, NULL, CLSCTX_INPROC_SERVER);
			if ( SUCCEEDED(hr) )
			{
				hr = _pAudioStream->GetFormat(&_wfmt);
				_pAudioData->SetFormat(&_wfmt);
			}
			else
			{
				_pAudioStream = NULL;
			}
		}
	}

	return hr;
}

HRESULT MAudioStream::ReleaseMediaStream()
{
	HRESULT hr;
	hr = CompletionStatus(COMPSTAT_ABORT);
	if ( SUCCEEDED(hr) )
	{
		_pAudioSample = NULL;
		_pAudioData = NULL;
		_pAudioStream = NULL;

		memset(&_wfmt, 0, sizeof(_wfmt));
	}

	return hr;
}

HRESULT MAudioStream::GetSampleData(LPBYTE pbData, DWORD& dwSize, DWORD dwFlags, DWORD dwTimeout)
{
	HRESULT hr = E_INVALIDARG;

	if ( pbData != NULL && dwSize > 0 )
	{
		hr = E_HANDLE;
		if ( _pAudioStream != NULL && _pAudioData != NULL )
		{
			hr = S_OK;
			if ( _pAudioSample == NULL )
			{
				hr = _pAudioStream->CreateSample(_pAudioData, 0, &_pAudioSample);
			}

			if ( SUCCEEDED(hr) )
			{
				hr = _pAudioData->SetBuffer(dwSize, pbData, 0);
				hr = _pAudioSample->Update(SSUPDATE_ASYNC, 0, NULL, NULL);

				if ( hr == MS_S_PENDING )
				{
					hr = _pAudioSample->CompletionStatus(dwFlags, dwTimeout);
				}

				if ( hr == MS_S_ENDOFSTREAM )
				{
					hr = S_FALSE;
				}
				if ( SUCCEEDED(hr) )
				{
					_pAudioData->GetInfo(NULL, NULL, &dwSize);
				}
			}
		}
	}

	return hr;
}

HRESULT MAudioStream::GetSampleTimes(STREAM_TIME *pStartTime, STREAM_TIME *pEndTime, STREAM_TIME *pCurrentTime)
{
	HRESULT hr = E_HANDLE;
	if ( _pAudioSample != NULL )
	{
		hr = _pAudioSample->GetSampleTimes(pStartTime, pEndTime, pCurrentTime);
	}
	return hr;
}

HRESULT MAudioStream::CompletionStatus(DWORD dwFlags, DWORD dwTimeout)
{
	HRESULT hr = E_HANDLE;
	if ( _pAudioSample != NULL )
	{
		hr = _pAudioSample->CompletionStatus(dwFlags, dwTimeout);
		if ( hr == MS_S_ENDOFSTREAM )
		{
			hr = S_FALSE;
		}
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//
MultimediaStream::MultimediaStream()
{
}

MultimediaStream::~MultimediaStream()
{
	Close();
}

bool MultimediaStream::IsValid() const
{
	return ( _pAMMS.p != NULL );
}

HRESULT MultimediaStream::Initialize(STREAM_TYPE streamType, DWORD dwFlags, IGraphBuilder* pFilterGraph)
{
	HRESULT hr = E_ACCESSDENIED;
	if ( !IsValid() )
	{
		hr = _pAMMS.CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER);
		if ( SUCCEEDED(hr) )
		{
			hr = _pAMMS->Initialize(streamType, dwFlags, pFilterGraph);
			if ( SUCCEEDED(hr) )
			{
				hr = _pAMMS->GetFilterGraph(&_pGB);
			}
		}
	}
	return hr;
}

HRESULT MultimediaStream::Close()
{
	HRESULT hr = S_FALSE;
	if ( IsValid() )
	{
		_pAMMS = NULL;
		_pGB = NULL;
		hr = S_OK;
	}
	return hr;
}

HRESULT MultimediaStream::GetFilterGraph(IGraphBuilder** ppFilterGraph)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		if ( _pGB && ppFilterGraph )
		{
			*ppFilterGraph = _pGB.p;
			(*ppFilterGraph)->AddRef();
			hr = S_OK;
		}
		else if ( _pGB.p != NULL )
		{
			hr = S_FALSE;
		}
		else
		{
			hr = E_POINTER;
		}
	}
	return hr;
}

HRESULT MultimediaStream::GetMediaStream(REFMSPID idPurpose, IMediaStream** ppMediaStream)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		CComQIPtr<IMultiMediaStream> pMS = _pAMMS;
		hr = pMS->GetMediaStream(idPurpose, ppMediaStream);
	}
	return hr;
}

HRESULT MultimediaStream::GetDuration(STREAM_TIME* pDuration)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		CComQIPtr<IMultiMediaStream> pMS = _pAMMS;
		hr = pMS->GetDuration(pDuration);
	}
	return hr;
}

HRESULT MultimediaStream::GetTime(STREAM_TIME* pCurrentTime)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		CComQIPtr<IMultiMediaStream> pMS = _pAMMS;
		hr = pMS->GetTime(pCurrentTime);
	}
	return hr;
}

HRESULT MultimediaStream::GetState(STREAM_STATE* pCurrentState)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		CComQIPtr<IMultiMediaStream> pMS = _pAMMS;
		hr = pMS->GetState(pCurrentState);
	}
	return hr;
}

HRESULT MultimediaStream::SetState(STREAM_STATE NewState)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		CComQIPtr<IMultiMediaStream> pMS = _pAMMS;
		hr = pMS->SetState(NewState);
	}
	return hr;
}

HRESULT MultimediaStream::Seek(STREAM_TIME seekTime)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		CComQIPtr<IMultiMediaStream> pMS = _pAMMS;
		hr = pMS->Seek(seekTime);
	}
	return hr;
}

HRESULT MultimediaStream::OpenFile(LPCTSTR lpszFileName, DWORD dwFlags)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		USES_CONVERSION;
		hr = _pAMMS->OpenFile(T2CW(lpszFileName), dwFlags);
		if ( SUCCEEDED(hr) && _pGB == NULL )
		{
			// check if we can get graph now
			hr = _pAMMS->GetFilterGraph(&_pGB);
		}
	}
	return hr;
}

HRESULT MultimediaStream::AddMediaStream(IUnknown* pStreamObject, const MSPID* pPurposeID, DWORD dwFlags, IMediaStream** ppNewStream)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		hr = _pAMMS->AddMediaStream(pStreamObject, pPurposeID, dwFlags, ppNewStream);
	}
	return hr;
}

HRESULT MultimediaStream::AddSourceFilter(LPCTSTR lpszFileName, LPCTSTR lpszFilterName, IBaseFilter** ppFilter)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		if ( _pGB != NULL )
		{
			USES_CONVERSION;
			hr = _pGB->AddSourceFilter(T2CW(lpszFileName), T2CW(lpszFilterName), ppFilter);
		}
	}
	return hr;
}

HRESULT MultimediaStream::AddFilter(LPCTSTR lpszFilterName, IBaseFilter* pFilter)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		USES_CONVERSION;
		CComQIPtr<IFilterGraph> pFG = _pGB;
		hr = pFG->AddFilter( pFilter, T2CW(lpszFilterName) );
	}
	return hr;
}

HRESULT MultimediaStream::FindFilterByName(LPCTSTR lpszFilterName, IBaseFilter** ppFilter)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		USES_CONVERSION;
		CComQIPtr<IFilterGraph> pFG = _pGB;
		hr = pFG->FindFilterByName( T2CW(lpszFilterName), ppFilter );
	}
	return hr;
}

HRESULT MultimediaStream::Render(DWORD dwFlags)
{
	HRESULT hr = E_HANDLE;
	if ( IsValid() )
	{
		hr = _pAMMS->Render(dwFlags);
	}
	return hr;
}