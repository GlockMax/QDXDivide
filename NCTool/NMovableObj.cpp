// NMovableObj.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "BMatr.h"
#include "NMovableObj.h"


// NMovableObj

NMovableObj::NMovableObj()
{
	Moving = false;
	pos_X = RP_ZER;
	pos_Y = RP_ZER;
	pos_Z = RP_ZER;
	bas_X = 0.;
	bas_Y = 0.;
	bas_Z = 0.;
}

NMovableObj::~NMovableObj()
{
}


// NMovableObj member functions
void NMovableObj::Serialize(CArchive &ar)
{
	NPassiveObject::Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.6")))
	{
		SerializeElements(ar, &Matr, 1);
		SerializeElements(ar, &pos_X, 1);
		SerializeElements(ar, &pos_Y, 1);
		SerializeElements(ar, &pos_Z, 1);
	}
}