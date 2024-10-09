#include "stdafx.h"
#include "BIPolar.h"
#include "BICyl.h"
#include "bspacepos.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CArray< BMatr, const BMatr &> BSpacePos::RotMatrs;
CArray< BPoint, const BPoint &> BSpacePos::ABCs;
CArray< BIPolar, const BIPolar &> BSpacePos::Polars;
CArray< BICyl, const BICyl &> BSpacePos::Cyls;
bool BSpacePos::LastMatrUsed = true;// ‘лаг сбрасываетс€ при добавлении новой матрицы и устанавливаетс€ при ее первом использовании
int BSpacePos::LastTurret = 0;
BMatr BSpacePos::LastCSMatr;
BPoint BSpacePos::LastABC(0., 0., 0., 1.);
class MTMachineTool *BSpacePos::pLastMT = NULL;

int BSpacePos::GetLastMatrInd(void)
{
	LastMatrUsed = true;
	return int(RotMatrs.GetSize()) - 1;
}
int BSpacePos::GetLastPolarInd(void)
{
	return int(Polars.GetSize()) - 1;
}
int BSpacePos::GetLastCylInd(void)
{
	return int(Cyls.GetSize()) - 1;
}
int BSpacePos::AddRotMatr(const BMatr & M, const BPoint & ABC)
{
	ABCs.Add(ABC);
	return int(RotMatrs.Add(M));
}
int BSpacePos::AddPolar(const BIPolar & Interp)
{
	return int(Polars.Add(Interp));
}
int BSpacePos::AddCyl(const BICyl & Interp)
{
	return int(Cyls.Add(Interp));
}

const BIPolar & BSpacePos::GetPolar(int i)
{
	return Polars.GetAt(i);
}

const BICyl & BSpacePos::GetCyl(int i)
{
	return Cyls.GetAt(i);
}

const BMatr & BSpacePos::GetRotMatr(int i)
{
	return RotMatrs.GetAt(i);
}
const BMatr *BSpacePos::GetRotMatrs()
{
	return RotMatrs.GetData();
}
const BPoint BSpacePos::GetABC(int i)
{
	return ABCs.GetAt(i);
}

void BSpacePos::ClearRotMatrs(void)
{
	Clear();
	RotMatrs.SetSize(1,32);
	RotMatrs[0] = BMatr();
	ABCs.SetSize(1,32);
	ABCs[0] = BPoint(0., 0., 0., 1.);
}

void BSpacePos::Clear(void)
{
	RotMatrs.RemoveAll();
	ABCs.RemoveAll();
	Polars.RemoveAll();
	Cyls.RemoveAll();
}

INT_PTR BSpacePos::GetMatrsSize(void)
{ 
	return RotMatrs.GetSize();
}
