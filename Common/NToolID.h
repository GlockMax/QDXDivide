#pragma once
#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components

const CHAR TOOL_ID_DELIM = '\n';
const CHAR TOOL_PRINT_DELIM = '_';

class AFX_EXT_CLASS NToolID :
	public CString
{
public:
	const static CHAR TOOL_ID_DELIM = '\n';
	const static CHAR TOOL_PRINT_DELIM = '_';

	NToolID(void);
	NToolID( const NToolID& strSrc ) :
		CString( strSrc )
	{
	}
	NToolID( const CString& strSrc ) :
		CString( strSrc )
	{
	}
	~NToolID(void);
	int GetTurretNum(void) const;
	int GetToolNum(void) const;
	const CString GetToolStr(void) const;
	const CString GetPrintStr(void) const;
	void Set( int ToolNum, int TurretNum) noexcept;
	const NToolID & CrFromPrintStr( const CString &PrintStr);

};
