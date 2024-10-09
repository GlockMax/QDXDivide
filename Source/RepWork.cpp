// RepWork.cpp : implementation file
//

#include "stdafx.h"
#include "ncm.h"
#include "RepWork.h"
#include "resource.h"
#include "RepEditor.h"
#include "MainFrm.h"
#include "NCMDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CNCMApp theApp;

/////////////////////////////////////////////////////////////////////////////
// RepWork dialog


RepWork::RepWork(CWnd* pParent /*=NULL*/)
	: CDialog(RepWork::IDD, pParent)
{
	//{{AFX_DATA_INIT(RepWork)
	m_ForAdd = _T("");
	m_ForRemove = _T("");
	//}}AFX_DATA_INIT
	NumOutItems=0;
}


void RepWork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RepWork)
	DDX_LBString(pDX, IDC_REPORTS_LIST_ACCESSEBLEREPORTS, m_ForAdd);
	DDX_LBString(pDX, IDC_REPORTS_LIST_SELLECTEDREPORTS, m_ForRemove);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RepWork, CDialog)
	//{{AFX_MSG_MAP(RepWork)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REPORTS_BUTTON_LOADREPORT, OnButtonLoadReport)
	ON_BN_CLICKED(IDC_REPORTS_EXIT, OnExit)
	ON_BN_CLICKED(IDC_REPORTS_BUTTON_UNLOADREPORT, OnButtonUnloadReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RepWork message handlers

BOOL RepWork::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *CheckB;
	CheckB=(CButton*)GetDlgItem(IDC_REPORTS_CHECK_ACTIVE);
	if(Active)
		CheckB->SetCheck(1);
	ReNew();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void RepWork::ReNew()
{
	CListBox *ListACCESS,*ListOut;
	int F=0,i=0;
	ListACCESS=(CListBox*)GetDlgItem(IDC_REPORTS_LIST_ACCESSEBLEREPORTS);
	ListOut=(CListBox*)GetDlgItem(IDC_REPORTS_LIST_SELLECTEDREPORTS);
	ListACCESS->ResetContent();
	ListOut->ResetContent();
	CString fname,InRep;
	CString path = AfxGetApp()->GetProfileString(((CNCMApp *)AfxGetApp())->Version,
			"MainPath");
	path=path+"\\";
	fname.LoadString(IDS_REPORT_DEF);
	InRep=((CNCMDoc *)((CMainFrame *)GetParent())->GetActiveDocument())->pReps->
		GetFirstOfAvailableReports(path+fname);
//	ListACCESS->AddString(InRep);
	while(InRep!="")
	{
		F=0;
		for(i=0;i<NumOutItems;i++)
		{
			if(InRep==OutList[i])
				F=1;
		}
		if(F!=1)
			ListACCESS->AddString(InRep);
		InRep=((CNCMDoc *)((CMainFrame *)GetParent())->GetActiveDocument())->pReps->
			GetNextAvailableReports(path+fname);
	}
	for(i=0;i<NumOutItems;i++)
		ListOut->AddString(OutList[i]);

}

void RepWork::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	if(NumOutItems!=0)
//		delete[] OutList;
	
}

void RepWork::AddToOutList(CString str)
{
	if(NumOutItems!=0)
	{
		CString *Old;
		Old=new CString [NumOutItems];
		int i;
		for(i=0;i<NumOutItems;i++)
			Old[i]=OutList[i];
		delete[] OutList;
		NumOutItems++;
		OutList=new CString [NumOutItems];
		for(i=0;i<(NumOutItems-1);i++)
			OutList[i]=Old[i];
		OutList[NumOutItems-1]=str;
		delete[] Old;
	}
	else
	{
		NumOutItems=1;
		OutList=new CString[NumOutItems];
		OutList[NumOutItems-1]=str;
	}
}

void RepWork::OnButtonLoadReport() 
{
	// TODO: Add your control notification handler code here
	RepWork::UpdateData();
	if(m_ForAdd!="")
		AddToOutList(m_ForAdd);
	ReNew();
}

void RepWork::OnExit() 
{
	// TODO: Add your control notification handler code here
	CButton *CheckB;
	CheckB=(CButton*)GetDlgItem(IDC_REPORTS_CHECK_ACTIVE);
	if(CheckB->GetCheck()==1)
		Active=true;
	else 
		Active=false;
	CDialog::OnOK();
}

void RepWork::OnButtonUnloadReport() 
{
	// TODO: Add your control notification handler code here
	RepWork::UpdateData();
	int i;
	if(m_ForRemove!="")
	{
		for(i=0;i<NumOutItems;i++)
		{
			if(OutList[i]==m_ForRemove)
			{
				RemoveFromOutList(i);
				break;
			}
		}
		ReNew();
	}

	
}

void RepWork::RemoveFromOutList(int n)
{
	if(NumOutItems==0)
		return;
	if(n>(NumOutItems-1))
		return;
	if(NumOutItems==1)
	{
		delete[] OutList;
		NumOutItems--;
		return;
	}
	int i;
	CString *Old;
	Old=new CString [NumOutItems];
	for(i=0;i<NumOutItems;i++)
		Old[i]=OutList[i];
	delete[] OutList;
	NumOutItems--;
	OutList=new CString [NumOutItems];
	for(i=0;i<n;i++)
	{
		OutList[i]=Old[i];
	}
	for(i=n+1;i<(NumOutItems+1);i++)
		OutList[(i-1)]=Old[i];
	delete[] Old;
}

//DEL void RepWork::OnButtonNewReport() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	RepEditor dRepEdit;
//DEL 	dRepEdit.DoModal();
//DEL 	ReNew();
//DEL 	
//DEL }


