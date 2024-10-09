#pragma once
#pragma warning(disable : 4995)

#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "ResLocal.h"
#include "NListCtrl.h"
#include "NMapStringToOb.h"	
#include "NToolsCrCommonPage.h"	
class NMapStringToOb;
class NToolPreViewDlg;

#define LIST_VIEW_ICON_WIDTH 18
#define LIST_VIEW_ICON_HEIGHT 19


// NToolsConCrPage dialog

class AFX_EXT_CLASS NToolsConCrPage : public NToolsCrCommonPage
{
	DECLARE_DYNAMIC(NToolsConCrPage)

public:
	NToolsConCrPage(UINT nIDTemplate);
	virtual ~NToolsConCrPage();

// Construction
public:
	virtual NToolCombined * GetNewTool();
	void SetToolCombined(const NToolCombined *pTool);
	void SetToolsList(NMapStringToOb *plist);

	bool m_IsNameAuto;	
	CUIntArray m_men_array;

// Dialog Data
	//{{AFX_DATA(NToolsConCrDial)
	CEdit	m_name_ctrl;
	CEdit	m_pos_ctrl;
	CEdit	m_pos_turret_ctrl;
	CString	m_name;
	int		m_PointSize;
	UINT	m_pos;
	UINT	m_pos_turret;
	UINT	m_pos_attach;
	CButton m_editool_ctrl;
	CButton m_cltool_ctrl;
	//}}AFX_DATA
	NListCtrl	m_ToolList;
	double m_pos_cshift_x, m_pos_cshift_y, m_pos_cshift_z;
	BOOL m_pos_cshift_auto;
	double m_LifeTime;
	BOOL m_IgnoreUpToolStockColl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NToolsConCrDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmap m_ColorButtonBmp2;
	CBitmap m_ColorButtonBmp1;
	virtual void NUpdateControls();
	void EditTool();
	virtual void NUpdate();
	
	HICON m_hIcon;
	bool m_bMoreFunction;
	GLdouble m_ViewMatrix[16];

	int m_num_rem;
	NToolCombined *m_pToolOld;
	NToolCombined *m_pToolProduced;
	NMapStringToOb *m_ptool_list;

	long m_lComToolID;
	CArray<long, long> m_aSimToolID;
	CArray<long, long> m_aSimToolType;

	CBitmap		m_ImageListBmp;
	CImageList  m_ImageList;
	// Generated message map functions
	//{{AFX_MSG(NToolsConCrDial)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeleteTool();
	virtual void OnOK();
	afx_msg void OnDblclkToolList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTool();
	afx_msg void OnKillfocusName();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void PrepNew(NMapStringToOb * plist);

	bool CheckActive(void);
	void ClearNewTool();
	virtual NToolCombined * CreateNewToolCom(const CString & name) = 0;
	afx_msg void OnBnClickedAutolength();
	virtual void OnCancel();
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
};

