
// MainDlg.h: файл заголовка
//

#pragma once
#include <vector>

// Диалоговое окно CMainDlg
class CMainDlg : public CDialogEx
{
// Создание
public:
	CMainDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ENCRYPT_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLicense();
	CString clientName;
	CString clientID;
	CString PathPPFileName;
	CString pathName;
	afx_msg void OnBnClickedBtnGetPpfile();
	afx_msg void OnBnClickedBtnGetLicense();
	afx_msg void OnBnClickedButtonDelete();
	std::vector<CString> vector_ppfiles;
};
