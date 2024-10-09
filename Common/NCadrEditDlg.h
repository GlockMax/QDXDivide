#pragma once
#include <afxdialogex.h>
class AFX_EXT_CLASS NCadrEditDlg : public CDialogEx
{
public:
    NCadrEditDlg(CWnd* pParent);
    ~NCadrEditDlg() override;
    const CString& GetCadrText() const noexcept { return cadrText; }
protected:
    CString cadrText;
    const class NCadrGeom* pCadrGeom;
public:
    INT_PTR DoModal(const CString& text, const class NCadrGeom* pGeom);
};

