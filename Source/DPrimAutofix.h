#pragma once

// диалоговое окно DPrimAutofix

#include "ResLocal.h"
#include "NCMDoc.h"
#include "DPrimBase.h"
#include "afxcolorbutton.h"

class DPrimAutofix : public DPrimBaseCommon
{
	DECLARE_DYNAMIC(DPrimAutofix)

public:
	DPrimAutofix(CNCMDoc* pDoc);
	virtual ~DPrimAutofix();

// Данные диалогового окна
	enum { IDD = IDD_MEASURE_AUTOFIX };

    /// Передача праметров
    void FromDialog();

	virtual BOOL OnInitDialog();

protected:
    void UpdateValues() override;
    void FillDataFromXml() override;
    void FillXMLfromDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();

    CNCMDoc* _doc;
    int _size;
    CMFCColorButton _color;
    float _width;
    int _eps;
};
