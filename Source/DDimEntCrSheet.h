#pragma once



// DDimEntCrSheet

class DDimEntCrSheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(DDimEntCrSheet)

public:
	DDimEntCrSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	DDimEntCrSheet(void);
	virtual ~DDimEntCrSheet();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};


