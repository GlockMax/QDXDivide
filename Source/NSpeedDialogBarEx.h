#pragma once

#include "SliderButton.h"

// NSpeedDialogBarEx

class NSpeedDialogBarEx : public CMFCToolBar
{
	friend class NSpeedControls;
	DECLARE_DYNAMIC(NSpeedDialogBarEx)

public:
	NSpeedDialogBarEx();
	virtual ~NSpeedDialogBarEx();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	CMFCToolBarButton ButtonB;
	CMFCToolBarButton ButtonF;
	CMFCToolBarButton ButtonBack;
	CMFCToolBarButton ButtonFor;
	CMFCToolBarButton ButtonBackP;
	CMFCToolBarButton ButtonForP;
protected:
	class NSpeedControls *pCommonBase;
	CSliderButton SliderAnim;
	CSliderButton SliderAnimF;
	CMFCToolBarEditBoxButton EditStep;
	CMFCToolBarEditBoxButton EditPause;
public:
	void SetSliderHeight(int Height);
protected:
	void Clear();
	void Clear4Pause();
	double GetAnimLengthStep(void);
	double GetTrackFeedScale(void);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void ReadEditNums();
	void Init(class NSpeedControls *pBase) { pCommonBase = pBase; }
	void SetAnimDialog(void);
	void SetResultDialog(void);
	void SetAnimFDialog(void);
	void EnableForBack();
	void SetAnimPause(bool Pause);
};


