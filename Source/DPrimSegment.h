#pragma once
#include "ResLocal.h"
#include "DPrimBase.h"
#include "NCMDoc.h"
#include "afxcolorbutton.h"

// ���������� ���� DPrimSegment

class DPrimSegment : public DPrimBase
{
	DECLARE_DYNAMIC(DPrimSegment)

public:
	DPrimSegment(CNCMDoc* pDoc);
	virtual ~DPrimSegment();

// ������ ����������� ����
	enum { IDD = IDD_MEASURE_LINES };

    /// ������� �� ������ "�������" 
    afx_msg void OnBnAutomatClicked();

    /// ������� �� ������ "��� �����" 
    afx_msg void OnBnTwoPtsClicked();

    /// ������� �� ������ "��� ���������" 
    afx_msg void OnBnTwoPlnsClicked();

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

    void BnAutomat(bool iNoCheck = false);
    void BnTwoPts(bool iNoCheck = false);
    void BnTwoPlns(bool iNoCheck = false);

	DECLARE_MESSAGE_MAP()
    virtual void PostNcDestroy();

    CNCMDoc* _doc;
    double _length; // �����
    float _width; // ������
    CMFCColorButton _color; // ����
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
