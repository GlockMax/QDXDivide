#pragma once
#include "ResInv.h"

// TDialog dialog

class TDialog : public CDialog
{
	DECLARE_DYNAMIC(TDialog)

public:
	TDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_CellSize;
	int m_DXNumber;
	afx_msg void OnBnClickedOk();
	BOOL m_DebugMode;
	BOOL m_Smooth;
	BOOL m_DrawFaces;
	BOOL m_MultiThread;
	BOOL m_MarchCubes;
	BOOL m_Async;
	double m_Dich;//!!!
	double m_Maxl;//!!!
	unsigned int m_Step;//!!!
	BOOL m_UseSlicedTree;
	BOOL m_SurfelVis;
	virtual BOOL OnInitDialog();
};
