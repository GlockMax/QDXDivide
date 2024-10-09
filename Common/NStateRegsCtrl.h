#pragma once
/////////////////////////////////////////////////////////////////////////////
// NStateRegsCtrl window
#include "NMapIntToDouble.h"
#include "..\source\gridctrl\gridctrl.h"

class AFX_EXT_CLASS NStateRegsCtrl : public CGridCtrl
{
	DECLARE_DYNAMIC(NStateRegsCtrl)
// Construction
public:
	NStateRegsCtrl();
    BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP );

// Implementation
public:
	void Update(NMapIntToDouble & Regs, const class NVarSet& VarSet, const vector<pair<int, int>> &VisibleRegisters);
	void SetSettings();
	virtual ~NStateRegsCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(NStateGeomCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool IsRegVisible(const vector<pair<int, int>> &VisibleRegisters, int Ind);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	NMapIntToDouble PrevRegs;
	bool Editable;
};

/////////////////////////////////////////////////////////////////////////////
