// NGridCellExp.cpp : implementation file
//

#include "stdafx.h"
#include "WM_APP_messages.h"
#include "NGridCellExp.h"

IMPLEMENT_DYNCREATE(NGridCellExp, CGridCell)

// NGridCellExp

NGridCellExp::NGridCellExp()
{
}

void NGridCellExp::OnEndEdit()
{
	Expression = GetText();
	CString SRes(Expression);
	AfxGetMainWnd()->SendMessage(WM_APP_GRID_EVALEXP, 0, (LPARAM)&SRes);
	SRes.TrimRight('0');
	SRes.TrimRight('.');
	SetText(SRes);
	CGridCell::OnEndEdit();
}

BOOL NGridCellExp::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
	if (!m_bEditing)
		if(!Expression.IsEmpty())
			SetText(Expression);
	return CGridCell::Edit(nRow, nCol, rect, point, nID, nChar);
}

void  NGridCellExp::SetText(LPCTSTR szText)
{
	if (*szText == '\0')
		CGridCell::SetText(CString(_T("0")));
	else
		CGridCell::SetText(szText);
}



