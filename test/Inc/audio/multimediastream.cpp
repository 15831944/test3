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
}

HRESULT MAudioStream::CompletionStatus(DWORD dwFlags, DWORD dwTimeout)
{
}

//////////////////////////////////////////////////////////////////////////
//
MultimediaStream::MultimediaStream()
{
}

MultimediaStream::~MultimediaStream()
{
}

bool MultimediaStream::IsValid() const
{
}

HRESULT MultimediaStream::Initialize(STREAM_TYPE streamType, DWORD dwFlags, IGraphBuilder* pFilterGraph)
{
}

HRESULT MultimediaStream::Close()
{
}

HRESULT MultimediaStream::GetFilterGraph(IGraphBuilder** ppFilterGraph)
{
}

HRESULT MultimediaStream::GetMediaStream(REFMSPID idPurpose, IMediaStream** ppMediaStream)
{
}

HRESULT MultimediaStream::GetDuration(STREAM_TIME* pDuration)
{
}

HRESULT MultimediaStream::GetTime(STREAM_TIME* pCurrentTime)
{
}

HRESULT MultimediaStream::GetState(STREAM_STATE* pCurrentState)
{
}

HRESULT MultimediaStream::SetState(STREAM_STATE NewState)
{
}

HRESULT MultimediaStream::Seek(STREAM_TIME seekTime)
{
}

HRESULT MultimediaStream::OpenFile(LPCTSTR lpszFileName, DWORD dwFlags)
{
}

HRESULT MultimediaStream::AddMediaStream(IUnknown* pStreamObject, const MSPID* pPurposeID, DWORD dwFlags, IMediaStream** ppNewStream)
{
}

HRESULT MultimediaStream::AddSourceFilter(LPCTSTR lpszFileName, LPCTSTR lpszName, IBaseFilter** ppFilter)
{
}

HRESULT MultimediaStream::AddFilter(LPCTSTR lpszFilterName, IBaseFilter* pFilter)
{
}

HRESULT MultimediaStream::FindFilterByName(LPCTSTR lpszFilterName, IBaseFilter** ppFilter)
{
}

HRESULT MultimediaStream::Render(DWORD dwFlags)
{
}