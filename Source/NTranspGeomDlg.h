#pragma once
#include "nstategeomctrl.h"
#include "ResLocal.h"

// NTranspGeomDlg dialog

class NTranspGeomDlg : public CDialog
{
	DECLARE_DYNAMIC(NTranspGeomDlg)

public:
	NTranspGeomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NTranspGeomDlg();

// Dialog Data
	enum { IDD = IDD_TRANSP_GEOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	NStateGeomCtrl Ctrl;
protected:
	CPoint BasePoint;
public:
	void Update(const NCadrGeom & Prev, const NCadrGeom & Next, const BPoint &ToolAxis);
};
