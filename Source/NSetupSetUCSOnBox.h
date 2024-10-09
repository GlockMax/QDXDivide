#pragma once
#include "ResLocal.h"
#include "NFloatEdit.h"
#include "NCStatic.h"
#include "NSetupSetBoxPointDlg.h"


// NSetupSetUCSOnBox dialog

class NSetupSetUCSOnBox : public NSetupSetBoxPointDlg
{
	DECLARE_DYNAMIC(NSetupSetUCSOnBox)

public:
	NSetupSetUCSOnBox(CWnd* pParent, class CNCMDoc *pDoc);   // standard constructor
	NSetupSetUCSOnBox(UINT nIDTemplate, CWnd* pParentWnd, class CNCMDoc *pDoc);
	virtual ~NSetupSetUCSOnBox();

// Dialog Data
	enum { IDD = IDD_SETUP_SETUCSONBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_Stock_Vx;
	double m_Stock_Vy;
	double m_Stock_Vz;
	double m_Stock_VAngl;
	BOOL m_SecondSolution;
	CButton m_SecondSolutionCtrl;
public:
	afx_msg void OnToolApply(void);
};
