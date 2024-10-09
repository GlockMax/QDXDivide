#pragma once
#include <afxdlgs.h>
#include "NCTOOL_API.h"
#include "NToolID.h"

class NCTOOL_API NToolCommonSheet : public CPropertySheet
{
public:
	NToolCommonSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	NToolCommonSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy() override;
public:
	virtual BOOL DestroyWindow();
};

