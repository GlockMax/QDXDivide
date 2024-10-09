#pragma once
#include "afxpropertygridctrl.h"
#include "afxbutton.h"
#include "SMPropGridCtrl.h"
#include "afxdialogex.h"


// SMDialog dialog

class SMDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SMDialog)

public:
	SMDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SMDialog();
	void SetGenMacro(class SGenMacro* pGenMacro) { genMacro_ = pGenMacro; }
	void FillFromGenMacro();
	void FillFromMacro();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MACRO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnApply();
	afx_msg void OnPdf();
	afx_msg void OnDefault();
	afx_msg void OnImport();
	afx_msg void OnTcnSelChangeList();
	afx_msg void OnComboSelect();
	afx_msg void OnGroupSelect();
	afx_msg void OnFavourites();

	bool CheckAllDefined();
	bool CheckMinMax() ;
	bool Apply();
	void UpdateParams();

	DECLARE_MESSAGE_MAP()
protected:
	class SGenMacro* genMacro_;
	CListBox listCtrl_;
	SMPropGridCtrl propCtrl_;
	CStatic staticCtrl_;
	CButton pdfReadCtrl_;
	CComboBox comboCtrl_;
	CComboBox groupCtrl_;
	CMFCButton butFavourite_;
	CButton checkOldV_;
	CButton butApply_;
	CButton butDefault_;
	CButton butImport_;
	CToolTipCtrl toolTip_;

	int listInd_;
	int xRightGap_;
	int yBotGap_;
	CImage image_;
	CRect origPictRectW_;
	CRect origPictRectC_;
	bool showFavourits_;
	int groupInd_;
	BOOL oldVersion_;

public:
	void InitComboCtrl();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void AdjustToPicture();
	bool LoadPicture(const CString& pictFile);
	bool LoadParamPicture(const CString& pictFile);
	bool LoadPdf(const CString& pdfFile);
	void RecalcFunctions();
	SMDefiniton* GetSelDef() const;
	BOOL PreTranslateMessage(MSG* pMsg) override;
	void AdjustListWidth();
};

