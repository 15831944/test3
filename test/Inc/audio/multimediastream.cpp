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
		}
	}
}

HRESULT MAudioStream::ReleaseMediaStream()
{
}

HRESULT MAudioStream::GetSampleData(LPBYTE pbData, DWORD& dwSize, DWORD dwFlags, DWORD dwTimeout)
{
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