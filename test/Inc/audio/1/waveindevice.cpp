#include "stdafx.h"
#include "waveindevice.hpp"

WaveInDevice::WaveInDevice()
{
	_hWaveIn = NULL;
	_pWaveNotify = NULL;
	_lDeviceState = waveClosed;
}

WaveInDevice::~WaveInDevice()
{
	Close();
}

UINT WaveInDevice::GetDevicesCount()
{
	return waveInGetNumDevs();
}

HRESULT WaveInDevice::GetDeviceCaps(UINT uDeviceID, WaveInCaps& wic)
{
	HRESULT hr;
    MMRESULT mmr = waveInGetDevCaps(uDeviceID, wic, wic.Size());
    hr = HRESULT_FROM_WIN32(mmr);
    return hr;
}

bool WaveInDevice::IsOpen() const
{
	return ( _hWaveIn != NULL );
}

DWORD_PTR WaveInDevice::GetId() const
{
	DWORD_PTR uID = static_cast<DWORD_PTR>(-1L);
    if ( IsOpen() )
    {
        uID = reinterpret_cast<DWORD_PTR>(_hWaveIn);
    }
    return uID;
}

WaveStatus WaveInDevice::GetDeviceStatus() const
{
	WaveStatus wStatus;
    wStatus = static_cast<WaveStatus>( 
        ::InterlockedCompareExchange(const_cast<LONG*>(&_lDeviceState), waveClosed, waveClosed)
        );
    return wStatus;
}

HRESULT WaveInDevice::GetPosition(WaveTime& wti) const
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInGetPosition(_hWaveIn, wti, wti.Size());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::Open(UINT uDeviceID, const WaveFormat& wfmt)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
    if ( !IsOpen() )
    {
        MMRESULT mmr = waveInOpen(&_hWaveIn, uDeviceID, const_cast<WaveFormat&>(wfmt),
                                  reinterpret_cast<DWORD_PTR>(WaveInDevice::waveInProc),
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

HRESULT WaveInDevice::Close()
{
	HRESULT hr = S_FALSE;
    if ( IsOpen() )
    {
        hr = Stop();
        MMRESULT mmr = waveInClose(_hWaveIn);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveClosed);
            _hWaveIn = NULL;
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::Start()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInStart(_hWaveIn);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveStarted);
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::Stop()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInReset(_hWaveIn);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, waveStopped);
            mmr = waveInStop(_hWaveIn);
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::Reset()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInReset(_hWaveIn);
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::Pause()
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        // can't pause input waveform
        MMRESULT mmr = waveInReset(_hWaveIn);
        if ( mmr == MMSYSERR_NOERROR )
        {
            ::InterlockedExchange(&_lDeviceState, wavePaused);
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::AddBuffer( WaveBuffer& wbuf )
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInAddBuffer(_hWaveIn, wbuf, wbuf.Size());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::PrepareBuffer( WaveBuffer& wbuf )
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInPrepareHeader(_hWaveIn, wbuf, wbuf.Size());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::UnprepareBuffer( WaveBuffer& wbuf )
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE);
    if ( IsOpen() )
    {
        MMRESULT mmr = waveInPrepareHeader(_hWaveIn, wbuf, wbuf.Size());
        hr = HRESULT_FROM_WIN32(mmr);
    }
    return hr;
}

HRESULT WaveInDevice::SetNotifyHandler(IWaveNotifyHandler* pNotify)
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
    }
    return hr;
}

void WaveInDevice::ProcessEvent(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if ( _pWaveNotify != NULL )
    {
        _pWaveNotify->ProcessEvent(this, uMsg, reinterpret_cast<WaveBuffer*>(dwParam1));
    }
}

void WaveInDevice::waveInProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance,
								DWORD dwParam1, DWORD dwParam2)
{
	WaveInDevice* _this = reinterpret_cast<WaveInDevice*>( dwInstance );
    if ( _this )
        _this->ProcessEvent(uMsg, dwParam1, dwParam2);
}