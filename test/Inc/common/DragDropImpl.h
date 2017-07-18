#ifndef __DRAGDROPIMPL_H__
#define __DRAGDROPIMPL_H__

//////////////////////////////////////////////////////////////////////////
//CDragSourceNotify
class CDragSourceNotify : IDropSourceNotify
{
public:
	CDragSourceNotify(void);
	~CDragSourceNotify(void);
	
public:	
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
	ULONG 	STDMETHODCALLTYPE AddRef(void);
	ULONG 	STDMETHODCALLTYPE Release(void);
	
	HRESULT STDMETHODCALLTYPE DragEnterTarget(HWND hwndTarget);
	HRESULT STDMETHODCALLTYPE DragLeaveTarget(void);
	
private:
	ULONG           m_cRefCount;	
};

//////////////////////////////////////////////////////////////////////////
//CEnumFormatEtc
class CEnumFormatEtc : public IEnumFORMATETC
{
public:
	CEnumFormatEtc(const CSimpleArray<FORMATETC>&  ArrFE);
	CEnumFormatEtc(const CSimpleArray<FORMATETC*>& ArrFE);
	
	STDMETHOD(QueryInterface)(REFIID, void FAR* FAR*);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);
	
	STDMETHOD(Next)(ULONG, LPFORMATETC, ULONG FAR *);
	STDMETHOD(Skip)(ULONG);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumFORMATETC FAR * FAR*);
	
private:
	ULONG			m_cRefCount;
	CSimpleArray<FORMATETC>  m_pFmtEtc;	
	int           	m_iCur;
};

//////////////////////////////////////////////////////////////////////////
//CIDropSource
class CIDropSource : public IDropSource
{
private:
	long 			m_cRefCount;
	bool 			m_bDropped;
	
public:
	CIDropSource(void);
	~CIDropSource(void); 
	
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			 /* [in] */ REFIID riid,
			 /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
			 
	virtual ULONG STDMETHODCALLTYPE AddRef(void);	
	virtual ULONG STDMETHODCALLTYPE Release(void);	
	
	//决定拖放操作是否应该继续,通过返回DRAGDROP_S_CANCEL来取消拖放操作;
	virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag(
			/* [in] */ BOOL fEscapePressed,
			/* [in] */ DWORD grfKeyState);
			
	//得到最终用户在拖放过程中的视觉反馈；	
	virtual HRESULT STDMETHODCALLTYPE GiveFeedback( 
			/* [in] */ DWORD dwEffect);
};

//////////////////////////////////////////////////////////////////////////
//CIDataObject
class CIDataObject : public IDataObject
{
public:
	CIDataObject(CIDropSource* pDropSource);
	~CIDataObject();
	
	void CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc);
	
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	//Render在FORMATETC结构体中描述的数据，并通过STGMEDIUM结构体来传递数据;
	virtual HRESULT	STDMETHODCALLTYPE GetData(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetcIn,
			/* [out] */ STGMEDIUM __RPC_FAR *pmedium);
			
	//Render在FORMATETC结构体中的数据，并通过调用者分配的STGMEDIUM结构体来传输数据;		
	virtual	HRESULT STDMETHODCALLTYPE GetDataHere(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
			/* [out][in] */ STGMEDIUM __RPC_FAR *pmedium);
			
	//判断数据对象是否可以Render在FORMATETC结构体中描述的数据;		
	virtual HRESULT STDMETHODCALLTYPE QueryGetData( 
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetc);
			
	//提供一个潜在不同的但逻辑相同的FORMATETC结构体数据;		
	virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatectIn,
			/* [out] */ FORMATETC __RPC_FAR *pformatetcOut);
	
	//提供一个通过FORMATETC结构和STGMEDIUM结构描述的源数据对象;	
	virtual	HRESULT	STDMETHODCALLTYPE SetData(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
			/* [unique][in] */ STGMEDIUM __RPC_FAR *pmedium,
			/* [in] */ BOOL fRelease);
	
	//创建并返回一个IEnumFORMATETC接口的指针来枚举数据对象支持的FORMATETC对象;
	virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc(
			/* [in] */ DWORD dwDirection,
			/* [out] */ IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenumFormatEtc);
	
	//创建一个在数据对象和通知接收器之间的连接，因此通知接收器能接收到数据对象中通知的改变;
	virtual HRESULT STDMETHODCALLTYPE DAdvise( 
			/* [in] */ FORMATETC __RPC_FAR *pformatetc,
			/* [in] */ DWORD advf,
			/* [unique][in] */ IAdviseSink __RPC_FAR *pAdvSink,
			/* [out] */ DWORD __RPC_FAR *pdwConnection);
	
	//销毁一个前面使用DAdvise方法安装的通知;
	virtual HRESULT STDMETHODCALLTYPE DUnadvise( 
			/* [in] */ DWORD dwConnection);		
	
	//创建和返回一个指向枚举当前通知连接的接口指针;
	virtual HRESULT STDMETHODCALLTYPE EnumDAdvise( 
			/* [out] */ IEnumSTATDATA __RPC_FAR *__RPC_FAR *ppenumAdvise);
			
private:
	CIDropSource* 	m_pDropSource;
	long 			m_cRefCount;	
	
	CSimpleArray<FORMATETC*> m_ArrFormatEtc;
	CSimpleArray<STGMEDIUM*> m_StgMedium;
};

//////////////////////////////////////////////////////////////////////////
//
class CIDropTarget : public IDropTarget
{
public:
	CIDropTarget(HWND m_hTargetWnd);
	virtual ~CIDropTarget();

public:
	void AddSuportedFormat(FORMATETC& ftetc) { m_formatetc.Add(ftetc); }	
	bool QueryDrop(DWORD grfKeyState, LPDWORD pdwEffect);
	
public:
	virtual bool OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef( void);
	virtual ULONG STDMETHODCALLTYPE Release( void);	
	
	//判断是否可以接受一个拖放操作,以及接受之后的效果;
	virtual HRESULT STDMETHODCALLTYPE DragEnter(
			/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);
		
	//提供通过DoDragDrop函数执行的目标反馈;	
	virtual HRESULT STDMETHODCALLTYPE DragOver( 
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);
			
	//导致一个Drop目标挂起它的返回行为		
	virtual HRESULT STDMETHODCALLTYPE DragLeave( void);

	//数据放在目标窗口	
	virtual HRESULT STDMETHODCALLTYPE Drop(
			/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);
	
protected:
	HWND 			m_hTargetWnd;	
private:
	DWORD 			m_cRefCount;
	bool 			m_bAllowDrop;
	struct IDropTargetHelper*	m_pDropTargetHelper;
	CSimpleArray<FORMATETC> 	m_formatetc;
	FORMATETC* 					m_pSupportedFrmt;
};

//////////////////////////////////////////////////////////////////////////
//
class CDragSourceHelper
{
public:
	CDragSourceHelper();
	virtual ~CDragSourceHelper();
	
public:
	HRESULT InitializeFromBitmap(
			HBITMAP hBitmap, 
			POINT& pt,	// cursor position in client coords of the window
			RECT& rc,	// selected item's bounding rect
			IDataObject* pDataObject,
			COLORREF crColorKey=GetSysColor(COLOR_WINDOW)// color of the window used for transparent effect.
			);
	
	HRESULT InitializeFromWindow(
			HWND hwnd,
			POINT& pt,
			IDataObject* pDataObject
			);
			
protected:
	IDragSourceHelper* pDragSourceHelper;		
};

#endif