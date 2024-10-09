#pragma once
#include "NFloatEdit.h"
#include "ResLocal.h"

// CrStockTemp dialog


class CrStockTemp : public CDialogEx
{
	DECLARE_DYNAMIC(CrStockTemp)

public:
	CrStockTemp(class CNCMDoc *pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~CrStockTemp();
	void Init();
    void UpdateDialog();
	void Close();

// Dialog Data
	enum { IDD = IDD_STOCK_TEMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCalc();
	afx_msg void OnTempStVis();
	DECLARE_MESSAGE_MAP()
public:
	class CNCMDoc *_doc;
	double TempStCen_X;
	double TempStCen_Y;
	double TempStCen_Z;
	double TempStSize_X;
	double TempStSize_Y;
	double TempStSize_Z;
	NFloatEdit CTempStCen_X;
	NFloatEdit CTempStCen_Y;
	NFloatEdit CTempStCen_Z;
	NFloatEdit CTempStSize_X;
	NFloatEdit CTempStSize_Y;
	NFloatEdit CTempStSize_Z;
	BOOL DepthTest;

	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
