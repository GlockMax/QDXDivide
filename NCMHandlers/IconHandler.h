// IconHandler.h : Declaration of the CIconHandler

#pragma once
#include "resource.h"       // main symbols
#include "NCMHandlers_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CIconHandler

class ATL_NO_VTABLE CIconHandler : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIconHandler, &CLSID_IconHandler>,
	public IIconHandler,
	public IPersistFile,
	public IExtractIcon
{
public:
	CIconHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ICONHANDLER)

DECLARE_NOT_AGGREGATABLE(CIconHandler)

BEGIN_COM_MAP(CIconHandler)
	COM_INTERFACE_ENTRY(IIconHandler)
	COM_INTERFACE_ENTRY(IPersistFile)
	COM_INTERFACE_ENTRY(IExtractIcon)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IPersistFile
	STDMETHOD(GetClassID)(CLSID*) { return E_NOTIMPL; }
	STDMETHOD(IsDirty)() { return E_NOTIMPL; }
	STDMETHOD(Save)(LPCOLESTR, BOOL) { return E_NOTIMPL; }
	STDMETHOD(SaveCompleted)(LPCOLESTR) { return E_NOTIMPL; }
	STDMETHOD(GetCurFile)(LPOLESTR*) { return E_NOTIMPL; }
	STDMETHOD(Load)(LPCOLESTR wszFile, DWORD /*dwMode*/)
	{
		USES_CONVERSION;
		lstrcpyn(m_szFilename, OLE2CT(wszFile), MAX_PATH);
		return S_OK;
	}
	// IExtractIcon
	STDMETHOD(GetIconLocation)(UINT uFlags, LPTSTR szIconFile, UINT cchMax,
		int* piIndex, UINT* pwFlags);
	STDMETHOD(Extract)(LPCTSTR pszFile, UINT nIconIndex, HICON* phiconLarge,
		HICON* phiconSmall, UINT nIconSize) {
		return S_FALSE;// Tell the shell to do the extracting itself.
	};

protected:
	TCHAR     m_szFilename[MAX_PATH];  // Full path to the file in question.
	DWORDLONG m_qwFileSize;             // File size; used by extraction method 2.
};

OBJECT_ENTRY_AUTO(__uuidof(IconHandler), CIconHandler)
