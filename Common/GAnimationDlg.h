#pragma once


// GAnimationDlg dialog
#include "..\surface\resource.h"

class AFX_EXT_CLASS GAnimationDlg : public CDialog
{
	DECLARE_DYNAMIC(GAnimationDlg)

public:
	GAnimationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GAnimationDlg();

// Dialog Data
	enum { IDD = IDD_ANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	int StepLength;
	bool Updated;
public:
	afx_msg void OnNMReleasedcaptureAnimSld(NMHDR *pNMHDR, LRESULT *pResult);
	bool IsUpdated(void);
};
