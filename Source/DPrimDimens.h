#pragma once
#include "ResLocal.h"
#include "DPrimBase.h"
#include "NCMDoc.h"
#include "afxcolorbutton.h"
#include "afxwin.h"

// диалоговое окно DPrimDimens

class DPrimDimens : public DPrimBase
{
	DECLARE_DYNAMIC(DPrimDimens)

public:
	DPrimDimens(CNCMDoc* pDoc);
	virtual ~DPrimDimens();

// Данные диалогового окна
	enum { IDD = IDD_MEASURE_SIZES };

    /// Передача праметров
    void FromDialog() override;

    void UpdateDialog();
	void UpdateValues() override;
    void FillDataFromXml() override;
    void FillXMLfromDialog() override;

    afx_msg void OnBnHideAllClick();

    afx_msg void OnBnDelAllClick();

    afx_msg void OnBnHideClick();

    afx_msg void OnBnDelClick();

    afx_msg void OnBnClick();
protected:
	void Calc_ind(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	void SetTexts() override;

	DECLARE_MESSAGE_MAP()

    CNCMDoc* _doc;
    float _width;
	int _dimTextSize;
	int _dimTextDigits;
	CMFCColorButton _color0;
    CMFCColorButton _color1;
    CMFCColorButton _colorBack;
    BOOL _createArc;
    BOOL _createPt;
    BOOL _hideWhenClosed;
    CString _textCoord;
    CString _textStatus;
    CString _textHelp;
    int _ind;

// For modes
	CMFCButton _MaskSizeLength;
	CMFCButton _MaskSizeRad;
	CMFCButton _MaskSizeDiam;
	CMFCButton _MaskSizeAngle;
	CMFCButton _MaskSizeSelect;
public:
	virtual BOOL OnInitDialog();
};
