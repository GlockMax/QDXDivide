#pragma once


// NFloatEdit

class AFX_EXT_CLASS NFloatEdit : public CEdit
{
	DECLARE_DYNAMIC(NFloatEdit)

public:
	NFloatEdit();
	virtual ~NFloatEdit();
	static void DDX_Text_my(CDataExchange* pDX, int nIDC, double& value);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


