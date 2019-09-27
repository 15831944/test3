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
	virtual bool	audio_openDevice(CDeviceInfo *pDevInfo);
	virtual void	audio_closeDevice();

	virtual void	audio_startStream();
	virtual void	audio_closeStream();
	virtual void	audio_stopStream();
	virtual void	audio_abortStream();
	
	virtual	void	audio_addBuffer(const char *pszDataBuff, int nBuffSize);
	virtual	void	audio_openBuffer();
	virtual void	audio_closeBuffer();

	virtual void	audio_setNotifyHandler(IAudioNotifyHandler *pNotify);

protected:
	static void CALLBACK audio_waveInProc(HWAVEIN hWaveIn, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	static void CALLBACK audio_waveOutProc(HWAVEOUT hWaveOut, UINT uiMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
	virtual void	audio_waveProcEvent(UINT uiDevMode, UINT uiMsg, DWORD dwParam1, DWORD dwParam2);

	void			audio_waveProcData(HWAVEIN hWaveIn, WAVEHDR *pWaveHdr);
	bool			audio_getDevInfo(DeviceMode emDevMode, std::vector<CDeviceInfo> &vecDevInfo);

private:
	IAudioNotifyHandler *m_pAudioNotifyHandler;
};

#endif