#pragma once

#include "ResLocal.h"
#include <vector>

// MTEditStlNameDlg dialog

/// Диалог со списком STL файлов для массовой загрузки stl файлов
class AFX_EXT_CLASS MTEditStlNamesDlg : public CDialog
{
	DECLARE_DYNAMIC(MTEditStlNamesDlg)

public:
	INT_PTR DoModal(const CString &path);
protected:
	CString m_path;
	virtual void DoDataExchange(CDataExchange* pDX);    //!< DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	MTEditStlNamesDlg(CWnd* pParent = NULL);   //!< standard constructor
	virtual ~MTEditStlNamesDlg();

// Dialog Data
	enum { IDD = IDD_PART_OPEN_MANY_STL };
	std::vector <CString> stringList;

protected:
	CListBox	m_ListBox;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};//class AFX_EXT_CLASS MTEditStlNameSDlg : public CDialog