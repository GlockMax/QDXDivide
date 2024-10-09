#pragma once
#include <afxdialogex.h>
#include <ResLocal.h>
class NTextCompDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NTextCompDlg)
	DECLARE_MESSAGE_MAP()
	enum { IDD = IDD_TEXT_COMPARE};
public:
	NTextCompDlg(class CNCMDoc* pDoc, CWnd* pParent = NULL);
	virtual ~NTextCompDlg();
	virtual BOOL OnInitDialog();
	void AdjustListWidth();
	const CString& GetName() const { return name_; }
	bool IsFileSelected() const { return fileSelected_; }
	int GetProgInd() const { return progInd_; }
	INT_PTR DoModal(int selProgInd);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnTcnSelChangeList();
	afx_msg void OnLoadFile();
protected:
	bool fileSelected_;// is file selected
	CString name_;// the name of the selected file
	int progInd_;// index of selected program
	CListBox listCtrl_;
	CNCMDoc& doc_;
	int selProgInd_;// index of the program to be ignored
};

