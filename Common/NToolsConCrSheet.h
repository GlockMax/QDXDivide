#pragma once
#include "CadrID.h"
#include "NToolCommonSheet.h"


// NToolsConCrSheet

class AFX_EXT_CLASS NToolsConCrSheet : public NToolCommonSheet
{
//	DECLARE_DYNAMIC(NToolsConCrSheet)

public:
	//NToolsConCrSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	//NToolsConCrSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	//NToolsConCrSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	NToolID& GetOldToolID() { return OldToolID; }
	cadrID& GetStoredCadrID() { return StoredCadrID; }
	NToolID& GetStoredToolID() { return StoredToolID; }
	virtual ~NToolsConCrSheet();
protected:

protected:
	NToolID OldToolID;
	cadrID StoredCadrID;
	NToolID StoredToolID;
//	DECLARE_MESSAGE_MAP()
};


