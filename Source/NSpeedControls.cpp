#include "stdafx.h"
#include "NCMProject.h"
#include "BConst.h"
#include "MainFrm.h"
#include "NTiParams.h"
#include "GAnimateCadr.h"
#include "NSpeedControls.h"


NSpeedControls::NSpeedControls()
{
	UpdatedAnim = false;
	LoadParams();
	EditStepNum = 1;
	EditPauseNum = 1;
	DlgBar.Init(this);
	SpeedRib.Init(this);
}


NSpeedControls::~NSpeedControls()
{
}

int NSpeedControls::LoadParams(void)
{
	NTiParams &Par = NCM_PROJECT.Defaults;

	CString Key = "Defaults/Sliders/Smooth";
	MIN_LIMIT = Par.GetDouble((Key + "@Min").GetBuffer(), 1.) * MM_I_SC;
	MID_LIMIT = Par.GetDouble((Key + "@Mid").GetBuffer(), 10.) * MM_I_SC;
	MAX_LIMIT = Par.GetDouble((Key + "@Max").GetBuffer(), 100.) * MM_I_SC;
	NSpeedPar::Extern.SetLengthStep(Par.GetDouble((Key + "@Def").GetBuffer(), 100.) * MM_I_SC);

	Key = "Defaults/Sliders/SmoothF";
	MIN_LIMIT_F = Par.GetDouble((Key + "@Min").GetBuffer(), 0.1);
	MAX_LIMIT_F = Par.GetDouble((Key + "@Max").GetBuffer(), 10.);
	NSpeedPar::Extern.SetTrackFeedScale(Par.GetDouble((Key + "@Def").GetBuffer(), 1.));

	return 0;
}

void NSpeedControls::SetUpdatedAnim(void)
{
	UpdatedAnim = true;
}

bool NSpeedControls::IsUpdatedAnim(void)
{
	bool b = UpdatedAnim;
	UpdatedAnim = false;
	return b;
}

int NSpeedControls::GetStepNum(void)
{
	if (SpeedRib.IsActive())
		return SpeedRib.GetStepNum();
	DlgBar.ReadEditNums();
	return EditStepNum;
}

int NSpeedControls::GetPauseNum(void)
{
	if (SpeedRib.IsActive())
		return SpeedRib.GetPauseNum();
	DlgBar.ReadEditNums();
	return EditPauseNum;
}

void NSpeedControls::SetAnimDialog(void)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if (pMain)
		if (pMain->IsAnimatePaused())
			return;
	if (SpeedRib.IsActive())
		SpeedRib.SetAnimDialog();
	else
		DlgBar.SetAnimDialog();
}


void NSpeedControls::SetResultDialog(void)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if (pMain)
		if (pMain->IsAnimatePaused())
			return;
	if (SpeedRib.IsActive())
		SpeedRib.SetResultDialog();
	else
		DlgBar.SetResultDialog();
}


void NSpeedControls::SetAnimFDialog(void)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if (pMain)
		if (pMain->IsAnimatePaused())
			return;
	if (SpeedRib.IsActive())
		SpeedRib.SetAnimFDialog();
	else
		DlgBar.SetAnimFDialog();
}


void NSpeedControls::EnableForBack()
{
	if (SpeedRib.IsActive())
		SpeedRib.EnableForBack();
	else
		DlgBar.EnableForBack();
}


void NSpeedControls::SetAnimPause(bool Pause)
{
	if (SpeedRib.IsActive())
		SpeedRib.SetAnimPause(Pause);
	else
		DlgBar.SetAnimPause(Pause);
}


void NSpeedControls::ForceRedraw()
{
	if (SpeedRib.IsActive())
		SpeedRib.ForceRedraw();
	else
		DlgBar.RedrawWindow();
}

void NSpeedControls::Reset(CMFCRibbonBar *pRibbon)
{
	SpeedRib.Reset(pRibbon);
}

double NSpeedControls::ExpScale(double Val, double Min, double Mid, double Max)
{
	Val *= 0.01;
	double Scale = Max - Min;
	double Base = 2. * log(Scale / (Mid - Min) - 1.);
	double Ret = Scale * (exp(Base * Val) - 1.) / (exp(Base) - 1.) + Min;
	return Ret;
}

double NSpeedControls::ExpScaleInv(double Val, double Min, double Mid, double Max)
{
	double Scale = Max - Min;
	double Base = 2. * log(Scale / (Mid - Min) - 1.);
	double Ret = log((Val - Min) * (exp(Base) - 1.) / Scale + 1.) / Base;
	return Ret * 100.;
}

void NSpeedControls::OnSlider()
{
	SpeedRib.OnSlider();
}

double NSpeedControls::GetAnimLengthStep(int Val)
{
	return ExpScale(Val, MIN_LIMIT,MID_LIMIT, MAX_LIMIT);
}

double NSpeedControls::GetTrackFeedScale(int Val)
{
	if (Val < 50)
		return (MIN_LIMIT_F * (50. - Val) + Val) / 50.;
	else
		return (Val + MAX_LIMIT_F * (Val - 50.)) / 50.;
}
