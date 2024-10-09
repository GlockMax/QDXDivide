// NPtrArray.cpp: implementation of the NPtrArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCMData.h"
#include "InfoProgram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCMData::NPtrArray::NPtrArray()
{

}

NCMData::NPtrArray::~NPtrArray()
{
}

void NCMData::NPtrArray::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
#ifdef _WIN64
		ar<<(int)GetSize();
#else
		ar<<GetSize();
#endif
	else
	{
		int i;
		ar>>i;
		SetSize(i);
	}
}

void NCMData::NPtrArray::CopyAndAdd(const void *p, const int size)
{
	void *pIntern = new BYTE(size);
	memcpy(pIntern,p,size);
	Add(pIntern);
}
