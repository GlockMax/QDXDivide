#pragma once
#include "ResLocal.h"
#include "NCMDoc.h"
#include "DPrimBase.h"
#include "afxcolorbutton.h"


// диалоговое окно DPrimArc

class DPrimArc : public DPrimBase
{
	DECLARE_DYNAMIC(DPrimArc)

public:
	DPrimArc(CNCMDoc* pDoc);
	virtual ~DPrimArc();

// Данные диалогового окна
	enum { IDD = IDD_MEASURE_ARC };

    /// Нажатие на кнопку "Автомат" 
    afx_msg void OnBnAutomatClicked();

    /// Нажатие на кнопку "Три точки" 
    afx_msg void OnBnThreePtsClicked();

    /// Нажатие на кнопку "Видимость" 
    afx_msg void OnBnVisibilityClicked();

    /// Нажатие на кнопку "Удалить" 
    afx_msg void OnBnDeleteClicked();

    /// Нажатие на кнопку "Сначала" 
    afx_msg void OnBnAgainClicked();

    /// Передача праметров точки в Dimensions
    void FromDialog() override;

    /// Вывод результата на форму
    void UpdateDialog();

protected:
	void Calc_ind(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	void SetTexts() override;

    void UpdateValues() override;
    void FillDataFromXml() override;
    void FillXMLfromDialog() override;

	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();

    void BnAutomat(bool iNoCheck = false);
    void BnThreePts(bool iNoCheck = false);

    CNCMDoc* _doc;
    double _length;
    float _width;
    CMFCColorButton _color;
    CString _textCoord;
    CString _textStatus;
    CString _textHelp;
    int _ind;
    int _isCircle;
// For modes
	CMFCButton _MaskAutoArc;
	CMFCButton _MaskThreePtArc;

public:
	virtual BOOL OnInitDialog();
};
