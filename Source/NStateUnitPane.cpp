#include "stdafx.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NCUnitState.h"
#include "NControlPanel.h"
#include "NSettingsDlg.h"
#include "NStateUnitPane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NStateUnitPane::NStateUnitPane(void)
{
	pUnitState = NULL;

	CString ElemName;
	NStateElem* pElem = nullptr;
	NStateNote* pNote = nullptr;
	// MashX
	(ElemName.LoadString(IDS_STATE_NAME_MashX));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&DynParams.DynX, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// MashY
	(ElemName.LoadString(IDS_STATE_NAME_MashY));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&DynParams.DynY, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// MashZ
	(ElemName.LoadString(IDS_STATE_NAME_MashZ));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&DynParams.DynZ, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// MashA
	(ElemName.LoadString(IDS_STATE_NAME_MashA));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&DynParams.DynA, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// MashB
	(ElemName.LoadString(IDS_STATE_NAME_MashB));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&DynParams.DynB, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// MashC
	(ElemName.LoadString(IDS_STATE_NAME_MashC));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&DynParams.DynC, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// Time
	(ElemName.LoadString(IDS_STATE_NAME_Time));
	pNote = new NStateNoNote;
	pElem = new NStateTime(&DynParams.Time, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));
	// CurToolN
	(ElemName.LoadString(IDS_STATE_NAME_CurToolN));
	pNote = new NStateNoNote;
	pElem = new NStateInt(&DynParams.ToolPosition, pNote);
	DynElemList.push_back(std::pair<CString, class NStateElem*>(ElemName, pElem));

}

NStateUnitPane::~NStateUnitPane(void)
{
	for each (auto p in DynElemList)
		delete p.second;
}

void NStateUnitPane::CreateStateView()
{
	lc.DeleteAllItems();
	// Update values
	auto VisElemCount = VisElemList.GetSize();
	if (VisElemCount == 0)
		return;
	DynElemInd.resize(DynElemList.size());
	for(int l = 0; l < DynElemInd.size(); ++l)
		DynElemInd.at(l) = -1;
	for (int i = 0, j = 0; i < VisElemCount; ++i)
	{
		CString ElemName = VisElemList[i];
		auto it = AllElemList.find(ElemName);
		if (it == AllElemList.end())
			continue;
		NStateElem* pElem = static_cast<NStateElem*>((void*)it->second);
		lc.InsertItem(j, ElemName);
		lc.SetItemData(j, 0);
		lc.SetItemText(j, 1, pElem->GetValueString());
		lc.SetItemText(j, 2, pElem->GetNoteString());
		for (int k = 0; k < DynElemList.size(); ++k)
		{
			if (DynElemList.at(k).first == ElemName)
				DynElemInd[k] = j;
		}
		++j;
	}
}

// Gets information from NCUnitState
void NStateUnitPane::ResetElements()
{
	DeleteElements();

	if(!pUnitState)
		return; 

	CString ElemName;
	NStateElem* pElem = nullptr;
	NStateNote* pNote = nullptr;

	// CurX
	(ElemName.LoadString(IDS_STATE_NAME_CurX));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurX, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurY
	(ElemName.LoadString(IDS_STATE_NAME_CurY));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurY, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurZ
	(ElemName.LoadString(IDS_STATE_NAME_CurZ));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurZ, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Time
	(ElemName.LoadString(IDS_STATE_NAME_Time));
	pNote = new NStateNoNote;
	pElem = new NStateTime(&pUnitState->Time, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Length
	(ElemName.LoadString(IDS_STATE_NAME_Length));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->Length, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Angle
	(ElemName.LoadString(IDS_STATE_NAME_Angle));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->Angle, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// AngleX
	(ElemName.LoadString(IDS_STATE_NAME_AngleX));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->AngleX, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// AngleY
	(ElemName.LoadString(IDS_STATE_NAME_AngleY));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->AngleY, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// AngleZ
	(ElemName.LoadString(IDS_STATE_NAME_AngleZ));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->AngleZ, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Volume
	//(ElemName.LoadString(IDS_STATE_NAME_Volume));
	//pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE_3, IDS_STATE_NOTE_INCH_3);
	//pElem = new NStateDouble(&pUnitState->Volume, pNote);
	//AllElemList[ElemName] = (CObject *&)pElem;
	// CadrTime
	(ElemName.LoadString(IDS_STATE_NAME_CadrTime));
	pNote = new NStateNoNote;
	pElem = new NStateTime(&pUnitState->CadrTime, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Life time
	(ElemName.LoadString(IDS_STATE_NAME_LIFETIME_REM));
	pNote = new NStateNoNote;
	pElem = new NStateTime(&pUnitState->RestLifeTime, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	(ElemName.LoadString(IDS_STATE_NAME_LIFETIME_CUR));
	pNote = new NStateNoNote;
	pElem = new NStateTime(&pUnitState->WorkedLifeTime, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;

	// Rc
	(ElemName.LoadString(IDS_STATE_NAME_Rc));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->Rc, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Ic
	(ElemName.LoadString(IDS_STATE_NAME_Ic));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->Ic, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Jc
	(ElemName.LoadString(IDS_STATE_NAME_Jc));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->Jc, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Kc
	(ElemName.LoadString(IDS_STATE_NAME_Kc));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->Kc, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CadrLength
	(ElemName.LoadString(IDS_STATE_NAME_CadrLength));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CadrLength, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// AbsoluteCoord
	(ElemName.LoadString(IDS_STATE_NAME_AbsoluteCoord));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateBool(&pUnitState->AbsoluteCoord, IDS_STATE_VALUE_ABSOLUTE_COORD, IDS_STATE_VALUE_RELATIVE_COORD, pNote);
	AllElemList[ElemName] = (CObject*&)pElem;
	// RCP
	(ElemName.LoadString(IDS_STATE_NAME_RCPMODE));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStatebool(&pUnitState->RCPActive, IDS_STATE_VALUE_RCPMODE_ON, IDS_STATE_VALUE_RCPMODE_OFF, pNote);
	AllElemList[ElemName] = (CObject*&)pElem;
	// Feed
	(ElemName.LoadString(IDS_STATE_NAME_Feed));
	pNote = new NStateEnumNote((UINT *)&pUnitState->FeedM);
	pElem = new NStateDouble(&pUnitState->Feed, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FeedScale
	(ElemName.LoadString(IDS_STATE_NAME_FeedScale));
//	pNote = new NStatePercentNote(&pUnitState->Feed, &pUnitState->FeedScale);
	pNote = new NStateNoNote;
	pElem = new NStatePercent(&pUnitState->FeedScale, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurPlane
	(ElemName.LoadString(IDS_STATE_NAME_CurPlane));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->CurPlane, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Coolant
	(ElemName.LoadString(IDS_STATE_NAME_Coolant));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->Coolant, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Speed
	(ElemName.LoadString(IDS_STATE_NAME_Speed));
	pNote = new NStateEnumNote((UINT *)&pUnitState->Spindle[SPINDLE_MILL].SpeedM);
	pElem = new NStateDouble(&pUnitState->Spindle[SPINDLE_MILL].Speed, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// SpindleDir
	(ElemName.LoadString(IDS_STATE_NAME_SpindleDir));
	pNote = new NStateNoNote;
	pElem = new NStateEnum((UINT *)&pUnitState->Spindle[SPINDLE_MILL].SpindleDir, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// SpeedTurn
	(ElemName.LoadString(IDS_STATE_NAME_SpeedTurn));
	pNote = new NStateEnumNote((UINT *)&pUnitState->Spindle[SPINDLE_TURN].SpeedM);
	pElem = new NStateDouble(&pUnitState->Spindle[SPINDLE_TURN].Speed, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// SpindleDirTurn
	(ElemName.LoadString(IDS_STATE_NAME_SpindleDirTurn));
	pNote = new NStateNoNote;
	pElem = new NStateEnum((UINT *)&pUnitState->Spindle[SPINDLE_TURN].SpindleDir, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurI
	(ElemName.LoadString(IDS_STATE_NAME_CurI));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurI, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurI*2
	(ElemName.LoadString(IDS_STATE_NAME_DiamI));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurI2, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurJ
	(ElemName.LoadString(IDS_STATE_NAME_CurJ));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurJ, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurK
	(ElemName.LoadString(IDS_STATE_NAME_CurK));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurK, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashX
	(ElemName.LoadString(IDS_STATE_NAME_MashX));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashX, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashX*2
	(ElemName.LoadString(IDS_STATE_NAME_DiamX));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashX2, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashY
	(ElemName.LoadString(IDS_STATE_NAME_MashY));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashY, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashZ
	(ElemName.LoadString(IDS_STATE_NAME_MashZ));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashZ, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashcXUCS
	(ElemName.LoadString(IDS_STATE_NAME_MashcXUCS));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashcXUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashcYUCS
	(ElemName.LoadString(IDS_STATE_NAME_MashcYUCS));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashcYUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashcZUCS
	(ElemName.LoadString(IDS_STATE_NAME_MashcZUCS));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashcZUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashXUCS
	(ElemName.LoadString(IDS_STATE_NAME_MashXUCS));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashXUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashYUCS
	(ElemName.LoadString(IDS_STATE_NAME_MashYUCS));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashYUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashZUCS
	(ElemName.LoadString(IDS_STATE_NAME_MashZUCS));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashZUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashA
	(ElemName.LoadString(IDS_STATE_NAME_MashA));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->MashA, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashB
	(ElemName.LoadString(IDS_STATE_NAME_MashB));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->MashB, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashC
	(ElemName.LoadString(IDS_STATE_NAME_MashC));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->MashC, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurA
	(ElemName.LoadString(IDS_STATE_NAME_CurA));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->CurA, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurB
	(ElemName.LoadString(IDS_STATE_NAME_CurB));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->CurB, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurC
	(ElemName.LoadString(IDS_STATE_NAME_CurC));
	pNote = new NStateNoNote;
	pElem = new NStateDouble(&pUnitState->CurC, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashcX
	(ElemName.LoadString(IDS_STATE_NAME_MashcX));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashcX, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashcY
	(ElemName.LoadString(IDS_STATE_NAME_MashcY));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashcY, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// MashcZ
	(ElemName.LoadString(IDS_STATE_NAME_MashcZ));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->MashcZ, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurR
	(ElemName.LoadString(IDS_STATE_NAME_CurR));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurR, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// ProgramName
	(ElemName.LoadString(IDS_STATE_NAME_ProgramName));
	pNote = new NStateNoNote;
	pElem = new NStateString(pUnitState->ProgramName, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurProg
	(ElemName.LoadString(IDS_STATE_NAME_CurProg));
	pNote = new NStateNoNote;
	pElem = new NStateString(pUnitState->GetCurProg(), pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurToolN
	(ElemName.LoadString(IDS_STATE_NAME_CurToolN));
	pNote = new NStateNoNote;
	pElem = new NStateString(pUnitState->CurToolN, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// NextToolNum
	(ElemName.LoadString(IDS_STATE_NAME_NextToolNum));
	pNote = new NStateNoNote;
	pElem = new NStateInt(&pUnitState->NextToolNum, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurCadr
	(ElemName.LoadString(IDS_STATE_NAME_CurCadr));
	pNote = new NStateNoNote;
	pElem = new NStateInt(&pUnitState->CurCadr, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurCadrAbs
	(ElemName.LoadString(IDS_STATE_NAME_CurCadrAbs));
	pNote = new NStateNoNote;
	pElem = new NStateInt(&pUnitState->CurHCadrAbs, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurCadrNum
	(ElemName.LoadString(IDS_STATE_NAME_CurCadrNum));
	pNote = new NStateNoNote;
	pElem = new NStateInt(&pUnitState->CurCadrNum, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurCurveType
	(ElemName.LoadString(IDS_STATE_NAME_CurCurveType));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->CurCurveType, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurDirComp
	(ElemName.LoadString(IDS_STATE_NAME_CurDirComp));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->CurDirComp, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurZComp
	(ElemName.LoadString(IDS_STATE_NAME_CurZComp));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->CurZComp, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurDiamComp
	(ElemName.LoadString(IDS_STATE_NAME_CurDiamComp));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurDiamComp, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurLengthComp
	(ElemName.LoadString(IDS_STATE_NAME_CurLengthComp));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurLengthComp, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurCycle
	(ElemName.LoadString(IDS_STATE_NAME_CurCycle));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->CurCycle, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// CurCycleRet
	(ElemName.LoadString(IDS_STATE_NAME_CurCycleRet));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->CurCycleRet, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Punch
	(ElemName.LoadString(IDS_STATE_NAME_Punch));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateEnum((UINT *)&pUnitState->Punch, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// PunchStep
	(ElemName.LoadString(IDS_STATE_NAME_CurPunchStep));
	pNote = new NStateBoolNote(&pUnitState->MetricCoord, IDS_STATE_NOTE_METRE, IDS_STATE_NOTE_INCH);
	pElem = new NStateDouble(&pUnitState->CurPunchStep, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FixX
	(ElemName.LoadString(IDS_STATE_NAME_FixX));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateBool(&pUnitState->FixX, IDS_STATE_VALUE_FIX_X, IDS_STATE_VALUE_UNFIX_X, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FixY
	(ElemName.LoadString(IDS_STATE_NAME_FixY));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateBool(&pUnitState->FixY, IDS_STATE_VALUE_FIX_Y, IDS_STATE_VALUE_UNFIX_Y, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FixZ
	(ElemName.LoadString(IDS_STATE_NAME_FixZ));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateBool(&pUnitState->FixZ, IDS_STATE_VALUE_FIX_Z, IDS_STATE_VALUE_UNFIX_Z, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FixA
	(ElemName.LoadString(IDS_STATE_NAME_FixA));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateBool(&pUnitState->FixA, IDS_STATE_VALUE_FIX_A, IDS_STATE_VALUE_UNFIX_A, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FixB
	(ElemName.LoadString(IDS_STATE_NAME_FixB));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateBool(&pUnitState->FixB, IDS_STATE_VALUE_FIX_B, IDS_STATE_VALUE_UNFIX_B, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// FixC
	(ElemName.LoadString(IDS_STATE_NAME_FixC));
	pNote = new NStateCodeNote("LineNumber", "Rapid");
	pElem = new NStateBool(&pUnitState->FixC, IDS_STATE_VALUE_FIX_C, IDS_STATE_VALUE_UNFIX_C, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// ActiveUCS
	(ElemName.LoadString(IDS_STATE_NAME_ActiveUCS));
	pNote = new NStateCodeNote("FunctionCode", "Rapid");
	pElem = new NStateEnumUCS((UINT *)&pUnitState->ActiveUCS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Electric current S
	(ElemName.LoadString(IDS_STATE_NAME_ElectrS));
	pNote = new NStateStrNote(IDS_STATE_NOTE_Electr);
	pElem = new NStateDouble(&pUnitState->ElectrS, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Electric current SL
	(ElemName.LoadString(IDS_STATE_NAME_ElectrSL));
	pNote = new NStateStrNote(IDS_STATE_NOTE_Electr);
	pElem = new NStateDouble(&pUnitState->ElectrSL, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// Electric current SF
	(ElemName.LoadString(IDS_STATE_NAME_ElectrSF));
	pNote = new NStateStrNote(IDS_STATE_NOTE_Electr);
	pElem = new NStateDouble(&pUnitState->ElectrSL, pNote);
	AllElemList[ElemName] = (CObject *&)pElem;
	// AdditionalCoordinates
	int CNum = pUnitState->AddCoords.GetSize() - 1;
	if(CNum > 0)
	{
		for(int i = 0; i < CNum; ++i)
		{
			(ElemName.LoadString(IDS_STATE_NAME_AddCoord));
			ElemName = pUnitState->AddCoords.GetAName(i) + ElemName;
			pNote = new NStateNoNote();
			pElem = new NStateAddCoord(pUnitState->AddCoords, i, pNote);
			AllElemList[ElemName] = (CObject *&)pElem;
		}
	}


}

void NStateUnitPane::DeleteElements()
{
	NStateParamsPane::DeleteElements();
	DynElemInd.clear();
}

void NStateUnitPane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
	NStateParamsPane::OnUpdate(pSender, lHint, pHint);
	if(lHint & N_SETTINGS)
	{
		CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
		if(pDoc)
		{
			// reset values if pointer changed
			NCUnitState * pState = pDoc->GetUnit()->GetState();
			if(pUnitState != pState)
			{
				pUnitState = pState;
				ResetElements();
			}
			CreateStateView();
		}
//		return;
	}
	if(lHint & N_UNIT_CHANGED)
	{
		CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
		if(pDoc)
		{
			NCUnitState * pState = pDoc->GetUnit()->GetState();
			if(pState/*pUnitState != pState*/)
			{
				pUnitState = pState;
				ResetElements();
			}
		}
		CreateStateView();
		UpdateControl();
		return;
	}
	if(lHint & N_STATE)
	{
		if(IsPaneVisible())
			UpdateControl();
		return;
	}
}

void NStateUnitPane::AnimateUpdateControl(const NDynParams& Params)
{
	if (!IsPaneVisible())
		return;
	DynParams = Params;
	for(int l = 0; l < DynElemInd.size(); ++l)
	{
		const auto j = DynElemInd.at(l);
		if (j < 0)
			continue;
		CString value = DynElemList.at(l).second->GetValueString();
		if (value == lc.GetItemText(j, 1))
		{
			if (lc.GetSafeHwnd())// To prevent assertion
				lc.SetItemData(j, RGB(0, 0, 255));
		}
		else
		{
			lc.SetItemData(j, RGB(255, 0, 0));
			lc.SetItemText(j, 1, value);
		}
	}
}

