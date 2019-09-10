#ifndef __AUDIO_H__
#define __AUDIO_H__

struct WavFormat {
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
	WORD wSize;
};

class CAudioData 
{
public:
	CAudioData()
	{
		memset(&m_stWavFormat, 0x0, sizeof(WavFormat));
	}
	
	~CAudioData() {}

private:
	WavFormat m_stWavFormat;
	
public:
	WavFormat* GetWaveFormat() { return &m_stWavFormat; }
	
	void SetWaveFormat(const WavFormat *pWaveFormat)
	{
		if (pWaveFormat == NULL)
		{
			return;
		}
		
		memcpy(&m_stWavFormat, pWaveFormat, sizeof(WavFormat));
	}
};	

#endif