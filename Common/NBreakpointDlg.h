#pragma once
#include "ResLocal.h"
#include "ncstatic.h"
//#include "afxwin.h"

enum StoneNames
{
	MiToolChange,
	MiProgStartEnd,
	MiZOnly,
	MiXOnly,
	MiYOnly,
	MiBookmark,
	MiCollision,
	MiError,
	MiProgOptStop,
	MiSpindleSpeed,
	MiSpindleForwardReverse,
	MiFeed,
	MiCoolant,
	MiPositioning,
	MiCircularInterpolation,
	MiLinearInterpolation,
	MiCycle,
	MiSubprogramCall,
	MiSubprogramStartEnd,
	MiToolLengthCompensationActive,
	MiToolLengthCompensationCancel,
	MiToolDiameterCompensationActive,
	MiToolDiameterCompensationCancel,
	MiLifeTime,
	MiCheckZeroFeed,
	MiCheckLengthComp,
	MiCheckLengthOffsetNum,
	MiCheckDiamOffsetNum,
	MiCheckIJKRDef,
	MiCheckDiamCompOff,
	MiCheckFeedTurnSpindleTurnOff,
	MiCheckFeedTurnSpindleMillOn,
	MiCheckFeedMillSpindleMillOff,
	MiCheckFeedMillSpindleTurnOn,
	MiCheckXYChamf
};
enum MiFunctions
{
	AddMi,
	RemoveMi
};
// NBrakpointDlg dialog

class AFX_EXT_CLASS NBreakpointDlg : public CDialog
{
	DECLARE_DYNAMIC(NBreakpointDlg)

public:
	NBreakpointDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NBreakpointDlg();

	void Reset();

// Dialog Data
	enum { IDD = IDD_BREAKPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	NCStatic m_St_par;
	NCStatic m_St_G;
	NCStatic m_St_MOTION;
	NCStatic m_St_SUBPROG;
	NCStatic m_St_LENGTHCOMP;
	NCStatic m_St_DIAMETERCOMP;
	afx_msg void OnBnClickedBreakpointaddOnOff();
	CButton m_BREAKPOINT_ON_OFF;
protected:
	void Update(void);
public:
	BOOL v_MiToolChange;
	BOOL v_MiProgStartEnd;
	BOOL v_MiZOnly;
	BOOL v_MiXOnly;
	BOOL v_MiYOnly;
	BOOL v_MiBookmark;
	BOOL v_MiCollision;
	BOOL v_MiProgOptStop;
	BOOL v_MiSpindleSpeed;
	BOOL v_MiSpindleForwardReverse;
	BOOL v_MiFeed;
	BOOL v_MiCoolant;
	BOOL v_MiPositioning;
	BOOL v_MiCircularInterpolation;
	BOOL v_MiLinearInterpolation;
	BOOL v_MiCycle;
	BOOL v_MiSubprogramCall;
	BOOL v_MiSubprogramStartEnd;
	BOOL v_MiToolLengthCompensationActive;
	BOOL v_MiToolLengthCompensationCancel;
	BOOL v_MiToolDiameterCompensationActive;
	BOOL v_MiToolDiameterCompensationCancel;
	BOOL v_MiLifeTime;
	BOOL v_BREAKPOINT_ON_OFF;
	afx_msg void OnStnClickedMiprogstartend();
	afx_msg void OnStnClickedMitoolchange();
	afx_msg void OnStnClickedMiprogoptstop();
	afx_msg void OnStnClickedMibookmark();
	afx_msg void OnStnClickedMicollision();
	afx_msg void OnStnClickedMiLifeTime();
	afx_msg void OnStnClickedBreakpointaddOnOff();
	void FillNames(CArray<enum StoneNames>& Names) const;
	void SetFromNames(const CArray<enum StoneNames>& Names);
	void Serialize(CArchive& ar) override;
};
