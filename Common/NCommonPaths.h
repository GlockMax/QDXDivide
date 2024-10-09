#pragma once
#include "NCMB_API.h"
enum CommonFolder
{
	CF_MACHINES,
	CF_UNITS,
	CF_PROTOTYPES,
	CF_SETTINGS,
	CF_DEMO,
	CF_TOOLS,
	CF_MACRO,
	CF_TEMP,
	CF_COMP
};

class NCMB_API NCommonPaths
{
public:


	NCommonPaths(const CStringA& Ver);
	~NCommonPaths(void);
	const CString GetPath(CommonFolder FolderID) const;
	CString GetProfileStr(const CString& Entry) const;
	const CString GetDefaultPath(CommonFolder FolderID) const;
	void SetPath(CommonFolder FolderID, const CString &Path);
protected:
	const CStringA& Version;
	const CString GetRegName(CommonFolder FolderID) const;
	const CString GetDefaultSuffix(CommonFolder FolderID) const;
};
