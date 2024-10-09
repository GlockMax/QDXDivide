// NControlPanel.cpp: implementation of the NStateElem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "math.h"
#include "NControlPanel.h"
#include "NCMDoc.h"
#include "NCUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateElem::NStateElem(NStateNote * pN)
{
	ASSERT(pN);
	pNote = pN;
}

NStateElem::~NStateElem()
{
	ASSERT(pNote);
	delete pNote;
}

//////////////////////////////////////////////////////////////////////
// NStateNote Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateNote::NStateNote()
{

}

NStateNote::~NStateNote()
{

}

//////////////////////////////////////////////////////////////////////
// NStateDouble Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


NStateDouble::NStateDouble(double * pD, NStateNote *pN)
: NStateElem(pN)
{
	pValue = pD;
}

NStateDouble::~NStateDouble()
{
}

CString NStateDouble::GetValueString()
{
	if(!pValue)
		return _T("");
	if(fabs(*pValue) < MINDPAR)
	{
		*pValue = 0.;
		return CString("0.");
	}
	// round to MINDPAR
	double b = *pValue;
	b /= MINDPAR;
	b += 0.5;
	b = floor(b);
	b *= MINDPAR;
	if(fabs(b) >= 0.1 || b == 0.)
	{
		char buffer[_CVTBUFSIZE];
		_gcvt( b, 10, buffer );
		return CString(buffer);
	}
	else 
	{
		CString s;
		s.Format("%8.5lf", b);
		while(s.Right(1) == '0')
			s.Delete(s.GetLength() - 1);
		return s;
	}
}

//////////////////////////////////////////////////////////////////////
// NStateCoordNote Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateBoolNote::NStateBoolNote(BOOL * pB, UINT idTrueString, UINT idFalseString)
{
//	ASSERT(pB);
	pValue = pB;
	TrueString.LoadString(idTrueString);
	FalseString.LoadString(idFalseString);
}

NStateBoolNote::~NStateBoolNote()
{

}

//////////////////////////////////////////////////////////////////////
// NStateNoNote Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateNoNote::NStateNoNote()
{

}

NStateNoNote::~NStateNoNote()
{

}

CString NStateNoNote::GetString()
{
	CString s = " ";
	return s;
}

CString NStateBoolNote::GetString()
{
	if(!pValue)
		return _T("");
	return *pValue ? TrueString : FalseString;
}

//////////////////////////////////////////////////////////////////////
// NStateString Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateString::NStateString(const CString &S, NStateNote * pN)
: NStateElem(pN), String(S)
{
}

NStateString::~NStateString()
{
}

CString NStateString::GetValueString()
{
	return String;
}

//////////////////////////////////////////////////////////////////////
// NStateInt Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateInt::NStateInt(int * pInt, NStateNote * pN)
: NStateElem(pN)
{
	ASSERT(pInt);
	pValue = pInt;
}

NStateInt::~NStateInt()
{
}

CString NStateInt::GetValueString()
{
	CString s;
	s.Format("%d", *pValue);
	return s;

}

//////////////////////////////////////////////////////////////////////
// NStateTime Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateTime::NStateTime(double * pD, NStateNote * pN)
: NStateElem(pN)
{
	ASSERT(pD);
	pValue = pD;
}

NStateTime::~NStateTime()
{

}

CString NStateElem::GetNoteString()
{
	return pNote->GetString();
}

NStateElem::NStateElem()
{
	pNote = NULL;
}

CString NStateTime::GetValueString()
{
	CTimeSpan t((unsigned long)fabs(*pValue));
	CString s(t.Format(":%M:%S"));
	CString h;
	h.Format("%d",t.GetTotalHours());
	if (*pValue < 0.)
		h = _T("-") + h;
	return h + s;
}
CString NStateTime::GetValue4Time()
{
	CString h;
	h.Format("%lf",*pValue);
	return h;
}

//////////////////////////////////////////////////////////////////////
// NStateBool Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateBool::NStateBool(BOOL * pB, UINT idTrueString, UINT idFalseString, NStateNote * pN)
: NStateElem(pN)
{
	ASSERT(pB);
	pValue = pB;
	TrueString.LoadString(idTrueString);
	FalseString.LoadString(idFalseString);
}

NStateBool::~NStateBool()
{

}

CString NStateBool::GetValueString()
{
	CString S;
	S = *pValue ? TrueString : FalseString;
	return S.SpanExcluding("\n");
}




NStatebool::NStatebool(bool* pB, UINT idTrueString, UINT idFalseString, NStateNote* pN)
	: NStateElem(pN)
{
	ASSERT(pB);
	pValue = pB;
	TrueString.LoadString(idTrueString);
	FalseString.LoadString(idFalseString);
}

NStatebool::~NStatebool()
{

}

CString NStatebool::GetValueString()
{
	CString S;
	S = *pValue ? TrueString : FalseString;
	return S.SpanExcluding("\n");
}

//////////////////////////////////////////////////////////////////////
// NStateCodeNote Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateCodeNote::NStateCodeNote(CString WKey, CString FKey)
{
	WordKey = WKey;
	FuncKey = FKey;
}

NStateCodeNote::~NStateCodeNote()
{

}

CString NStateCodeNote::GetString()
{
	CNCMDoc *pDoc = (CNCMDoc *)(((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument());
	ASSERT(pDoc);
	NCUnit * pUnit = pDoc->GetUnit();
	if(pUnit == NULL)
		return "";
	CString WordStr, FuncStr;
	pUnit->GetWordValue(WordKey, WordStr);
	if(!pUnit->GetFuncNum(FuncKey, FuncStr))
		pUnit->GetMiscFuncValue(FuncKey, FuncStr);
	if (FuncStr.IsEmpty())
		return _T("");
	return WordStr + FuncStr;
}

//////////////////////////////////////////////////////////////////////
// NStateLineSpeedNote Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateEnumNote::NStateEnumNote(UINT * pV)
{
	ASSERT(pV);
	pValue = pV;
}

NStateEnumNote::~NStateEnumNote()
{

}

CString NStateEnumNote::GetString()
{
	CString S;
	S.LoadString(*pValue);
	return S.SpanExcluding("\n");
}

//////////////////////////////////////////////////////////////////////
// NStatePercent Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStatePercent::NStatePercent(double * pD, NStateNote * pN)
: NStateElem(pN)
{
	ASSERT(pD);
	pValue = pD;
}

NStatePercent::~NStatePercent()
{

}

CString NStatePercent::GetValueString()
{
	CString S;
	S.Format("%d", (int)(*pValue * 100.));
	S += " %";
	return S;
}

//////////////////////////////////////////////////////////////////////
// NStatePercentNote Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStatePercentNote::NStatePercentNote(double * pV, double * pP)
{
	ASSERT(pV);
	ASSERT(pP);
	pValue = pV;
	pPercent = pP;
}

NStatePercentNote::~NStatePercentNote()
{

}

CString NStatePercentNote::GetString()
{
	char buffer[50];   
	_gcvt( (*pValue) * (*pPercent), 10, buffer );
	CString s(buffer);
	return s;
}

//////////////////////////////////////////////////////////////////////
// NStateEnum Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStateEnum::NStateEnum(UINT * pV, NStateNote * pN)
:  NStateElem(pN)
{
	ASSERT(pV);
	pValue = pV;
}

NStateEnum::~NStateEnum()
{

}

CString NStateEnum::GetValueString()
{
	CString S;
	S.LoadString(*pValue);
	return S.SpanExcluding("\n");
}

CString NStateEnum::MakeNoteString(CString & buf) const
{
	int i = buf.Find("\n");
	if (i<0) return "";
	buf.Delete(0, i + 1);

	CString WordKey = buf.SpanExcluding("\n");
	i = WordKey.GetLength();
	if (i == 0) return "";
	buf.Delete(0, i + 1);

	CString FuncKey = buf.SpanExcluding("\n");
	i = FuncKey.GetLength();
	if (i == 0) return "";
	CNCMDoc *pDoc = (CNCMDoc *)(((CFrameWnd *)AfxGetMainWnd())->GetActiveDocument());
	ASSERT(pDoc);
	NCUnit * pUnit = pDoc->GetUnit();
	if (pUnit == NULL)
		return "";
	CString WordStr, FuncStr;
	pUnit->GetWordValue(WordKey, WordStr);
	if (!pUnit->GetFuncNum(FuncKey, FuncStr))
		pUnit->GetMiscFuncValue(FuncKey, FuncStr);
	if (FuncStr.IsEmpty())
		return FuncStr;
	return WordStr + FuncStr;
}

CString NStateEnum::GetNoteString()
{
	CString buf;
	buf.LoadString(*pValue);
	return MakeNoteString(buf);
}

CString NStateBool::GetNoteString()
{
	CString WordKey, FuncKey, buf;
	buf = *pValue ? TrueString : FalseString;
	int i = buf.Find("\n");
	if (i < 0) return "";
	buf.Delete(0, i + 1);

	WordKey = buf.SpanExcluding("\n");
	i = WordKey.GetLength();
	if (i == 0) return "";
	buf.Delete(0, i + 1);

	FuncKey = buf.SpanExcluding("\n");
	i = FuncKey.GetLength();
	if (i == 0) return "";
	CNCMDoc* pDoc = (CNCMDoc*)(((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument());
	ASSERT(pDoc);
	NCUnit* pUnit = pDoc->GetUnit();
	if (pUnit == NULL)
		return "";
	CString WordStr, FuncStr;
	pUnit->GetWordValue(WordKey, WordStr);
	if (!pUnit->GetFuncNum(FuncKey, FuncStr))
		pUnit->GetMiscFuncValue(FuncKey, FuncStr);
	if (FuncStr.IsEmpty())
		return _T("");
	return WordStr + FuncStr;
}

CString NStatebool::GetNoteString()
{
	CString WordKey, FuncKey, buf;
	buf = *pValue ? TrueString : FalseString;
	int i = buf.Find("\n");
	if (i < 0) return "";
	buf.Delete(0, i + 1);

	WordKey = buf.SpanExcluding("\n");
	i = WordKey.GetLength();
	if (i == 0) return "";
	buf.Delete(0, i + 1);

	FuncKey = buf.SpanExcluding("\n");
	i = FuncKey.GetLength();
	if (i == 0) return "";
	CNCMDoc* pDoc = (CNCMDoc*)(((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument());
	ASSERT(pDoc);
	NCUnit* pUnit = pDoc->GetUnit();
	if (pUnit == NULL)
		return "";
	CString WordStr, FuncStr;
	pUnit->GetWordValue(WordKey, WordStr);
	if (!pUnit->GetFuncNum(FuncKey, FuncStr))
		pUnit->GetMiscFuncValue(FuncKey, FuncStr);
	if (FuncStr.IsEmpty())
		return _T("");
	return WordStr + FuncStr;
}

//////////////////////////////////////////////////////////////////////
// NStateDouble Class
//////////////////////////////////////////////////////////////////////


NStateAddCoord::NStateAddCoord(const class SMachState &iCoords, int iInd, NStateNote *pN)
: NStateDouble(NULL, pN)
, Coords(iCoords)
, Ind(iInd)
{
}

NStateAddCoord::~NStateAddCoord()
{
}

CString NStateAddCoord::GetValueString()
{
	if(Coords.GetSize() - 1 < Ind || Ind < 0)
		return _T("");
	double Buf = Coords.GetCoord(Ind);
	pValue = &Buf;
	CString Ret(NStateDouble::GetValueString());
	pValue = NULL;
	return Ret;
}
//////////////////////////////////////////////////////////////////////
// NStateStrNote Class
//////////////////////////////////////////////////////////////////////

NStateStrNote::NStateStrNote(const CString &Str) :
String(Str)
{
}
NStateStrNote::NStateStrNote(UINT idString)
{
	String.LoadString(idString);
}

NStateStrNote::~NStateStrNote()
{

}

CString NStateStrNote::GetString()
{
	return String;
}

NStateEnumUCS::NStateEnumUCS(UINT * pV, NStateNote * pN) : NStateEnum(pV, pN)
{
}

NStateEnumUCS::~NStateEnumUCS()
{
}

CString NStateEnumUCS::GetNoteString()
{
	CString F;
	F.LoadString(IDS_STATE_VALUE_UCSS);
	CString S;
	S.Format(F, *pValue + 1, *pValue + 1);
	return MakeNoteString(S);
}

CString NStateEnumUCS::GetValueString()
{
	CString S;
	S.LoadString(IDS_STATE_VALUE_UCSS);
	CString F = S.SpanExcluding("\n");
	S.Format(F, *pValue + 1);
	return S;
}

