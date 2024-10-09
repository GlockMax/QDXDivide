#pragma once
#include "ResLocal.h"
#include "NCMDoc.h"
#include "DPrimBase.h"
#include "afxcolorbutton.h"


// ���������� ���� DPrimArc

class DPrimArc : public DPrimBase
{
	DECLARE_DYNAMIC(DPrimArc)

public:
	DPrimArc(CNCMDoc* pDoc);
	virtual ~DPrimArc();

// ������ ����������� ����
	enum { IDD = IDD_MEASURE_ARC };

    /// ������� �� ������ "�������" 
    afx_msg void OnBnAutomatClicked();

    /// ������� �� ������ "��� �����" 
    afx_msg void OnBnThreePtsClicked();

    /// ������� �� ������ "���������" 
    afx_msg void OnBnVisibilityClicked();

    /// ������� �� ������ "�������" 
    afx_msg void OnBnDeleteClicked();

    /// ������� �� ������ "�������" 
    afx_msg void OnBnAgainClicked();

    /// �������� ��������� ����� � Dimensions
    void FromDialog() override;

    /// ����� ���������� �� �����
    void UpdateDialog();

protected:
	void Calc_ind(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
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
