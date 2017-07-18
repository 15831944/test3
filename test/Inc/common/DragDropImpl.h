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
	
	//�����ϷŲ����Ƿ�Ӧ�ü���,ͨ������DRAGDROP_S_CANCEL��ȡ���ϷŲ���;
	virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag(
			/* [in] */ BOOL fEscapePressed,
			/* [in] */ DWORD grfKeyState);
			
	//�õ������û����ϷŹ����е��Ӿ�������	
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

	//Render��FORMATETC�ṹ�������������ݣ���ͨ��STGMEDIUM�ṹ������������;
	virtual HRESULT	STDMETHODCALLTYPE GetData(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetcIn,
			/* [out] */ STGMEDIUM __RPC_FAR *pmedium);
			
	//Render��FORMATETC�ṹ���е����ݣ���ͨ�������߷����STGMEDIUM�ṹ������������;		
	virtual	HRESULT STDMETHODCALLTYPE GetDataHere(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
			/* [out][in] */ STGMEDIUM __RPC_FAR *pmedium);
			
	//�ж����ݶ����Ƿ����Render��FORMATETC�ṹ��������������;		
	virtual HRESULT STDMETHODCALLTYPE QueryGetData( 
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetc);
			
	//�ṩһ��Ǳ�ڲ�ͬ�ĵ��߼���ͬ��FORMATETC�ṹ������;		
	virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatectIn,
			/* [out] */ FORMATETC __RPC_FAR *pformatetcOut);
	
	//�ṩһ��ͨ��FORMATETC�ṹ��STGMEDIUM�ṹ������Դ���ݶ���;	
	virtual	HRESULT	STDMETHODCALLTYPE SetData(
			/* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
			/* [unique][in] */ STGMEDIUM __RPC_FAR *pmedium,
			/* [in] */ BOOL fRelease);
	
	//����������һ��IEnumFORMATETC�ӿڵ�ָ����ö�����ݶ���֧�ֵ�FORMATETC����;
	virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc(
			/* [in] */ DWORD dwDirection,
			/* [out] */ IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenumFormatEtc);
	
	//����һ�������ݶ����֪ͨ������֮������ӣ����֪ͨ�������ܽ��յ����ݶ�����֪ͨ�ĸı�;
	virtual HRESULT STDMETHODCALLTYPE DAdvise( 
			/* [in] */ FORMATETC __RPC_FAR *pformatetc,
			/* [in] */ DWORD advf,
			/* [unique][in] */ IAdviseSink __RPC_FAR *pAdvSink,
			/* [out] */ DWORD __RPC_FAR *pdwConnection);
	
	//����һ��ǰ��ʹ��DAdvise������װ��֪ͨ;
	virtual HRESULT STDMETHODCALLTYPE DUnadvise( 
			/* [in] */ DWORD dwConnection);		
	
	//�����ͷ���һ��ָ��ö�ٵ�ǰ֪ͨ���ӵĽӿ�ָ��;
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
	
	//�ж��Ƿ���Խ���һ���ϷŲ���,�Լ�����֮���Ч��;
	virtual HRESULT STDMETHODCALLTYPE DragEnter(
			/* [unique][in] */ IDataObject __RPC_FAR *pDataObj,
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);
		
	//�ṩͨ��DoDragDrop����ִ�е�Ŀ�귴��;	
	virtual HRESULT STDMETHODCALLTYPE DragOver( 
			/* [in] */ DWORD grfKeyState,
			/* [in] */ POINTL pt,
			/* [out][in] */ DWORD __RPC_FAR *pdwEffect);
			
	//����һ��DropĿ��������ķ�����Ϊ		
	virtual HRESULT STDMETHODCALLTYPE DragLeave( void);

	//���ݷ���Ŀ�괰��	
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