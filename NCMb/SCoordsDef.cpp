#include "stdafx.h"
#include "MTCoordinate.h"
#include "SMachState.h"
#include "SCoordsDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CString SCoordsDef::DummyName;

SCoordsDef::SCoordsDef(const SCoordsDef& In)
{
	Names.Append(In.Names);
}

SCoordsDef::SCoordsDef(int iSize)
{
//	InstancesCount = 0;
	Define(iSize);
}
bool SCoordsDef::Define(int iSize)
{
	Names.SetSize(iSize, 1);
	return true;
}

SCoordsDef::SCoordsDef(const CString &InitStr)
{
//	InstancesCount = 0;
	Define(InitStr);
}
bool SCoordsDef::Define(const CString &InitStr)
{
	//ASSERT(InstancesCount <= 0);
	//if(InstancesCount > 0)
	//	return false;
	int CurIndex = 0;
	Names.SetSize(0, 1);
	while(CurIndex >= 0)
	{
		CString Name = InitStr.Tokenize(_T("\n"), CurIndex);
		if(CurIndex >= 0)
			Names.Add(Name);
	}
	return true;
}
bool SCoordsDef::Define(const CooArray &InitCoo)
{
	//ASSERT(InstancesCount <= 0);
	//if(InstancesCount > 0)
	//	return false;
	int CurIndex = 0;
	Names.SetSize(InitCoo.GetSize(), 1);
	for(int i = 0; i < Names.GetSize(); ++i)
	{
		Names[i] = InitCoo.GetAt(i).AxisName;
	}
	return true;
}

SCoordsDef::~SCoordsDef(void)
{
	//ASSERT(InstancesCount == 0);
}

const SCoordsDef& SCoordsDef::operator=(const SCoordsDef& In)
{
	Names.Copy(In.Names);
	return *this;
}

SMachState *SCoordsDef::GenNewState(void)
{
	return new SMachState(this);
}

int SCoordsDef::GetInd(const CString &Name) const
{
	for(int i = 0; i < GetSize(); ++i)
	{
		if(Name == GetName(i))
			return i;
	}
	return -1;
}

