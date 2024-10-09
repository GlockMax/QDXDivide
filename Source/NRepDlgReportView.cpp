// NRepDlgReportView.cpp : implementation file
//

#include "stdafx.h"
#include "NCMVersion.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "NRepDlgReportView.h"


// NRepDlgReportView dialog

IMPLEMENT_DYNAMIC(NRepDlgReportView, CDialog)
NRepDlgReportView::NRepDlgReportView(CWnd* pParent /*=NULL*/)
	: CDialog(NRepDlgReportView::IDD, pParent)
{
}

NRepDlgReportView::~NRepDlgReportView()
{
}

void NRepDlgReportView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NRepDlgReportView, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TABLE, OnTcnSelchangeTabTable)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
END_MESSAGE_MAP()


// NRepDlgReportView message handlers

BOOL NRepDlgReportView::OnInitDialog()
{
	CDialog::OnInitDialog();

	Tabs=(CTabCtrl*)GetDlgItem(IDC_TAB_TABLE);
	List=(CListCtrl*)GetDlgItem(IDC_LIST_REPORT);
	int i;
	CString str;
	for(i=0;i<pReps->GetSize();i++)
	{
		str=(*pReps)[i].GetName();
		Tabs->InsertItem(i,str);
	}
	Tabs->SetCurSel(0);
	m_SelectedTab=0;
	FillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void NRepDlgReportView::FillList(void)
{
	CString str;
	CStdioFile file;
	if(m_SelectedTab==-1||m_SelectedTab>=pReps->GetSize())
		return;
	List->DeleteAllItems();
	int k,i;
	k=List->GetHeaderCtrl()->GetItemCount();
	for(i=0;i<k;i++)
		List->DeleteColumn(0);
	str = NCM_PROJECT.GetPrototypesPath() + _T("temp\\");
	str+=(*pReps)[m_SelectedTab].GetName() + _T(".tmp");
	if(!file.Open(str,CFile::typeText|CFile::modeRead))
		return;
	file.SeekToBegin();
	CStringArray sa;
	(*pReps)[m_SelectedTab].GetTabs(sa);
	for(i=0;i<sa.GetSize();i++)
	{
		List->InsertColumn(i,sa[i],LVCFMT_LEFT,50);
	}
	int itemn,fildn;
	CString temp;
	itemn=0;
	while(file.ReadString(str))
	{
		if(str.Find("|")==-1)
			continue;
		fildn=0;
		k=str.Find("|");
		List->InsertItem(itemn,"");
		while(k!=-1)
		{
			temp=str.Left(k);
			str.Delete(0,k+1);
			List->SetItemText(itemn,fildn,temp);
			fildn++;
			k=str.Find("|");
		}
		itemn++;
	}
	file.Close();
}

void NRepDlgReportView::OnTcnSelchangeTabTable(NMHDR *pNMHDR, LRESULT *pResult)
{
	int k;
	k=Tabs->GetCurSel();
	m_SelectedTab=k;
	FillList();
	*pResult = 0;
}

void NRepDlgReportView::OnBnClickedButtonSave()
{
	CString str,mes;
	CFileDialog fd(false);
	if(fd.DoModal()!=IDOK)
		return;
	str=fd.GetPathName();
	CStdioFile file;
if(!file.Open(str,CFile::typeText|CFile::modeCreate|CFile::modeWrite))
	{
		mes.LoadString(IDS_REP_WRONGPATH);
		AfxMessageBox(mes);
		return;
	}
	if(m_SelectedTab<0||m_SelectedTab>=pReps->GetSize())
		return;
	int i,j,k;
	str="";
	str.LoadString(IDS_REP_HEAD);
	str+=" "+(*pReps)[m_SelectedTab].GetName()+"\n";
	file.WriteString(str);
	str="";
	str.LoadString(IDS_REP_CONDITION);
	str+=" "+(*pReps)[m_SelectedTab].GetCondition()+"\n";
	file.WriteString(str);
	str="";
	str.LoadString(IDS_REP_DESCRIPTION);
	str+="\n"+(*pReps)[m_SelectedTab].GetDescription()+"\n";
	file.WriteString(str);
	str="";
	str.LoadString(IDS_REP_RESTABLE);
	str+="\n";
	file.WriteString(str);
	str="";
	CStringArray sa;
	(*pReps)[m_SelectedTab].GetTabs(sa);
	for(i=0;i<sa.GetSize();i++)
	{
		str+="|"+sa[i];
		for(j=sa[i].GetLength();j<14;j++)
		{
			str+=" ";
		}
	}
	str+="|\n";
	file.WriteString(str);
	CString temp;
	for(i=0;i<List->GetItemCount();i++)
	{
		str="";
		for(j=0;j<sa.GetSize();j++)
		{
			temp=List->GetItemText(i,j);
			str+="|"+temp;
			for(k=temp.GetLength();k<14	;k++)
			{
				str+=" ";
			}
		}
		str+="|\n";
		file.WriteString(str);
	}
	file.Close();
}
