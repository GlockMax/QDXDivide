#pragma once
#include "NFloatEdit.h"
#include "ResLocal.h"
#include "NToolsCrCommonPage.h"

// NToolsCrBorePage dialog

class NToolsCrBorePage : public NToolsCrCommonPage
{
	DECLARE_DYNAMIC(NToolsCrBorePage)

public:
	NToolsCrBorePage();
	virtual ~NToolsCrBorePage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLS_CREATE_BORE };
#endif
	bool IsOkInput();
	void IsError(int IDErr) const;
	const CString& GetName() const;
	double CalcTorusRad() const;
	bool IsTorusBottom() const;

	CStatic	m_Draw2;
	CStatic	m_Draw;

	double	m_displacement;
	double	m_D;
	double	m_H;
	double	m_h;
	double	m_L;
	double	m_W;
	double	m_a;
	double	m_R;
	CString	m_name;
	class NToolVert *pToolVert;
	double	m_h1;
	NFloatEdit	m_R_ctrl;
	NFloatEdit	m_a_ctrl;
	NFloatEdit	m_W_ctrl;
	NFloatEdit	m_L_ctrl;
	CString	m_FileName;
	BOOL	m_check_file;
	BOOL m_FlipXY;

protected:
	bool IsNameAuto;
	CButton m_browse_ctrl;
	CEdit	m_name_ctrl;
	BOOL	m_check2;
	BOOL	m_Torus;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedCheckFile();
	afx_msg void OnDiskCheck2();
	afx_msg void OnChangeAlf();
	afx_msg void OnChangeW();
	afx_msg void OnChangeL();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
