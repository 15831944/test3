#ifndef  _AUDIO_CAPTURE_H_
#define _AUDIO_CAPTURE_H_

#define AUDIO_HDRCOUNT	  10
class CWaveAudio
{
public:
	CWaveAudio();
	~CWaveAudio();

public:
	BOOL					CreateAudioProc(UINT uiDevID, UINT nSpanTime=10);
	void					CloseAudioProc();

	void					SetParentProc(void *pSpeechServer);

protected:
	static DWORD WINAPI		WaveAudioThreadProc(LPVOID lpParam);

protected:
	void					AudioWavInfo();

	BOOL					OpenAudioData(UINT uiDevID);
	BOOL					SetAudioEvent(BOOL bFlag, HWAVEIN hWaveIn);

	void					ClearAudioData(HWAVEIN hWaveIn);	//WAVEHDR *pWaveInHdr
	BOOL					SetAudioData(HWAVEIN hWaveIn, WAVEFORMATEX *pWaveformat);	//WAVEHDR *pWaveInHdr
				
protected:
	BOOL					OpenAudioDev(UINT uiDevID, UINT uiThreadId, WAVEFORMATEX *pWaveformat, HWAVEIN &hWaveIn);
	void					CloseAudioDev(HWAVEIN hWaveIn);
	
	BOOL					SetAudioDev(BOOL bFlag, HWAVEIN hWaveIn);

protected:
	HANDLE					m_hThread;
	HWAVEIN					m_hWaveIn;

	HANDLE					m_hStartEvent;
	HANDLE					m_hEndEvent;

	WAVEFORMATEX			m_stWaveFormat;
	WAVEHDR					m_stWaveHdr[AUDIO_HDRCOUNT];

private:
	mutable volatile BOOL	m_bExit;

	DWORD					m_dwThreadID;
	DWORD					m_dwWaitTime;

	void					*m_pSpeechServer;
};

#endif // !_AUDIO_CAPTURE_H_