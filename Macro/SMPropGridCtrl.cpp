#include "pch.h"
#include "SMProperty.h"
#include "SMDefiniton.h"
#include "SMPropGridCtrl.h"

SMPropGridCtrl::SMPropGridCtrl() : CMFCPropertyGridCtrl()
{
    pDef_ = nullptr;
}

int SMPropGridCtrl::OnDrawProperty(CDC* pDC, CMFCPropertyGridProperty* pProp) const
{
    SMProperty* pSMProp = static_cast<SMProperty*>(pProp);
    auto col = RGB(200, 255, 200);
    if (pSMProp->par_.IsRequired())
        col = RGB(255, 255, 200);

    if (!pSMProp->par_.IsMinMaxValid(CString(pProp->GetValue().bstrVal)))
        pDC->SetTextColor(RGB(255, 100, 100));

    if (!pProp->IsSelected())
    {
        auto leftWidth = GetLeftColumnWidth();
        CRect rect = pProp->GetRect();
        rect.right = rect.left + leftWidth;
        rect.bottom -= 1;
        pDC->FillSolidRect(rect, col);
    }
    auto res = CMFCPropertyGridCtrl::OnDrawProperty(pDC, pProp);
    return res;
}


void SMPropGridCtrl::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
    SMProperty* pSMProp = static_cast<SMProperty*>(pProp);
    pProp->SetValue(pSMProp->FormatProperty());
    GetValuesFromCtrl();
    UpdateFunctionsVal();
    CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

void SMPropGridCtrl::SetDef(SMDefiniton* pDef)
{
    pDef_ = pDef;
}

void SMPropGridCtrl::GetValuesFromCtrl() const
{
    if (pDef_ == nullptr)
        return;
    for (int i = 0, k = 0; i < pDef_->params_.size(); ++i)
    {
        if (!pDef_->hideFormulas_ || !pDef_->params_.at(k).GetIsFunc())// don't show func parameter if hideFormulas_
        {
            pDef_->params_.at(i).SetStrVal(GetProperty(k));
            ++k;
        }
    }
}

void SMPropGridCtrl::UpdateFunctionsVal() const
{
    if (pDef_ == nullptr)
        return;
    pDef_->RecalcFunctions();
    if (pDef_->hideFormulas_)
        return;
    for (int i = 0; i < GetPropertyCount(); ++i)
    {
        SMProperty& prop = static_cast<SMProperty&>(*GetProperty(i));
        if (prop.GetParameter().GetIsFunc())
            prop.Update();
    }
}

