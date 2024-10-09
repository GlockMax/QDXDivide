// NRepDlgRepEditor.cpp : implementation file
//

#include "stdafx.h"
#include "NCMVersion.h"
#include "NEvalExp.h"
#include "NCUnit.h"
#include "NCMProject.h"
#include "NRepDlgRepEditor.h"


// NRepDlgRepEditor dialog

IMPLEMENT_DYNAMIC(NRepDlgRepEditor, CDialog)
NRepDlgRepEditor::NRepDlgRepEditor(CWnd* pParent /*=NULL*/)
	: CDialog(NRepDlgRepEditor::IDD, pParent)
	, pReps(NULL)
	, fEnableEdit(false)
	, NewRep(false)
	, m_Selected(0)
{
}
NRepDlgRepEditor::~NRepDlgRepEditor()
{
}
void NRepDlgRepEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(NRepDlgRepEditor, CDialog)
	ON_LBN_SELCHANGE(ID_LIST_REPDLG_REPS, OnLbnSelchangeListReps)
//	ON_LBN_DBLCLK(ID_LIST_REPDLG_REPS, OnLbnDblclkListReps)
ON_BN_CLICKED(IDC_BUTTON_REMOVE_REP, OnBnClickedButtonRemoveRep)
ON_BN_CLICKED(IDC_BUTTON_ADD_REP, OnBnClickedButtonAddRep)
ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON_ADD_FILD, OnBnClickedButtonAddFild)
ON_BN_CLICKED(IDC_BUTTON_REMOVE_FILD, OnBnClickedButtonRemoveFild)
ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
ON_BN_CLICKED(IDOK, OnBnClickedOk)
ON_LBN_DBLCLK(IDC_LIST_UNIT_PARAM, OnLbnDblclkListUnitParam)
ON_BN_CLICKED(IDC_BUTTON_PLUS, OnBnClickedButtonPlus)
ON_BN_CLICKED(IDC_BUTTON_MIN, OnBnClickedButtonMin)
ON_BN_CLICKED(IDC_BUTTON_MULT, OnBnClickedButtonMult)
ON_BN_CLICKED(IDC_BUTTON_DEV, OnBnClickedButtonDev)
ON_BN_CLICKED(IDC_BUTTON_LESS, OnBnClickedButtonLess)
ON_BN_CLICKED(IDC_BUTTON_MORE, OnBnClickedButtonMore)
ON_BN_CLICKED(IDC_BUTTON_EQV, OnBnClickedButtonEqv)
ON_BN_CLICKED(IDC_BUTTON_NOT, OnBnClickedButtonNot)
ON_BN_CLICKED(IDC_BUTTON_BRACKETL, OnBnClickedButtonBracketl)
ON_BN_CLICKED(IDC_BUTTON_BRACKETR, OnBnClickedButtonBracketr)
ON_BN_CLICKED(IDC_BUTTON_OR, OnBnClickedButtonOr)
ON_BN_CLICKED(IDC_BUTTON_AND, OnBnClickedButtonAnd)
ON_LBN_DBLCLK(IDC_LIST_REP_FILDS, OnLbnDblclkListRepFilds)
ON_LBN_DBLCLK(IDC_LIST_UNIT_FILDS, OnLbnDblclkListUnitFilds)
ON_EN_UPDATE(IDC_EDIT_NAME, OnEnUpdateEditName)
ON_EN_CHANGE(IDC_EDIT_NAME, OnEnChangeEditName)
ON_EN_UPDATE(IDC_EDIT_CNDITIOn, OnEnUpdateEditCndition)
ON_EN_UPDATE(IDC_EDIT_DESCRIPTION, OnEnUpdateEditDescription)
END_MESSAGE_MAP()
// NRepDlgRepEditor message handlers
BOOL NRepDlgRepEditor::OnInitDialog()
{
	fModif=false;
	fWM=false;
	CDialog::OnInitDialog();
	CButton *pBut;
	HINSTANCE hins;
	HICON icon;
	m_Selected=-1;
	hins=AfxGetResourceHandle( );
	pBut=(CButton*)GetDlgItem(IDC_BUTTON_DOWN);
	icon=::LoadIcon(hins,MAKEINTRESOURCE(IDI_ICON_ARROWDOWN));
	pBut->SetIcon(icon);
	pBut=(CButton*)GetDlgItem(IDC_BUTTON_UP);
	icon=::LoadIcon(hins,MAKEINTRESOURCE(IDI_ICON_ARROWUP));
	pBut->SetIcon(icon);
	EditCondition=(CEdit*)GetDlgItem(IDC_EDIT_CNDITIOn);
	EditDescription=(CEdit*)GetDlgItem(IDC_EDIT_DESCRIPTION);
	EditName=(CEdit*)GetDlgItem(IDC_EDIT_NAME);
	ListAvailebleUnitFilds=(CListBox*)GetDlgItem(IDC_LIST_UNIT_FILDS);
	ListCondUnitParam=(CListBox*)GetDlgItem(IDC_LIST_UNIT_PARAM);
	ListTableFilds=(CListBox*)GetDlgItem(IDC_LIST_REP_FILDS);
	ListReps=(CListBox*)GetDlgItem(ID_LIST_REPDLG_REPS);
	FillRepsList();
	FillListCondUnitParam();
	Disable(true);
	//Disable(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void NRepDlgRepEditor::FillRepsList(void)
{
	EditCondition->SetWindowText("");
	EditName->SetWindowText("");
	EditDescription->SetWindowText("");
	ListReps->ResetContent();
	ListTableFilds->ResetContent();
	ListAvailebleUnitFilds->ResetContent();
	int i;
	for(i=0;i<pReps->GetSize();i++)
	{
		ListReps->AddString((*pReps)[i].GetName());
	}
}
void NRepDlgRepEditor::Disable(bool flag)
{
	fEnableEdit=flag;
	if(flag)
	{
		EditName->EnableWindow(FALSE);
		EditCondition->EnableWindow(FALSE);
		EditDescription->EnableWindow(FALSE);
		ListAvailebleUnitFilds->EnableWindow(FALSE);
		ListCondUnitParam->EnableWindow(FALSE);
		ListTableFilds->EnableWindow(FALSE);
		return;
	}
	EditName->EnableWindow(TRUE);
	EditCondition->EnableWindow(TRUE);
	EditDescription->EnableWindow(TRUE);
	ListAvailebleUnitFilds->EnableWindow(TRUE);
	ListCondUnitParam->EnableWindow(TRUE);
	ListTableFilds->EnableWindow(TRUE);
}
void NRepDlgRepEditor::OnLbnSelchangeListReps()
{
	int k,i,j;
	fModif=false;
	NRepReport *pRep;
	CString str,mes;
	CStringArray tabs;
	double t;
	bool f=true;
	if(m_Selected!=-1)
	{
		pRep=&(*pReps)[m_Selected];
		EditCondition->GetWindowText(str);
		NEvalExp Eval(pUnit);
		if(!Eval.Evaluate(str,&t))
		{
			mes.LoadString(IDS_REP_WRONGCONDITION);
			AfxMessageBox(mes);
			ListReps->SetCurSel(m_Selected);
			return;
		}
		pRep->SetCondition(str);
		EditDescription->GetWindowText(str);
		i=0;
		while(i<str.GetLength())
		{
			if(str[i]=='\n')
				str.Delete(i);
			i++;
		}
		pRep->SetDescription(str);
		for(i=0;i<ListTableFilds->GetCount();i++)
		{
			ListTableFilds->GetText(i,str);
			tabs.Add(str);
		}
		pRep->SetTabs(tabs);
		EditName->GetWindowText(str);
		if(pRep->GetName()!=str)
		{
			if(pReps->FindReport(str)!=-1)
			{
				mes.LoadString(IDS_REP_WRONGREPNAME);
				AfxMessageBox(mes);
				ListReps->SetCurSel(m_Selected);
				return;
			}
			f=false;
		}
		pRep->SetName(str);
	}
	fEnableEdit=true;
	NewRep=false;
	k=ListReps->GetCurSel();
	m_Selected=k;
	if(!f)
	{
		FillRepsList();
	}
	ListReps->SetCurSel(m_Selected);
	pRep=&(*pReps)[k];
	str=pRep->GetCondition();
	EditCondition->SetWindowText(str);
	str=pRep->GetDescription();
	i=0;
	while(i<str.GetLength())
	{
		if(str[i]=='\r')
		{
			i++;
			str.Insert(i,'\n');
		}
		i++;
	}
	EditDescription->SetWindowText(str);
	str=pRep->GetName();
	EditName->SetWindowText(str);
	pRep->GetTabs(tabs);
	k = int(tabs.GetSize());
	ListTableFilds->ResetContent();
	ListAvailebleUnitFilds->ResetContent();
	for(i=0;i<tabs.GetSize();i++)
	{
		ListTableFilds->AddString(tabs[i]);
	}
	for(i=0;i<FildsStr.GetSize();i++)
	{
		f=false;
		for(j=0;j<tabs.GetSize();j++)
		{
			if(tabs[j]==FildsStr[i])
			{
				f=true;
				break;
			}
		}
		if(!f)
			ListAvailebleUnitFilds->AddString(FildsStr[i]);
	}
	Disable(false);
	fModif=true;
}
void NRepDlgRepEditor::FillListCondUnitParam(void)
{
	int i;
	ListCondUnitParam->ResetContent();
	for(i=0;i<FildsStr.GetSize();i++)
	{
		ListCondUnitParam->AddString(FildsStr[i]);
	}
}

void NRepDlgRepEditor::OnBnClickedButtonRemoveRep()
{
	int k;
	k=ListReps->GetCurSel();
	if(k==LB_ERR)
		return;
	m_Selected=-1;
	pReps->RemoveAt(k);
	FillRepsList();
	Disable(true);
}

void NRepDlgRepEditor::OnBnClickedButtonAddRep()
{
	EditCondition->SetWindowText("");
	EditName->SetWindowText("");
	EditDescription->SetWindowText("");
	ListTableFilds->ResetContent();
	ListAvailebleUnitFilds->ResetContent();
	NewRep=true;
	fEnableEdit=true;
	Disable(false);
	for(int i=0;i<FildsStr.GetSize();i++)
	{
		ListAvailebleUnitFilds->AddString(FildsStr[i]);
	}
	m_Selected=-1;
}

void NRepDlgRepEditor::OnBnClickedButtonSave()
{	
	fWM=true;
	if(!NewRep)
		return;
	int i;
	NRepReport Rep;
	CString str,mes;
	CStringArray tabs;
	double t;
	EditName->GetWindowText(str);
	if(pReps->FindReport(str)!=-1)
	{
		mes.LoadString(IDS_REP_WRONGREPNAME),
		AfxMessageBox(mes);
		return;
	}
	Rep.SetName(str);
	EditCondition->GetWindowText(str);
	NEvalExp Eval(pUnit);
	if(!Eval.Evaluate(str,&t))
	{
		mes.LoadString(IDS_REP_WRONGCONDITION);
		AfxMessageBox(mes);
		return;
	}
	Rep.SetCondition(str);
	EditDescription->GetWindowText(str);
	i=0;
	while(i<str.GetLength())
	{
		if(str[i]=='\n')
			str.Delete(i);
		i++;
	}
	Rep.SetDescription(str);
	for(i=0;i<ListTableFilds->GetCount();i++)
	{
		ListTableFilds->GetText(i,str);
		tabs.Add(str);
	}
	Rep.SetTabs(tabs);
	Rep.SetMode(0);
	pReps->AddReport(Rep);
	FillRepsList();
	Disable(true);
}

void NRepDlgRepEditor::OnBnClickedButtonAdd()
{
	int k;
	k=ListCondUnitParam->GetCurSel();
	if(k==LB_ERR)
		return;
	CString str,cond;
	ListCondUnitParam->GetText(k,str);
	str="["+str+"]";
	EditCondition->GetWindowText(cond);
	CPoint pt;
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,str);
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButton3()
{
	NEvalExp Eval(pUnit);
	double t;
	CString str,mes;
	EditCondition->GetWindowText(str);
	if(Eval.Evaluate(str,&t))
		mes.LoadString(IDS_REP_CORRECTCONDITION);
	else
		mes.LoadString(IDS_REP_WRONGCONDITION);
	AfxMessageBox(mes);

}

void NRepDlgRepEditor::OnBnClickedButtonAddFild()
{
	if(fModif)
		fWM=true;
	int k;
	CString str;
	k=ListAvailebleUnitFilds->GetCurSel();
	if(k==LB_ERR)
		return;
	ListAvailebleUnitFilds->GetText(k,str);
	ListAvailebleUnitFilds->DeleteString(k);
	ListTableFilds->AddString(str);
	if(k<ListAvailebleUnitFilds->GetCount())
		ListAvailebleUnitFilds->SetCurSel(k);
	else if(ListAvailebleUnitFilds->GetCount()!=0)
		ListAvailebleUnitFilds->SetCurSel(ListTableFilds->GetCount()-1);

}

void NRepDlgRepEditor::OnBnClickedButtonRemoveFild()
{
	if(fModif)
		fWM=true;
	int k;
	CString str;
	k=ListTableFilds->GetCurSel();
	if(k==LB_ERR)
		return;
	ListTableFilds->GetText(k,str);
	ListTableFilds->DeleteString(k);
	ListAvailebleUnitFilds->AddString(str);
	if(k<ListTableFilds->GetCount())
		ListTableFilds->SetCurSel(k);
	else if(ListTableFilds->GetCount()!=0)
		ListTableFilds->SetCurSel(ListTableFilds->GetCount()-1);

}

void NRepDlgRepEditor::OnBnClickedButtonUp()
{
	if(fModif)
		fWM=true;
	int k;
	CString str;
	k=ListTableFilds->GetCurSel();
	if(k==LB_ERR||k==0)
		return;
	ListTableFilds->GetText(k,str);
	ListTableFilds->InsertString(k-1,str);
	ListTableFilds->DeleteString(k+1);
	ListTableFilds->SetCurSel(k-1);
	CStringArray sa;
	for(int i=0;i<ListTableFilds->GetCount();i++)
	{
		ListTableFilds->GetText(i,str);
		sa.Add(str);
	}
	(*pReps)[m_Selected].SetTabs(sa);
}

void NRepDlgRepEditor::OnBnClickedButtonDown()
{
	if(fModif)
		fWM=true;
	int k;
	CString str;
	k=ListTableFilds->GetCurSel();
	if(k==LB_ERR||k==ListTableFilds->GetCount()-1)
		return;
	ListTableFilds->GetText(k,str);
	ListTableFilds->InsertString(k+2,str);
	ListTableFilds->DeleteString(k);
	ListTableFilds->SetCurSel(k+1);
	CStringArray sa;
	for(int i=0;i<ListTableFilds->GetCount();i++)
	{
		ListTableFilds->GetText(i,str);
		sa.Add(str);
	}
	(*pReps)[m_Selected].SetTabs(sa);
}

void NRepDlgRepEditor::OnBnClickedOk()
{
	CString str,mes;
	CStringArray tabs;
	NRepReport *pRep;
	double t;
	int i;
	bool f;
	if(m_Selected!=-1)
	{
		pRep=&(*pReps)[m_Selected];
		EditCondition->GetWindowText(str);
		NEvalExp Eval(pUnit);
		if(!Eval.Evaluate(str,&t))
		{
			mes.LoadString(IDS_REP_WRONGCONDITION);
			AfxMessageBox(mes);
			ListReps->SetCurSel(m_Selected);
			return;
		}

		pRep->SetCondition(str);
		EditDescription->GetWindowText(str);
		pRep->SetDescription(str);
		for(i=0;i<ListTableFilds->GetCount();i++)
		{
			ListTableFilds->GetText(i,str);
			tabs.Add(str);
		}
		pRep->SetTabs(tabs);
		EditName->GetWindowText(str);
		if(pRep->GetName()!=str)
		{
			if(pReps->FindReport(str)!=-1)
			{
				mes.LoadString(IDS_REP_WRONGREPNAME);
				AfxMessageBox(mes);
				ListReps->SetCurSel(m_Selected);
				return;
			}
			f=false;
		}
		pRep->SetName(str);
	}	
	if(fWM)
	{
		CString msg;
		msg.LoadString(IDS_REP_SAVECHANGES);
		if(AfxMessageBox(msg,MB_YESNO) == IDYES)
		{
			str.LoadString(IDS_REPORT_DEF);
			str = NCM_PROJECT.GetPrototypesPath() + str;
			pReps->SaveReportsDefinition(str);
		}
	}
	OnOK();
}

void NRepDlgRepEditor::OnLbnDblclkListUnitParam()
{
	int k;
	k=ListCondUnitParam->GetCurSel();
	if(k==LB_ERR)
		return;
	CString str,cond;
	ListCondUnitParam->GetText(k,str);
	str="["+str+"]";
	EditCondition->GetWindowText(cond);
	CPoint pt;
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,str);
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonPlus()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"+");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonMin()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"-");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonMult()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"*");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonDev()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"/");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonLess()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"<");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonMore()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,">");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonEqv()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"=");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonNot()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"!");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonBracketl()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"[");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonBracketr()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"]");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonOr()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"|");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnBnClickedButtonAnd()
{
	CString cond;
	int k;
	CPoint pt;
	EditCondition->GetWindowText(cond);
	pt=EditCondition->GetCaretPos();
	k=EditCondition->CharFromPos(pt);
	cond.Insert(k,"&");
	EditCondition->SetWindowText(cond);
}

void NRepDlgRepEditor::OnLbnDblclkListRepFilds()
{
	if(fModif)
		fWM=true;
	int k;
	CString str;
	k=ListTableFilds->GetCurSel();
	if(k==LB_ERR)
		return;
	ListTableFilds->GetText(k,str);
	ListTableFilds->DeleteString(k);
	ListAvailebleUnitFilds->AddString(str);
	if(k<ListTableFilds->GetCount())
		ListTableFilds->SetCurSel(k);
	else if(ListTableFilds->GetCount()!=0)
		ListTableFilds->SetCurSel(ListTableFilds->GetCount()-1);
}

void NRepDlgRepEditor::OnLbnDblclkListUnitFilds()
{
	if(fModif)
		fWM=true;
	int k;
	CString str;
	k=ListAvailebleUnitFilds->GetCurSel();
	if(k==LB_ERR)
		return;
	ListAvailebleUnitFilds->GetText(k,str);
	ListAvailebleUnitFilds->DeleteString(k);
	ListTableFilds->AddString(str);
	if(k<ListAvailebleUnitFilds->GetCount())
		ListAvailebleUnitFilds->SetCurSel(k);
	else if(ListAvailebleUnitFilds->GetCount()!=0)
		ListAvailebleUnitFilds->SetCurSel(ListTableFilds->GetCount()-1);
}
void NRepDlgRepEditor::OnEnUpdateEditName()
{
}

void NRepDlgRepEditor::OnEnChangeEditName()
{
	if(fModif)
		fWM=true;
}

void NRepDlgRepEditor::OnEnUpdateEditCndition()
{
	if(fModif)
		fWM=true;
}

void NRepDlgRepEditor::OnEnUpdateEditDescription()
{
	if(fModif)
		fWM=true;
}
