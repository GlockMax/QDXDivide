#pragma once
class NSpeedRibbon
{
	friend class NSpeedControls;
public:
	NSpeedRibbon();
	~NSpeedRibbon();
	void Reset(CMFCRibbonBar *pRibbon);
	bool IsActive() const { return pRibbonBar != nullptr; }
	void OnSlider();
protected:
	int GetStepNum(void);
	int GetPauseNum(void);
	void Init(class NSpeedControls *pBase) { pCommonBase = pBase; }
	void CopyButton(CMFCRibbonButton *pDest, CMFCRibbonButton *pSrc);
	// Common methods
	void SetAnimDialog(void);
	void SetResultDialog(void);
	void SetAnimFDialog(void);
	void EnableForBack();
	void SetAnimPause(bool Pause);
	void ForceRedraw();

protected:
	class NSpeedControls *pCommonBase;
	CMFCRibbonBar *pRibbonBar;
	CMFCRibbonSlider *pSlider;
	CMFCRibbonEdit *pEdit;
	CMFCRibbonButton *pButtonF;
	CMFCRibbonButton *pButtonB;
	CMFCRibbonButton *pButtonFPause;
	CMFCRibbonButton *pButtonBPause;
	CMFCRibbonButton *pPlaceF;
	CMFCRibbonButton *pPlaceB;
	UINT SliderState;
	int SliderValAnim;
	int SliderValAnimF;
	int SliderValWFr;
	int OldSliderVal;
};

