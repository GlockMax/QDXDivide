#pragma once
#include "NRepReportsArray.h"
#include "ResInv.h"
#include "ncunit.h"

class NEvalExp;

// NRepDlgRepEditor dialog

class NRepDlgRepEditor : public CDialog
{
	DECLARE_DYNAMIC(NRepDlgRepEditor)

public:
	NRepDlgRepEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~NRepDlgRepEditor();

// Dialog Data
	enum { IDD = IDD_REP_REP_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	NCUnit *pUnit;
	bool fModif,fWM;
	NRepReportsArray* pReps;//Диалоги
	void FillRepsList(void);
	virtual BOOL OnInitDialog();
	void Disable(bool flag);
	CStringArray FildsStr;
	CEdit *EditName;
	CListBox *ListReps;
	CEdit *EditCondition;
	CListBox *ListCondUnitParam;
	CEdit *EditDescription;
	CListBox *ListTableFilds;
	CListBox *ListAvailebleUnitFilds;
	//NEvalExp *Eval;
	class NCUnit* pUnit;
protected:
	bool fEnableEdit;
	bool NewRep;
public:
	afx_msg void OnLbnSelchangeListReps();
//	afx_msg void OnLbnDblclkListReps();
	void FillListCondUnitParam(void);
protected:
	int m_Selected;
public:
	afx_msg void OnBnClickedButtonRemoveRep();
	afx_msg void OnBnClickedButtonAddRep();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonAddFild();
	afx_msg void OnBnClickedButtonRemoveFild();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkListUnitParam();
	afx_msg void OnBnClickedButtonPlus();
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnBnClickedButtonMult();
	afx_msg void OnBnClickedButtonDev();
	afx_msg void OnBnClickedButtonLess();
	afx_msg void OnBnClickedButtonMore();
	afx_msg void OnBnClickedButtonEqv();
	afx_msg void OnBnClickedButtonNot();
	afx_msg void OnBnClickedButtonBracketl();
	afx_msg void OnBnClickedButtonBracketr();
	afx_msg void OnBnClickedButtonOr();
	afx_msg void OnBnClickedButtonAnd();
	afx_msg void OnLbnDblclkListRepFilds();
	afx_msg void OnLbnDblclkListUnitFilds();
	afx_msg void OnEnUpdateEditName();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnEnUpdateEditCndition();
	afx_msg void OnEnUpdateEditDescription();
};
