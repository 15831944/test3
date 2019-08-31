#ifndef __AUDIO_INTERFACE_H__
#define __AUDIO_INTERFACE_H__

class CDevData
{
public:
	CDevData();	
	~CDevData();

	CDevData(const CDevData& audioDev);
	CDevData& operator=(const CDevData& audioDev);

public:
	void			SetDevType(UINT uiDevType);
	UINT			GetDevType();

	void			SetDevId(LPCTSTR lpszDevId);
	std::string		GetDevId();

	void			SetDevFriendName(LPCTSTR lpszDevName);
	std::string		GetDevFriendName();

	void			SetDevState(UINT uiDevState);
	UINT			GetDevState();

	void			SetDevFormat(WAVEFORMATEX *pWavFormat);
	WAVEFORMATEX*	GetDevFormat();

protected:
	typedef struct{
		UINT	uiDevType;
		TCHAR	szDeviceId[MAX_PATH];
		TCHAR	szFriendlyName[MAX_PATH];
		UINT	uiDevState;
		WAVEFORMATEX stDevFormat;
	}DevHandle;

private:
	DevHandle	m_stAudioDev;
};

class CAudioInterface
{
public:
	virtual	int		audio_geterror() = 0;

	virtual bool	audio_init() = 0;
	virtual void	audio_uninit() = 0;

	virtual bool	audio_enumDevice(std::vector<CDevData> &vecAudioDev) = 0;
	virtual bool	audio_openDevice()  = 0;
	virtual void	audio_closeDevice() = 0;

	virtual void	audio_startStream() = 0;
	virtual void	audio_closeStream() = 0;
	virtual void	audio_stopStream()  = 0;
	virtual void	audio_abortStream() = 0;
	
protected:
	int	m_iError;
};

#endif