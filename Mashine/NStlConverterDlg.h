#pragma once
#include <map>

// NStlConverterDlg dialog

class NStlConverterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NStlConverterDlg)

public:
	NStlConverterDlg(enum DocElements DEl, CWnd* pParent = nullptr);   // standard constructor
	virtual ~NStlConverterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STL_CONVERTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString SrcFileName;
	CString DestFileName;
	CComboBox FormatCtrl;
	int Format;
protected:
	double Accuracy;
	double MinAngle;
	enum DocElements LastDEl;
	static std::map<enum DocElements, std::pair<double /*Accuracy*/, double/*MinAngle*/>> AccValues;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	double GetAccuracy() const { return Accuracy; }
	double GetMinAngle() const { return MinAngle; }
	static void ResetAccValues();
};
