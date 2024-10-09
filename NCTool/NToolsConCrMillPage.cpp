#include "stdafx.h"
#include "ntoolmill.h"
#include "NToolsCrDial.h"
#include "NToolsCrDiskSheet.h"
#include "NMendrelDial.h"
#include "ntoolsconcrmillpage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(NToolsConCrMillPage, NToolsConCrPage)
NToolsConCrMillPage::NToolsConCrMillPage(void) 
	: NToolsConCrPage(NToolsConCrMillPage::IDD)
{
	m_displacement = 0.0;
	m_IsNameAuto = true;
}

NToolsConCrMillPage::~NToolsConCrMillPage(void)
{
}

void NToolsConCrMillPage::AddDiskTool(bool Probe)
{
	NToolsCrDiskSheet DiskSheet(IDS_DISK_TOOL);
	if (Probe)
	{
		DiskSheet.tab_pos = 1;
		DiskSheet.m_roundpage.m_check_probe = TRUE;
	}
	else
	{
		DiskSheet.tab_pos = 0;
		DiskSheet.m_roundpage.m_check_probe = FALSE;
	}
	NTool *pTool;

	// задание смещения нового инструмента
	// новый ставиться после инструмента с самой большой Z
	double Zdis = 0;
	for (int i = 0; i<m_pToolProduced->num_tools; i++)
	{
		double d;
		d = m_pToolProduced->GetTool(i)->GetZdisp();
		d = d + m_pToolProduced->GetTool(i)->Gabar.dZ();
		if (d > Zdis)
			Zdis = d;
	}
	DiskSheet.m_fasetpage.m_displacement =
		DiskSheet.m_roundpage.m_displacement =
		DiskSheet.m_anglepage.m_displacement =
		DiskSheet.m_anglespage.m_displacement =
		DiskSheet.m_broundpage.m_displacement =
		DiskSheet.m_aptpage.m_displacement =
		DiskSheet.m_borepage.m_displacement =
		DiskSheet.m_shapedpage.m_displacement = Zdis;

	if (DiskSheet.DoModal() == IDOK)
	{
		pTool = DiskSheet.GetNewTool();

		if (pTool == NULL)
			return;
		pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive);
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}

void NToolsConCrMillPage::DoDataExchange(CDataExchange* pDX)
{
	NToolsConCrPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_DISPLACEMENT, m_displacement);
	DDV_MinMaxDouble(pDX, m_displacement, -500., 500.);
}

BEGIN_MESSAGE_MAP(NToolsConCrMillPage, NToolsConCrPage)
	ON_BN_CLICKED(IDC_ADD_TOOL, OnAddTool)
	ON_BN_CLICKED(IDC_ADD_MENDREL, OnAddMendrel)
	ON_BN_CLICKED(IDC_ADD_DISKTOOL, OnAddDiskTool)
	ON_BN_CLICKED(IDC_ADD_PROBE, OnAddProbe)
END_MESSAGE_MAP()

void NToolsConCrMillPage::OnOK()
{
	if(!CheckActive())
		return;

	int i, index;
	double f;
	CString str;
	int k = m_ToolList.GetNumEditColumn();
	for (i=0; i<m_ToolList.GetItemCount(); i+=k)
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
	for (int i=0; i<m_pToolProduced->num_tools; i++)
	{
		m_ToolList.GetEditCtrl(i)->GetWindowText(str);
		f = atof(str);
		index = int(m_ToolList.GetItemData(i));
		((NToolCombinedMill *)m_pToolProduced)->GetTool(index)->SetZdisp(f + m_pToolProduced->Zdisp);

		if (!(40 <= m_pToolProduced->GetTool(index)->GetType() &&
			  m_pToolProduced->GetTool(index)->GetType() <= 43))
		{
			m_pToolProduced->GetTool(index)->SetColors(
				m_pToolProduced->Color, m_pToolProduced->ColorPassive);
		}

	}
	NToolsConCrPage::OnOK();
}

void NToolsConCrMillPage::NUpdate()
{
	NToolsConCrPage::NUpdate();
}

void  NToolsConCrMillPage::NUpdateControls()
{
	NToolsConCrPage::NUpdateControls();

}
NToolCombinedMill * NToolsConCrMillPage::GetNewTool()
{
	return (NToolCombinedMill *)NToolsConCrPage::GetNewTool();
}

void NToolsConCrMillPage::OnAddTool() 
{
	NTool *pTool;
	
	// задание смещения нового инструмента
	// новый ставится после инструмента с самой большой Z
	double Zdis = 0;
	for (int i=0; i<m_pToolProduced->num_tools; i++)
	{
		double d;
		d = ((NToolCombinedMill *)m_pToolProduced)->GetTool(i)->GetZdisp();
		d = d + ((NToolCombinedMill *)m_pToolProduced)->GetTool(i)->Gabar.dZ();
		if (d > Zdis)
			Zdis = d;
	}

	NToolsCrDial CreateDial;
	CreateDial.m_displacement = Zdis;

	bool f = false;
	while (!f)
	{
		if( CreateDial.DoModal() == IDOK ) 
		{
			pTool=CreateDial.GetNewTool();
	
			if ( pTool == NULL )
				return;

			pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive); 
			m_pToolProduced->AddTool(pTool);

			NUpdate();
			return;
		}
		f = true;
		if (CreateDial.GetOtherTool() == true)
		{
			NToolsCrDiskSheet DiskSheet(IDS_DISK_TOOL);

			DiskSheet.m_fasetpage.m_displacement = 
			DiskSheet.m_roundpage.m_displacement =
			DiskSheet.m_anglepage.m_displacement =
			DiskSheet.m_anglespage.m_displacement = 
			DiskSheet.m_broundpage.m_displacement = 
			DiskSheet.m_aptpage.m_displacement =
			DiskSheet.m_shapedpage.m_displacement =
			DiskSheet.m_borepage.m_displacement =
				CreateDial.m_displacement;

			if (DiskSheet.DoModal() == IDOK)
			{
				pTool=DiskSheet.GetNewTool();
				
				if ( pTool == NULL )
					return;
				pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive); 
				m_pToolProduced->AddTool(pTool);

				NUpdate();
			}
			else
			{
				DiskSheet.ClearTools();
				CreateDial.SetOtherTool(false);
				f = false;
			}
		}
	}
}
void NToolsConCrMillPage::OnAddProbe()
{
	AddDiskTool(true);
}
	// добавление дискового инструмента
void NToolsConCrMillPage::OnAddDiskTool() 
{
	AddDiskTool(false);
}

// добавление новой оправки
void NToolsConCrMillPage::OnAddMendrel() 
{
	NTool *pTool;
	NMendrelDial CreateDial;
	
	// задание смещения нового инструмента
	// новый ставится после инструмента с самой большой Z
	double Zdis = 0;
	double Dbot = 0.;
	for (int i = 0; i < m_pToolProduced->num_tools; i++)
	{
		double d;
		d = m_pToolProduced->GetTool(i)->GetZdisp();
		d = d + m_pToolProduced->GetTool(i)->Gabar.dZ();
		if (d > Zdis)
		{
			Zdis = d;
			double R, E, F, B, S, H;
			m_pToolProduced->GetTool(i)->GetStdParams(Dbot, R, E, F, B, S, H);
		}
	}
	CreateDial.m_displacement = Zdis;
	if (Zdis > 0.)// adjust diameter to topmost tool
	{
		CreateDial.m_dprevious = ceil(Dbot * 1000.) / 1000;
	}
	if( CreateDial.DoModal() == IDOK ) 
	{
		pTool=CreateDial.GetNewTool();

		if ( pTool == NULL )
			return;
		auto Color2Set = pTool->IsShank() ? m_pToolProduced->ColorPassive : m_pToolProduced->ColorHolder;
		pTool->SetColors(Color2Set, Color2Set);
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}
NToolCombinedMill * NToolsConCrMillPage::CreateNewToolCom(const CString & name)
{
	NToolCombinedMill *pToolCom = new NToolCombinedMill(name);
	pToolCom->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
	return pToolCom;
}

