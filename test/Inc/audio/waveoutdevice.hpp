#ifndef __WAVEOUTDEVICE_HPP__
#define __WAVEOUTDEVICE_HPP__

#include "wavebase.hpp"

class WaveoutDevice : public IWaveDevice
{
public:
	WaveoutDevice();
	virtual ~WaveoutDevice();
	
	static  UINT 		GetDevicesCount();
    static  HRESULT 	GetDeviceCaps(UINT uDeviceID, WaveOutCaps& woc);
	
public:
	virtual bool		IsOpen() const;
	
	virtual DWORD_PTR	GetId() const;
	virtual WaveStatus 	GetDeviceStatus() const;
	virtual HRESULT 	GetPosition(WaveTime& wti) const;
	
	virtual HRESULT 	Open(UINT uDeviceID, const WaveFormat& wfmt);
    virtual HRESULT 	Close();
    virtual HRESULT 	Start();
    virtual HRESULT 	Stop();
    virtual HRESULT 	Reset();
    virtual HRESULT 	Pause();
	
	virtual HRESULT 	AddBuffer( WaveBuffer& wbuf );
    virtual HRESULT 	PrepareBuffer( WaveBuffer& wbuf );
	
	virtual HRESULT 	UnprepareBuffer( WaveBuffer& wbuf );
    virtual HRESULT 	SetNotifyHandler(IWaveNotifyHandler* pNotify);
	
public:
	HRESULT 			BreakLoop();
    HRESULT 			SetVolume(DWORD dwVolume);
    HRESULT 			SetPlaybackRate(DWORD dwRate);

protected:
	virtual void 		ProcessEvent(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static  void 		CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

private:
	HWAVEOUT             _hWaveOut;     ///< Wave Audio output
    long                 _lDeviceState; ///< Wave state
    IWaveNotifyHandler*  _pWaveNotify;  ///< Audio Event handler		
};

#endif