#pragma once

// диалоговое окно DPrimBase

#include "ResLocal.h"
#include "NCMDoc.h"
#include "afxcolorbutton.h"
class DPrimBaseCommon : public CPropertyPage
{
    DECLARE_DYNAMIC(DPrimBaseCommon)
public:
    DPrimBaseCommon(UINT nIDTemplate);

protected:
    DECLARE_MESSAGE_MAP()
    virtual void UpdateValues() = 0;
    virtual void FillDataFromXml() = 0;
    virtual void FillXMLfromDialog() = 0;
    void OnUpdateDialog() { if (UpdateData() != 0) UpdateValues(); }
    afx_msg void OnSaveParameters();
};

class DPrimBase : public DPrimBaseCommon
{
	DECLARE_DYNAMIC(DPrimBase)

public:
	DPrimBase(CNCMDoc* pDoc, UINT nIDTemplate);
	virtual ~DPrimBase();
	virtual BOOL OnInitDialog();
    virtual void FromDialog();

    /// Нажатие на кнопку 
    afx_msg void OnBnClicked();



protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual void SetTexts() = 0;

	DECLARE_MESSAGE_MAP()

private:
	void UpdateBNSButtons();
private:
	CNCMDoc* _doc;
    BOOL _skeleton;
    BOOL _dimObj;
    BOOL _trajectory;
    BOOL _model;
    BOOL _endPt;
    BOOL _midPt;
    BOOL _center;
    BOOL _ptOnElem;
    BOOL _wholeObj;
    CMFCButton _bns[4];
	CMFCButton _MaskSkeleton;
    CMFCButton _MaskPrimObj;
    CMFCButton _MaskTrajectory;
    CMFCButton _MaskModel;
    CMFCButton _MaskWholeObj;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
