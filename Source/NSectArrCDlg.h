#pragma once
#include "ResLocal.h"
#include "NSectArrDlg.h"
// NSectArrCDlg dialog

class NSectArrCDlg : public NSectArrDlg
{
	DECLARE_DYNAMIC(NSectArrCDlg)

public:
	NSectArrCDlg();   // standard constructor
	virtual ~NSectArrCDlg();

// Dialog Data
	enum { IDD = IDD_WELD_PSECT_CIRCLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_counter;
	double m_sangl;
	double m_eangl;
	double m_ix2;
	double m_iy2;
	double m_iz2;
	virtual void SetParams(const class NSectArrParams &SectParams);
	virtual const class NSectArrParams GetParams(void);
	afx_msg void OnBnClickedColorbypart();
};
