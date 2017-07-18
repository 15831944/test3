#include "stdafx.h"
#include "waveoutdevice.hpp"

WaveoutDevice::WaveoutDevice()
{
	_hWaveOut = NULL;
	_pWaveNotify = NULL;
	_lDeviceState = waveClosed;
}

WaveoutDevice::~WaveoutDevice()
{
	Close();
}

bool WaveoutDevice::IsOpen()
{
	return ( _hWaveOut != NULL );
}

DWORD_PTR WaveoutDevice::GetId()
{
	DWORD_PTR uID = static_cast<DWORD_PTR>(-1L);
	if (IsOpen())
    {
        uID = reinterpret_cast<DWORD_PTR>(_hWaveOut);
    }
    return uID;
}

WaveStatus WaveoutDevice::GetDeviceStatus()
{
	WaveStatus wStatus;
	wStatus = static_cast<WaveStatus>( 
	);
}

HRESULT WaveoutDevice::GetPosition(WaveTime& wti)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
	if (IsOpen())
	{
		MMRESULT mmr = waveOutGetPosition(_hWaveOut, wti, wti.Size());
		hr = HRESULT_FROM_WIN32(mmr);
	}
	return hr;
}

HRESULT WaveoutDevice::Open(UINT uDeviceID, const WaveFormat& wfmt)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	if (!IsOpen())
	{
		MMRESULT mmr = waveOutOpen(&_hWaveOut, uDeviceID, const_cast<WaveFormat&>(wfmt),
								  reinterpret_cast<DWORD_PTR>(WaveOutDevice::waveOutProc),
                                  reinterpret_cast<DWORD_PTR>(this),
                                  CALLBACK_FUNCTION);
		if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveStopped);
        }
		hr = HRESULT_FROM_WIN32(mmr);	
	}
	return hr;
}

HRESULT WaveoutDevice::Close()
{
	HRESULT hr = S_FALSE;
    if (IsOpen())
    {
        hr = Stop();
        MMRESULT mmr = waveOutClose(_hWaveOut);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveClosed);
            _hWaveOut = NULL;
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::Start()
{
}

HRESULT WaveoutDevice::Stop()
{
}

HRESULT WaveoutDevice::Reset()
{
}

HRESULT WaveoutDevice::Pause()
{
}

HRESULT WaveoutDevice::AddBuffer( WaveBuffer& wbuf )
{
}

HRESULT WaveoutDevice::PrepareBuffer( WaveBuffer& wbuf )
{
}

HRESULT WaveoutDevice::UnprepareBuffer( WaveBuffer& wbuf )
{
}

HRESULT WaveoutDevice::SetNotifyHandler(IWaveNotifyHandler* pNotify)
{
}

HRESULT WaveoutDevice::BreakLoop()
{
}

HRESULT WaveoutDevice::SetVolume(DWORD dwVolume)
{
}

HRESULT WaveoutDevice::SetPlaybackRate(DWORD dwRate)
{
}

void WaveoutDevice::ProcessEvent(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
}

void WaveoutDevice::waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
}