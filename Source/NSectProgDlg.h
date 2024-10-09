#pragma once
#include "NLineWidthCtrl.h"
#include "ResLocal.h"

// NSectProgDlg dialog

class NSectProgDlg : public CDialog
{
	DECLARE_DYNAMIC(NSectProgDlg)
public:
	NSectProgDlg(CWnd* pParent = NULL);   // standard constructor
	NSectProgDlg(class CNCMDoc *pD, class NProgramWeld *pProg);   // standard constructor
	virtual ~NSectProgDlg();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedWsApply();
	void ApplyChanges(void);

// Dialog Data
	enum { IDD = IDD_WELD_DSECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	class CNCMDoc *pDoc;
	class NProgramWeld *pProg;

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_vdsect;
	int m_step;
	BOOL m_vstep;
	NLineWidthCtrl m_widthctrl;
	int m_thick;
	NLineWidthCtrl m_dwidthctrl;
	int m_dthick;
	int m_endline;
	int m_startline;
	CString m_list;
protected:
	virtual void PostNcDestroy();
};
