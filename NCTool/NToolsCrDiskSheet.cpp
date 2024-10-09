// NToolsCrDiskSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ConstDef.h"
#include "ResLocal.h"
#include "NToolStd.h"
#include "NToolVert.h"
#include "NToolDiskStd.h"
#include "NToolBRound.h"
#include "NToolCombinedMill.h"
#include "NToolsCrDiskSheet.h"
#include "NToolsCrFasetPage.h"
#include "NToolsCrRoundPage.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NToolsCrDiskSheet

IMPLEMENT_DYNAMIC(NToolsCrDiskSheet, CPropertySheet)

NToolsCrDiskSheet::NToolsCrDiskSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddControlPages();

	pToolShaped = NULL;
	pToolDisk = NULL;
	pTool = NULL;
	tab_pos = 0;
}

void NToolsCrDiskSheet::AddControlPages()
{
/*	m_hIcon = AfxGetApp()->LoadIcon(IDR_NPROGRAMVIEW_TMPL);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;*/
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags |= PSH_HASHELP;  // Lose the Help button

	ToolPics.Create(IDB_TOOL_DISK_PICS,30,10,RGB(255,255,255));
	
	AddPage(&m_fasetpage);
	AddPage(&m_roundpage);
	AddPage(&m_anglepage);
	AddPage(&m_anglespage);
	AddPage(&m_aptpage);
	AddPage(&m_shapedpage);
	AddPage(&m_broundpage);
	AddPage(&m_borepage);
}

NToolsCrDiskSheet::~NToolsCrDiskSheet()
{
}


BEGIN_MESSAGE_MAP(NToolsCrDiskSheet, CPropertySheet)
	//{{AFX_MSG_MAP(NToolsCrDiskSheet)
	ON_COMMAND(IDOK, OnOK)
	//}}AFX_MSG_MAP
	// добавл€ютс€ вручную
END_MESSAGE_MAP()
// IDHELP
/////////////////////////////////////////////////////////////////////////////
// NToolsCrDiskSheet message handlers

BOOL NToolsCrDiskSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	CTabCtrl *Tab = CPropertySheet::GetTabControl();
	ASSERT(Tab != NULL);
	Tab->SetImageList(&ToolPics);
	TCITEM tc;
	tc.mask = TCIF_IMAGE;
	for (int i = 0; i < 8; i++)
	{
		Tab->GetItem(i, &tc);
		tc.iImage = i;
		Tab->SetItem(i, &tc);
	}
	
	SetActivePage(tab_pos);

	ModifyStyleEx(0, WS_EX_CONTEXTHELP);

	return bResult;
}

//NToolDiskStd* NToolsCrDiskSheet::GetNewTool()
NToolMill* NToolsCrDiskSheet::GetNewTool()
{
	switch(tab_pos)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 5:
		return pToolDisk;	
	case 4:
		return pTool;	
	case 6:
	case 7:
		return pToolShaped;
	}
	return NULL;
}

bool NToolsCrDiskSheet::Apply()
{
	int ActivIndex = GetActiveIndex();
	CString name;
	double diam,b,r1,r2,r3,r4,l,r,
		alf1,alf2,faset1,faset2,h1,h2,Displacement;
	bool Probe;
	bool IsNameAuto;
	int type;

	if (pToolDisk) delete pToolDisk;
	if (pToolShaped) delete pToolShaped;
	Probe = false;
	switch(ActivIndex)
	{
	case 0: // инструмент с фасками
		if(!m_fasetpage.IsOkInput())
			return false;
		name = m_fasetpage.m_name;
		diam = m_fasetpage.m_diam;
		b =	m_fasetpage.m_b;
		r1 = r2 = r3 = r4 = 0.;
		alf1 = m_fasetpage.m_alf1;
		alf2 = m_fasetpage.m_alf2;
		faset1 = m_fasetpage.m_faset1;
		faset2 = m_fasetpage.m_faset2;
		h1 = m_fasetpage.m_h1;
		h2 = m_fasetpage.m_h2;
		IsNameAuto = m_fasetpage.IsNameAuto;
		Displacement = m_fasetpage.m_displacement;
		type = 21;
		break;
	case 1: // инструмент со скруглени€ми
		if(!m_roundpage.IsOkInput())
			return false;
		name = m_roundpage.m_name;
		diam = m_roundpage.m_diam;
		b =	m_roundpage.m_b;
		if (m_roundpage.m_r1 != 0.)
		{
			r1 = r2 = m_roundpage.m_r1;
			r3 = r4 = 0.;
		}
		else
		{
			r1 = m_roundpage.m_r2;
			r2 = m_roundpage.m_r3;
			r3 = r4 = 0.;
		}
		alf1 = alf2 = faset1 = faset2 = 0.;
		h1 = m_roundpage.m_h1;
		h2 = m_roundpage.m_h2;
		IsNameAuto = m_roundpage.IsNameAuto;
		Displacement = m_roundpage.m_displacement;
		Probe = (m_roundpage.m_check_probe == TRUE);
		type = 22;
		break;
	case 2: // одноугловой инструмент
		if(!m_anglepage.IsOkInput())
			return false;
		name = m_anglepage.m_name;
		diam = m_anglepage.m_diam;
		b =	m_anglepage.m_b;
		r2 = r3 = 0.;
		r1 = m_anglepage.m_r;
		r4 = m_anglepage.m_r;
		alf1 = 0.;
		alf2 = m_anglepage.m_alf;
		faset1 = 0.;
		faset2 = b-r1-r4*tan((PI/2.-(alf2*PI/180.))/2.);
		h1 = m_anglepage.m_h1;
		h2 = m_anglepage.m_h2;
		IsNameAuto = m_anglepage.IsNameAuto;
		Displacement = m_anglepage.m_displacement;
		type = 23;
		break;
	case 3: // двухугловой инструмент
		if(!m_anglespage.IsOkInput())
			return false;
		name = m_anglespage.m_name;
		diam = m_anglespage.m_diam;
		b =	m_anglespage.m_b;
		r1 = r2 = 0.;
		r3 = m_anglespage.m_r;
		r4 = m_anglespage.m_r;
		alf1 = m_anglespage.m_alf1;
		alf2 = m_anglespage.m_alf2;
		faset1 = m_anglespage.m_faset - r3*tan((PI/2.-(alf1*PI/180.))/2.);
		faset2 = b - m_anglespage.m_faset;
		faset2 = faset2 - r4*tan((PI/2.-(alf2*PI/180.))/2.);
		h1 = m_anglespage.m_h1;
		h2 = m_anglespage.m_h2;
		IsNameAuto = m_anglespage.IsNameAuto;
		Displacement = m_anglespage.m_displacement;
		type = 24;
		break;
	case 4: // инструмент ј–“ 7
		if(!m_aptpage.IsOkInput())
			return false;
		name = m_aptpage.m_name;
		diam = m_aptpage.m_diam;
		b =	m_aptpage.m_h;
		alf1 = m_aptpage.m_alf;
		alf2 = m_aptpage.m_beta;
		r1 = m_aptpage.m_r;
		faset1 = m_aptpage.m_e;
		faset2 = m_aptpage.m_f;
		h1 = m_aptpage.m_hm;
		IsNameAuto = m_aptpage.IsNameAuto;
		Displacement = m_aptpage.m_displacement;
		type = 10;
		break;
	case 5: // произвольный дисковый инструмент
		if(!m_shapedpage.IsOkInput())
			return false;
		name = m_shapedpage.m_name;
		diam = m_shapedpage.m_diam;
		b =	m_shapedpage.m_b;
		r1 = m_shapedpage.m_r1;
		r2 = m_shapedpage.m_r2;
		r3 = m_shapedpage.m_r3;
		r4 = m_shapedpage.m_r4;
		alf1 = m_shapedpage.m_alf1;
		alf2 = m_shapedpage.m_alf2;
		if (alf1 != 0.)
			faset1 = m_shapedpage.m_faset1 - r3*tan((PI/2.-(alf1*PI/180.))/2.);
		else
			faset1 = m_shapedpage.m_faset1;
		if (alf2 != 0.)
			faset2 = m_shapedpage.m_faset2 - r4*tan((PI/2.-(alf2*PI/180.))/2.);
		else
			faset2 = m_shapedpage.m_faset2;
		h1 = m_shapedpage.m_h1;
		h2 = m_shapedpage.m_h2;
		IsNameAuto = m_shapedpage.IsNameAuto;
		Displacement = m_shapedpage.m_displacement;
		type = 20;
		break;
	case 6: // вогнутый 
		if(!m_broundpage.IsOkInput())
			return false;
		name = m_broundpage.m_name;
		diam = m_broundpage.m_diam;
		b = m_broundpage.m_b;
		r = m_broundpage.m_r;
		l = m_broundpage.m_l;
		h1 = m_broundpage.m_h1;
		IsNameAuto = m_broundpage.IsNameAuto;
		Displacement = m_broundpage.m_displacement;
		type = 71;
		break;
	case 7: // vert 
		if(!m_borepage.IsOkInput())
			return false;
		name = m_borepage.m_name;
		Displacement = m_borepage.m_displacement;
		type = 72;
		break;
	}
	
	tab_pos = ActivIndex;
	int IsErr;
	if (type == 10)
	{	// простой инструмент
		pTool = new NToolStd(name, diam, b, alf1, r1, 
			alf2, faset1, faset2, h1);
		pTool->IsNameAuto = IsNameAuto;
		pTool->SetType(type);
		IsErr = pTool->GenInit();	
		pTool->SetZdisp(Displacement);
		pTool->SetProbe(Probe);
	}
	else if( type == 71)
	{ // с вогнутым радиусом
		pToolShaped = new NToolBRound(name, diam, b, l, r, h1);
//		pToolShaped->IsNameAuto = IsNameAuto;
		pToolShaped->SetType(type);
		IsErr = pToolShaped->GenInit();	
		pToolShaped->SetZdisp(Displacement);
		pToolShaped->SetProbe(Probe);
	}
	else if( type == 72)
	{
		IsErr = 0;
		if (m_borepage.m_check_file)
		{
			if (m_borepage.pToolVert == nullptr)
			{
				IsErr = -1;
			}
			else
			{
				pToolShaped = m_borepage.pToolVert;
			}
		}
		else
		{
			pToolShaped = new NToolVert(m_borepage.m_name, m_borepage.m_a, m_borepage.m_D, m_borepage.m_H, m_borepage.m_h, m_borepage.m_L, m_borepage.m_W, m_borepage.m_R);
			if (pToolShaped->GenInit() != 0)
				IsErr = -1;
		}
		if (IsErr >= 0)
		{
			pToolShaped->SetZdisp(Displacement);
			pToolShaped->SetProbe(Probe);
		}
	}
	else
	{
		// дисковый инструмент
		pToolDisk = new NToolDiskStd(name, diam, b, alf1, alf2,
			r1, r2, r3, r4, faset1, faset2, h1, h2);
		pToolDisk->IsNameAuto = IsNameAuto;
		pToolDisk->SetType(type);
		pToolDisk->SetProbe(Probe);
		IsErr = pToolDisk->GenInit();	
		pToolDisk->SetZdisp(Displacement);
		pToolDisk->SetProbe(Probe);
	}
	if (IsErr != 0)
	{
		switch(ActivIndex)
		{
		case 0:
			m_fasetpage.IsError(IsErr);
			break;
		case 1:
			m_roundpage.IsError(IsErr);
			break;
		case 2:
			m_anglepage.IsError(IsErr);
			break;
		case 3:
			m_anglespage.IsError(IsErr);
			break;
		case 4:
			m_aptpage.IsError(IsErr);
			break;
		case 5:
			m_shapedpage.IsError(IsErr);
			break;
		case 6:
			m_broundpage.IsError(IsErr);
			break;
		case 7:
			m_borepage.IsError(IsErr);
			break;
		}
		if(pToolShaped) delete pToolShaped;
		if(pToolDisk) delete pToolDisk;
		if(pTool) delete pTool;
		pToolShaped = NULL;
		pToolDisk = NULL;
		pTool = NULL;
		return false;
	}
	return true;
}

void NToolsCrDiskSheet::OnOK()
{
	if (Apply())
		EndDialog(IDOK);
}

void NToolsCrDiskSheet::ClearTools()
{
	if(pToolDisk) delete pToolDisk;
	if(pTool) delete pTool;

}
