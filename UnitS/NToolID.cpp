#include "ntoolid.h"


// String format : <tool num > TOOL_ID_DELIM <turret num>
NToolID::NToolID(void) : CString()
{
}

NToolID::~NToolID(void)
{
}

int NToolID::GetTurretNum(void) const
{
	int i = Find(TOOL_ID_DELIM);
	if(i < 0)
		return 0;
	int res = atoi(Right(GetLength() - i - 1));
	return res;
}

int NToolID::GetToolNum(void) const
{
	return atoi(GetToolStr());
}

const CString NToolID::GetToolStr(void) const
{
	int i = Find(TOOL_ID_DELIM);
	if(i < 0)
		return *this;
	return Left(i);
}
const CString NToolID::GetPrintStr(void) const
{
	int i = Find(TOOL_ID_DELIM);
	if(i < 0)
		return *this;

	return Right(GetLength() - i - 1) + CString(TOOL_PRINT_DELIM) + Left(i);
}

void NToolID::Set( int ToolNum, int TurretNum) noexcept
{
	if(TurretNum == 0)
		Format(_T("%d"), ToolNum);
	else
		Format(_T("%d") + CString(TOOL_ID_DELIM) + _T("%d"), ToolNum, TurretNum); 
}

const NToolID & NToolID::CrFromPrintStr(const CString &PrintStr)
{
	int i = PrintStr.Find(TOOL_PRINT_DELIM);
	if(i < 0)
		Set(atoi(PrintStr), 0);
	else
		Set(atoi(PrintStr.Right(PrintStr.GetLength() - i - 1)),
			atoi(PrintStr.Left(i)));
	return *this;
}
