#pragma once
#include "..\surface\resource.h"
#include "afxcmn.h"

// NInterfaceDlg dialog

class AFX_EXT_CLASS NInterfaceDlg : public CDialog
{
	DECLARE_DYNAMIC(NInterfaceDlg)

public:
	NInterfaceDlg(CWnd* pParent);   // standard constructor
	virtual ~NInterfaceDlg();

// Dialog Data
	enum { IDD = IDC_TYPE_TREE };

protected:
	CWnd* m_pParent;
	int m_nID;
	bool m_bBuildState;	// текущее состояние (строим - true или нет - false)
	bool m_bBuildMesh;  // что строим ? - дерево
	bool m_bBuildSur;	// что строим ? - поверхности
	bool m_bViewSur;	// что смотрим ? - поверхности
	bool m_bViewMesh;	// что смотрим ? - сеть
	bool m_bViewPoint;	// что смотрим ? - сеть в виде точек
	bool m_bViewNum;	// что смотрим ? - номера точек
	BOOL m_bViewNorm;	// что смотрим ? - нормали сеток
	bool m_bSound;		// использование сзувокого сигнала
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBuon1();
	afx_msg void OnBnClickedBuildMesh();
	afx_msg void OnBnClickedBuildSur();
	afx_msg void OnBnClickedTypeMesh();
	afx_msg void OnBnClickedTypePl();
	afx_msg void OnBnClickedTypeTr();
	afx_msg void OnBnClickedBreak();
	afx_msg void OnBnClickedBuild();
	afx_msg void OnBnClickedSound();
	afx_msg void OnBnClickedNum();
	afx_msg void OnBnClickedNorm();
	virtual BOOL OnInitDialog();
protected:
public:
	LPARAM GetViewType(void);
	LPARAM GetJobType(void);
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	BOOL Create(void);
	bool IsSound(void);
	void NUpdate(void);
	void SetInitSate(bool VolumeOn);
protected:
public:
	afx_msg void OnBnClickedTypeSur();
	afx_msg void OnBnClickedTypeBsp();
	afx_msg void OnBnClickedBuildInit();
};
