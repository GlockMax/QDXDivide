#include "stdafx.h"
#include "NCommonPaths.h"

NCommonPaths::NCommonPaths(const CStringA& Ver) : Version(Ver)
{
}

NCommonPaths::~NCommonPaths(void)
{
}
const CString NCommonPaths::GetPath(CommonFolder FolderID) const
{
	CString path = GetProfileStr(GetRegName(FolderID));

	if(path.IsEmpty())
		path = GetDefaultPath(FolderID);

	return path;

}

CString NCommonPaths::GetProfileStr(const CString& Entry) const
{
	// instead of AfxGetApp()->GetProfileString to ensure NCMHandlers works properly
	bool RegistryError = false;
	HKEY hkResult;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Inteks\\NCManager\\") + Version, 0, KEY_QUERY_VALUE, &hkResult)
		!= ERROR_SUCCESS)
	{
		RegistryError = true;
	}

	unsigned char buf[256];
	unsigned long s = 256;
	if (RegQueryValueExA(hkResult, Entry, NULL, NULL, buf, &s)
		!= ERROR_SUCCESS)
	{
		RegistryError = true;
	}
	if (RegistryError)
		return CString();
	else
		return CString(buf);
}

const CString NCommonPaths::GetDefaultPath(CommonFolder FolderID) const
{
	switch (FolderID)
	{
	case CF_MACHINES:
		return GetProfileStr(_T("DataPath")) + _T("def\\machines\\");
	case CF_UNITS:
		return GetProfileStr(_T("DataPath")) + _T("def\\");
	case CF_PROTOTYPES:
		return GetProfileStr(_T("DataPath")) + _T("def\\prt\\");
	case CF_SETTINGS:
		return GetProfileStr(_T("DataPath")) + _T("def\\dfn\\");
	case CF_TOOLS:
		return GetProfileStr(_T("DataPath")) + _T("demo\\");
	case CF_DEMO:
		return GetProfileStr(_T("DataPath")) + _T("demo\\");
	case CF_MACRO:
		return GetProfileStr(_T("DataPath")) + _T("def\\macros\\");
	case CF_TEMP:
		return GetProfileStr(_T("DataPath")) + _T("temp\\");
	case CF_COMP:
		return GetProfileStr(_T("DataPath")) + _T("def\\compare\\");
	default:
		return _T("");
	}
}

void NCommonPaths::SetPath(CommonFolder FolderID, const CString &Path)
{
	AfxGetApp()->WriteProfileString(Version, GetRegName(FolderID), Path);
}

const CString NCommonPaths::GetRegName(CommonFolder FolderID) const
{
	switch(FolderID)
	{
	case CF_MACHINES:
		return _T("MachinesPath");
	case CF_UNITS:
		return _T("UnitsPath");
	case CF_PROTOTYPES:
		return _T("PrototypesPath");
	case CF_SETTINGS:
		return _T("SettingsPath");	
	case CF_TOOLS:
		return _T("ToolsPath");
	default:
		return _T("");
	}
}
const CString NCommonPaths::GetDefaultSuffix(CommonFolder FolderID) const
{
	switch(FolderID)
	{
	case CF_MACHINES:
		return _T("def\\machines\\");
	case CF_UNITS:
		return _T("def\\");
	case CF_PROTOTYPES:
		return _T("def\\prt\\");
	case CF_SETTINGS:
		return _T("");	
	case CF_TOOLS:
		return _T("demo\\");	
	default:
		return _T("");
	}
}
