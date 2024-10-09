#pragma once

// NCollSettingsDlg dialog

class AFX_EXT_CLASS NCollSettingsDlg : public CDialog
{
	friend class NCollManagerOrd;
	DECLARE_DYNAMIC(NCollSettingsDlg)

public:
	NCollSettingsDlg(bool CutDisabled, CWnd* pParent = NULL);   // standard constructor
	virtual ~NCollSettingsDlg();
	BOOL OnInitDialog() override;

// Dialog Data
	enum { IDD = IDD_COLLSETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	double m_MargMach;
	double m_MargTool;
	double m_MargStock;
	double m_MargPart;
	double m_MargFix;
	double m_TurnOffset;
	double m_XYOffset;
	double m_ZOffset;
	double m_XYOffsetSh;
	double m_ZOffsetSh;
	double m_CheckStep;
	BOOL m_MaskMach;
	BOOL m_MaskTool;
	BOOL m_MaskStock;
	BOOL m_MaskPart;
	BOOL m_MaskFix;
	BOOL m_MaskCut;
	BOOL m_MaskHold;
	BOOL m_NotIgnoreLimits;
	BOOL m_IgnoreUp;
	CMFCButton m_MaskFixCtrl;
	CMFCButton m_MaskPartCtrl;
	CMFCButton m_MaskToolCtrl;
	CMFCButton m_MaskMachCtrl;
	CMFCButton m_MaskStockCtrl;
	CMFCButton m_MaskCutCtrl;
	CMFCButton m_MaskHoldCtrl;
	bool m_CuttingDisabled;
};
