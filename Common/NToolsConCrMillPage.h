#pragma once
#include "NToolCombinedMill.h"
#include "ntoolsconcrpage.h"

class AFX_EXT_CLASS NToolsConCrMillPage : public NToolsConCrPage
{
	DECLARE_DYNAMIC(NToolsConCrMillPage)

public:
	NToolsConCrMillPage(void);
	virtual ~NToolsConCrMillPage(void);
	void AddDiskTool(bool Probe);
// Dialog Data
	enum { IDD = IDD_TOOLSCON_CREATE };

	double	m_displacement;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void NUpdate();
	virtual void NUpdateControls();
	afx_msg void OnAddMendrel();
	afx_msg void OnAddDiskTool();
	afx_msg void OnAddProbe();
	afx_msg void OnAddTool();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolCombinedMill * GetNewTool() override;
	virtual NToolCombinedMill * CreateNewToolCom(const CString & name);
};
