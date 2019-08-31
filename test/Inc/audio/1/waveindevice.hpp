#ifndef __WAVEINDEVICE_HPP__
#define __WAVEINDEVICE_HPP__

#include "wavebase.hpp"

class WaveInDevice : public IWaveDevice
{
public:
	WaveInDevice();
	virtual ~WaveInDevice();
	
	static  UINT 		GetDevicesCount();
    static  HRESULT 	GetDeviceCaps(UINT uDeviceID, WaveInCaps& wic);
	
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
	
protected:
	virtual void 		ProcessEvent(UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static  void 		CALLBACK waveInProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance,
								DWORD dwParam1, DWORD dwParam2);

private:
	HWAVEIN             _hWaveIn;      ///< Wave Audio Input
    long                _lDeviceState; ///< Wave state
    IWaveNotifyHandler* _pWaveNotify;  ///< Audio Event handler							
};

#endif