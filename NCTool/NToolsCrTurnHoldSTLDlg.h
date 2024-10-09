#pragma once
#include "ResLocal.h"
#include "NToolsCrTurn.h"

// NToolsCrTurnHoldSTLDlg dialog

class NToolsCrTurnHoldSTLDlg : public NToolsCrTurn
{
	DECLARE_DYNAMIC(NToolsCrTurnHoldSTLDlg)

public:
	NToolsCrTurnHoldSTLDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~NToolsCrTurnHoldSTLDlg();
	class NToolTurn *CreateTool();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_HOLDSTL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	class NToolTurn *pTool;
	afx_msg void OnBnClickedLoadTurnholderStl();
	CString FileName;
	double m_Y;
	float m_DX;
	float m_DY;
	float m_DZ;
	const CString &GetFileName(void) const { return FileName;}
	NToolTurn *GetNewTool() const { return pTool; };
	virtual void OnOK();
	virtual void OnCancel();
};
