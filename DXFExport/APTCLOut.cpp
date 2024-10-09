// APTCLOut.cpp: implementation of the APTCLOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "reslocal.h"
#include "APTCLOut.h"
#include "AptCLFile.h"
#include "NCUnitState.h"
#include "NCadrGeom.h"
#include "curve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APTCLOut::APTCLOut(const CString &ProgName, const CString &FileName)
{
	pPrevState = NULL;
	pCurrState = NULL;
	pOutFile = new AptCLFile;
	if(!pOutFile->Create(FileName))
	{
		AfxMessageBox(IDS_FILEOPEN_ERR);
		pOutFile = NULL;
		return;
	}
	PName = ProgName;
}

APTCLOut::~APTCLOut()
{
	if(pPrevState)
		delete pPrevState;
	if(pCurrState)
		delete pCurrState;
	if(pOutFile)
	{
		pOutFile->Close();
		delete pOutFile;
	}
}

void APTCLOut::Init(const NCUnitState *pInitState)
{
	pPrevState = new NCUnitState(*pInitState);
}

void APTCLOut::PostState(const NCUnitState *pState, const NCadrGeom &Geom)
{
	if(!pPrevState)
		return; // Init was not called
	if(!pOutFile)
		return; // File was not opened

	pCurrState = new NCUnitState(*pState);
//Feed
	if(pCurrState->Feed != pPrevState->Feed)
		pOutFile->Feed(int(pCurrState->Feed));
// End feed

// Diameter compensation
	if(pCurrState->CurDirComp != pPrevState->CurDirComp)
	{
		if(pCurrState->CurDirComp == LR_OFF)
			pOutFile->CutComOf();
		else
			pOutFile->CutComOn(pCurrState->CurDirComp == LEFT ,
				pCurrState->CurToolN.GetToolNum());// ???
	}

// End diameter compensation

// Coolant
	if(pCurrState->Coolant != pPrevState->Coolant)
	{
		if(pCurrState->Coolant == COOLOFF)
			pOutFile->CoolantOff();
		else
		{
			int n = 0;
			switch(pCurrState->Coolant)
			{
			case FLOOD:
				n = 1;
				break;
			case MIST:
				n = 2;
				break;
			}
			pOutFile->Coolant(n);
		}
	}

// End coolant

// Subroutine
	if(pCurrState->OrderChanged)
	{
	}
// End subroutine

// Cycles
	if(pCurrState->CurCycle != pPrevState->CurCycle)
	{
		if(pCurrState->CurCycle == CYCLE_OFF)
			pOutFile->CycleOff();
		else
		{
			int type = 0; // Cycle type
			switch(pCurrState->CurCycle)
			{
			case DRILL:
			case DRILL_OLD:
			case DRILL_X:
			case DRILL_Y:
			case DRILL_HM:
			case DRILL_MP81:
			case DRILL_HMC:
			case DRILL_HN:
			case DRILLDWELL:
			case DRILLPECK:
			case LAS:
				type = 0;
				break;
			case RHTAPPINGX:
			case RHTAPPINGZ:
			case RHTAPPINGPLANE:
			case TAPPING:
			case LEFTTAPPING:
			case NUMERICON:
			case RIGHTRHTAPPING:
				type = 2;
				break;
			case BORE:
			case BACKBORE:
			case BOREDWELL:
				type = 3;
				break;
			case REAMDWELL:
			case REAM:
				type = 4;
				break;
			}
			pOutFile->CycleOn(type,
				atof(pCurrState->R1) - atof(pCurrState->CycleZ),//???
				int(pCurrState->Feed),0,//???
				0.,//???
				atof(pCurrState->R2) - atof(pCurrState->R1));//???
		}
	}
	else
		if(pCurrState->CurCycle != CYCLE_OFF) // CYCLE is active but was not activated in this cadr
		{
			pOutFile->Line(pCurrState->MashX,pCurrState->MashY,pCurrState->MashZ);// Только для CycleNeedsGeom = Yes
		}

// End cycles

// Spindle on
	{
		const RotDir &CurSpDir = pCurrState->Spindle[SPINDLE_MILL].SpindleDir;
		const RotDir &PrevSpDir = pPrevState->Spindle[SPINDLE_MILL].SpindleDir;
		if(CurSpDir != PrevSpDir)
		{
			if(PrevSpDir == R_UNDEF)
				pOutFile->Spindl(CurSpDir == R_CW, int(pCurrState->Spindle[SPINDLE_MILL].Speed));
		}
	}
	{
		const RotDir &CurSpDir = pCurrState->Spindle[SPINDLE_TURN].SpindleDir;
		const RotDir &PrevSpDir = pPrevState->Spindle[SPINDLE_TURN].SpindleDir;
		if(CurSpDir != PrevSpDir)
		{
			if(PrevSpDir == R_UNDEF)
				pOutFile->Spindl(CurSpDir == R_CW, int(pCurrState->Spindle[SPINDLE_TURN].Speed));
		}
	}
// End spindle on

// Coord 3X
	if(pCurrState->Geom.HaveGeom() && pCurrState->CurCycle == CYCLE_OFF)
	{
		switch(pCurrState->CurCurveType)
		{
		case fast:
			if(pCurrState->CurCycle == CYCLE_OFF)
				// The word FAST may not appear if cycle is active 
				pOutFile->Fast(pCurrState->MashX,pCurrState->MashY,pCurrState->MashZ);
			else
				pOutFile->Line(pCurrState->MashX,pCurrState->MashY,pCurrState->MashZ);
			break;
		case line:
			pOutFile->Line(pCurrState->MashX,pCurrState->MashY,pCurrState->MashZ);
			break;
		case ccwarc:
		case cwarc:
			{
				double i,j,k;
				Geom.GetI(&i,&j,&k);
				pOutFile->Arc(pCurrState->CurCurveType == cwarc,
					pPrevState->MashX,pPrevState->MashY,pPrevState->MashZ,
					pCurrState->MashX,pCurrState->MashY,pCurrState->MashZ,
					i,j,k,
					0,0,1);// ???
				break;
			}
		}
	}
// End coord 3X

// Tool change
	if(pCurrState->CurToolN != pPrevState->CurToolN)
		pOutFile->LoadTl(pCurrState->CurToolN.GetToolNum());
// End tool change

// Spindle off
	{
		const RotDir &CurSpDir = pCurrState->Spindle[SPINDLE_MILL].SpindleDir;
		const RotDir &PrevSpDir = pPrevState->Spindle[SPINDLE_MILL].SpindleDir;
		if(CurSpDir != PrevSpDir)
		{
			if(CurSpDir == R_UNDEF)
				pOutFile->SpindlOff();
		}
	}
	{
		const RotDir &CurSpDir = pCurrState->Spindle[SPINDLE_TURN].SpindleDir;
		const RotDir &PrevSpDir = pPrevState->Spindle[SPINDLE_TURN].SpindleDir;
		if(CurSpDir != PrevSpDir)
		{
			if(CurSpDir == R_UNDEF)
				pOutFile->SpindlOff();
		}
	}
// End spindle off

	delete pPrevState;
	pPrevState = pCurrState;
	pCurrState = NULL;
}
