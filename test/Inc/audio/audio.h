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

struct AudioDev {
	WavFormat  stWavFormat;
	
	struct AudioDev() {
		memset(&stWavFormat, 0x0, sizeof(WavFormat));
	};
};

#endif