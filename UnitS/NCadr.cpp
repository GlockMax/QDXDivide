#include "..\Common\NCadr.h"
// NCadr.cpp: implementation of the NCadr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCycle.h"
#include "NCadr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NCadr, CObject, 0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



NCadr::~NCadr()
{
	delete pCycle;
}

NCadr::NCadr(const CString& t)
{
	OrText = t;
	ReText.Empty();
	pCycle = nullptr;
	TextChanged = false;
	ErrCode = 0;
}


const CString& NCadr::GetOrText() const noexcept
{
	return OrText;
}

const CString& NCadr::GetReText() const noexcept
{

	return TextChanged ? ReText : OrText;
}

CString& NCadr::GetReText2Ch() noexcept
{
	return ReText;
}

CString& NCadr::GetTextMod() noexcept
{
	return OrText;
}

void NCadr::CheckReText() noexcept
{
	TextChanged = OrText != ReText;
	if (!TextChanged)
		ReText.Empty();
}

void NCadr::ClearReText() noexcept
{
	ReText.Empty();
	TextChanged = false;
}

void NCadr::SetOrText(const CString& str)
{
	ReText.Empty();
	OrText = str;
}

void NCadr::SetReText(const CString& str)
{
	ReText = str;
}

void NCadr::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar<<OrText;
	}
	else
	{
		ar>>OrText;
	}
}

NCadr::NCadr() noexcept
{
	pCycle = nullptr;
	TextChanged = false;
	ErrCode = 0;
}
