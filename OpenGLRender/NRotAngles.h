#pragma once
#include "ResLocal.h"

// NRotAngles dialog

class NRotAngles : public CDialogEx
{
	DECLARE_DYNAMIC(NRotAngles)

public:
	NRotAngles(CWnd* pParent = NULL);   // standard constructor
	virtual ~NRotAngles();

// Dialog Data
	enum { IDD = IDD_ROTANGLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void Apply(void);
	class BOpenGLWnd* pActiveWnd;
	bool BufRotCenterVis;

	DECLARE_MESSAGE_MAP()
public:
	double m_X;
	double m_Y;
	double m_Z;
	afx_msg void OnBnClickedRotangcountXMinus();
	afx_msg void OnBnClickedRotangcountX();
	afx_msg void OnBnClickedRotangcountYMinus();
	afx_msg void OnBnClickedRotangcountY();
	afx_msg void OnBnClickedRotangcountZMinus();
	afx_msg void OnBnClickedRotangcountZ();
	afx_msg void OnClose();
	virtual INT_PTR DoModal(class BOpenGLWnd* pWnd);
};
