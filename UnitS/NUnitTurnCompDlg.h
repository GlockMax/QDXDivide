#pragma once

#include "ResLocal.h"
#include "..\source\gridctrl\gridctrl.h"

// NUnitTurnCompDlg dialog

class NUnitTurnCompDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(NUnitTurnCompDlg)

public:
	class NTurnCompElem
	{
	public:
		double R;
		double X;
		double Z;
		int K;
	};
	CMap <int, int, NTurnCompElem, NTurnCompElem&> Elems;
	NUnitTurnCompDlg();
	virtual ~NUnitTurnCompDlg();

// Dialog Data
	enum { IDD = IDD_UNIT_TURNCOMP };



protected:
	CGridCtrl m_GridCtrl;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedTurnCorAdd();
	afx_msg void OnBnClickedClearAll();
};
