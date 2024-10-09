#include "stdafx.h"
#include "ntoolmill.h"
#include "NToolsCrDial.h"
#include "NToolsCrWeldSheet.h"
#include "NMendrelDial.h"
#include "NToolCombinedMill.h"
#include "ntoolsconcrweldpage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(NToolsConCrWeldPage, NToolsConCrPage)
NToolsConCrWeldPage::NToolsConCrWeldPage(void) 
	: NToolsConCrPage(NToolsConCrWeldPage::IDD)
{
	m_displacement = 0.0;
	m_IsNameAuto = true;
}

NToolsConCrWeldPage::~NToolsConCrWeldPage(void)
{
}

void NToolsConCrWeldPage::DoDataExchange(CDataExchange* pDX)
{
	NToolsConCrPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
}

BEGIN_MESSAGE_MAP(NToolsConCrWeldPage, NToolsConCrPage)
	ON_BN_CLICKED(IDC_ADD_TOOL, OnAddTool)
	ON_BN_CLICKED(IDC_ADD_MENDREL, OnAddMendrel)
	ON_BN_CLICKED(IDC_ADD_DISKTOOL, OnAddDiskTool)
END_MESSAGE_MAP()

void NToolsConCrWeldPage::OnOK()
{
	if(!CheckActive())
		return;

	NToolsConCrPage::OnOK();

	double f;
	CString str;
	int k = m_ToolList.GetNumEditColumn();
	for (int i = 0; i < m_ToolList.GetItemCount(); i += k)
	{
		m_ToolList.GetEditCtrl(i/k)->GetWindowText(str);
		if (str.GetLength() <= 0)
			return;
		if (sscanf(str, "%lf", &f) == 0)
		{
			AfxMessageBox(IDS_MES_DIS);
			m_ToolList.GetEditCtrl(i/k)->SetFocus();
			return;
		}
	}
	m_pToolProduced->Zdisp = m_displacement;
	for (int i = 0; i < m_pToolProduced->num_tools; ++i)
	{
		m_ToolList.GetEditCtrl(i)->GetWindowText(str);
		f = atof(str);
		int index = (int)m_ToolList.GetItemData(i);
		((NToolCombinedMill *)m_pToolProduced)->GetTool(index)->SetZdisp(f + m_pToolProduced->Zdisp);

		if (!(80 <= m_pToolProduced->GetTool(index)->GetType() &&
			  m_pToolProduced->GetTool(index)->GetType() < 81))
		{
			m_pToolProduced->GetTool(index)->SetColors(
				m_pToolProduced->Color, m_pToolProduced->ColorPassive);
		}

	}
}

NToolCombinedWeld * NToolsConCrWeldPage::GetNewTool()
{
	return (NToolCombinedWeld *)NToolsConCrPage::GetNewTool();
}

void NToolsConCrWeldPage::OnAddTool() 
{
	NToolsCrWeldSheet CreateSheet(this);
	if(CreateSheet.DoModal() == IDOK)
	{
		NTool *pTool = CreateSheet.GetNewTool();
		
		if ( pTool == NULL )
			return;
		pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive); 
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}
// добавление дискового инструмента
void NToolsConCrWeldPage::OnAddDiskTool() 
{
	//NTool *pTool;
	//NToolsCrTurnSheet DiskSheet(this);
	//
	//// задание смещения нового инструмента
	//// новый ставиться после инструмента с самой большой Z
	//double Zdis = 0;
	//for (int i=0; i<m_pToolProduced->num_tools; i++)
	//{
	//	double d;
	//	d = m_pToolProduced->GetTool(i)->GetZdisp();
	//	d = d + m_pToolProduced->GetTool(i)->Gabar.dZ();
	//	if (d > Zdis)
	//		Zdis = d;
	//}
	//DiskSheet.m_fasetpage.m_displacement = 
	//DiskSheet.m_roundpage.m_displacement =
	//DiskSheet.m_anglepage.m_displacement =
	//DiskSheet.m_anglespage.m_displacement = 
	//DiskSheet.m_broundpage.m_displacement = 
	//DiskSheet.m_aptpage.m_displacement =
	//DiskSheet.m_shapedpage.m_displacement = Zdis;

	//if (DiskSheet.DoModal() == IDOK)
	//{
	//	pTool=DiskSheet.GetNewTool();
	//	
	//	if ( pTool == NULL )
	//		return;
	//	pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive); 
	//	m_pToolProduced->AddTool(pTool);

	//	NUpdate();
	//}
}

// добавление новой оправки
void NToolsConCrWeldPage::OnAddMendrel() 
{
	NTool *pTool;
	NMendrelDial CreateDial;
	
	// задание смещения нового инструмента
	// новый ставится после инструмента с самой большой Z
	double Zdis = 0;
	for (int i=0; i<m_pToolProduced->num_tools; i++)
	{
		double d;
		d = m_pToolProduced->GetTool(i)->GetZdisp();
		d = d + m_pToolProduced->GetTool(i)->Gabar.dZ();
		if (d > Zdis)
			Zdis = d;
	}
	CreateDial.m_displacement = Zdis;
	if( CreateDial.DoModal() == IDOK ) 
	{
		pTool=CreateDial.GetNewTool();

		if ( pTool == NULL )
			return;
		pTool->SetColors(m_pToolProduced->ColorHolder, m_pToolProduced->ColorHolder); 
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}
NToolCombinedWeld * NToolsConCrWeldPage::CreateNewToolCom(const CString & name)
{
	NToolCombinedWeld *pToolCom = new NToolCombinedWeld(name);
	pToolCom->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
	return pToolCom;
}
