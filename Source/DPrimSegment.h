#pragma once
#include "ResLocal.h"
#include "DPrimBase.h"
#include "NCMDoc.h"
#include "afxcolorbutton.h"

// диалоговое окно DPrimSegment

class DPrimSegment : public DPrimBase
{
	DECLARE_DYNAMIC(DPrimSegment)

public:
	DPrimSegment(CNCMDoc* pDoc);
	virtual ~DPrimSegment();

// Данные диалогового окна
	enum { IDD = IDD_MEASURE_LINES };

    /// Нажатие на кнопку "Автомат" 
    afx_msg void OnBnAutomatClicked();

    /// Нажатие на кнопку "Две точки" 
    afx_msg void OnBnTwoPtsClicked();

    /// Нажатие на кнопку "Две плоскости" 
    afx_msg void OnBnTwoPlnsClicked();

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

    void BnAutomat(bool iNoCheck = false);
    void BnTwoPts(bool iNoCheck = false);
    void BnTwoPlns(bool iNoCheck = false);

	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();

    CNCMDoc* _doc;
    double _length; // длина
    float _width; // штрина
    CMFCColorButton _color; // цвет
    CString _textCoord;
    CString _textStatus;
    CString _textHelp;
    int _ind;
// For modes
	CMFCButton _MaskAutoLine;
	CMFCButton _MaskTwoPtLine;
	CMFCButton _MaskIntrsectLine;
public:
	virtual BOOL OnInitDialog();
};
