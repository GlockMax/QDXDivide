#pragma once
#include "NSpeedDialogBarEx.h"
#include "NSpeedRibbon.h"

class NSpeedControls
{
	friend class NSpeedDialogBarEx;
	friend class NSpeedRibbon;
public:
	NSpeedControls();
	~NSpeedControls();
	void OnSlider();
protected:
	double MIN_LIMIT = 0.5;
	double MID_LIMIT = 1.5;
	double MAX_LIMIT = 50.;
	double MIN_LIMIT_F = 0.01;
	double MAX_LIMIT_F = 10.;

	static const int SLIDER_WIDTH = 200;
	static const int EDIT_WIDTH = 48;

	bool UpdatedAnim;
	int EditStepNum;
	int EditPauseNum;
public:
	NSpeedDialogBarEx DlgBar;
	NSpeedRibbon SpeedRib;
protected:
	int LoadParams(void);
	void SetUpdatedAnim(void);
	static double ExpScale(double Val, double Min, double Mid, double Max);
	static double ExpScaleInv(double Val, double Min, double Mid, double Max);
	double GetAnimLengthStep(int Val);
	double GetTrackFeedScale(int Val);
public:
	bool IsUpdatedAnim(void);
	int GetStepNum(void);
	int GetPauseNum(void);
	// Common methods
	void SetAnimDialog(void);
	void SetResultDialog(void);
	void SetAnimFDialog(void);
	void EnableForBack();
	void SetAnimPause(bool Pause);
	void ForceRedraw();
	// END: Common methods
	void Reset(CMFCRibbonBar *pRibbon);
};

