#pragma once
#include "nlinewidthctrl.h"
#include "NSectArrParams.h"
#include "NColorButton.h"
// NSectArrDlg dialog

class NSectArrDlg : public CMFCPropertyPage 
{
	DECLARE_DYNAMIC(NSectArrDlg)

public:
	NSectArrDlg();   // standard constructor
	NSectArrDlg(UINT ResourceID);   // standard constructor
	virtual ~NSectArrDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CImageList IList;
	NSectArrParams SectParams;

	DECLARE_MESSAGE_MAP()
public:
	double m_x;
	double m_y;
	double m_z;
	double m_ix;
	double m_iy;
	double m_iz;
	int m_thick;
	NLineWidthCtrl m_widthctrl;
	NColorButton m_color;
	BOOL m_colorbypart;
	virtual void SetParams(const class NSectArrParams &SectParams);
	virtual const class NSectArrParams GetParams(void);
	virtual BOOL OnInitDialog();
	void OnBnClickedColorbypart();
};
