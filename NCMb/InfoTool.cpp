// InfoTool.cpp: implementation of the InfoTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InfoTool::InfoTool()
{
	Clear();
}

InfoTool::~InfoTool()
{

}

void InfoTool::Serialize(CArchive &ar)
{
	SerializeElements(ar,&Time,1);
	SerializeElements(ar,&FastLength,1);
	SerializeElements(ar,&WorkLength,1);
	SerializeElements(ar,&CutVolume,1);
	SerializeElements(ar,&Pos,1);
	SerializeElements(ar,&Name,1);
	SerializeElements(ar,&Type,1);
}

void InfoTool::Clear()
{
	Time = 0.;
	FastLength = 0.;
	WorkLength = 0.;
	FastTime = 0.;
	WorkTime = 0.;
	CutVolume = 0.;
	MechEnergy = 0.;
	Pos = 0;
	Name = "Empty";
	Type = "Undef";
}
