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

UINT WaveOutDevice::GetDevicesCount()
{
    return waveOutGetNumDevs();
}

HRESULT WaveOutDevice::GetDeviceCaps(UINT uDeviceID, WaveOutCaps& woc)
{
    HRESULT hr;
    MMRESULT mmr = waveOutGetDevCaps(uDeviceID, woc, woc.Size());
    hr = HRESULT_FROM_WIN32(mmr);
    return hr;
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
        ::InterlockedCompareExchange(const_cast<LONG*>(&_lDeviceState), waveClosed, waveClosed)
        );
    return wStatus;
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
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutRestart(_hWaveOut);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveStarted);
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::Stop()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutReset(_hWaveOut);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveStopped);
            mmr = waveOutPause(_hWaveOut);
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::Reset()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutReset(_hWaveOut);
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::Pause()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutPause(_hWaveOut);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, wavePaused);
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::AddBuffer( WaveBuffer& wbuf )
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutWrite(_hWaveOut, wbuf, wbuf.GetBufferLength());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::PrepareBuffer( WaveBuffer& wbuf )
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutPrepareHeader(_hWaveOut, wbuf, wbuf.Size());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::UnprepareBuffer( WaveBuffer& wbuf )
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutUnprepareHeader(_hWaveOut, wbuf, wbuf.Size());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::SetNotifyHandler(IWaveNotifyHandler* pNotify)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
    if ( pNotify != NULL )
    {
        if ( _pWaveNotify == NULL ) {
            ::InterlockedExchangePointer(reinterpret_cast<void**>(&_pWaveNotify), pNotify);
            hr = S_OK;
        }
    }
    else
    {
        ::InterlockedExchangePointer(reinterpret_cast<void**>(&_pWaveNotify), NULL);
        hr = S_OK;
    }
    return hr;
}

HRESULT WaveoutDevice::BreakLoop()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutBreakLoop(_hWaveOut);
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::SetVolume(DWORD dwVolume)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutSetVolume(_hWaveOut, dwVolume);
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveoutDevice::SetPlaybackRate(DWORD dwRate)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveOutSetPlaybackRate(_hWaveOut, dwRate);
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

void WaveoutDevice::ProcessEvent(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if ( _pWaveNotify != NULL )
    {
        _pWaveNotify->ProcessEvent(this, uMsg, reinterpret_cast<WaveBuffer*>(dwParam1));
    }
}

void WaveoutDevice::waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	WaveOutDevice* _this = reinterpret_cast<WaveOutDevice*>( dwInstance );
    if ( _this )
        _this->ProcessEvent(uMsg, dwParam1, dwParam2);
}