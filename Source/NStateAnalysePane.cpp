#include "stdafx.h"
#include "NCMDoc.h"
#include "NCUnit.h"
#include "NControlPanel.h"
#include "NAnalyseState.h"
#include "NStateSettingsDlg.h"
#include "NStateWVel.h"
#include "NStateAnalysePane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NStateAnalysePane::NStateAnalysePane(void)
{
	pAnalyseState = NULL;
}

NStateAnalysePane::~NStateAnalysePane(void)
{
}

void NStateAnalysePane::SetAnalyseState(NAnalyseState *pState)
{
	if(pAnalyseState != pState)
	{
		pAnalyseState = pState;
		ResetElements();
	}
}

void NStateAnalysePane::OnUpdate(CDockablePane* pSender, LPARAM lHint, CObject* pHint) 
{
	NStateParamsPane::OnUpdate(pSender, lHint, pHint);
	if(lHint & N_NEWDOC)
	{
		ResetElements();
		CreateStateView();
		CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
		if(pDoc)
		{
			SetAnalyseState(&pDoc->GetAnalyseState());
			WeldParams.Define(pDoc->GetUnit());
			UpdateControl();
		}
	}
	if(lHint & N_SETTINGS)
	{
		CNCMDoc *pDoc = (CNCMDoc *)GetDocument();
		if(pDoc)
		{
			SetAnalyseState(&pDoc->GetAnalyseState());
			WeldParams.Define(pDoc->GetUnit());
		}
//		return;
	}
	if(lHint & N_ANALYSE || lHint & N_STATE)
	{
		if(!IsPaneVisible())
			return;

		WeldParams.ClearActual();
		UpdateControl();
		return;
	}
}

void NStateAnalysePane::ResetElements()
{
	DeleteElements();

	CString ElemName;
	NStateElem * pElem;
	NStateNote * pNote;

	// Volume
	if(pAnalyseState)
	{
		ElemName.LoadString(IDS_STATE_NAME_Volume);
		pNote = new NStateNoNote();
		pElem = new NStateDouble(&pAnalyseState->CurVolume, pNote);
		AllElemList[ElemName] = (CObject *&)pElem;
	}

	BOOL *pMetricCoord = NULL;
	NCUnit *pUnit = WeldParams.GetUnit();
	if(pUnit)
	{
		NCUnitState *pState = pUnit->GetState();
		if(pState)
			pMetricCoord = &pState->MetricCoord;
	}
	WeldParams.ClearActual();
}
