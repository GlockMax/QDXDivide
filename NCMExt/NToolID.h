#pragma once

const CHAR TOOL_ID_DELIM = '\n';
const CHAR TOOL_PRINT_DELIM = '_';

class NToolID :
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
	const NToolID & CrFromPrintStr(const CString &PrintStr);

};
