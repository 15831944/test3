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

}

UINT MixerDevice::GetMixerDeviceID(HMIXEROBJ hmxobj, DWORD flags)
{

}

bool MixerDevice::IsOpen() const
{
	return (_hMixer != NULL);
}

UINT MixerDevice::GetId() const
{

}

HRESULT MixerDevice::Open(UINT uMixerId, DWORD fdwOpen, HWND hWndCallback)
{

}

HRESULT MixerDevice::Close()
{

}

HRESULT MixerDevice::GetControlDetails(MixerControlDetails& mxcd, DWORD fdwDetails) const
{
}

HRESULT MixerDevice::SetControlDetails(const MixerControlDetails& mxcd, DWORD fdwDetails)
{

}

HRESULT MixerDevice::GetLineControls(MixerLineControls& mxlcs, DWORD fdwControls) const
{

}

HRESULT MixerDevice:::GetLineInfo(MixerLineInfo& mxli, DWORD fdwInfo) const
{

}

HRESULT MixerDevice::GetDeviceCaps(MixerCaps& mxcaps) const
{

}

HRESULT MixerDevice::MixerMessage(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{

}