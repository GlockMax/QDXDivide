#pragma once
#include "NToolsConCrPage.h"
#include "NToolCombinedWeld.h"

class AFX_EXT_CLASS NToolsConCrWeldPage : public NToolsConCrPage
{
	DECLARE_DYNAMIC(NToolsConCrWeldPage)

public:
	NToolsConCrWeldPage(void);
	virtual ~NToolsConCrWeldPage(void);

// Dialog Data
	enum { IDD = IDD_TOOLSCON_CREATE_WELD };

	double	m_displacement;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnAddMendrel();
	afx_msg void OnAddDiskTool();
	afx_msg void OnAddTool();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolCombinedWeld * GetNewTool() override;
	virtual NToolCombinedWeld * CreateNewToolCom(const CString & name);
};
