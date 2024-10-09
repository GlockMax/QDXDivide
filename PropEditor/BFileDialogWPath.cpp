// BFileDialogWPath.cpp : implementation file
//

#include "stdafx.h"
#include "NCMVersion.h"
#include "NCMProject.h"
#include "BFileDialogWPath.h"


// BFileDialogWPath

IMPLEMENT_DYNAMIC(BFileDialogWPath, CFileDialog)

BFileDialogWPath::BFileDialogWPath(const CString &RegName, BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	RegName2Store = RegName;
}

BFileDialogWPath::~BFileDialogWPath()
{
}

INT_PTR BFileDialogWPath::DoModal()
{
	CString Section;
	if (RegName2Store == _T("LastProgPath"))
		Section = _T("Program");
	else if (RegName2Store == _T("LastFixtPath"))
		Section = _T("Fixture");
	else if (RegName2Store == _T("LastStockPath"))
		Section = _T("Stock");
	else if (RegName2Store == _T("LastPartPath"))
		Section = _T("Part");

	bool LoadFromProjectFolder = NCM_PROJECT.Defaults.GetBool(_T("Defaults/") + Section + _T("/LoadFromProjectFolder@Enable"), false);

	CString RegName2Read = LoadFromProjectFolder ? _T("LastOpenedPath") : RegName2Store;
	auto Path = AfxGetApp()->GetProfileString(NCMVersion, RegName2Read, _T(""));
	if(!Path.IsEmpty())
	{
		Path = Path.Left(Path.ReverseFind('\\') + 1);
		m_ofn.lpstrInitialDir = Path;
	}
	auto Ret = CFileDialog::DoModal();
	if(Ret == IDOK)
	{
		auto Pos = GetStartPosition();
		if(Pos != NULL)
			AfxGetApp()->WriteProfileString(NCMVersion, RegName2Store, GetNextPathName(Pos));
	}
	return Ret;
}


BEGIN_MESSAGE_MAP(BFileDialogWPath, CFileDialog)
END_MESSAGE_MAP()



// BFileDialogWPath message handlers


