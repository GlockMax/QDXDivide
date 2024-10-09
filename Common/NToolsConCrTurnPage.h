#pragma once
#include "ResLocal.h"
#include "NToolsConCrPage.h"
#include "NToolCombinedTurn.h"

// NToolsConCrTurnPage dialog

class AFX_EXT_CLASS NToolsConCrTurnPage : public NToolsConCrPage
{
	DECLARE_DYNAMIC(NToolsConCrTurnPage)

public:
	NToolsConCrTurnPage();
	virtual ~NToolsConCrTurnPage();

// Dialog Data
	enum { IDD = IDD_TOOLSCON_CREATE_TURN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual NToolCombinedTurn * GetNewTool() override;
	virtual NToolCombinedTurn * CreateNewToolCom(const CString & name);
	afx_msg void OnBnClickedAddToolInsert();
	double m_Xdisplacement;
	double m_Zdisplacement;
	afx_msg void OnBnClickedAddTurnholder();
	afx_msg void OnBnClickedAddTurnholderSTL();
	double GenZdisp(void);
	afx_msg void OnBnClickedAddToolSimple();
	double m_MainAngle;
};
