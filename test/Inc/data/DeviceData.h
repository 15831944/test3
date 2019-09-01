#ifndef __DEVICE_DATA_H__
#define __DEVICE_DATA_H__

class CDeviceData
{
public:
	CDeviceData();	
	~CDeviceData();

	CDeviceData(const CDeviceData& audioDev);
	CDeviceData& operator=(const CDeviceData& audioDev);

public:
	void			SetDevType(UINT uiDevType);
	UINT			GetDevType();

	void			SetDevId(LPCTSTR lpszDevId);
	std::string		GetDevId();

	void			SetDevFriendName(LPCTSTR lpszDevName);
	std::string		GetDevFriendName();

	void			SetDevState(UINT uiDevState);
	UINT			GetDevState();

	//void			SetDevFormat(WAVEFORMATEX *pWavFormat);
	//WAVEFORMATEX*	GetDevFormat();

protected:
	typedef struct{
		UINT	uiDevType;
		TCHAR	szDeviceId[MAX_PATH];
		TCHAR	szFriendlyName[MAX_PATH];
		UINT	uiDevState;
		//WAVEFORMATEX stDevFormat;
	}DevHandle;

private:
	DevHandle	m_stAudioDev;
};


#endif