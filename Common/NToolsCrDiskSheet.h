#pragma once
// NToolsCrDiskSheet.h : header file
//
#include "NToolDiskStd.h"
#include "NToolStd.h"
#include "NToolsCrFasetPage.h"
#include "NToolsCrRoundPage.h"
#include "NToolsCrAnglePage.h"
#include "NToolsCrAnglesPage.h"
#include "NToolsCrBRoundPage.h"	
#include "NToolsCrBorePage.h"	
#include "NToolsCrAptPage.h"
#include "NToolsCrShapedPage.h"
/////////////////////////////////////////////////////////////////////////////
// NToolsCrDiskSheet
class NTool;
class NToolStd;
class NToolDiskStd;
class NToolShaped;
class NToolCombined;

class AFX_EXT_CLASS NToolsCrDiskSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(NToolsCrDiskSheet)

// Construction
public:
	NToolsCrDiskSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	bool Apply();

public:
	void ClearTools();
	int tab_pos;
	NToolMill* GetNewTool();
	CButton m_button;
	NToolsCrFasetPage m_fasetpage;
	NToolsCrRoundPage m_roundpage;
	NToolsCrAnglePage m_anglepage;
	NToolsCrAnglesPage m_anglespage;
	NToolsCrAptPage m_aptpage;
	NToolsCrShapedPage m_shapedpage;
	NToolsCrBRoundPage m_broundpage;
	NToolsCrBorePage m_borepage;
	virtual ~NToolsCrDiskSheet();

protected:
	NToolStd *pTool;
	NToolDiskStd *pToolDisk;
	NToolShaped *pToolShaped;
	HICON m_hIcon;
	void AddControlPages(void);
	CImageList ToolPics;
	//{{AFX_MSG(NToolsCrDiskSheet)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
