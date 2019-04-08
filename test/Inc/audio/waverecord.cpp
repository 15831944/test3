#include "stdafx.h"
#include "waverecord.h"

WaveRecord::WaveRecord()
{
	m_bExit = FALSE;
	
	m_dwThreadID = 0;
	m_dwWaitTime = 0;
	
	m_dwDataSize = 0;
	m_pWavData = NULL;
	
	m_hThread = NULL;
	m_hWaveIn = NULL;
	
	m_stcWaveformat.wFormatTag = WAVE_FORMAT_PCM;
	m_stcWaveformat.nChannels = 1;			//����������, 2����
	m_stcWaveformat.cbSize = 0;
	m_stcWaveformat.wBitsPerSample = 16;	//��������, 16bits/��	
	m_stcWaveformat.nSamplesPerSec = 8000;	//������, 16000��/��
	m_stcWaveformat.nBlockAlign = 2;		//һ����Ĵ�С������bit���ֽ�������������
	m_stcWaveformat.nAvgBytesPerSec = 16000;//ÿ��������ʣ�����ÿ���ܲɼ������ֽڵ�����
	
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEndEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
}

WaveRecord::~WaveRecord()
{
	if (m_hStartEvent != NULL)
	{
		CloseHandle(m_hStartEvent);
		m_hStartEvent = NULL;
	}

	if (m_hEndEvent != NULL)
	{
		CloseHandle(m_hEndEvent);
		m_hEndEvent = NULL;
	}
}

WaveRecord& WaveRecord::Instance()
{
	static WaveRecord inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
DWORD WaveRecord::WaveRecordThreadProc(LPVOID lpParam)
{
	WaveRecord* pWaveRecordProc = (WaveRecord*)lpParam;
	if (pWaveRecordProc != NULL)
	{
		pWaveRecordProc->RecordWavInfo();
	}
	
	return 0;
}

void WaveRecord::RecordWavInfo()
{
	BOOL bRet = FALSE;
	
	do
	{
		while(WaitForSingleObject(m_hEndEvent, m_dwWaitTime) != WAIT_OBJECT_0)
		{
			if (!m_bExit)
			{
			}
			else
			{
			}
		}
		
		bRet = TRUE;
	} while (FALSE);
}

void WaveRecord::WaveInCallBackProc(HWAVEIN hWavIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
}

//////////////////////////////////////////////////////////////////////////
//
BOOL WaveRecord::OpenRecordDev(UINT uiDevID, WAVEFORMATEX *pWaveformat, HWAVEIN &hWaveIn)
{
	BOOL bRet = FALSE;
	
	UINT uiDevNum = 0;
	MMRESULT hResult = 0;
	
	do
	{
		uiDevNum = waveOutGetNumDevs();
		if (uiDevNum == 0)
		{
			bRet = FALSE;
			break
		}
		
		if (uiDevID > (uiDevNum-1))
		{
			bRet = FALSE;
			break;
		}
		
		hResult = waveInOpen(&hWaveIn, uiDevID, pWaveformat, (DWORD_PTR)WaveInCallBackProc, (DWORD)this, CALLBACK_FUNCTION);
		
		bRet = TRUE;
	} while (FALSE);
}

void WaveRecord::CloseRecordDev(HWAVEIN hWaveIn)
{
}
