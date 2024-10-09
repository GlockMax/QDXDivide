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

    /// ������� �� ������ "�������" 
    afx_msg void OnBnAutomatClicked();

    /// ������� �� ������ "��������" 
    afx_msg void OnBnIntersectClicked();

    /// ������� �� ������ "�������" 
    afx_msg void OnBnCreateClicked();

    /// ������� �� ������ "���������" 
    afx_msg void OnBnVisibilityClicked();

    /// ������� �� ������ "�������" 
    afx_msg void OnBnDeleteClicked();

    /// �������� ��������� ����� � Dimensions
    void FromDialog() override;

    /// ����� ���������� �� �����
    void UpdateDialog();

protected:
	void Calc_ind(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SetTexts() override;

	void UpdateValues() override;
    void FillDataFromXml() override;
    void FillXMLfromDialog() override;

    CNCMDoc* _doc; // ��������

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();

    double _x, _y, _z; // ���������� �����
    float _r; // ������
    CString _textCoord;
    CString _textNormal;
    CString _textStatus;
	CMFCColorButton _color;// ����
	int _ind;
	CMFCButton _autoPT;	
	CMFCButton _intersectPT;
public:
	virtual BOOL OnInitDialog();
};
