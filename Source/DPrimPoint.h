#pragma once
#include "ResLocal.h"
#include "DPrimBase.h"
#include "NCMDoc.h"

// DPrimPoint dialog

class DPrimPoint : public DPrimBase
{
	DECLARE_DYNAMIC(DPrimPoint)

public:
	DPrimPoint(CNCMDoc* pDoc);
	virtual ~DPrimPoint();

// Dialog Data
	enum { IDD = IDD_MEASURE_POINTS };

    /// Нажатие на кнопку "Автомат" 
    afx_msg void OnBnAutomatClicked();

    /// Нажатие на кнопку "Пересечь" 
    afx_msg void OnBnIntersectClicked();

    /// Нажатие на кнопку "Создать" 
    afx_msg void OnBnCreateClicked();

    /// Нажатие на кнопку "Видимость" 
    afx_msg void OnBnVisibilityClicked();

    /// Нажатие на кнопку "Удалить" 
    afx_msg void OnBnDeleteClicked();

    /// Передача праметров точки в Dimensions
    void FromDialog() override;

    /// Вывод результата на форму
    void UpdateDialog();

protected:
	void Calc_ind(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetTexts() override;

	void UpdateValues() override;
    void FillDataFromXml() override;
    void FillXMLfromDialog() override;

    CNCMDoc* _doc; // документ

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

    double _x, _y, _z; // координаты точки
    float _r; // радиус
    CString _textCoord;
    CString _textNormal;
    CString _textStatus;
	CMFCColorButton _color;// цвет
	int _ind;
	CMFCButton _autoPT;	
	CMFCButton _intersectPT;
public:
	virtual BOOL OnInitDialog();
};
