#ifndef __WAVEBASE_HPP__
#define __WAVEBASE_HPP__

//WAVEINCAPS
class WaveInCaps : WAVEINCAPS
{
public:
	WaveInCaps()
	{
		memset(this, 0x0, Size());
	}
	
public:
	DWORD 			GetManufacturerId() 	const { return wMid; }	//������ID
    DWORD 			GetProductId() 			const { return wPid; }	//��ƷID
    MMVERSION 		GetVersion() 			const { return vDriverVersion; }	//�汾�ţ����ֽ������汾�ţ����ֽ��Ǵΰ汾��
    DWORD 			GetFormats()			const { return dwFormats; }			//֧�ֵĸ�ʽ
    WORD 			GetChannels() 			const { return wChannels; }			//����
    LPCTSTR 		GetProductName() 		const { return szPname; }			//��Ʒ����
    UINT 			Size() 					const { return sizeof(WAVEINCAPS); }
    operator 		LPWAVEINCAPS() 			{ return static_cast<WAVEINCAPS*>(this); }	
};

//WAVEOUTCAPS
class WaveOutCaps : WAVEOUTCAPS
{
public:
	WaveOutCaps()
	{
		memset(this, 0x0, Size());
	}
	
public:
	DWORD			GetManufacturerId() const { return wMid; }	//������ID
	DWORD			GetProductId() 		const { return wPid; }	//��ƷID
	MMVERSION       GetVersion()        const { return vDriverVersion; }	//�汾�ţ����ֽ������汾�ţ����ֽ��Ǵΰ汾��
	DWORD 			GetFormats() 		const { return dwFormats; }			//֧�ֵĸ�ʽ
	WORD 			GetChannels() 		const { return wChannels; }			//����
	LPCTSTR 		GetProductName() 	const { return szPname; }			//��Ʒ����
	UINT 			Size() 				const { return sizeof(WAVEOUTCAPS); }
	operator 		LPWAVEOUTCAPS() 	{ return static_cast<WAVEOUTCAPS*>(this); }
};

//WAVEFORMATEX
class WaveFormat : WAVEFORMATEX
{
public:
	WaveFormat()
	{
		memset(this, 0, Size());
	}
	
	WaveFormat(const WAVEFORMATEX& wfmt)
	{
		memcpy(this, &wfmt, Size());
	}
	
public:
	UINT 			Size() 					const { return sizeof(WAVEFORMATEX); }	
	operator 		LPWAVEFORMATEX() 		{ return static_cast<WAVEFORMATEX*>(this); }
};

//WAVEFORMATEXTENSIBLE
class WaveFormatExtensible : WAVEFORMATEXTENSIBLE
{
public:
	WaveFormatExtensible()
	{
		memset(this, 0, Size());
	}
	
	WaveFormatExtensible(const WAVEFORMATEXTENSIBLE& wfmt)
	{
		memcpy(this, &wfmt, Size());
	}
	
public:
	UINT 			Size() 					const { return sizeof(WAVEFORMATEXTENSIBLE); }
    operator 		LPWAVEFORMATEX() 		{ return static_cast<WAVEFORMATEX*>(&Format); }
    operator 		PWAVEFORMATEXTENSIBLE() { return static_cast<WAVEFORMATEXTENSIBLE*>(this); }	
};

//MMTIME
class WaveTime : MMTIME
{
public:
	WaveTime()
	{
		memset(this, 0, Size());
	}

	WaveTime(const MMTIME& mmt)
	{
		memcpy(this, &mmt, Size());
	}

public:
	DWORD 			GetType() 				const { return wType; }
    void 			SetType(DWORD type) 	{ wType = type; }
    DWORD 			GetTimeInfo() 			const { return u.ms; }
	void 			SetTimeInfo(DWORD info) { u.ms = info; }
	
    UINT 			Size() 					const { return sizeof(MMTIME); }
    operator 		LPMMTIME() 				{ return static_cast<MMTIME*>(this); }
};

//WAVEHDR
class WaveBuffer : WAVEHDR
{
public:
	WaveBuffer()
	{
		memset(this, 0, Size());
	}

	~WaveBuffer()
	{
		FreeMemory();
	}
	
public:
	LPSTR			GetBuffer()				const { return lpData; }	//ָ�������ݻ����ָ��
	DWORD 			GetBufferLength() 		const { return dwBufferLength; }	//�����С
	void 			SetBufferLength(DWORD bufferLength) 	{ dwBufferLength = bufferLength; }		//
	
	DWORD 			GetBytesRecorded() 		const { return dwBytesRecorded; }	//����ͷ�ṹ��������ʱ����ʶ���������ݵ�����
    void 			SetBytesRecorded(DWORD bytesRecorded) 	{ dwBytesRecorded = bytesRecorded; }	//
	
	DWORD 			GetFlags() 				const { return dwFlags; }	//��ʶ������
    void 			SetFlags(DWORD flags) 	{ dwFlags = flags; }
	
	DWORD 			GetLoops() 				const { return dwLoops; }	//ѭ������
    void 			SetLoops(DWORD loops) 	{ dwLoops = loops; }
	
	bool 			IsDone() 				const { return (dwFlags&WHDR_DONE)!=0; }		//
	bool 			IsPrepared() 			const { return (dwFlags&WHDR_PREPARED) != 0; }	//
	
	UINT 			Size() 					const { return sizeof(WAVEHDR); }
  
public:
	bool 			Allocate(DWORD size);
    void 			FreeMemory();
	
	operator 		LPWAVEHDR() 			{ return static_cast<WAVEHDR*>(this); }
	WaveBuffer* 	GetNext() 				{ reinterpret_cast<WaveBuffer*>( lpNext ); }

private:
	WaveBuffer(const WAVEHDR& whdr);
	WaveBuffer(const WaveBuffer& wbuf);	
};

inline bool WaveBuffer::Allocate(DWORD size)
{
	if( lpData != NULL )
	{
		lpData = static_cast<LPSTR>( HeapReAlloc(GetProcessHeap(), WAVE_HEAP_FLAGS, lpData, size) );
	}
	else
	{
		lpData = static_cast<LPSTR>( HeapAlloc(GetProcessHeap(), WAVE_HEAP_FLAGS, size) );
	}
	
	if ( lpData != NULL ) 
	{
		dwBufferLength = size;
	}
	
    return ( lpData != NULL );
}

inline void WaveBuffer::FreeMemory()
{
	if ( lpData != NULL ) 
	{
        HeapFree(GetProcessHeap(), 0, lpData);
        lpData = NULL; 
    }
}

//
enum WaveStatus
{
    waveClosed = 0,  ///< Closed
    waveStopped,     ///< Stopped
    waveStarted,     ///< Started
    wavePaused       ///< Paused
};

class IWaveNotifyHandler;

//IWaveDevice
class IWaveDevice
{
public:
	virtual bool		IsOpen() 	const = 0;
	
	virtual DWORD_PTR	GetId()		const = 0;
	virtual WaveStatus	GetDeviceStatus()	const = 0;
	virtual HRESULT 	GetPosition( WaveTime& wti ) const = 0;
	
	virtual HRESULT		Open(UINT uDeviceID, const WaveFormat& wfmt) = 0;
	virtual HRESULT 	Close() = 0;
    virtual HRESULT 	Start() = 0;
    virtual HRESULT 	Stop()  = 0;
    virtual HRESULT 	Reset() = 0;
    virtual HRESULT 	Pause() = 0;
	
    virtual HRESULT 	AddBuffer( WaveBuffer& wbuf ) = 0;
    virtual HRESULT 	PrepareBuffer( WaveBuffer& wbuf ) = 0;
    virtual HRESULT 	UnprepareBuffer( WaveBuffer& wbuf ) = 0;
    virtual HRESULT 	SetNotifyHandler(IWaveNotifyHandler* pNotify) = 0;
};

//IWaveNotifyHandler
class IWaveNotifyHandler
{
public:
    virtual void ProcessEvent(IWaveDevice* pDevice, UINT uMsg, WaveBuffer* pwbuf) = 0;
};

#endif