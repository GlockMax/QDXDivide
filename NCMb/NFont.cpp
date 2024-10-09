#include "..\Common\NFont.h"
// NFont.cpp: implementation of the NFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NFont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NFont::NFont()
{

}

NFont::~NFont()
{

}

bool NFont::Read(const CString &str)
{
	// str = "[size] [name]"
	int Size;
	CString Buf;
	sscanf(str, _T("%s"), Buf.GetBuffer(2));
	Buf.ReleaseBuffer();
	if(sscanf(LPCTSTR(Buf), _T("%d"), &Size) != 1)
		return false;
	FontSize = Size;
	FontName = str.Right(str.GetLength() - Buf.GetLength() - 1);
	return true;
}

CString NFont::Write()
{
	CString str;
	str.Format(_T("%d %s"), FontSize, FontName);
	return str;
}

void NFont::MakeCFont(CFont& ExtFont, const CDC& DC)
{
	::DeleteObject(ExtFont.Detach());

	LOGFONT lf;
	AFX_GLOBAL_DATA* pGlobData = GetGlobalData();
	pGlobData->fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	pGlobData->GetNonClientMetrics(info);

	lf.lfHeight = -MulDiv(FontSize, GetDeviceCaps(DC.m_hDC, LOGPIXELSY), 72);
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	memcpy_s(lf.lfFaceName, 31, FontName.GetBuffer(), FontName.GetLength());
	lf.lfFaceName[min(FontName.GetLength(), 31)] = '\x0';
	ExtFont.CreateFontIndirect(&lf);
}

void NFont::Set(const CString &Name, int Size)
{
	FontName = Name;
	FontSize = Size;
}

NFont & NFont::operator =(NFont &F)
{
	FontName = F.FontName;
	FontSize = F.FontSize;
	return *this;
}
