#pragma once
#include "resource.h"
#include "EditorDEF.h"
#include "AfxTempl.h"
// DlgEditorSettings dialog

class AFX_EXT_CLASS DlgEditorSettings : public CDialog
{
	DECLARE_DYNAMIC(DlgEditorSettings)

public:
	editor_cfg *pConfig;
	COLORREF CurColor;
	DlgEditorSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgEditorSettings();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDITORSETTINGS };

protected:
	CArray <editor_style*> StylesArray;
	CListBox *pListStyles;
	
	int CurStyleNum;
	CButton m_ColorCtrl;
	CBitmap ColorButtonBmp;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonColor();
	void Renew(void);
	void UpdateColorButton(void);
	virtual BOOL OnInitDialog();
protected:
	CString EditDescription;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeListStyles();
};
