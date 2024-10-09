// NExprEdit.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "float.h"
#include "ResLocal.h"
#include "WM_APP_messages.h"
#include "NExprEdit.h"



// NExprEdit

IMPLEMENT_DYNAMIC(NExprEdit, CEdit)

NExprEdit::NExprEdit()
{
}

NExprEdit::~NExprEdit()
{
}


BEGIN_MESSAGE_MAP(NExprEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// NExprEdit message handlers

void NExprEdit::OnKillFocus(CWnd* pNewWnd)
{
	CString Expression;
	GetWindowText(Expression);
	AfxGetMainWnd()->SendMessage(WM_APP_GRID_EVALEXP, 0, (LPARAM)&Expression);
	Expression.TrimRight('0');
	Expression.TrimRight('.');
	SetWindowText(Expression);
	CEdit::OnKillFocus(pNewWnd);
}

void NExprEdit::DDX_Text_my(CDataExchange* pDX, int nIDC, double& value)
{
	pDX->PrepareEditCtrl(nIDC);
	HWND hWndCtrl;
	pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);
	
	const int TEXT_BUFFER_SIZE = 400;
	TCHAR szBuffer[TEXT_BUFFER_SIZE];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		CString Expression(szBuffer);
		AfxGetMainWnd()->SendMessage(WM_APP_GRID_EVALEXP, 0, (LPARAM)&Expression);
		Expression.TrimRight('0');
		Expression.TrimRight('.');
		double d;
		if (_sntscanf_s(Expression.GetBuffer(), Expression.GetLength(), _T("%lf"), &d) != 1)
		{
			AfxMessageBox(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		value = floor(d * 1000. + 0.5) / 1000;
	}
	else
	{
		value = floor(value * 1000. + 0.5) / 1000;
		ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer) -1, _T("%.*g"), 15, value));
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}
