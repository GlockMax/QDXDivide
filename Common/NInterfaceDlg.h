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
	bool m_bBuildState;	// ������� ��������� (������ - true ��� ��� - false)
	bool m_bBuildMesh;  // ��� ������ ? - ������
	bool m_bBuildSur;	// ��� ������ ? - �����������
	bool m_bViewSur;	// ��� ������� ? - �����������
	bool m_bViewMesh;	// ��� ������� ? - ����
	bool m_bViewPoint;	// ��� ������� ? - ���� � ���� �����
	bool m_bViewNum;	// ��� ������� ? - ������ �����
	BOOL m_bViewNorm;	// ��� ������� ? - ������� �����
	bool m_bSound;		// ������������� ��������� �������
	
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
