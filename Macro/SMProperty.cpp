#include "pch.h"
#include "SMParameter.h"
#include "SMDialog.h"
#include "SMEdit.h"
#include "SMProperty.h"

SMProperty::SMProperty(SMParameter& par, SMDialog* pDialog)
    : par_(par), CMFCPropertyGridProperty(par.desc_, COleVariant(par.value_), par.help_), parentDialog_(pDialog)
{
    if (!par.allowAlpha_ && !par_.comboBoxEnabled_)
    {
        CString minVal = par.min_;
        bool minPresent = FormatNumber(minVal);
        CString maxVal = par.max_;
        bool maxPresent = FormatNumber(maxVal);
        if (minPresent || maxPresent)
        {
            CString descr;
            descr.Format(_T(" (min: %s max: %s)"), minVal, maxVal);
            SetDescription(par.help_ + descr);
        }
    }
    if (par_.comboBoxEnabled_)
    {
        SMParameter::ComboStrings strs;
        ParseComboBoxCFG(strs);
        for each (auto el in strs)
            AddOption(el.first);
        AllowEdit(FALSE);
    }
}

SMProperty::~SMProperty()
{
}

HBRUSH SMProperty::OnCtlColor(CDC* pDC, UINT nCtlColor)
{
    //pDC->SetBkMode(OPAQUE);
    //pDC->SetBkColor(RGB(200, 255, 0));
   return CMFCPropertyGridProperty::OnCtlColor(pDC, nCtlColor);
    //auto m_pEditBkBrush = new CBrush(RGB(0, 0, 0));
    //pDC->SetTextColor(RGB(0, 255, 0));
    //pDC->SetBkColor(RGB(0, 0, 0));
    //return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
 }

void SMProperty::Update()
{
    SetValue(COleVariant(par_.value_));
}

void SMProperty::OnSetSelection(CMFCPropertyGridProperty*)
{
    if (parentDialog_ == nullptr)
        return;
    parentDialog_->LoadParamPicture(par_.paramPicture_);
}

BOOL SMProperty::OnEndEdit()
{
    return CMFCPropertyGridProperty::OnEndEdit();
}

void SMProperty::SetValue(const COleVariant& varValue)
{
    CMFCPropertyGridProperty::SetValue(varValue);
}

BOOL SMProperty::PushChar(UINT nChar)
{
    return CMFCPropertyGridProperty::PushChar(nChar);
}

CString SMProperty::FormatProperty()
{
    auto val = CString(GetValue().bstrVal);
    if(val.IsEmpty() || par_.allowAlpha_ || par_.comboBoxEnabled_)
        return CMFCPropertyGridProperty::FormatProperty();
    // a number
    FormatNumber(val);
    return val;
}

bool SMProperty::FormatNumber(CString& val) const
{
    if (val.IsEmpty())
        return false;
    if (par_.decimals_ == 0)
    {// integer
        int ival = atoi(val);
        if (ival < 0)
            val.Delete(0);
        if (par_.digits_ > 0) // 0 means any number of digits
        {
            int zerNum = par_.digits_ - val.GetLength();
            for (int i = 0; i < zerNum; ++i)
                val = '0' + val;
        }
        if (ival < 0)
            val = '-' + val;
        else if (par_.useSign_)
            val = '+' + val;
        return val;
    }
    else
    {//
        if (par_.decimals_ > 0)
        {
            double dval = atof(val);
            CString format(_T(".%d"));
            format.Format(format, par_.decimals_);
            if (par_.useSign_)
                format = _T("+") + format;
            format = _T("%") + format + _T("lf");
            val.Format(format, dval);
            if (par_.digits_ != 0)// 0 - any
            {
                int insertInd = 0;
                if (val[0] == '-' || val[0] == '+')
                    insertInd = 1;
                int dotInd = val.Find('.', 0);
                if (dotInd >= 0)
                {
                    for (int i = 0; i < par_.digits_ - (dotInd - insertInd); ++i)
                        val.Insert(insertInd, '0');
                }
            }
        }
    }
    return true;
}

CWnd* SMProperty::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
    SMEdit* pWndEdit = new SMEdit(par_, parentDialog_);

    DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL;
        
    DWORD addStyle = 0;
    if (par_.allowAlpha_)
    {
        addStyle |= par_.upperCase_ ? ES_UPPERCASE : 0;
    }
    dwStyle |= addStyle;

    if (!m_bEnabled || !m_bAllowEdit)
    {
        dwStyle |= ES_READONLY;
    }

    pWndEdit->Create(dwStyle, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);

    bDefaultFormat = TRUE;
    return pWndEdit;
}

CString SMProperty::GetValueTooltip()
{
    return CMFCPropertyGridProperty::GetValueTooltip();
}

bool SMProperty::ParseComboBoxCFG(SMParameter::ComboStrings& strs) const
{
    if (!par_.comboBoxEnabled_ || par_.comboBoxCFG_.IsEmpty())
        return false;
    strs.clear();
    CString str = par_.comboBoxCFG_;
    if (str[0] != '[' || str[str.GetLength() - 1] != ']')
        return false;
    str.Delete(0);
    str.Delete(str.GetLength() - 1);
    for (bool stop = false; !str.IsEmpty() && !stop;)
    {
        CString parenthPart;
        int commaInd = str.Find(_T("},"), 0);
        ++commaInd;
        if (commaInd == 0)
        {
            parenthPart = str;
            stop = true;
        }
        else
        {
            parenthPart = str.Left(commaInd);
            str.Delete(0, commaInd + 1);
        }
        if (parenthPart.IsEmpty())
            continue;
        if (parenthPart[0] != '{' || parenthPart[parenthPart.GetLength() - 1] != '}')
            return false;
        parenthPart.Delete(0);
        parenthPart.Delete(parenthPart.GetLength() - 1);
        int ind = 0;
        CString label = parenthPart.Tokenize(_T(","), ind);
        int ind1 = 0;
        label.Tokenize(_T(":"), ind1);// skip "Label"
        label = label.Tokenize(_T(":"), ind1);
        label = label.Mid(1, label.GetLength() - 2);
        CString value = parenthPart.Tokenize(_T(","), ind);
        ind1 = 0;
        value.Tokenize(_T(":"), ind1);// skip "Value"
        value = value.Tokenize(_T(":"), ind1);
        value = value.Mid(1, value.GetLength() - 2);
        strs.push_back(SMParameter::LabelVal(label, value));
    }
    return false;
}
