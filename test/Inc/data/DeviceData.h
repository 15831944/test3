#ifndef __DEVICE_DATA_H__
#define __DEVICE_DATA_H__

enum DevType{
	DEV_EMPTYTYPE = 0,
	DEV_AUDIOTYPE,
	DEV_VIDEOTYPE,
};

enum DataType{
	DATA_EMPTYTYPE = 0,
	DATA_RENDERTYPE,
	DATA_CAPTURETYPE,
};

struct WavFormat{
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
	WORD wSize;
};

struct DevHandle{
	int	nDevType;
	int	nDevState;
	int nDataType;

	std::string	strDevId;
	std::string strDevName;
	WavFormat  stWavFormat;

	struct DevHandle(){
		nDevType  = 0;
		nDevState = 0;
		nDataType = 0;

		strDevId = "";
		strDevName = "";
		memset(&stWavFormat, 0x0, sizeof(WavFormat));
	}
};

class CDeviceData
{
public:
	CDeviceData();	
	~CDeviceData();

	CDeviceData(const CDeviceData& audioDev);
	CDeviceData& operator=(const CDeviceData& audioDev);

public:
	void			SetDevType(int nDevType);
	int				GetDevType();

	void			SetDevState(int nDevState);
	int				GetDevState();

	void			SetDataType(int nDataType);
	int				GetDataType();

	void			SetDevId(std::string strDevId);
	std::string		GetDevId();

	void			SetDevName(std::string strDevName);
	std::string		GetDevName();

	void			SetWavFormat(WavFormat *pWavFormat);
	WavFormat*		GetWavFormat();

private:
	DevHandle		m_stAudioDev;
};


#endif