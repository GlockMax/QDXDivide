#pragma once
#include <vector>
#include <afxpropertygridctrl.h>
#include "SMParameter.h"

class SMProperty : public CMFCPropertyGridProperty
{
    friend class SMPropGridCtrl;
public:
    SMProperty(class SMParameter& par, class SMDialog* pDialog);
    ~SMProperty();
    HBRUSH OnCtlColor(CDC* pDC, UINT nCtlColor) override;
    void Update();
    bool ParseComboBoxCFG(SMParameter::ComboStrings& strs) const;
    const auto& GetParameter() const { return par_; }
protected:
    SMParameter& par_;
    class SMDialog* parentDialog_;
protected:
    void OnSetSelection(CMFCPropertyGridProperty* /*pOldSel*/) override;
    BOOL OnEndEdit() override;
    void SetValue(const COleVariant& varValue) override;
    BOOL PushChar(UINT nChar) override;
    CString FormatProperty() override;
    bool FormatNumber(CString& val) const;
    CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat) override;
    CString GetValueTooltip() override;
};

