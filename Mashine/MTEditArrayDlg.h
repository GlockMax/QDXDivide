#pragma once
#include "ResLocal.h"


// MTEditArrayDlg dialog

class MTEditArrayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MTEditArrayDlg)

public:
	MTEditArrayDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MTEditArrayDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MACHINE_ARRAY };
#endif
	int GetCount() { return m_Count; }
	double GetLinearStep() { return m_LinearStep; }
	bool GetRotate() { return m_Rotate == TRUE; }
	void SetRotary();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	int m_Count;
	int m_LinearStep;
	BOOL m_Rotate;
	bool Rotary;
public:
	virtual BOOL OnInitDialog();
};
