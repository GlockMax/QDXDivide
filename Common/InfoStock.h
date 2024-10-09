// InfoStock.h: interface for the InfoStock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOSTOCK_H__1EFCFA13_C87C_11D5_B0C2_00403328DC2D__INCLUDED_)
#define AFX_INFOSTOCK_H__1EFCFA13_C87C_11D5_B0C2_00403328DC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBox.h"
#include "BPoint.h"

class InfoStock  
{
public:
	CStringA Type;
	CStringA StlName;
	BBox Gabar;
	BPoint RotBase;
	int BasePoint;
public:
	void Serialize(CArchive &ar);
	InfoStock();
	virtual ~InfoStock();

};

#endif // !defined(AFX_INFOSTOCK_H__1EFCFA13_C87C_11D5_B0C2_00403328DC2D__INCLUDED_)
