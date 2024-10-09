#include "stdafx.h"
#include "BMatr.h"
#include "math.h"
#include "tbladeedge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

TBladeEdge::TBladeEdge(void)
: CircCenter(0.,0.,0.,-1.), Attrib(BE_NOTCUT)
{
}

TBladeEdge::~TBladeEdge(void)
{
}

void TBladeEdge::InsertAfter(TBladeEdge * pEdge)
{
	if(!pEdge)
		return;
	pEdge->pNext = pNext;
	pEdge->pPrev = this;
	pNext->pPrev = pEdge;
	pNext = pEdge;
}
BPoint TBladeEdge::GetVec() const
{
	return pNext->Pb - Pb;
}
void TBladeEdge::Trans(const BMatr &M)
{
	Pb = Pb * M;
	CircCenter = CircCenter * M;
}
const BPoint &TBladeEdge::GetPoint() const
{
	return Pb;
}
void TBladeEdge::SetPoint(const BPoint &P)
{
	Pb = P;
}
void TBladeEdge::SetPointH(double h)
{
	Pb.SetH(h);
}
const BPoint &TBladeEdge::GetCircCenter() const
{
	return CircCenter;
}
void TBladeEdge::SetCircCenter(const BPoint &P)
{
	CircCenter = P;
}
double TBladeEdge::GetRad() const
{
	return sqrt((Pb - CircCenter).D2());
}

int TBladeEdge::InsertTangRad(double Rad)
{// Dosn't check anything
	if(Rad == 0.)
		return 0;
	BPoint e0(GetPrev()->Pb - Pb); e0 = e0 * (1. / sqrt(e0.D2()));
	BPoint e1(GetNext()->Pb - Pb); e1 = e1 * (1. / sqrt(e1.D2()));

	BPoint e1s(e1 * BMatr().RotZ(BPoint(0.,0.,0.,1.), 90.)); 

	double Lambda = Rad / (e0 * e1s);

	CircCenter = (e0 + e1) * Lambda;

	TBladeEdge *pNewEdge = new TBladeEdge;
	pNewEdge->Pb = Pb + e1 * (CircCenter * e1);

	BPoint P0 = Pb + e0 * (CircCenter * e0);

	CircCenter = Pb + CircCenter;

	Pb = P0;

	InsertAfter(pNewEdge);
	
	return 0;
}


void TBladeEdge::Serialize(CArchive & ar)
{
	SerializeElements(ar, &Attrib, 1);
	SerializeElements(ar, &Pb, 1);
	SerializeElements(ar, &CircCenter, 1);
}
