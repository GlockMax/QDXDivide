#pragma once
#include "ResLocal.h"
#include "cadrID.h"
#include "QColorButton.h"


class QComparison;
// QComparisonDlg dialog

class QComparisonDlg : public CDialog
{
	DECLARE_DYNAMIC(QComparisonDlg)

public:
	QComparisonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~QComparisonDlg();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApply();//применение цвета
    afx_msg LRESULT OnDataReady(WPARAM wParam, LPARAM lParam);//вызывается по завершению потока расчёта
	double *m_Lim;//массив диапазонов
	unsigned int k_Color;//количество цветов для диапазонов
	QColorButton *m_ColorCtrl;//массив кнопок в диалоге
	CEdit MaxNumberCtrl;
	int MaxNumber;
	CString m_Dist;
	CString ProgName;
	CString Cadr;
	cadrID CadrID;

protected:
	virtual void OnCancel();
	void SetDefaultLim();
	QComparison *pParComp;
	void UpdateColorButton(COLORREF NewColor, QColorButton &m_ColorCtrl);//обновляет цвет на кнопке
	COLORREF clrInit;//цвет по умолчанию при выборе цвета
    UINT_PTR ProgTimer;
    bool changed;
public:
	void SetParComp(QComparison * pComp);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedColor01();//обработчик нажатия на кнопку для выбора цвета
	afx_msg void OnBnClickedColor02();
	afx_msg void OnBnClickedColor03();
	afx_msg void OnBnClickedColor04();
	afx_msg void OnBnClickedColor05();
	afx_msg void OnBnClickedColor06();
	afx_msg void OnBnClickedColor07();
	afx_msg void OnBnClickedColor08();
	afx_msg void OnBnClickedColor09();
	afx_msg void OnBnClickedColor10();
	afx_msg void OnBnClickedColor11();

	afx_msg void OnBnClickedDefaultColor();
	afx_msg void OnBnClickedDefaultLim();
	afx_msg void OnBnClickedGotoLine();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	void StartProgressBar();
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void SetDistance(const class BPoint& P, double D);
	void ClearDistance();
};
