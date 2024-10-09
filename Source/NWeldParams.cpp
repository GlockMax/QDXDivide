#include "stdafx.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "SCoordsDef.h"
#include "NProgramWeld.h"
#include "NWeldParams.h"

NWeldParams::NWeldParams(void)
{
	Clear();
	pUnit = NULL;
}

NWeldParams::~NWeldParams(void)
{
}

double *NWeldParams::GetParPtr(int Ind)
{
	return Values + Ind;
}

void NWeldParams::EnsureActual()
{
	if(Actual)
		return;
	if(!pUnit)
		return;
	NProgram *pProg = pUnit->GetCurRunProg();
	if(!pProg)
		return;
	if(!pProg->IsKindOf(RUNTIME_CLASS(NProgramWeld)))
		return;
	Actual = true;
	NProgramWeld *pProgW = (NProgramWeld *)pProg;
	const NWeldTrajectories &Traj = pProgW->GetTrajectories();
	int AbsCadrNum = pUnit->GetState()->GetCurHCadr();
	Traj.GetCadrLenghs(AbsCadrNum, Values[0], Values[1], Values[2], Values[18]);
	// Machine parameters
	MTMachineTool &Mach = pUnit->GetDoc()->GetMachTool();
	SCoordsDef CDef(0);
	SMachState MStateWork(NULL);
	Mach.GetState(MStateWork, CDef);
	SMachState MState(NULL);
//	Mach.GetRealState(MState, CDef);
	for(int i = 0; i < MState.GetSize() - 1; ++i)
	{
		CString CoordName = MState.GetAName(i);
		if(CoordName.GetLength() != 1)
			continue;
		switch(CoordName[0])
		{
		case 'X':
			Values[ 6] = MState.GetCoord(i);
			break;
		case 'Y':
			Values[ 7] = MState.GetCoord(i);
			break;
		case 'Z':
			Values[ 8] = MState.GetCoord(i);
			break;
		case 'A':
			Values[ 9] = MState.GetCoord(i);
			break;
		case 'B':
			Values[10] = MState.GetCoord(i);
			break;
		case 'C':
			Values[11] = MState.GetCoord(i);
			break;
		}
	}
	// Calculate velocities
	double Time = pUnit->GetState()->CadrTime;
	if(Time <= 0.)
	{
		for(int k = 12; k < 18; ++k)
			Values[k] = 0.;
		Values[3] = 0.;
		Values[4] = 0.;
		Values[5] = 0.;
		return;
	}
	Values[3] = Values[0] * 60. / Time;
	Values[4] = Values[1] * 60. / Time;
	Values[5] = Values[2] * 60. / Time;
//	Mach.ProcCadrStep(*pUnit->GetSt(), true);
	SCoordsDef OldDef(0);
	SMachState OldState(NULL);
//	Mach.GetRealState(OldState, OldDef); // Set previous state
	for(int i = 0; i < OldState.GetSize() - 1; ++i)
	{
		CString CoordName = OldState.GetAName(i);
		if(CoordName.GetLength() != 1)
			continue;
		switch(CoordName[0])
		{
		case 'X':
			Values[12] = OldState.GetCoord(i);
			break;
		case 'Y':
			Values[13] = OldState.GetCoord(i);
			break;
		case 'Z':
			Values[14] = OldState.GetCoord(i);
			break;
		case 'A':
			Values[15] = OldState.GetCoord(i);
			break;
		case 'B':
			Values[16] = OldState.GetCoord(i);
			break;
		case 'C':
			Values[17] = OldState.GetCoord(i);
			break;
		}
	}
	for(int k = 12; k < 18; ++k)
		Values[k] = (Values[k - 6] - Values[k]) * 60. / Time;

	//Mach.SetStateWORecalc(pUnit->GetState()->MachConfNum, MStateWork); // Restore original state

}

void NWeldParams::Define(class NCUnit *pUn)
{
	pUnit = pUn;
	Clear();
}


void NWeldParams::Clear(void)
{
	for(int i = 0; i < PARAMS_NUM; ++i)
		Values[i] = 0.;
	Actual = false;
}
