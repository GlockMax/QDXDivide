// NToolTurn.cpp: implementation of the NToolTurn class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NToolTurn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolTurn, NTool, 0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NToolTurn::NToolTurn()
	: NTool(), Displacement(0.,0.,0.,0.), MainAngle(0.)
{
	SetType(50);// Turn tool
	Zdisp = 0.;
}
NToolTurn::NToolTurn(const CString &name)
	: NTool(name), Displacement(0.,0.,0.,0.), MainAngle(0.)
{
	SetType(50);// Turn tool
	Zdisp = 0.;
}

NToolTurn::~NToolTurn()
{

}

void NToolTurn::Draw(RenderMode GlobRMode, int step)
{
	if(!IsShapeVisible())
		return;
}
void NToolTurn::FillPage(CDialog *pPage)
{
	NTool::FillPage(pPage);
}

void NToolTurn::FillFromPage(CDialog *pPage)
{
	NTool::FillFromPage(pPage);
}
// создание копии инструмента
NToolTurn* NToolTurn::CreateCopyTool()
{
	return NULL;
}
void NToolTurn::CreateCopyTo(NToolTurn *pCreatedTool)
{
	NTool::CreateCopyTo(pCreatedTool);
	pCreatedTool->Displacement = Displacement;
	pCreatedTool->MainAngle = MainAngle;
}
void NToolTurn::Serialize(CArchive &ar)
{
	NTool::Serialize(ar);
	SerializeElements(ar, &Displacement, 1);
	SerializeElements(ar, &MainAngle, 1);
}

void NToolTurn::SetDisplacement(const BPoint & RP, double MAngle)
{
	Displacement = RP;
	MainAngle = MAngle;
}

const BPoint & NToolTurn::GetDisplacement(void)
{
	return Displacement;
}
double NToolTurn::GetMainAngle(void)
{
	return MainAngle;
}

BMatr NToolTurn::GetPosMatr() const
{
	// Сначала поворот на -MainAngle, затем смещение
	// все делается в системе координат токарного инструмента, поэтому компаненты Displacement переставлены
	return BMatr().RotY(0., 0., 0., MainAngle) * BMatr().Trans(-Displacement.GetY(), 0, -Displacement.GetX());
}
