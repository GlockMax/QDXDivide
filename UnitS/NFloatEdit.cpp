// NFloatEdit.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "float.h"
#include "ResLocal.h"
#include "NFloatEdit.h"



// NFloatEdit

IMPLEMENT_DYNAMIC(NFloatEdit, CEdit)

NFloatEdit::NFloatEdit()
{
}

NFloatEdit::~NFloatEdit()
{
}


BEGIN_MESSAGE_MAP(NFloatEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// NFloatEdit message handlers

void NFloatEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	CString str;
	GetWindowText(str);
	CString s1 = str.SpanIncluding("+-");
	if(s1.GetLength() > 1)
	{
		AfxMessageBox(IDS_MES_WNUM);
		return;
	}
	str.Delete(0, s1.GetLength());
	s1 = str.SpanIncluding("0123456789");
	str.Delete(0, s1.GetLength());
	if(str.GetLength() == 0)
		return;
	s1 = str.SpanIncluding(".");
	if(s1.GetLength() > 1)
	{
		AfxMessageBox(IDS_MES_WNUM);
		return;
	}
	str.Delete(0, s1.GetLength());
	if(str.GetLength() == 0)
		return;
	s1 = str.SpanIncluding("0123456789");
	str.Delete(0, s1.GetLength());
	if(str.GetLength() != 0)
	{
		AfxMessageBox(IDS_MES_WNUM);
		return;
	}
}

void NFloatEdit::DDX_Text_my(CDataExchange* pDX, int nIDC, double& value)
{
	pDX->PrepareEditCtrl(nIDC);
	HWND hWndCtrl;
	pDX->m_pDlgWnd->GetDlgItem(nIDC, &hWndCtrl);
	
	const int TEXT_BUFFER_SIZE = 400;
	TCHAR szBuffer[TEXT_BUFFER_SIZE];
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));
		double d;
		if (_sntscanf_s(szBuffer, _countof(szBuffer), _T("%lf"), &d) != 1)
		{
			AfxMessageBox(AFX_IDP_PARSE_REAL);
			pDX->Fail();            // throws exception
		}
		value = ceil(d * 1000.) / 1000;
	}
	else
	{
		value = ceil(value * 1000.) / 1000;
		ATL_CRT_ERRORCHECK_SPRINTF(_sntprintf_s(szBuffer, _countof(szBuffer), _countof(szBuffer) -1, _T("%.*g"), 15, value));
		AfxSetWindowText(hWndCtrl, szBuffer);
	}
}
