#pragma once
#include "afxpropertysheet.h"

class NSectArrSheet :
	public CMFCPropertySheet
{
	DECLARE_DYNAMIC(NSectArrSheet)
public:
	NSectArrSheet(NWeldPart &iWeldPart);
	~NSectArrSheet(void);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NSettingsDlg)
	public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	protected:
	//}}AFX_VIRTUAL
protected:
	NWeldPart &WeldPart;
	//{{AFX_MSG(NSettingsDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
