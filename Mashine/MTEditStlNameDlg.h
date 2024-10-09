#pragma once

/*!
\file MTEditStlName.h
\brief Описание классов MTEditStlNameDlg и CCustomDlgProp
Перегруженный элемент списка свойств для выбора stl файла детали.
Диалог показывает список stl файлов в каталоге текущего станка.
*/

#include "ResLocal.h"

// MTEditStlNameDlg dialog

/// Диалог со списком STL файлов
class AFX_EXT_CLASS MTEditStlNameDlg : public CDialog
{
	DECLARE_DYNAMIC(MTEditStlNameDlg)

public:
	INT_PTR DoModal(const CString &path, const CString &name);
protected:
	CString m_path;
	virtual void DoDataExchange(CDataExchange* pDX);    //!< DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	MTEditStlNameDlg(CWnd* pParent = NULL);   //!< standard constructor
	virtual ~MTEditStlNameDlg();

// Dialog Data
	enum { IDD = IDD_PART_OPEN_STL };
	CString	m_name;

protected:
	CListBox	m_ListBox;

	afx_msg void OnDblclkStlName();

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};//class AFX_EXT_CLASS MTEditStlNameDlg : public CDialog



class CCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	CCustomDlgProp(const CString& strName, const COleVariant& varValue, const CString & strDescr);
	void SetMachinePath(const CString &path);

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);

	CString MachinePath;
};


