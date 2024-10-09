// NRepDlgActiveReports.cpp : implementation file
//

#include "stdafx.h"
#include "NRepDlgActiveReports.h"
#include ".\nrepdlgactivereports.h"
#include "NRepReportsArray.h"


// NRepDlgActiveReports dialog

IMPLEMENT_DYNAMIC(NRepDlgActiveReports, CDialog)
NRepDlgActiveReports::NRepDlgActiveReports(CWnd* pParent /*=NULL*/)
	: CDialog(NRepDlgActiveReports::IDD, pParent)
{
}

NRepDlgActiveReports::~NRepDlgActiveReports()
{
}

void NRepDlgActiveReports::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NRepDlgActiveReports, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// NRepDlgActiveReports message handlers

BOOL NRepDlgActiveReports::OnInitDialog()
{
	CDialog::OnInitDialog();
	ListPassive=(CListBox*)GetDlgItem(IDC_LIST_PASSIVE);
	ListActive=(CListBox*)GetDlgItem(IDC_LIST_ACTIVE);
	int i;
	for(i=0;i<pRepArray->GetSize();i++)
	{
		if((*pRepArray)[i].GetMode()==0)
			ListPassive->AddString((*pRepArray)[i].GetName());
		else
			ListActive->AddString((*pRepArray)[i].GetName());
	}
	return TRUE;
}

void NRepDlgActiveReports::OnBnClickedButtonAdd()
{
	int k;
	k=ListPassive->GetCurSel();
	if(k==LB_ERR)
		return;
	CString str;
	ListPassive->GetText(k,str);
	ListPassive->DeleteString(k);
	if(k<ListPassive->GetCount())
	{
		ListPassive->SetCurSel(k);
	}
	else
	{
		ListPassive->SetCurSel(ListPassive->GetCount()-1);
	}
	ListActive->AddString(str);
}

void NRepDlgActiveReports::OnBnClickedButtonRemove()
{
	int k;
	k=ListActive->GetCurSel();
	if(k==LB_ERR)
		return;
	CString str;
	ListActive->GetText(k,str);
	ListActive->DeleteString(k);
	if(k<ListActive->GetCount())
	{
		ListActive->SetCurSel(k);
	}
	else
	{
		ListActive->SetCurSel(ListActive->GetCount()-1);
	}
	ListPassive->AddString(str);
}

void NRepDlgActiveReports::OnBnClickedOk()
{
	CString str;
	int i,k;
	for(i=0;i<ListPassive->GetCount();i++)
	{
		ListPassive->GetText(i,str);
		k=pRepArray->FindReport(str);
		if(k!=-1)
		{
			(*pRepArray)[k].SetMode(0);
		}
	}
	for(i=0;i<ListActive->GetCount();i++)
	{
		ListActive->GetText(i,str);
		k=pRepArray->FindReport(str);
		if(k!=-1)
		{
			(*pRepArray)[k].SetMode(1);
		}
	}
	OnOK();
}
