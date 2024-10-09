#pragma once

#include "ResLocal.h"
#include "QMeasure.h"
#include "ncstatic.h"



// QMeasureDlg dialog

class QMeasureDlg : public CDialog
{
	DECLARE_DYNAMIC(QMeasureDlg)

public:
	QMeasureDlg(CWnd* pParent, UINT id);   // standard constructor
	virtual ~QMeasureDlg();

// Dialog Data
//	enum { IDD = IDD_MEASURE };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	void QMeasureDlg::OnCancel() override;
	DECLARE_MESSAGE_MAP()
	QMeasure *pQMeasure;
	CString str;
	CEdit XFirstC;
	CEdit YFirstC;
	CEdit ZFirstC;
	CEdit XThirdC;
	CEdit YThirdC;
	CEdit ZThirdC;
	CEdit m_Cadr;
	CEdit m_ProgName;
	CBitmap bmp;
	CToolTipCtrl m_ToolTip; 
public:
	afx_msg void OnBnClickedCalc();
	void ReDraw(void);
	void SetQMeasure(QMeasure * parent);
	void SetPoint(BPoint point);
	void SetVector(BPoint vector);
	void SetNotDefine(int Num);
	BPoint GetPoint(void);
	BPoint GetVector(void);
	CEdit MeasureResult;
	CEdit m_EndPoint;
	CButton m_Bind;
	CButton m_Back;
	CButton m_Calc;
	CButton m_Goto;
	NCStatic m_IDC_MES1;
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedGotoLine();
	BOOL OnInitDialog() override;
	CString ProgName;
	CString Cadr;
	cadrID CadrID;
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
