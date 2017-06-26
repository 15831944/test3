#include "StdAfx.h"
#include <shlobj.h>
#include <atlbase.h>
#include "DragDropImpl.h"

//////////////////////////////////////////////////////////////////////////
//CDragSourceNotify
CDragSourceNotify::CDragSourceNotify()
				  :m_cRefCount(0), 
{
}

CDragSourceNotify::~CDragSourceNotify()
{
}

HRESULT STDMETHODCALLTYPE CDragSourceNotify::QueryInterface(REFIID riid, void** ppvObject)
{
	if(!ppvObject)
	{
		return E_POINTER;
	}
	
	if(IsEqualIID(riid, IID_IUnknown))
	{
		*ppvObject = static_cast<IUnknown*>(this);
	}
	else if(IsEqualIID(riid, IID_IDropSourceNotify))
	{
		*ppvObject = static_cast<IDropSourceNotify*>(this);
	}
	else
	{
		*ppvObject = NULL;
        return E_NOINTERFACE;
	}
	
	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CDragSourceNotify::AddRef()
{
	return InterlockedIncrement(&m_cRefCount);
}

ULONG STDMETHODCALLTYPE CDragSourceNotify::Release()
{
	ULONG uRet = InterlockedDecrement(&m_cRefCount);
	if(!uRet)
	{
		delete this;
	}
	return uRet;
}

HRESULT STDMETHODCALLTYPE CDragSourceNotify::DragEnterTarget(HWND hwndTarget)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDragSourceNotify::DragLeaveTarget()
{
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
//CEnumFormatEtc
CEnumFormatEtc::CEnumFormatEtc(const CSimpleArray<FORMATETC>&  ArrFE)
{
	ATLTRACE("CEnumFormatEtc::CEnumFormatEtc()\n");
	for(int i=0; i < ArrFE.GetSize(); ++i)
	{
		m_pFmtEtc.Add(ArrFE[i]);
	}	
}

CEnumFormatEtc::CEnumFormatEtc(const CSimpleArray<FORMATETC*>& ArrFE)
{
	ATLTRACE("CEnumFormatEtc::CEnumFormatEtc()\n");
	for(int i=0; i < ArrFE.GetSize(); ++)
	{
		m_pFmtEtc.Add(*ArrFE[i]);
	}	
}

STDMETHODIMP  CEnumFormatEtc::QueryInterface(REFIID refiid, void FAR* FAR* ppv)
{
	ATLTRACE("CEnumFormatEtc::QueryInterface()\n");
	if(ppv == NULL)
	{
		return E_POINTER;
	}
	
	*ppv = NULL;
	if (IID_IUnknown==refiid || IID_IEnumFORMATETC==refiid)
	{
		*ppv = static_cast<IEnumFORMATETC*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}
	
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CEnumFormatEtc::AddRef(void)
{
	ATLTRACE("CEnumFormatEtc::AddRef()\n");
	return ++m_cRefCount;
}

STDMETHODIMP_(ULONG) CEnumFormatEtc::Release(void)
{
	ATLTRACE("CEnumFormatEtc::Release()\n");
	ULONG ulCount = --m_cRefCount;
	ATLASSERT(ulCount >= 0);
	
	if(ulCount == 0)
	{
		delete this;
	}
	
	return ulCount;
}

STDMETHODIMP CEnumFormatEtc::Next( ULONG celt,LPFORMATETC lpFormatEtc, ULONG FAR *pceltFetched)
{
	ATLTRACE("CEnumFormatEtc::Next()\n");
	if(celt <= 0)
	{
		return E_INVALIDARG;
	}
	
	if (celt != 1 && pceltFetched == NULL && lpFormatEtc == NULL)
	{
		return E_POINTER;
	}
	
	*pceltFetched = 0;
	if(m_iCur >= m_pFmtEtc.GetSize())
	{
		return S_FALSE;
	}
	
	ULONG ulRet = celt;
	while(m_iCur < m_pFmtEtc.GetSize() && ulRet > 0)
	{
		*lpFormatEtc++ = m_pFmtEtc[m_iCur++];
		--ulRet;
	}
	
	if(pceltFetched != NULL)
	{
		*pceltFetched = celt - ulRet;
	}
	
	return (ulRet == 0) ? S_OK : S_FALSE;
}

STDMETHODIMP CEnumFormatEtc::Skip(ULONG celt)
{
	ATLTRACE("CEnumFormatEtc::Skip()\n");
	if((m_iCur + int(celt)) >= m_pFmtEtc.GetSize())
	{
		return S_FALSE;
	}
	
	m_iCur += celt;
	return S_OK;
}

STDMETHODIMP CEnumFormatEtc::Reset(void)
{
	ATLTRACE("CEnumFormatEtc::Reset()\n");
	m_iCur = 0;
	
	return S_OK;
}

STDMETHODIMP CEnumFormatEtc::Clone(IEnumFORMATETC FAR * FAR*ppCloneEnumFormatEtc)
{
	ATLTRACE("CEnumFormatEtc::Clone()\n");
	
	if(ppCloneEnumFormatEtc == NULL)
	{
		return E_POINTER;
	}
	
	CEnumFormatEtc *newEnum = new CEnumFormatEtc(m_pFmtEtc);
	if(newEnum ==NULL)
	{
		return E_OUTOFMEMORY; 
	}
	
	newEnum->AddRef();
	newEnum->m_iCur = m_iCur;
	
	*ppCloneEnumFormatEtc = newEnum;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
//CIDropSource
CIDropSource::CIDropSource()
			 :m_cRefCount(0), m_bDropped(false)
{
}

CIDropSource::~CIDropSource()
{
}

STDMETHODIMP CIDropSource::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if(!ppvObject)
	{
		return E_POINTER;
	}
	
	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource))
	{
		*ppvObject = static_cast<IDropSource*>(this);
	}
	else
	{
		*ppvObject = NULL;
        return E_NOINTERFACE;
	}
	
	AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CIDropSource::AddRef(void)
{
	ATLTRACE("CIDropSource::AddRef\n");
	return ++m_cRefCount;
}

STDMETHODIMP_(ULONG) CIDropSource::Release(void)
{
	ATLTRACE("CIDropSource::Release\n");
	ULONG ulRet = --m_cRefCount;
	
	ATLASSERT(ulRet >= 0);
	
	if(ulRet==0)
	{
		delete this;
	}
	
	return uRet;
}

STDMETHODIMP CIDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	ATLTRACE("CIDropSource::QueryContinueDrag\n");
	
	if(fEscapePressed)
	{
		return DRAGDROP_S_CANCEL;
	}
	
	if(!(grfKeyState & (MK_LBUTTON|MK_RBUTTON)))
	{
		m_bDropped = true;
		return DRAGDROP_S_DROP;
	}
	
	return S_OK;
}

STDMETHODIMP CIDropSource::GiveFeedback(DWORD dwEffect)
{
	ATLTRACE("CIDropSource::GiveFeedback\n");
	return DRAGDROP_S_USEDEFAULTCURSORS;
}
//////////////////////////////////////////////////////////////////////////
//CIDataObject
CIDataObject::CIDataObject(CIDropSource* pDropSource)
			 :m_cRefCount(0), m_pDropSource(pDropSource)
{
}		

CIDataObject::~CIDataObject()
{
	for(int i = 0; i < m_StgMedium.GetSize(); ++i)
	{
		ReleaseStgMedium(m_StgMedium[i]);
		delete m_StgMedium[i];
	}
	for(int j = 0; j < m_ArrFormatEtc.GetSize(); ++j)
		delete m_ArrFormatEtc[j];
}	

STDMETHODIMP CIDataObject::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	ATLTRACE("CIDataObject::QueryInterface()\n");
	if(!ppvObject)
	{
		return E_POINTER;
	}
	
	if(IsEqualIID(IID_IUnknown, riid) || IsEqualIID(IID_IDataObject, riid))
	{
		*ppvObject=static_cast<IDataObject*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	
	AddRef();
    return S_OK;
} 

STDMETHODIMP_(ULONG) CIDataObject::AddRef( void)
{
	ATLTRACE("CIDataObject::AddRef()\n");
	return ++m_cRefCount;
}

STDMETHODIMP_(ULONG) CIDataObject::Release( void)
{
	ATLTRACE("CIDataObject::Release()\n");	
	
	ULONG ulRet = --m_cRefCount;
	ATLASSERT(ulRet >= 0);
	
	if(ulRet==0)
	{
		delete this;
	}
	
	return ulRet;
}

STDMETHODIMP CIDataObject::GetData(FORMATETC __RPC_FAR *pformatetcIn, STGMEDIUM __RPC_FAR *pmedium)
{
	ATLTRACE("CIDataObject::GetData\n");
	if(pformatetcIn == NULL || pmedium == NULL)
	{
		return E_INVALIDARG;
	}
	
	pmedium->hGlobal = NULL;
	ATLASSERT(m_StgMedium.GetSize() == m_ArrFormatEtc.GetSize());
	for(int i=0; i < m_ArrFormatEtc.GetSize(); ++i)
	{
		if((pformatetcIn->tymed & m_ArrFormatEtc[i]->tymed) &&
		   (pformatetcIn->dwAspect == m_ArrFormatEtc[i]->dwAspect) &&
		   (pformatetcIn->cfFormat == m_ArrFormatEtc[i]->cfFormat)
		  )
		{
			CopyMedium(pmedium, m_StgMedium[i], m_ArrFormatEtc[i]);
			return S_OK;
		}
	}
	
	return DV_E_FORMATETC;
}

STDMETHODIMP CIDataObject::GetDataHere(FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pmedium)
{
	ATLTRACE("CIDataObject::GetDataHere\n");
	
	return E_NOTIMPL;
}

STDMETHODIMP CIDataObject::QueryGetData(FORMATETC __RPC_FAR *pformatetc)
{
	ATLTRACE("CIDataObject::QueryGetData\n");
	if(pformatetc == NULL)
	{
		return E_INVALIDARG;
	}
	
	if (!(DVASPECT_CONTENT & pformatetc->dwAspect))
	{
		return (DV_E_DVASPECT);
	}
	
	HRESULT  hr = DV_E_TYMED;
	for(int i = 0; i < m_ArrFormatEtc.GetSize(); ++i)
	{
		if(pformatetc->tymed & m_ArrFormatEtc[i]->tymed)
		{
			if(pformatetc->cfFormat == m_ArrFormatEtc[i]->cfFormat)
			{
				return S_OK;
			}
			else
			{
				hr = DV_E_CLIPFORMAT;
			}
		}
		else
		{
			hr = DV_E_TYMED;
		}
	}
	
	return hr;
} 

STDMETHODIMP CIDataObject::GetCanonicalFormatEtc(FORMATETC __RPC_FAR *pformatectIn, FORMATETC __RPC_FAR *pformatetcOut)
{
	ATLTRACE("CIDataObject::GetCanonicalFormatEtc\n");
	if(pformatetcOut == NULL)
	{
		return E_INVALIDARG;
	}
	
	return DATA_S_SAMEFORMATETC;
}

STDMETHODIMP CIDataObject::SetData(FORMATETC __RPC_FAR *pformatetc, STGMEDIUM __RPC_FAR *pmedium, BOOL fRelease)
{
	ATLTRACE("CIDataObject::SetData\n");
	if(pformatetc == NULL || pmedium == NULL)
	{
		return E_INVALIDARG;
	}
	
	ATLASSERT(pformatetc->tymed == pmedium->tymed);
	
	STGMEDIUM* pStgMed = new STGMEDIUM;
	FORMATETC* fetc = new FORMATETC;
	
	if(fetc == NULL || pStgMed == NULL)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(fetc, sizeof(FORMATETC));
	ZeroMemory(pStgMed, sizeof(STGMEDIUM));
	
	*fetc = *pformatetc;
	m_ArrFormatEtc.Add(fetc);
	
	if(fRelease)
	{
		*pStgMed = *pmedium;
	}
	else
	{
		CopyMedium(pStgMed, pmedium, pformatetc);
	}
	m_StgMedium.Add(pStgMed);
	
	return S_OK;
}

STDMETHODIMP CIDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenumFormatEtc)
{
	ATLTRACE("CIDataObject::EnumFormatEtc\n");
	if(ppenumFormatEtc == NULL)
	{
		return E_POINTER;
	}
	
	*ppenumFormatEtc=NULL;
	switch (dwDirection)
	{
	case DATADIR_GET:
		{
			*ppenumFormatEtc= new CEnumFormatEtc(m_ArrFormatEtc);
			if(*ppenumFormatEtc == NULL)
			{
				return E_OUTOFMEMORY;
			}
			(*ppenumFormatEtc)->AddRef();
		}
		break;
		
	case DATADIR_SET:
		{
		}
		break;
		
	default:
		{
			return E_NOTIMPL;
		}
		break;
	}
	
	return S_OK;
}

STDMETHODIMP CIDataObject::DAdvise(FORMATETC __RPC_FAR *pformatetc, DWORD advf, IAdviseSink __RPC_FAR *pAdvSink, DWORD __RPC_FAR *pdwConnection)
{
	ATLTRACE("CIDataObject::DAdvise\n");
	return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP CIDataObject::DUnadvise(DWORD dwConnection)
{
	ATLTRACE("CIDataObject::DUnadvise\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CIDataObject::EnumDAdvise(IEnumSTATDATA __RPC_FAR *__RPC_FAR *ppenumAdvise) 
{
	ATLTRACE("CIDataObject::EnumDAdvise\n");
	return OLE_E_ADVISENOTSUPPORTED;
}

void CIDataObject::CopyMedium(STGMEDIUM* pMedDest, STGMEDIUM* pMedSrc, FORMATETC* pFmtSrc)
{
	switch(pMedSrc->tymed)
	{
	case TYMED_HGLOBAL:
		{
			pMedDest->hGlobal = (HGLOBAL)OleDuplicateData(pMedSrc->hGlobal,pFmtSrc->cfFormat, NULL);
		}
		break;
		
	case TYMED_GDI:
		{
			pMedDest->hBitmap = (HBITMAP)OleDuplicateData(pMedSrc->hBitmap,pFmtSrc->cfFormat, NULL);
		}
		break;
		
	case TYMED_MFPICT:
		{
			pMedDest->hMetaFilePict = (HMETAFILEPICT)OleDuplicateData(pMedSrc->hMetaFilePict,pFmtSrc->cfFormat, NULL);
		}
		break;
		
	case TYMED_ENHMF:
		{
			pMedDest->hEnhMetaFile = (HENHMETAFILE)OleDuplicateData(pMedSrc->hEnhMetaFile,pFmtSrc->cfFormat, NULL);
		}
		break;
		
	case TYMED_FILE:
		{
			pMedSrc->lpszFileName = (LPOLESTR)OleDuplicateData(pMedSrc->lpszFileName,pFmtSrc->cfFormat, NULL);
		}
		break;
		
	case TYMED_ISTREAM:
		{
			pMedDest->pstm = pMedSrc->pstm;
			pMedSrc->pstm->AddRef();
		}
		break;
		
	case TYMED_ISTORAGE:
		{
			pMedDest->pstg = pMedSrc->pstg;
			pMedSrc->pstg->AddRef();
		}
		break;
		
	case TYMED_NULL:
		{
		}
		break;
		
	default:
		break;
	}
	
	pMedDest->tymed = pMedSrc->tymed;
	pMedDest->pUnkForRelease = NULL;
	if(pMedSrc->pUnkForRelease != NULL)
	{
		pMedDest->pUnkForRelease = pMedSrc->pUnkForRelease;
		pMedSrc->pUnkForRelease->AddRef();
	}
}
//////////////////////////////////////////////////////////////////////////
//
CIDropTarget::CIDropTarget(HWND hTargetWnd)
			 :m_hTargetWnd(hTargetWnd),
			  m_cRefCount(0),
			  m_bAllowDrop(false),
			  m_pDropTargetHelper(NULL),
			  m_pSupportedFrmt(NULL),
			  m_pIDataObject(NULL)
{
	ATLASSERT(m_hTargetWnd != NULL);
	if(FAILED(CoCreateInstance(CLSID_DragDropHelper,NULL,CLSCTX_INPROC_SERVER,
							   IID_IDropTargetHelper,(LPVOID*)&m_pDropTargetHelper)))
	{
		m_pDropTargetHelper = NULL;
	}	
}

CIDropTarget::~CIDropTarget()
{
	if(m_pDropTargetHelper != NULL)
	{
		m_pDropTargetHelper->Release();
		m_pDropTargetHelper = NULL;
	}
}

HRESULT STDMETHODCALLTYPE CIDropTarget::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if(ppvObject == NULL)
	{
		return E_POINTER;
	}
	
	*ppvObject = NULL;
	
	if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
	{
		*ppvObject = static_cast<IDropTarget*>(this);
	}
	else
	{
		return	E_NOINTERFACE;
	}
	
	AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE CIDropTarget::AddRef(void)
{
	ATLTRACE("CIDropTarget::AddRef()\n");
	return ++m_cRefCount; 
}

ULONG STDMETHODCALLTYPE CIDropTarget::Release(void)
{
	ATLTRACE("CIDropTarget::Release()\n");
	ULONG ulRet = --m_cRefCount;
	ATLASSERT(ulRet >= 0);
	if(ulRet == 0)
	{
		delete this;
	}
	
	return ulRet;
}

HRESULT STDMETHODCALLTYPE CIDropTarget::DragEnter(
						IDataObject __RPC_FAR *pDataObj,
						DWORD grfKeyState,
						POINTL pt,
						DWORD __RPC_FAR *pdwEffect)
{
	ATLTRACE("CIDropTarget::DragEnter()\n");
	if(pDataObj == NULL || pdwEffect == NULL)
	{
		return E_INVALIDARG;
	}
	
	if(m_pDropTargetHelper)
	{
		m_pDropTargetHelper->DragEnter(m_hTargetWnd, pDataObj,(LPPOINT)&pt, *pdwEffect);
	}
	
	m_pSupportedFrmt = NULL;
	for(int i =0; i<m_formatetc.GetSize(); ++i)
	{
		m_bAllowDrop = (pDataObj->QueryGetData(&m_formatetc[i]) == S_OK) ? true : false;
		
		if(m_bAllowDrop)
		{
			m_pSupportedFrmt = &m_formatetc[i];
			break;
		}
	}
	
	QueryDrop(grfKeyState, pdwEffect);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CIDropTarget::DragOver(
						DWORD grfKeyState,
						POINTL pt,
						DWORD __RPC_FAR *pdwEffect)
{
	ATLTRACE("CIDropTarget::DragOver()\n");
	if(pdwEffect == NULL)
	{
		return	E_POINTER;
	}
	
	if(m_pDropTargetHelper)
	{
		m_pDropTargetHelper->DragOver((LPPOINT)&pt, *pdwEffect);
	}
	
	QueryDrop(grfKeyState, pdwEffect);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CIDropTarget::DragLeave()
{
	ATLTRACE("CIDropTarget::DragLeave\n");
	if(m_pDropTargetHelper)
	{
		m_pDropTargetHelper->DragLeave();
	}
	
	m_bAllowDrop = false;
	m_pSupportedFrmt = NULL;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CIDropTarget::Drop(
						IDataObject __RPC_FAR *pDataObj,
						DWORD grfKeyState,
						POINTL pt, 
						DWORD __RPC_FAR *pdwEffect)
{
	ATLTRACE("CIDropTarget::Drop\n");
	if (pDataObj == NULL || pdwEffect == NULL)
	{
		return E_INVALIDARG;
	}
	
	if(m_pDropTargetHelper)
	{
		m_pDropTargetHelper->Drop(pDataObj, (LPPOINT)&pt, *pdwEffect);
	}
	
	if(QueryDrop(grfKeyState, pdwEffect))
	{
		if(m_bAllowDrop && m_pSupportedFrmt != NULL)
		{
			STGMEDIUM medium;
			if(pDataObj->GetData(m_pSupportedFrmt, &medium) == S_OK)
			{
				if(OnDrop(m_pSupportedFrmt, medium, pdwEffect, pt))
				{
					ReleaseStgMedium(&medium);
				}
			}
		}
	}
	
	m_bAllowDrop=false;
	*pdwEffect = DROPEFFECT_NONE;
	m_pSupportedFrmt = NULL;
	return S_OK;
}

bool CIDropTarget::QueryDrop(DWORD grfKeyState, LPDWORD pdwEffect)
{
	ATLTRACE("CIDropTarget::QueryDrop\n");
	DWORD dwOKEffects = *pdwEffect; 
	
	if(!m_bAllowDrop)
	{
	   *pdwEffect = DROPEFFECT_NONE;
	   return false;
	}
	
	//CTRL+SHIFT  -- DROPEFFECT_LINK
	//CTRL        -- DROPEFFECT_COPY
	//SHIFT       -- DROPEFFECT_MOVE
	//no modifier -- DROPEFFECT_MOVE or whatever is allowed by src
	
	*pdwEffect = (grfKeyState & MK_CONTROL) ?
				 ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_LINK : DROPEFFECT_COPY ):
				 ( (grfKeyState & MK_SHIFT) ? DROPEFFECT_MOVE : 0 );
				 
	if(*pdwEffect == 0) 
	{
		if (DROPEFFECT_COPY & dwOKEffects)
			*pdwEffect = DROPEFFECT_COPY;
		else if (DROPEFFECT_MOVE & dwOKEffects)
			*pdwEffect = DROPEFFECT_MOVE; 
		else if (DROPEFFECT_LINK & dwOKEffects)
			*pdwEffect = DROPEFFECT_LINK; 
		else 
		{
			*pdwEffect = DROPEFFECT_NONE;
		}
	}
	else
	{
		if(!(*pdwEffect & dwOKEffects))
		{
			*pdwEffect = DROPEFFECT_NONE;
		}
	}
	
	return (DROPEFFECT_NONE == *pdwEffect)?false:true;
}