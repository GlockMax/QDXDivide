#pragma once
#include "NColorButton.h"
#include "NLineWidthCtrl.h"
#include "NProgramWeld.h"


// NWeldProgPropDlg dialog
  
class NWeldProgPropDlg : public CDialog
{
	DECLARE_DYNAMIC(NWeldProgPropDlg)

public:
	NWeldProgPropDlg(NProgramWeld &Program, CNCMDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~NWeldProgPropDlg();

// Dialog Data
	enum { IDD = IDD_WELD_PROPERT_PROGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	NProgramWeld &Program;
	CNCMDoc *pDoc;
public:
	afx_msg void OnBnClickedWsApply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	void FillProgram();
	NLineWidthCtrl m_thick1;
	NLineWidthCtrl m_thick2;
	NLineWidthCtrl m_thick3;
	NLineWidthCtrl m_thick4;
	NLineWidthCtrl m_thick5;
	virtual BOOL OnInitDialog();
	CString m_comment;
	NColorButton m_color1;
	NColorButton m_color2;
	NColorButton m_color3;
	NColorButton m_color4;
	NColorButton m_color5;
	int m_th1;
	int m_th2;
	int m_th3;
	int m_th4;
	int m_th5;
	BOOL m_Vis1;
	BOOL m_Vis2;
	BOOL m_Vis3;
	BOOL m_Vis4;
	BOOL m_Vis5;
};
