#pragma once
#include "ResLocal.h"
#include "NSectArrDlg.h"
// NSectArrLDlg dialog

class NSectArrLDlg : public NSectArrDlg
{
	DECLARE_DYNAMIC(NSectArrLDlg)

public:
	NSectArrLDlg();   // standard constructor
	virtual ~NSectArrLDlg();

// Dialog Data
	enum { IDD = IDD_WELD_PSECT_LINEAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_left;
	BOOL m_right;
	int m_counter;
	double m_step;
	virtual void SetParams(const class NSectArrParams &SectParams);
	virtual const class NSectArrParams GetParams(void);
	afx_msg void OnBnClickedColorbypart();
};
