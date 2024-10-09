#pragma once


// BFileDialogWPath

class AFX_EXT_CLASS BFileDialogWPath : public CFileDialog
{
	DECLARE_DYNAMIC(BFileDialogWPath)

public:
	BFileDialogWPath(const CString &RegName, BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	virtual ~BFileDialogWPath();
	virtual INT_PTR DoModal();


protected:
	CString RegName2Store;
	bool LoadFromProjectFolder;
	DECLARE_MESSAGE_MAP()
};


