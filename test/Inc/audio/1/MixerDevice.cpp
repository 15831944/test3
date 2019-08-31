#include "stdafx.h"
#include "MixerDevice.hpp"

MixerDevice::MixerDevice()
{

}

MixerDevice::~MixerDevice()
{
	Close();
}

UINT MixerDevice::GetNumDevices()
{
	return mixerGetNumDevs();
}

UINT MixerDevice::GetMixerDeviceID(HMIXEROBJ hmxobj, DWORD flags)
{
	UINT uMixerId = INVALID_MIXER_ID;
	
	MMRESULT mmr = mixerGetID(hmxobj, &uMixerId, flags);
//	if(mmr == MMSYSERR_NOERROR)
	return uMixerId;
}

bool MixerDevice::IsOpen() const
{
	return (_hMixer != NULL);
}

UINT MixerDevice::GetId() const
{
	UINT uMixerId = INVALID_MIXER_ID;
	
	if( IsOpen() )
	{
		MMRESULT mmr = mixerGetID(reinterpret_cast<HMIXEROBJ>(_hMixer), &uMixerId, MIXER_OBJECTF_HMIXER);
		
		(mmr);
	}
	
	return uMixerId;
}

HRESULT MixerDevice::Open(UINT uMixerId, DWORD fdwOpen, HWND hWndCallback)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
	if ( !IsOpen() )
    {
        MMRESULT mmr = mixerOpen(&_hMixer, uMixerId, reinterpret_cast<DWORD_PTR>(hWndCallback),
                                 NULL, fdwOpen);
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::Close()
{
	HRESULT hr = S_FALSE;
	
    if ( IsOpen() )
    {
        MMRESULT mmr = mixerClose(_hMixer);
        if ( mmr == MMSYSERR_NOERROR )
        {
            _hMixer = NULL;
        }
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::GetControlDetails(MixerControlDetails& mxcd, DWORD fdwDetails) const
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
    if ( IsOpen() )
    {
        fdwDetails |= MIXER_OBJECTF_HMIXER;
        MMRESULT mmr = mixerGetControlDetails(reinterpret_cast<HMIXEROBJ>(_hMixer),
                        mxcd, fdwDetails);
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::SetControlDetails(const MixerControlDetails& mxcd, DWORD fdwDetails)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
    if ( IsOpen() )
    {
        fdwDetails |= MIXER_OBJECTF_HMIXER;
        MMRESULT mmr = mixerSetControlDetails(reinterpret_cast<HMIXEROBJ>(_hMixer),
                        const_cast<MixerControlDetails&>(mxcd), fdwDetails);
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::GetLineControls(MixerLineControls& mxlcs, DWORD fdwControls) const
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
    if ( IsOpen() )
    {
        fdwControls |= MIXER_OBJECTF_HMIXER;
		
        MMRESULT mmr = mixerGetLineControls(reinterpret_cast<HMIXEROBJ>(_hMixer),
                        mxlcs, fdwControls);
						
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::GetLineInfo(MixerLineInfo& mxli, DWORD fdwInfo) const
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
    if ( IsOpen() )
    {
        fdwInfo |= MIXER_OBJECTF_HMIXER;
		
        MMRESULT mmr = mixerGetLineInfo(reinterpret_cast<HMIXEROBJ>(_hMixer),
                        mxli, fdwInfo);
						
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::GetDeviceCaps(MixerCaps& mxcaps) const
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
    if ( IsOpen() )
    {
        MMRESULT mmr = mixerGetDevCaps(reinterpret_cast<UINT_PTR>(_hMixer), mxcaps, mxcaps.Size());
		
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}

HRESULT MixerDevice::MixerMessage(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);
	
    if ( IsOpen() )
    {
        UINT_PTR uMixerId = INVALID_MIXER_ID;
		
        MMRESULT mmr = mixerGetID(reinterpret_cast<HMIXEROBJ>(_hMixer), &uMixerId, MIXER_OBJECTF_HMIXER);
		
        if ( mmr == MMSYSERR_NOERROR )
        {
            mmr = mixerMessage(reinterpret_cast<HMIXER>(uMixerId), uMsg, dwParam1, dwParam2);
        }
		
        hr = HRESULT_FROM_WIN32(mmr);
    }
	
    return hr;
}