#pragma once
#include "ResLocal.h"
#include "QDivideBase.h"


// QDivideDlg dialog

class AFX_EXT_CLASS QDivideDlg : public CDialog
{
	DECLARE_DYNAMIC(QDivideDlg)

public:
	QDivideDlg(CWnd* pParent, QDivideBase*& pDivide);   // standard constructor
	virtual ~QDivideDlg();

	void SetQDivide(QDivideBase*& pDivide);

// Dialog Data
	enum { IDD = IDD_DIVIDE };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	void QDivideDlg::OnCancel() override;
	void PostNcDestroy() override;
	//virtual void QDivideDlg::CheckRadioButton();

	QDivideBase*& pQDivide;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRevert();
	afx_msg void OnBnClickedDel1();
	afx_msg void OnBnClickedDel2();
};
