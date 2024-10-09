// InfoStock.cpp: implementation of the InfoStock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ncmProject.h"
#include "InfoStock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InfoStock::InfoStock() : RotBase(0., 0., 0., 1.)
{
	BasePoint = 0;
}

InfoStock::~InfoStock()
{

}

void InfoStock::Serialize(CArchive &ar)
{
	SerializeElements(ar,&BasePoint,1);

	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.8"))
	{
		double B[6];
		BOOL Def;
		SerializeElements(ar, B, 6);
		SerializeElements(ar, &Def, 1);
	}
	else
		SerializeElements(ar, &Gabar, 1);
//	Gabar.Serialize(ar);
	if(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.2"))
	{
		double pbuf;
		SerializeElements(ar, &pbuf, 1);
	}
	SerializeElements(ar,&RotBase,1);
	SerializeElements(ar,&StlName,1);
	SerializeElements(ar,&Type,1);
}
