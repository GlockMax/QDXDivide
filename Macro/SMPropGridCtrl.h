#pragma once
#include <afxpropertygridctrl.h>
class SMPropGridCtrl : public CMFCPropertyGridCtrl
{
public:
    SMPropGridCtrl();
    int OnDrawProperty(CDC* pDC, CMFCPropertyGridProperty* pProp) const override;
    void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const override;
    void SetDef(class SMDefiniton* pDef);
    void GetValuesFromCtrl() const;
    void UpdateFunctionsVal() const;
protected:
    class SMDefiniton* pDef_;
};

