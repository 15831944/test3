#ifndef __MULTIMEDIASTREAM_HPP__
#define __MULTIMEDIASTREAM_HPP__

#pragma warning(push)
#include <atlbase.h>
#include <Strmif.h>
#include <mmreg.h>
#include <amstream.h>
#pragma warning(pop)

#pragma comment(lib, "Strmiids.lib")

//MAudioStream
class MAudioStream
{
public:
	MAudioStream();
	~MAudioStream();
	
public:
	const WAVEFORMATEX&		GetWaveFormat() const;
	
	HRESULT 				SetMediaStream(IMediaStream* pMediaStream);
	HRESULT					ReleaseMediaStream();
	
	HRESULT					GetSampleData(LPBYTE pbData, DWORD& dwSize, DWORD dwFlags = COMPSTAT_WAIT, DWORD dwTimeout = INFINITE);
	HRESULT					GetSampleTimes(STREAM_TIME *pStartTime, STREAM_TIME *pEndTime, STREAM_TIME *pCurrentTime);
	
	HRESULT 				CompletionStatus(DWORD dwFlags = COMPSTAT_WAIT, DWORD dwTimeout = INFINITE);
	
protected:
	CComPtr<IAudioMediaStream> 	_pAudioStream;   	///< Audio Stream
    CComPtr<IAudioData>        	_pAudioData;     	///< Audio Stream data
    CComPtr<IAudioStreamSample> _pAudioSample;  	///< Audio Stream sample
	
private:
	WAVEFORMATEX        		_wfmt;				///< Audio format
};


//MultimediaStream
class MultimediaStream
{
public:
	MultimediaStream();
	~MultimediaStream();

public:
	bool					IsValid() const;

	HRESULT					Initialize(STREAM_TYPE streamType, DWORD dwFlags, IGraphBuilder* pFilterGraph);
	HRESULT					Close();
	
	HRESULT 				GetFilterGraph(IGraphBuilder** ppFilterGraph);
    HRESULT 				GetMediaStream(REFMSPID idPurpose, IMediaStream** ppMediaStream);
    HRESULT 				GetDuration(STREAM_TIME* pDuration);
    HRESULT 				GetTime(STREAM_TIME* pCurrentTime);
    HRESULT 				GetState(STREAM_STATE* pCurrentState);
    HRESULT 				SetState(STREAM_STATE NewState);
	
    HRESULT 				Seek(STREAM_TIME seekTime);
    HRESULT 				OpenFile(LPCTSTR lpszFileName, DWORD dwFlags);
	
    HRESULT 				AddMediaStream(IUnknown* pStreamObject, const MSPID* pPurposeID, DWORD dwFlags, IMediaStream** ppNewStream);
    HRESULT 				AddSourceFilter(LPCTSTR lpszFileName, LPCTSTR lpszFilterName, IBaseFilter** ppFilter);
    HRESULT 				AddFilter(LPCTSTR lpszFilterName, IBaseFilter* pFilter);
	
    HRESULT 				FindFilterByName(LPCTSTR lpszFilterName, IBaseFilter** ppFilter);
    HRESULT 				Render(DWORD dwFlags);
	
protected:
	CComPtr<IGraphBuilder> 			_pGB;		///< Filter graph
    CComPtr<IAMMultiMediaStream> 	_pAMMS;		///< Multimedia stream	
};

#endif