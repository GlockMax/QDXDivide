// NToolsConCrTurnPage.cpp : implementation file
//

#include "stdafx.h"
#include "NMapStringToOb.h"
#include "NToolsCrDial.h"
#include "NToolsCrDiskSheet.h"
#include "NToolCombinedTurn.h"
#include "NToolsCrTurnSheet.h"
#include "NToolsCrTurnHoldSheet.h"
#include "NToolsCrTBOut.h"
#include "NToolTurnBladeMill.h"
#include "NToolTurnHolderSTL.h"
#include "NToolsCrTurnHoldSTLDlg.h"
#include "NToolsConCrTurnPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


// NToolsConCrTurnPage dialog

IMPLEMENT_DYNAMIC(NToolsConCrTurnPage, NToolsConCrPage)
NToolsConCrTurnPage::NToolsConCrTurnPage()
	: NToolsConCrPage(NToolsConCrTurnPage::IDD)
	, m_Xdisplacement(0)
	, m_Zdisplacement(0)
	, m_MainAngle(0)
{
}

NToolsConCrTurnPage::~NToolsConCrTurnPage()
{
}

void NToolsConCrTurnPage::DoDataExchange(CDataExchange* pDX)
{
	NToolsConCrPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DISPLACEMENT_X, m_Xdisplacement);
	DDX_Text(pDX, IDC_DISPLACEMENT_Z, m_Zdisplacement);
	DDX_Text(pDX, IDC_COM_ANGLE, m_MainAngle);
	DDV_MinMaxDouble(pDX, m_MainAngle, -360, 360);
}


BEGIN_MESSAGE_MAP(NToolsConCrTurnPage, NToolsConCrPage)
	ON_BN_CLICKED(IDC_ADD_TOOL_INSERT, OnBnClickedAddToolInsert)
	ON_BN_CLICKED(IDC_ADD_TURNHOLDER, OnBnClickedAddTurnholder)
	ON_BN_CLICKED(IDC_ADD_TURNHOLDER_STL, OnBnClickedAddTurnholderSTL)
	ON_BN_CLICKED(IDC_ADD_TOOL_SIMPLE, OnBnClickedAddToolSimple)
END_MESSAGE_MAP()


// NToolsConCrTurnPage message handlers


void NToolsConCrTurnPage::OnOK()
{
	if(!CheckActive())
		return;

	((NToolCombinedTurn *)m_pToolProduced)->Displacement.Set(m_Xdisplacement, m_Zdisplacement, 0., 0.);
	((NToolCombinedTurn *)m_pToolProduced)->MainAngle = m_MainAngle;
	m_pToolProduced->RestChildrenDispl();
	m_MainAngle = ((NToolCombinedTurn*)m_pToolProduced)->MainAngle;
	NToolsConCrPage::OnOK();
}

NToolCombinedTurn * NToolsConCrTurnPage::GetNewTool()
{
	return (NToolCombinedTurn *)NToolsConCrPage::GetNewTool();
}

NToolCombinedTurn * NToolsConCrTurnPage::CreateNewToolCom(const CString & name)
{
	NToolCombinedTurn *pToolCom = new NToolCombinedTurn(name);
	pToolCom->Color.SetRGB(NToolCombined::Palet.GetRecurColor(0));
	return pToolCom;
}

void NToolsConCrTurnPage::OnBnClickedAddToolInsert()
{
	NToolsCrTurnSheet CreateSheet(this);
	if(CreateSheet.DoModal() == IDOK)
	{
		NTool *pTool = CreateSheet.GetNewTool();
		
		if ( pTool == NULL )
			return;
		pTool->SetZdisp(GenZdisp());
			pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive); 
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}

void NToolsConCrTurnPage::OnBnClickedAddTurnholder()
{
	NToolsCrTurnHoldSheet CreateSheet(this);
	if(CreateSheet.DoModal() == IDOK)
	{
		NTool *pTool = CreateSheet.GetNewTool();
		
		if ( pTool == NULL )
			return;

		pTool->SetZdisp(GenZdisp());
		pTool->SetColors(m_pToolProduced->ColorHolder, m_pToolProduced->ColorHolder); 
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}

void NToolsConCrTurnPage::OnBnClickedAddTurnholderSTL()
{
	NToolsCrTurnHoldSTLDlg Dlg;
	// Get default displacement from the first turn blade
	for (int i = 0; i < m_pToolProduced->num_tools; ++i)
	{
		NTool *pTool = m_pToolProduced->GetTool(i);
		if(pTool->IsKindOf(RUNTIME_CLASS(NToolTurnBlade)))
		{
			const BPoint &Disp = ((NToolTurnBlade *)pTool)->GetDisplacement();
			Dlg.m_Xdisp = Disp.GetX(); 
			Dlg.m_Y = Disp.GetZ(); 
			Dlg.m_Zdisp = Disp.GetY(); 
			break;
		}
	}
	if(Dlg.DoModal() == IDOK)
	{
		NTool *pTool = Dlg.GetNewTool();
		
		if ( pTool == NULL )
			return;

		pTool->SetZdisp(GenZdisp());
		pTool->SetColors(m_pToolProduced->ColorHolder, m_pToolProduced->ColorHolder); 
		m_pToolProduced->AddTool(pTool);

		NUpdate();
	}
}

double NToolsConCrTurnPage::GenZdisp(void)
{
	// задание смещения нового инструмента
	// новый ставиться после инструмента с самой большой Z
	double Zdis = 0;
	for (int i = 0; i < m_pToolProduced->num_tools; i++)
	{
		double d;
		d = ((NToolCombinedTurn*)m_pToolProduced)->GetTool(i)->GetZdisp();
		d = d + 0.1;// Zdisp для токарного инструмента используется только для упорядочения списка
		if (d > Zdis)
			Zdis = d;
	}
	return Zdis;
}

void NToolsConCrTurnPage::OnBnClickedAddToolSimple()
{
	
	// задание смещения нового инструмента
	// новый ставится после инструмента с самой большой Z
	double Zdis = 0;
	for (int i=0; i<m_pToolProduced->num_tools; i++)
	{
		double d;
		NTool* pLocTool = ((NToolCombinedTurn*)m_pToolProduced)->GetTool(i);
		NToolTurnBladeMill* pBladeMillTool = dynamic_cast<NToolTurnBladeMill*>(pLocTool);
		if (pBladeMillTool == nullptr)
			continue;
		NToolMill* pMillTool = pBladeMillTool->GetMillTool();
		d = pMillTool->GetZdisp();
		d = d + pMillTool->Gabar.dZ();
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
			NToolMill *pMillTool=CreateDial.GetNewTool();
	
			if ( pMillTool == NULL )
				return;
			NTool* pTool = new NToolTurnBladeMill();
			((NToolTurnBladeMill *)pTool)->SetMillTool(pMillTool);
			pTool->SetColors(m_pToolProduced->Color, m_pToolProduced->ColorPassive); 
			pTool->GenInit();
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
			DiskSheet.m_borepage.m_displacement =
			DiskSheet.m_shapedpage.m_displacement = CreateDial.m_displacement;

			if (DiskSheet.DoModal() == IDOK)
			{
				NTool* pTool = DiskSheet.GetNewTool();
				
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
