#include "stdafx.h"
#include "MainFrm.h"
#include "ResInv.h"
#include "ResLocal.h"
#include "NSpeedControls.h"
#include "GAnimateCadr.h"
#include "NSpeedRibbon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NSpeedRibbon::NSpeedRibbon()
{
	pRibbonBar = nullptr;
	pSlider = nullptr;
	pEdit = nullptr;
	pButtonF = nullptr;
	pButtonB = nullptr;
	pButtonFPause = nullptr;
	pButtonBPause = nullptr;
	pPlaceF = nullptr;
	pPlaceB = nullptr;
	SliderState = 0;
	SliderValAnim = 0;
	SliderValAnimF = 0;
	SliderValWFr = 0;
	OldSliderVal = -1;
}

NSpeedRibbon::~NSpeedRibbon()
{
	delete pButtonB;
	delete pButtonF;
}

void NSpeedRibbon::Reset(CMFCRibbonBar *pRibbon)
{
	pRibbonBar = nullptr;
	if (pRibbon == nullptr)
		return;
	pSlider = static_cast<CMFCRibbonSlider *>(pRibbon->FindByID(ID_SPEED_SLD, FALSE, TRUE));
	if (pSlider == nullptr)
		return;
	CMFCRibbonPanel *pPanel = pSlider->GetParentPanel();
	if (pPanel == nullptr)
		return;
	pEdit = static_cast<CMFCRibbonEdit *>(pPanel->FindByID(IDC_STEP_NUM));
	if (pEdit == nullptr)
		return;
	pPlaceB = static_cast<CMFCRibbonButton *>(pPanel->FindByID(ID_STEP_BACK));
	pPlaceF = static_cast<CMFCRibbonButton *>(pPanel->FindByID(ID_STEP_FOR));
	pButtonBPause = static_cast<CMFCRibbonButton *>(pRibbon->FindByID(ID_PAUSE_BACK, FALSE, TRUE));
	pButtonFPause = static_cast<CMFCRibbonButton *>(pRibbon->FindByID(ID_PAUSE_FORWARD, FALSE, TRUE));
	delete pButtonB;
	delete pButtonF;
	pButtonB = new CMFCRibbonButton;
	pButtonF = new CMFCRibbonButton;
	CopyButton(pButtonB, pPlaceB);
	CopyButton(pButtonF, pPlaceF);
	if ((pButtonB == nullptr) || (pButtonF == nullptr) || (pButtonBPause == nullptr) || (pButtonFPause == nullptr))
		return;
	//CMFCRibbonPanel *pBufPanel = pButtonBPause->GetParentPanel();
	//if (pBufPanel == nullptr)
	//	return;
	//pBufPanel->Remove(pBufPanel->GetIndex(pButtonBPause), FALSE);

	//pBufPanel = pButtonFPause->GetParentPanel();
	//if (pBufPanel == nullptr)
	//	return;
	//pBufPanel->Remove(pBufPanel->GetIndex(pButtonFPause), FALSE);
	pRibbonBar = pRibbon;
}

void NSpeedRibbon::SetAnimDialog(void)
{
	if (!IsActive())
		return;
	SliderState = IDC_ANIM_SLD;
	SliderValAnim = int(NSpeedControls::ExpScaleInv(NSpeedPar::Extern.GetLengthStep(), pCommonBase->MIN_LIMIT, pCommonBase->MID_LIMIT, pCommonBase->MAX_LIMIT));
	pSlider->SetPos(SliderValAnim);

	CString Num;
	Num.Format("%d", pCommonBase->EditStepNum);
	pEdit->SetEditText(Num);
	pEdit->Redraw();

	CopyButton(pPlaceF, pButtonF);
	CopyButton(pPlaceB, pButtonB);
}	


void NSpeedRibbon::SetResultDialog(void)
{
	if (!IsActive())
		return;

}

void NSpeedRibbon::SetAnimFDialog(void)
{
	if (!IsActive())
		return;
	SliderState = IDC_ANIMF_SLD;
	double dVal = NSpeedPar::Extern.GetTrackFeedScale();
	SliderValAnimF = int(
		(dVal < 1.) ? (dVal - pCommonBase->MIN_LIMIT_F) * 50. / (1. - pCommonBase->MIN_LIMIT_F)
		: (dVal - 1.) * 50. / (pCommonBase->MAX_LIMIT_F - 1.) + 50.
		);

	pSlider->SetPos(SliderValAnimF);

	CString Num;
	Num.Format("%d", pCommonBase->EditStepNum);
	pEdit->SetText(Num);

	CopyButton(pPlaceF, pButtonF);
	CopyButton(pPlaceB, pButtonB);
}

void NSpeedRibbon::EnableForBack()
{
	if (!IsActive())
		return;

}

void NSpeedRibbon::SetAnimPause(bool Pause)
{
	if (!IsActive())
		return;
	CString Num;
	if (Pause)
	{
		Num.Format("%d", pCommonBase->EditPauseNum);
		CopyButton(pPlaceF, pButtonFPause);
		CopyButton(pPlaceB, pButtonBPause);
	}
	else
	{
		Num.Format("%d", pCommonBase->EditStepNum);
		CopyButton(pPlaceF, pButtonF);
		CopyButton(pPlaceB, pButtonB);
	}
	pEdit->SetEditText(Num);
	pEdit->Redraw();
}

void NSpeedRibbon::ForceRedraw()
{
	if (!IsActive())
		return;
	pEdit->Redraw();
	pSlider->Redraw();
}

void NSpeedRibbon::OnSlider()
{
	if (!IsActive())
		return;

	// Sinchronize all sliders
	int Val = OldSliderVal;
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
	pRibbonBar->GetElementsByID(ID_SPEED_SLD, arButtons);
	for (int i = 0; i < arButtons.GetSize(); ++i)
	{
		CMFCRibbonSlider *pS = static_cast<CMFCRibbonSlider *>(arButtons[i]);
		if (pS != nullptr)
		{
			int V = pS->GetPos();
			if (V != OldSliderVal)
			{
				Val = V;
				break;
			}
		}
	}
	if (OldSliderVal == Val)
		return;
	OldSliderVal = Val;
	for (int i = 0; i < arButtons.GetSize(); ++i)
	{
		CMFCRibbonSlider *pS = static_cast<CMFCRibbonSlider *>(arButtons[i]);
		if (pS != nullptr)
		{
			pS->SetPos(Val);
		}
	}
	// END: Sinchronize all sliders

	switch (SliderState)
	{
	case IDC_ANIM_SLD:
		NSpeedPar::Extern.SetLengthStep(pCommonBase->GetAnimLengthStep(Val));
		break;
	case IDC_ANIMF_SLD:
		NSpeedPar::Extern.SetTrackFeedScale(pCommonBase->GetTrackFeedScale(Val));
		break;
	}
	pCommonBase->SetUpdatedAnim();
}

int NSpeedRibbon::GetStepNum(void)
{
	CString Num = pEdit->GetEditText();
	int N = atoi(Num);
	if (N < 1)
		N = 1;
	pCommonBase->EditStepNum = N;
	return pCommonBase->EditStepNum;
}

int NSpeedRibbon::GetPauseNum(void)
{
	CString Num = pEdit->GetEditText();
	int N = atoi(Num);
	if (N < 1)
		N = 1;
	pCommonBase->EditPauseNum = N;
	return pCommonBase->EditPauseNum;
}

void NSpeedRibbon::CopyButton(CMFCRibbonButton *pDest, CMFCRibbonButton *pSrc)
{
	CMFCRibbonBaseElement *pD = static_cast<CMFCRibbonBaseElement *>(pDest);
	CMFCRibbonBaseElement *pS = static_cast<CMFCRibbonBaseElement *>(pSrc);
	if (pD == nullptr || pS == nullptr)
		return;
	pD->CopyFrom(*pS);
}
