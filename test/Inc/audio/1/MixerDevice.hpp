#ifndef __MIXERDEVICE_HPP__
#define __MIXERDEVICE_HPP__

#include <winbase.h>
#include <mmreg.h>

#pragma warning(push)
#pragma warning(disable:4201)
#include <Mmsystem.h>
#pragma warning(pop)

static const UINT INVALID_MIXER_ID = static_cast<UINT>(-1L);

//
class MixerControlDetails : MIXERCONTROLDETAILS
{
public:
	MixerControlDetails()	{ memset(this, 0, Size()); cbStruct = Size(); }
	~MixerControlDetails();

public:
	UINT Size() const		{ return sizeof(MIXERCONTROLDETAILS); }
	operator LPMIXERCONTROLDETAILS() { return static_cast<MIXERCONTROLDETAILS *>(this); }
};

//
class MixerControl : MIXERCONTROL
{
public:
	MixerControl()			{ memset(this, 0, Size()); cbStruct = Size(); }
	~MixerControl();

public:
	UINT Size() const		{ return sizeof(MIXERCONTROL); }
	operator LPMIXERCONTROL() { return static_cast<MIXERCONTROL *>(this); }
};

//
class MixerLineControls : MIXERLINECONTROLS
{
public:
	MixerLineControls()		{ memset(this, 0, Size()); cbStruct = Size(); }
	~MixerLineControls();

public:
	UINT Size() const		{ return sizeof(MIXERLINECONTROLS); }
	operator LPMIXERLINECONTROLS() { return static_cast<MIXERLINECONTROLS *>(this); }
};

//
class MixerLineInfo : MIXERLINE
{
public:
	MixerLineInfo()			{ memset(this, 0, Size()); cbStruct = Size(); }
	~MixerLineInfo();

public:
	UINT Size() const		{ return sizeof(MIXERLINE); }
	operator LPMIXERLINE()	{ return static_cast<MIXERLINE *>(this); }
};

//
class MixerCaps : MIXERCAPS
{
public:
	MixerCaps()				{ memset(this, 0, Size()); }
	~MixerCaps();

public:
	DWORD GetManufacturerId() const		{ return wMid; }
	DWORD GetProductId() const			{ return wPid; }
	MMVERSION GetVersion() const		{ return vDriverVersion; }
	DWORD GetSupportFlag() const		{ return fdwSupport; }
	DWORD GetDestinations() const		{ return cDestinations; }
	LPCTSTR GetProductName()			{ return szPname; }
	UINT Size() const					{ return sizeof(MIXERCAPS); }

public:
	operator LPMIXERCAPS()				{ return static_cast<MIXERCAPS *>(this); }
};

//
class MixerDevice
{
public:
	MixerDevice();
	~MixerDevice();

public:
	static UINT GetNumDevices();
	static UINT GetMixerDeviceID(HMIXEROBJ hmxobj, DWORD flags);

public:
	bool IsOpen() const;
	UINT GetId() const;
	HRESULT Open(UINT uMixerId, DWORD fdwOpen, HWND hWndCallback);
	HRESULT Close();
	HRESULT GetControlDetails(MixerControlDetails& mxcd, DWORD fdwDetails) const;
	HRESULT SetControlDetails(const MixerControlDetails& mxcd, DWORD fdwDetails);
	HRESULT GetLineControls(MixerLineControls& mxlcs, DWORD fdwControls) const;
	HRESULT GetLineInfo(MixerLineInfo& mxli, DWORD fdwInfo) const;
	HRESULT GetDeviceCaps(MixerCaps& mxcaps) const;
	HRESULT MixerMessage(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private:
	HMIXER  _hMixer;    ///< Mixer handle
};

#endif