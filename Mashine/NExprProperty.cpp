// NExprProperty.cpp : implementation file
//

#include "stdafx.h"
#include "WM_APP_messages.h"
#include "NExprProperty.h"


// NExprProperty
IMPLEMENT_DYNAMIC(NExprProperty, CMFCPropertyGridProperty)

NExprProperty::NExprProperty(const CString& strName, const COleVariant& varValue, const CString& strDesc) :CMFCPropertyGridProperty(strName, varValue, strDesc)
{
}

NExprProperty::~NExprProperty()
{
}

BOOL NExprProperty::OnEdit(LPPOINT lptClick)
{
	if (!m_bInPlaceEdit)
		if (!Expression.IsEmpty())
			CMFCPropertyGridProperty::SetValue((_variant_t)Expression);
	return CMFCPropertyGridProperty::OnEdit(lptClick);
}

BOOL NExprProperty::OnEndEdit()
{
	Expression = m_varValue;
	CString SRes(Expression);
	AfxGetMainWnd()->SendMessage(WM_APP_GRID_EVALEXP, 0, (LPARAM)&SRes);
	SRes.TrimRight('0');
	SRes.TrimRight('.');
	m_varValue = COleVariant(SRes);
	dVal = COleVariant(atof(SRes));
	return CMFCPropertyGridProperty::OnEndEdit();
}

void NExprProperty::SetValue(const COleVariant& varValue)
{
	if (varValue.vt == VT_R8)
	{
		CString str;
		str.Format("%#.4lf", varValue.dblVal);
		str.TrimRight('0');
		str.TrimRight('.');
		dVal = varValue.dblVal;
		Expression.Empty();
		CMFCPropertyGridProperty::SetValue((_variant_t)str);
	}
	else
		CMFCPropertyGridProperty::SetValue(varValue);

}

double NExprProperty::GetDValue() const
{
	return atof(CString(m_varValue.bstrVal));
}

void NExprProperty::SetOriginalValue(const COleVariant& varValue)
{
	if (varValue.vt == VT_R8)
	{
		CString str;
		str.Format("%#.4lf", varValue.dblVal);
		str.TrimRight('0');
		str.TrimRight('.');
		CMFCPropertyGridProperty::SetOriginalValue((_variant_t)str);
	}
	else
		CMFCPropertyGridProperty::SetOriginalValue(varValue);
}
