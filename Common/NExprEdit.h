#pragma once


// NFloatEdit

class AFX_EXT_CLASS NExprEdit : public CEdit
{
	DECLARE_DYNAMIC(NExprEdit)

public:
	NExprEdit();
	virtual ~NExprEdit();
	static void DDX_Text_my(CDataExchange* pDX, int nIDC, double& value);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


