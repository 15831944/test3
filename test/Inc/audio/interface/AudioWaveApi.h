#ifndef __AUDIO_WAVEAPI_H__
#define __AUDIO_WAVEAPI_H__

#include "AudioInterface.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class CAudioWaveAPi : public CAudioInterface
{
public:
	CAudioWaveAPi();
	~CAudioWaveAPi();
	
public:
	virtual	int		audio_geterror();
	
	virtual bool	audio_init();
	virtual void	audio_uninit();

	virtual bool	audio_enumDevice(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo);
	virtual bool	audio_openDevice(CDeviceInfo &devInfo, CDeviceHandle &devHandle);
	virtual void	audio_closeDevice(CDeviceHandle &devHandle);

	virtual void	audio_startStream(CDeviceHandle &devHandle);
	virtual void	audio_closeStream(CDeviceHandle &devHandle);
	virtual void	audio_stopStream(CDeviceHandle &devHandle);
	virtual void	audio_abortStream(CDeviceHandle &devHandle);

	virtual void	audio_waveProcEvent(UINT uiMsg, DWORD dwParam1, DWORD dwParam2);

protected:
	static void CALLBACK audio_waveInProc(HWAVEIN hWaveIn, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	static void CALLBACK audio_waveOutProc(HWAVEOUT hWaveOut, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
protected:
	bool			audio_getDevInfo(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo);
	void			audio_waveApiProc();
};

#endif