#pragma once
#include "ResLocal.h"
#include "NStockSection.h"
#include "NSectSliderCtrl.h"


// NStockSectionDlg dialog

class NStockSectionDlg : public CDialog
{
	DECLARE_DYNAMIC(NStockSectionDlg)

public:
	NStockSectionDlg(CWnd* pParent, NStockSection *pSect);   // standard constructor
	virtual ~NStockSectionDlg();

// Dialog Data
	enum { IDD = IDD_ST_SECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnCancel();
	virtual void OnOK();
	NStockSection *pSection;
	NSectSliderCtrl m_SldCtrl;
	CComboBox m_ComboBox;
	void AngleChanged();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	CButton m_PickButton;
	int m_SldValue;
	afx_msg void OnBnClickedButton1();
	void InvalidateView(void);
	int GetPlaneNum(void);
	void ResetSlider(void);
	double m_Dist;
	double m_Angle;
protected:
	double m_VStep;
	CBitmap bmp;
public:
	afx_msg void OnBnClickedUpBut();
	afx_msg void OnBnClickedDwBut();
	afx_msg void OnEnKillfocusDist();
	void CalcSldValue(void);
	void SldValueChanged(void);
	void DistChanged(void);
	afx_msg void OnBnClickedSectFill();
	afx_msg void OnBnClickedCutAll();
	CButton m_FillSect;
	CButton m_CutAll;
	afx_msg void OnEnKillfocusVstep2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
