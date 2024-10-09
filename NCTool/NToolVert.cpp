#include "stdafx.h"
#include "ConstDef.h"
#include "NToolsCrDiskSheet.h"
#include "NCadrGeom.h"
#include "NToolVert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NToolVert, NToolShaped, 0)

NToolVert::NToolVert()
{
	SetType(72);
}

//

NToolVert::NToolVert(const CString & Name, const CString & FileName, const NContour& Cont, bool CutCont) : NToolShaped(Name, Cont, CutCont)
{
	SetType(72);
	m_FileName = FileName;
}

NToolVert::NToolVert(const CString &Name, double a, double D, double H, double h, double L, double W, double R) : NToolShaped(Name)
{
	m_a = a;
	m_D = D;
	m_H = H;
	m_h = h;
	m_L = L;
	m_W = W;
	m_R = R;
}

void NToolVert::Serialize(CArchive &ar)
{
	NToolShaped::Serialize(ar);
	SerializeElements(ar, &m_a, 1);
	SerializeElements(ar, &m_D, 1);
	SerializeElements(ar, &m_H, 1);
	SerializeElements(ar, &m_h, 1);
	SerializeElements(ar, &m_L, 1);
	SerializeElements(ar, &m_R, 1);
	SerializeElements(ar, &m_W, 1);
	SerializeElements(ar, &m_FileName, 1);

	if (ar.IsLoading())
	{
		double bz = Zdisp;
		GenInit();
		SetZdisp(bz);
	}
}


NToolVert::~NToolVert()
{
}

NToolVert* NToolVert::CreateCopy()
{
	NToolVert *pCreatedTool = new NToolVert;

	NToolShaped::CreateCopyTo(pCreatedTool);

	pCreatedTool->m_a = m_a;
	pCreatedTool->m_D = m_D;
	pCreatedTool->m_H = m_H;
	pCreatedTool->m_h = m_h;
	pCreatedTool->m_L = m_L;
	pCreatedTool->m_R = m_R;
	pCreatedTool->m_W = m_W;
	pCreatedTool->m_FileName = m_FileName;
	return pCreatedTool;
}

int NToolVert::GenInit()
{
	if (m_FileName.IsEmpty())
	{
		if (!MakeContour())
			return -1;
	}

	if (NToolShaped::GenInit() < 0)
		return -1;

	VertTool = false;
	if (m_FileName.IsEmpty())
		SetH(m_L);
	else
		SetH(Gabar.Zmax);

// Check contour geometry
// contour should be right in Z direction
	int ExtrNum = 0; // a number of extremums
	bool PrevDirLR = false;
	int i = 0;
	// find initial direction
	for (; i < Cont.num; ++i)
	{
		if (Cont.lns[i * 3].GetX() == Cont.lns[i * 3 + 3].GetX())
			continue;
		PrevDirLR = (Cont.lns[i * 3].GetX() < Cont.lns[i * 3 + 3].GetX());
		break;
	}
	if (i == Cont.num)
		return -1;
	for (++i; i < Cont.num; ++i)
	{
		if (Cont.lns[i * 3].GetX() == Cont.lns[i * 3 + 3].GetX())
			continue;
		bool CurDirLR = (Cont.lns[i * 3].GetX() < Cont.lns[i * 3 + 3].GetX());
		if (CurDirLR != PrevDirLR)
		{
			PrevDirLR = CurDirLR;
			break;
		}
	}
	if (i == Cont.num)
		return -1;
	// first extremum found
	for (++i; i < Cont.num; ++i)
	{
		if (Cont.lns[i * 3].GetX() == Cont.lns[i * 3 + 3].GetX())
			continue;
		bool CurDirLR = (Cont.lns[i * 3].GetX() < Cont.lns[i * 3 + 3].GetX());
		if (CurDirLR != PrevDirLR)
		{
			PrevDirLR = CurDirLR;
			break;
		}
	}
	if (i != Cont.num)
		return -1;// second extremum found
	Cont = Cont * BMatr().Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., Zdisp, 1.));
	SetType(72);
	return 0;
}

NToolVert * NToolVert::MakeTool(const CString & Name, const CString & FileName, NContour & ToolCont, bool CutCont)
{
	if (ToolCont.num <= 0)
		return nullptr;
	// Check last point
	BPoint P = ToolCont.lns.GetAt(ToolCont.lns.GetSize() - 1);
	if (fabs(P.GetX()) > 1.e-4)
	{
		P.SetX(0.);
		ToolCont.AddFollow(P, P, P);
		ToolCont.Types[ToolCont.num - 1] = HORIZONTAL;
	}
	// Check first line
	if (ToolCont.Types[0] != HORIZONTAL && ToolCont.Types[0] != SPHERE)
	{
		ToolCont.Types.InsertAt(0, HORIZONTAL);
		BPoint Sp = ToolCont.lns.GetAt(0);
		Sp.SetX(0.01);
		ToolCont.lns.InsertAt(3, Sp);
		ToolCont.lns.InsertAt(3, Sp);
		ToolCont.lns.InsertAt(3, Sp);
		++ToolCont.num;
	}
	ToolCont.SetMiddlePoints();
	ToolCont.Invert();
	NToolVert *pVert = new NToolVert(_T("VertTool"), FileName, ToolCont, CutCont);
	if (pVert->GenInit() < 0)
	{
		delete pVert;
		return nullptr;
	}
	return pVert;
}

void NToolVert::FillSheet(CPropertySheet *pDial)
{
	NToolShaped::FillSheet(pDial);
	NToolsCrDiskSheet *pD = (NToolsCrDiskSheet *)pDial;
	int TypePage = NToolShaped::GetType();
	switch (TypePage)
	{
	case 72: // 
		pD->m_borepage.m_name = Name;
		pD->m_borepage.m_FileName = m_FileName;
		pD->m_borepage.m_check_file = !m_FileName.IsEmpty();
		if (!pD->m_borepage.m_check_file)
		{
			pD->m_borepage.m_a = m_a;
			pD->m_borepage.m_D = m_D;
			pD->m_borepage.m_H = m_H;
			pD->m_borepage.m_h = m_h;
			pD->m_borepage.m_L = m_L;
			pD->m_borepage.m_R = m_R;
			pD->m_borepage.m_W = m_W;
		}
		pD->tab_pos = 7;
		break;
	}
}

bool NToolVert::MakeContour()
{
	Cont.Clear();
	NCadrGeom Cadr;
	Cadr.Set(line, 0., m_H, 0., m_D * 0.5, m_H, 0.);
	Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
	if (m_a != 0.)
	{
		Cadr.SetE(Cadr.GetE() - BPoint(0., m_H - m_L, 0., 0.));
		Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
		if (m_R == 0.)
		{
			Cadr.SetE(Cadr.GetE() - BPoint(m_L * tan(m_a * PI / 180.), m_L, 0., 0.));
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.SetE(Cadr.GetE() - BPoint(m_W - 2. * m_L * tan(m_a * PI / 180.), 0., 0., 0.));
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.SetE(Cadr.GetE() + BPoint(-m_L * tan(m_a * PI / 180.), m_L, 0., 0.));
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
		}
		else
		{
			BPoint Corner(m_D * 0.5 - m_L * tan(m_a * PI / 180.), 0., 0., 1.);
			double Dx = m_R * tan(PI / 4. - m_a * PI / 360.);
			BPoint Vec(m_L * tan(m_a * PI / 180.), m_L, 0., 0.);
			Vec = Vec * (Dx / sqrt(Vec.D2()));
			BPoint p0 = Corner + Vec;
			BPoint C(Corner + BPoint(-Dx, m_R, 0., 0.));

			Cadr.SetE(p0);
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.SetType(cwarc);
			Cadr.SetI(C - p0);
			Cadr.SetE(Corner + BPoint(-Dx, 0., 0., 0.));
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());

			double Bot = m_W - 2. * m_L * tan(m_a * PI / 180.);
			if (fabs(Bot - 2. * Dx) > MIND)
			{
				Cadr.SetType(line);
				Cadr.SetE(Cadr.GetE() + BPoint(-Bot + 2. * Dx, 0., 0., 0.));
				Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
				Cadr.SetType(cwarc);
			}

			Corner += BPoint(-Bot, 0., 0., 0.);
			Vec.SetX(-Vec.GetX());
			p0 = Corner + Vec;
			C += BPoint(-Bot + 2. * Dx, 0., 0., 0.);

			Cadr.SetI(C - Cadr.GetB());
			Cadr.SetE(p0);
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.SetType(line);
			Cadr.SetE(m_D * 0.5 - m_W, m_L, 0.);
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
		}
	}
	else
	{
		if (m_R == 0.)
		{
			Cadr.SetE(Cadr.GetE() - BPoint(0., m_H, 0., 0.));
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.SetE(Cadr.GetE() - BPoint(m_W, 0., 0., 0.));
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
		}
		else
		{
			Cadr.SetE(m_D * 0.5, m_R, 0.);
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.Set(cwarc, m_D * 0.5, m_R, 0., m_D * 0.5 - m_R, 0., 0., -m_R, 0., 0., XY);
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			if (m_W != m_R * 2.)
			{
				Cadr.SetType(line);
				Cadr.SetE(Cadr.GetE() - BPoint(m_W - m_R * 2., 0., 0., 0.));
				Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			}
			Cadr.Set(cwarc, Cadr.GetE().GetX(), Cadr.GetE().GetY(), 0., Cadr.GetE().GetX() - m_R, Cadr.GetE().GetY() + m_R, 0., 0., m_R, 0, XY);
			Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
			Cadr.SetType(line);
		}
	}
	Cadr.SetE(m_D * 0.5 - m_W, m_H - m_h, 0.);
	Cont.AddGeomElem(&Cadr); Cadr.SetB(Cadr.GetE());
	Cadr.SetE(0., m_H - m_h, 0.);
	Cont.AddGeomElem(&Cadr);
	Cont.ExchangeYZ();
	return true;
}

bool NToolVert::GetStdParams(double &Diameter, double &CornerRadius, double &ELength, double &FLength, double &BottomAngle, double &SideAngle, double &Height)
{
	Diameter = m_D;
	CornerRadius = m_R;
	ELength = 0.;
	FLength = 0.;
	BottomAngle = 0.;
	SideAngle = m_a;
	Height = m_H;
	return true;
}
