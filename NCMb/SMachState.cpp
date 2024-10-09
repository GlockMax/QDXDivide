#include "stdafx.h"
#include "ResInv.h"
#include "MTCoordinate.h"
#include "SCoordsDef.h"
#include "BPoint.h"
#include "SMachStateFix.h"
#include "SMachState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

SMachState::SMachState(const SCoordsDef *pDef)
{
	pCoordsDef = pDef;
	//if(pCoordsDef)
	//	++pCoordsDef->InstancesCount;
	Coords.resize(GetSize());
	ClearValues();
}

SMachState::SMachState(const BPoint& XYZ, const BPoint& ABC, const SCoordsDef* pDef)
{
	if (pDef == nullptr)
		return;
	if (pDef->GetSize() != 6)
		return;
	pCoordsDef = pDef;
	//if (pCoordsDef)
	//	++pCoordsDef->InstancesCount;
	Coords.resize(GetSize());
	Coords[0] = XYZ.GetX();
	Coords[1] = XYZ.GetY();
	Coords[2] = XYZ.GetZ();
	Coords[3] = ABC.GetX();
	Coords[4] = ABC.GetY();
	Coords[5] = ABC.GetZ();
	Coords[GetSize() - 1] = 1.;
}

SMachState::SMachState(const SMachStateFix1& FixState, const SCoordsDef* pDef)
{
	pCoordsDef = pDef;
	Coords.resize(GetSize());
	for (int i = 0; i < GetSize(); ++i)
		Coords[i] = FixState.GetCoord(i);
}

SMachState::SMachState(const SMachState &In)
{
	pCoordsDef = In.pCoordsDef;
	//if(pCoordsDef)
	//	++pCoordsDef->InstancesCount;
	Coords = In.Coords;
}

SMachState::~SMachState(void)
{
	//if(pCoordsDef)
	//	--pCoordsDef->InstancesCount;
}
const SMachState & SMachState::operator = (const SMachState &In)
{
	pCoordsDef = In.pCoordsDef;
	Coords = In.Coords;
	return *this;
}
int SMachState::GetSize() const
{
	return pCoordsDef == NULL ? 1 : pCoordsDef->GetSize() + 1;
}
double SMachState::GetCoord(const CString &Name) const
{
	// Returns the value of the coordinate with the given Name
	// Returns 0 if the coordinate with the given Name is not present
	int i;
	if((i = GetInd(Name)) >= 0)
		return Coords[i];
	return 0.;
}

void SMachState::SetCoord(const CString &Name, double Val)
{
	// Does nothing if the coordinate with the given Name is not present
	int i;
	if((i = GetInd(Name)) >= 0)
		Coords[i] = Val;
}
void  SMachState::SetCoord(int i, double Val)
{
	if(i < 0 || i >= GetSize())
		return;
	Coords[i] = Val; 
}
void SMachState::AddXYZ(const BPoint& XYZ)
{
	Coords[0] += XYZ.GetX();
	Coords[1] += XYZ.GetY();
	Coords[2] += XYZ.GetZ();
}
int SMachState::GetInd(const CString &Name) const
{
	if(pCoordsDef)
		return pCoordsDef->GetInd(Name);
	else
		return -1;
}

const CString &SMachState::GetAName(int i) const
{
	if(pCoordsDef)
		return pCoordsDef->GetName(i);
	else
		return SCoordsDef::DummyName;
}
SMachState SMachState::operator + (const SMachState &P) const
{
	SMachState Ret(pCoordsDef);
	for(int i = 0; i < GetSize(); ++i)
		Ret.Coords[i] = Coords[i] + P.Coords[i];
	return Ret;
}
SMachState SMachState::operator - (const SMachState &P) const
{
	SMachState Ret(pCoordsDef);
	for(int i = 0; i < GetSize(); ++i)
		Ret.Coords[i] = Coords[i] - P.Coords[i];
	return Ret;
}
SMachState SMachState::operator * (double a) const
{
	SMachState Ret(pCoordsDef);
	for(int i = 0; i < GetSize(); ++i)
		Ret.Coords[i] = Coords[i] * a;
	return Ret;
}
SMachState operator * (const double a, const SMachState &P)
{
	return P * a;
}

bool SMachState::GetCooArray(CooArray &Array) const
{
	Array.SetSize(GetSize() - 1);
	for(int i = 0; i < GetSize() - 1; ++i)
		Array[i].Set(GetAName(i), GetCoord(i));

	return true;
}
bool SMachState::Merge2CooArray(CooArray& Array, const SMachState& State)
{
	bool Res = true; // false if State contains "existing" name
	auto InitialSize = Array.GetSize();
	for (int i = 0; i < State.GetSize() - 1; ++i)
	{
		int ind = -1;
		for (int k = 0; k < InitialSize; ++k)// it is assumed that State contains unique names
		{
			if (State.GetAName(i) == Array[k].AxisName)
			{
				ind = k;
				Res = false;
				break;
			}
		}
		if (ind < 0)
			Array.Add(MTCoordinate(State.GetAName(i), State.GetCoord(i)));
		else
			Array[ind].Value = State.GetCoord(i);
	}
	return Res;
}
bool SMachState::IsVector() const
{ 
	return Coords[GetSize() - 1] == 0.;
}

bool SMachState::IsZero() const
{ 
	for(int i = 0; i < GetSize(); ++i)
		if(Coords[i] != 0.)
			return false;
	return true;
}

void SMachState::ClearValues()
{
	for(int i = 0; i < GetSize() - 1; ++i)
		Coords[i] = 0.;
	Coords[GetSize() - 1] = 1.;
}

void SMachState::ReplaceCoord(const SMachState &In)
{
	// Replace values of the coordinates from In
	for(int i = 0; i < In.GetSize() - 1; ++i)
		SetCoord(In.GetAName(i), In.GetCoord(i));
}
const CString SMachState::MakeCadr(void)
{
	CString CadrStr;
	for(int i = 0; i < GetSize() - 1; ++i)
	{
		CString Val;
		Val.Format(IDS_TILT_FORMAT, GetCoord(i));
		CadrStr = CadrStr + GetAName(i) + Val;
	}
	return CadrStr;
}
