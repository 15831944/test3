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
	DWORD 			GetManufacturerId() 	const { return wMid; }	//制造商ID
    DWORD 			GetProductId() 			const { return wPid; }	//产品ID
    MMVERSION 		GetVersion() 			const { return vDriverVersion; }	//版本号；高字节是主版本号，低字节是次版本号
    DWORD 			GetFormats()			const { return dwFormats; }			//支持的格式
    WORD 			GetChannels() 			const { return wChannels; }			//声道
    LPCTSTR 		GetProductName() 		const { return szPname; }			//产品名称
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
	DWORD			GetManufacturerId() const { return wMid; }	//制造商ID
	DWORD			GetProductId() 		const { return wPid; }	//产品ID
	MMVERSION       GetVersion()        const { return vDriverVersion; }	//版本号；高字节是主版本号，低字节是次版本号
	DWORD 			GetFormats() 		const { return dwFormats; }			//支持的格式
	WORD 			GetChannels() 		const { return wChannels; }			//声道
	LPCTSTR 		GetProductName() 	const { return szPname; }			//产品名称
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
	LPSTR			GetBuffer()				const { return lpData; }	//指向波形数据缓冲的指针
	DWORD 			GetBufferLength() 		const { return dwBufferLength; }	//缓冲大小
	void 			SetBufferLength(DWORD bufferLength) 	{ dwBufferLength = bufferLength; }		//
	
	DWORD 			GetBytesRecorded() 		const { return dwBytesRecorded; }	//波形头结构用于输入时，标识缓冲中数据的数量
    void 			SetBytesRecorded(DWORD bytesRecorded) 	{ dwBytesRecorded = bytesRecorded; }	//
	
	DWORD 			GetFlags() 				const { return dwFlags; }	//标识缓冲区
    void 			SetFlags(DWORD flags) 	{ dwFlags = flags; }
	
	DWORD 			GetLoops() 				const { return dwLoops; }	//循环次数
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