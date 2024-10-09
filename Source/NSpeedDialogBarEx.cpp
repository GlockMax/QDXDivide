// NSpeedDialogBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "BConst.h"
#include "NCMProject.h"
#include "MainFrm.h"
#include "GAnimateCadr.h"
#include "NSpeedControls.h"
#include "NSpeedDialogBarEx.h"

// NSpeedDialogBarEx

IMPLEMENT_DYNAMIC(NSpeedDialogBarEx, CMFCToolBar)
NSpeedDialogBarEx::NSpeedDialogBarEx() :
	ButtonBack(ID_PAUSE_BACK, 0),
	ButtonFor(ID_PAUSE_FORWARD, 0),
	ButtonBackP(ID_PAUSE_BACK, 0),
	ButtonForP(ID_PAUSE_FORWARD, 0),
	ButtonB(ID_STEP_BACK, 0),
	ButtonF(ID_STEP_FOR, 0),
	EditStep(IDC_STEP_NUM, 0, CBS_DROPDOWN, NSpeedControls::EDIT_WIDTH ),
	EditPause(ID_PAUSE_NUM, 0, CBS_DROPDOWN, NSpeedControls::EDIT_WIDTH ),
	SliderAnim(IDC_ANIM_SLD),
	SliderAnimF(IDC_ANIMF_SLD)
{
	SliderAnim.SetRange(0, 100);
	SliderAnimF.SetRange(0, 100);
}

NSpeedDialogBarEx::~NSpeedDialogBarEx()
{
}


BEGIN_MESSAGE_MAP(NSpeedDialogBarEx, CMFCToolBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// NSpeedDialogBarEx message handlers



void NSpeedDialogBarEx::Clear()
{
	if(GetCount() > 1)
	{
		CString Num;
		((CMFCToolBarEditBoxButton *)GetButton(1))->GetEditBox()->GetWindowText(Num);
		int N = atoi(Num);
		if(N < 1)
			N = 1;
		switch(GetButton(1)->m_nID)
		{
		case IDC_STEP_NUM:
			pCommonBase->EditStepNum = N;
			break;
		case ID_PAUSE_NUM:
			pCommonBase->EditPauseNum = N;
			break;
		}
	}
	RemoveAllButtons();
}
void NSpeedDialogBarEx::Clear4Pause()
{
	if(GetCount() > 1)
	{
		CString Num;
		((CMFCToolBarEditBoxButton *)GetButton(1))->GetEditBox()->GetWindowText(Num);
		int N = atoi(Num);
		if(N < 1)
			N = 1;
		switch(GetButton(1)->m_nID)
		{
		case IDC_STEP_NUM:
			pCommonBase->EditStepNum = N;
			break;
		case ID_PAUSE_NUM:
			pCommonBase->EditPauseNum = N;
			break;
		}
	}
	RemoveButton(2);
	RemoveButton(1);
	RemoveButton(0);
}

void NSpeedDialogBarEx::SetResultDialog(void)
{
}

void NSpeedDialogBarEx::SetAnimDialog(void)
{
	Clear();

	int StepLength = int(NSpeedControls::ExpScaleInv(NSpeedPar::Extern.GetLengthStep(), pCommonBase->MIN_LIMIT, pCommonBase->MID_LIMIT, pCommonBase->MAX_LIMIT));


	SliderAnim.SetValue(StepLength);
	InsertButton(ButtonB, -1);
	InsertButton(EditStep, -1);
	InsertButton(ButtonF, -1);
 	InsertButton(SliderAnim, -1);
	CString Num;
	Num.Format("%d", pCommonBase->EditStepNum);
	((CMFCToolBarEditBoxButton *)GetButton(1))->SetContents(Num);

	UpdateData(FALSE);
	AdjustLayout();
}
void NSpeedDialogBarEx::SetAnimFDialog(void)
{
	Clear();

	double dVal = NSpeedPar::Extern.GetTrackFeedScale();
	int iVal = int(
		(dVal < 1.) ? (dVal - pCommonBase->MIN_LIMIT_F) * 50. / (1. - pCommonBase->MIN_LIMIT_F)
		: (dVal - 1.) * 50. / (pCommonBase->MAX_LIMIT_F - 1.) + 50.
		);

	SliderAnimF.SetValue(iVal);
	InsertButton(ButtonB, -1);
	InsertButton(EditStep, -1);
	InsertButton(ButtonF, -1);
 	InsertButton(SliderAnimF, -1);
	CString Num;
	Num.Format("%d", pCommonBase->EditStepNum);
	((CMFCToolBarEditBoxButton *)GetButton(1))->SetContents(Num);

	UpdateData(FALSE);
	AdjustLayout();
}

void NSpeedDialogBarEx::OnPaint()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	if(pMain)
	{
		if(pMain->IsAnimatePaused())
		{
			UINT Style0 = (pMain->IsPauseAnimatePause() && pMain->GetPauseAnimBack()) ? TBBS_PRESSED : 0 
				| (pMain->GetPauseAnimFor() ? TBBS_DISABLED : 0);


			UINT Style2 = (pMain->IsPauseAnimatePause() && pMain->GetPauseAnimFor()) ? TBBS_PRESSED : 0
				| (pMain->GetPauseAnimBack() ? TBBS_DISABLED : 0);

			//if (NCM_PROJECT.GetGlobalState().GetModelMode() != NGlobalState::BSP)
			//{
			//	Style0 |= TBBS_DISABLED;
			//	Style2 |= TBBS_DISABLED;
			//}

			GetButton(0)->SetImage(pMain->GetPauseAnimBack() ? ButtonBack.GetImage() : ButtonBackP.GetImage());
			GetButton(2)->SetImage(pMain->GetPauseAnimFor() ? ButtonFor.GetImage() : ButtonForP.GetImage());

			GetButton(0)->SetStyle(Style0); 
			GetButton(2)->SetStyle(Style2); 
		}
	}
	CMFCToolBar::OnPaint();
}

//void NSpeedDialogBarEx::DisableForBack()
//{
//	SetButtonStyle(0, TBBS_DISABLED);
//	SetButtonStyle(2, TBBS_DISABLED);
//}
void NSpeedDialogBarEx::EnableForBack()
{
	SetButtonStyle(0, 0);
	SetButtonStyle(2, 0);
}
void NSpeedDialogBarEx::SetAnimPause(bool Pause)
{
	Clear4Pause();
	if(Pause)
	{
		InsertButton(ButtonFor, 0);
		InsertButton(EditPause, 0);
		InsertButton(ButtonBack, 0);
		CString Num;
		Num.Format("%d", pCommonBase->EditPauseNum);
		((CMFCToolBarEditBoxButton *)GetButton(1))->SetContents(Num);
	}
	else
	{
		InsertButton(ButtonF, 0);
		InsertButton(EditPause, 0);
		InsertButton(ButtonB, 0);
		CString Num;
		Num.Format("%d", pCommonBase->EditStepNum);
		((CMFCToolBarEditBoxButton *)GetButton(1))->SetContents(Num);
	}
	UpdateData(FALSE);
	AdjustLayout();
}

int NSpeedDialogBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMFCToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	SliderAnim.m_iWidth = pCommonBase->SLIDER_WIDTH;
	SliderAnimF.m_iWidth = pCommonBase->SLIDER_WIDTH;
	//SliderAnim.SetStyle(TBS_AUTOTICKS);

	return 0;
}

void NSpeedDialogBarEx::ReadEditNums()
{
	if (GetCount() > 1)
	{
		CString Num;
		((CMFCToolBarEditBoxButton *)GetButton(1))->GetEditBox()->GetWindowText(Num);
		int N = atoi(Num);
		if (N < 1)
			N = 1;
		switch (GetButton(1)->m_nID)
		{
		case IDC_STEP_NUM:
			pCommonBase->EditStepNum = N;
			break;
		case ID_PAUSE_NUM:
			pCommonBase->EditPauseNum = N;
			break;
		}
	}
}

double NSpeedDialogBarEx::GetAnimLengthStep(void)
{
	int StepLength = CSliderButton::GetPos(IDC_ANIM_SLD);
	return pCommonBase->GetAnimLengthStep(StepLength);
}

double NSpeedDialogBarEx::GetTrackFeedScale(void)
{
	int Val = CSliderButton::GetPos(IDC_ANIMF_SLD);
	return pCommonBase->GetTrackFeedScale(Val);
}

void NSpeedDialogBarEx::SetSliderHeight(int Height)
{
	SliderAnim.SetHeight(Height);
	SliderAnimF.SetHeight(Height);
}

void NSpeedDialogBarEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
	case SB_THUMBPOSITION:
	case SB_LINELEFT: // works when left arrow pressed
	case SB_LINERIGHT:
		UpdateData();
		switch (pScrollBar->GetDlgCtrlID())
		{
		case IDC_ANIM_SLD:
			NSpeedPar::Extern.SetLengthStep(GetAnimLengthStep());
			break;
		case IDC_ANIMF_SLD:
			NSpeedPar::Extern.SetTrackFeedScale(GetTrackFeedScale());
			break;
		}
		pCommonBase->SetUpdatedAnim();
		break;
	}

	CMFCToolBar::OnHScroll(nSBCode, nPos, pScrollBar);

}
