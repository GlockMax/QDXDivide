#include "stdafx.h"
#include "NSurfSettings.h"
#include "NCadrsProcessing.h"
#include "NMeshArray.h"
#include "BPoint.h"
#include "NLine.h"
#include "NToolCombined.h"
#include "NToolDiskStd.h"
#include "NCadrGeom.h"
#include "NToolStd.h"
#include "NToolStdCC.h"
#include "curve.h"
#include "BCurve.h"
#include "ConstDef.h"

#include "math.h"
#include "NRectMesh.h"
#include "BPointsBuf.h"
#include "NCMProject.h"
#include "FChain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

int compare_double(const void *a, const void *b)
{
	if(*(double*)a>*(double*)b)
		return -1;
	return (*(double*)a==*(double*)b) ? 0 : 1;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCadrsProcessing::NCadrsProcessing()
{
	pComTool = NULL;
	Rmax = 0.;
	ApproximatedFragment = false;
	CutHelix = false;
	int *pInt;
	if (NCM_PROJECT.IntParams.Lookup("CutHelix", pInt))
		if (*pInt != 0)
			CutHelix = true;
}

NCadrsProcessing::~NCadrsProcessing()
{
	Clear();
}

// начать передачу кадров
// возвращает: -1 при ошибке
int NCadrsProcessing::BeginIssue(const NToolCombined *pT)
{
	if(!pT)
		return -1;
	if(pT->num_tools <= 0)
		return -1;
	Clear();
	pComTool = pT;
	CadrProc.SetToolsNum(pComTool->num_tools);
// Put cutting tools first for collision detection
	int NewToolNum = 0;
	for(int iTool = 0; iTool < pComTool->num_tools; ++iTool)
	{// Put cutting tools
		NTool *pSimTool = pComTool->GetTool(iTool);
		if(pSimTool)
		{
			if(pSimTool->IsCutting())
			{
				NContour *pContour = new NContour;
				if(NTool::CreateSimGeneratrix(pSimTool, *pContour))
				{
					Rmax = max(pContour->GetMaxX(), Rmax);
					CadrProc.SetToolsContour(NewToolNum, pContour, pSimTool->IsCutting(), pSimTool->IsShank(), pSimTool->GetColor(), pSimTool->IsConcave());
					if(pSimTool->IsStdCC())
						CadrProc.SetDataConcave(NewToolNum, ((NToolStdCC *)pSimTool)->GetConcaveRad());
					++NewToolNum;
				}
				else
					delete pContour;
			}
		}
	}
	if(NTool::GetToolStockCollision() == CWS_ALL)
	{		
		for(int iTool = 0; iTool < pComTool->num_tools; ++iTool)
		{// Put non cutting tools
			NTool *pSimTool = pComTool->GetTool(iTool);
			if(pSimTool)
			{
				if(!pSimTool->IsCutting())
				{
					NContour *pContour = new NContour;
					if(NTool::CreateSimGeneratrix(pSimTool, *pContour))
					{
						Rmax = max(pContour->GetMaxX(), Rmax);
						CadrProc.SetToolsContour(NewToolNum, pContour, pSimTool->IsCutting(), pSimTool->IsShank(), pSimTool->GetColor(), pSimTool->IsConcave());
						if(pSimTool->IsStdCC())
							CadrProc.SetDataConcave(NewToolNum, ((NToolStdCC *)pSimTool)->GetConcaveRad());
						++NewToolNum;
					}
					else
						delete pContour;
				}
			}
		}
	}

	CadrProc.CreateWConts();

	return 0;
}


// определение траектории движения для одного кадра
// (состоит из одного или нескольких участков)
BOOL NCadrsProcessing::CreateDirection(int index, NContour &CadrContour) const
{
	// задание отрезка:
	// 0 - начальная точка
	// 3 - конечная точка
	// точки 1, 2 - совпадают и равны (p0 + p3)/2

	// задание дуги окружности:
	// 0 - начальная точка
	// 3 - конечная точка
	// касательные в начальной и конечной точках перпендикулярны 
	// векторам с-p0 и с-p3 соответственно

	if (Cadrs.GetSize() <= index)
		return FALSE;

	CadrContour.Clear();

	double i,j,k,xn,yn,zn;
	BPoint pb(0., 0., 0., 1.), pe(0., 0., 0., 1.), pn(0., 0., 0., 1.), pi(0., 0., 0., 1.);
	NLine ln;
	double eps = MIND;

	BPoint Pb = Cadrs[index]->GetB();
	BPoint Pe = Cadrs[index]->GetE();
	BPoint e = Pe - Pb;
	NCadrGeom *pGeom = Cadrs[index];

	int type = pGeom->GetType().type;
	if(pGeom->IsLine())
	{
		CadrContour.AddFirst(Pb,(Pb + Pe)*0.5,(Pb + Pe)*0.5,Pe);
	}
	else if(pGeom->IsArc())
	{
		Cadrs[index]->GetI(&i,&j,&k);
		Cadrs[index]->GetN(&xn,&yn,&zn);

		// Pb - begin; Pe - end; Pi - Pcenter-Pb
		pb = Pb;
		pe = Pe;
		pn.Set(xn, yn, zn, 0);
		pi.Set(i, j, k, 0);

		BMatr Transform;
		BPoint a(0., 0., 0., 1.), b(0., 0., 0., 1.), c(0., 0., 0., 1.), q(0., 0., 0., 1.), tmp(0., 0., 0., 1.);
		double CosAlf, alf;

		Transform.e0cong(pn,BPoint(0.,0.,1.,0.));

		pb = pb * Transform;
		pe = pe * Transform;
		pi = pi * Transform;

		c = pb+pi;
		BPoint CenterPoint = c;

		Transform.Trans(c, BPoint(0.,0.,0.,1.));

		pb = pb * Transform;
		pe = pe * Transform;
		pi = pi * Transform;
		c = c * Transform;

		// определение числа сегментов окружности
		
		// проверка на совпадение начальной и конечной точек
		double dp1p2 = (pe-pb).D2();
		if(dp1p2 < eps*eps )// pb=pe
		{
			pb = ( pb + pe )*0.5;
			pe = pb;
		}
		q = pb + pi - pe;// Pq - Pcenter-Pe
		double r2 = pi.D2();
		if(r2 == 0.)
			return false;
		double sp = pi * q;
		BPoint nv = q % pi;// normal vector

		bool sign = (nv.GetZ() >= 0.);//"sign" of vector cross product
		// find coordinate that has maximum module
		double max = fabs(nv.GetZ());
	//	max = fabs(n.GetX())>fabs(n.GetY())?n.GetX():n.GetY();
	//	max = fabs(max)>fabs(n.GetZ())?max:n.GetZ();
		if(fabs(max) < MCR * r2)
			sign = true;
		int k;//Number of segments
		if(sp > cos(120.*PI/180.)*r2)// Angle < 2 || >358
		{
			if( (!sign && type == ccwarc) ||
				( sign && type ==  cwarc))// Angle < 120
				k=1;	
			else// Angle > 240
				k=3;
		}
		else if (sp == 0.) // Angle 90
		{
			k = 1;
		}
		else	// Angle >120 && <240
		{
			k=2;
		}

		if(dp1p2 < eps*eps && sp > 0.)// Angle = 360
		{
			sign = (type == ccwarc);
			k = 3;
		}

		a = pi*(-1);
		b = (pe-pb)-pi;
		CosAlf = a.Cos2(a, b);
		CosAlf = CosAlf*sqrt(a.D2()*b.D2());	// cos alf
		if(CosAlf < -1.)
			CosAlf = -1.;
		else if(CosAlf > 1.)
			CosAlf = 1.;
		alf = acos(CosAlf);
		if(sign == (type == ccwarc))
			alf = 2*PI - alf;

		if (k != 1)
		{
			alf = alf/k;
		
			if(type == cwarc) 
				alf = -alf;
			CosAlf = cos(alf);
			Transform.RotZ(c, alf*180./PI);
		}

		NLine ln;
		// Create control points for arc

		switch(k)
		{
		case 3:
			q = pb * Transform;

			ln.Set(pb, c, c, q);
			ln.BuildBallArc();

			CadrContour.AddFirst(ln);
			
			pb = q;
		case 2:
			q = pb * Transform;

			ln.Set(pb, c, c, q);
			ln.BuildBallArc();

			if(CadrContour.num == 0)
				CadrContour.AddFirst(ln);
			else
				CadrContour.AddFollow(ln);

			pb = q;
		case 1:// single segment
			ln.Set(pb, c, c, pe);
			ln.BuildBallArc();

			if(CadrContour.num == 0)
				CadrContour.AddFirst(ln);
			else
				CadrContour.AddFollow(ln);

		}

		// возвращение в начальное положение
		Transform.Trans(BPoint(0.,0.,0.,1.), CenterPoint);
		CadrContour = CadrContour*Transform;
		Transform.e0cong(BPoint(0.,0.,1.,0.),pn);
		CadrContour = CadrContour*Transform;
	}
	return TRUE;
}

void NCadrsProcessing::SavePlanes(MeshArr &arr)
{
	if(!DEBUGMODE)
		return;
	CStdioFile f;
	CString str, s;
	int i, j, k;
	BOOL res;
	CString path, fname;

	//path = _T("C:\\VCPROJECTS\\NCM25_test\\Report");
	path = _T("C:\\VCPROJECTS\\NCM31\\Report");
	fname = _T("Meshs.txt");

	res = f.Open(path+"\\"+fname,CFile::modeWrite | CFile::modeCreate);
	if (!res)
	{
		AfxMessageBox("Невозможно открыть файл для записи координат!");
		return;
	}
	//поставить позицию курсора в конец файла
	f.SeekToEnd();
	
	f.WriteString("---------------------");
	str.Format("%s", "Сетки - ");
	f.WriteString(str);
	COleDateTime t = COleDateTime::GetCurrentTime();
	str = t.Format("%d %B %Y : %H %M %S");
	f.WriteString(str+"\n");
	
	for (i=0; i<arr.GetSize(); i++)
	{
		str.Format("Сетка %d, кадр %d\n", i+1, arr[i]->m_CadrNum);
		f.WriteString(str);
		for (j=0; j<((NRectMesh *)arr[i])->m_r; j++)
		{
			j == 0 ? str = "  нач.: " : str = "  кон.: ";
			s.Format("(%d) ", ((NRectMesh *)arr[i])->m_arr[j].GetSize());
			str += s;
			for (k=0; k<((NRectMesh *)arr[i])->m_arr[j].GetSize(); k++)
			{
				s.Format("%d-%d ", k, ((NRectMesh *)arr[i])->m_arr[j].GetAt(k));
				str += s;
			}
			f.WriteString(str+"\n");
		}
	}
	f.Close();
	return;
}
INT_PTR NCadrsProcessing::SendCadrsArray(NCadrGeom * InArr, INT_PTR GSize, INT_PTR  StartInd)
{
	Clear();
	ApproximatedFragment = false;
	if(GSize <= StartInd)
		return 0;
	// Find first cadr
	NCadrGeom *pPrev;
	int n = 0;
	for(; n + StartInd < GSize; ++n)
	{
		pPrev = &InArr[n + StartInd];
		ReplaceRotateCadr(pPrev);
		if(MayBeApproximated(pPrev, n))
		{
			if(n == 0)
			{
				if(GSize > 1)
				{
					delete Cadrs[Cadrs.GetSize() - 1];
					Cadrs[Cadrs.GetSize() - 1] = new NCadrGeom(InArr[1 + StartInd]);
				}
				return Cadrs.GetSize() - 1;// Special additional cadr have been added in MayBeApproximated
			}
			else
				return Cadrs.GetSize();// Undef cadrs precede the cadr which may be approximated. "Cadrs" contains undef cadrs only .
		}
		//if(!ApproxSpline(pPrev))
			Cadrs.Add(pPrev);
		if(pPrev->HaveGeom())
			break;
	}
	if(pPrev->Length() < MIND)
		pPrev->SetType(undef);
	if(n + StartInd == GSize)
		return Cadrs.GetSize();
	if(pPrev->IsVertical())
		return Cadrs.GetSize();
	NCadrGeom::FitCadrs(NULL, pPrev, false);

	NCadrGeom *pCur;
	for(auto i = n + StartInd + 1;  i < GSize; ++i)
	{
		pCur = &InArr[i];
		if(pCur->IsUndef())
		{
			Cadrs.Add(pCur);		
			continue;
		}
		ReplaceRotateCadr(pCur);
		if(pPrev->Length() < MIND)
			pPrev->SetType(undef);
		if (pCur->IsVertical() || pCur->IsNotXYArc() || pComTool->IsConcave())
			break;
		if (pCur->IsArc())
			if (IsLarge(pCur))
			{
				// Add one extra geom cadr
				Cadrs.Add(pCur);
				return Cadrs.GetSize() - 1;
			}
		if(pCur->IsRotate() || (pCur->IsSpline() && pCur->GetCurve()))// Simple rotate cadr has been replaced earlier
		{
			// Add one extra geom cadr
			NCadrGeom *pNewGeom = new NCadrGeom(*(Cadrs.GetAt(Cadrs.GetSize() - 1)));
			pNewGeom->Reverse();
			Cadrs.Add(pNewGeom);
			break;
		}
		if(NGlobalState::DX != NCM_PROJECT.GetGlobalState().GetModelMode() && !(pCur->IsArc() && IsSmall(pCur)))
			NCadrGeom::FitCadrs(pPrev, pCur, false);
		Cadrs.Add(pCur);
		pPrev = pCur;
	}
	return Cadrs.GetSize();
}
double NCadrsProcessing::MaxRad(const BPoint & e0, const BPoint & P0, const BPoint & P1, const BPoint & P2)
{
	BPoint e1 = P2 - P1;
	double dl1 = sqrt(e1.D2());
	e1 = e1 * (1./dl1);
	BPoint l = P1 - P0;
	double dl = sqrt(l.D2());
	l = l * (1./dl);
	double ca = e0 * l;
	double cb = l * e1;
	BPoint sa = e0 % l;
	BPoint sb = l % e1;
	if( sa * sb < 0. )
		ca = 1.;
	double am = fabs((1-ca));
	double bm = fabs((1-cb));
	bm = max(M1C, bm);
	double ap = fabs((1+ca));
	double bp = fabs((1+cb));
	double R0 = dl * sqrt(ap * bp) / (sqrt(am * bp) + sqrt(bm * ap));
	double R1 = dl1*sqrt(bp/bm);
	return min(R0,R1);
}

INT_PTR NCadrsProcessing::BuildSurf2d(MeshsArr &PlaneArray, INT_PTR SInd, bool ChainEndBis)
{
	NLine Line;
	NContour DirectContour;
	auto first = SInd;
	for(; first < Cadrs.GetSize(); ++first)
	{
		if(Cadrs[first]->HaveGeom())
			break;
	}
	if(first == Cadrs.GetSize())
		return first;
	auto next = first + 1;
	for(; next < Cadrs.GetSize(); ++next)
	{
		if(Cadrs[next]->IsRotate())
		{// Next cadr can't be found
			next = Cadrs.GetSize();
			break;
		}
		if(Cadrs[next]->HaveGeom())
		{
			if(!Cadrs[next]->IsHorizontal())
			{// Next cadr can't be found
				next = Cadrs.GetSize();
				break;
			}
			else
				Cadrs[next]->MakeHorizontal();
			break;// Next cadr found
		}
	}
	if (!CreateDirection(int(first), DirectContour))
		return 0;
	
	DirectContour.GetLine(0, Line);
	// построить сетки для одного кадра
	BPoint e(0., 0., 0., 0.); // Start direction of the next cadr
	if(next < Cadrs.GetSize())
	{
		// Calculate start dir of the next cadr
		e = Cadrs.GetAt(next)->GetStartDirP();
	}
	CadrProc.m_CadrNum = int(first);
	if(Cadrs[first]->IsLine())
	{
		CadrProc.BuildSurf2d(Line, e, PlaneArray, ChainEndBis);
	}
	else if(Cadrs[first]->IsArc())
	{
		for( int kc = 0; kc < DirectContour.num - 1; ++kc)// more than one part in the arc
		{
			BPoint dir = Line.GetDir(1);
			CadrProc.BuildSurf2dArc(Line, dir, PlaneArray, ChainEndBis, false);
			DirectContour.GetLine(kc+1, Line);
		}
		CadrProc.BuildSurf2dArc(Line, e, PlaneArray, ChainEndBis, true);
	}

//	if(DEBUGMODE)
//		SavePlanes(PlaneArray);
	return next;
}
INT_PTR NCadrsProcessing::BuildSurf2dArcNew(MeshsArr& PlaneArray, INT_PTR SInd, bool ChainEndBis)
{
	auto next = SInd + 1;
	for (; next < Cadrs.GetSize(); ++next)
	{
		if (Cadrs[next]->IsRotate())
		{// Next cadr can't be found
			next = Cadrs.GetSize();
			break;
		}
		if (Cadrs[next]->HaveGeom())
		{
			if (!Cadrs[next]->IsHorizontal())
			{// Next cadr can't be found
				next = Cadrs.GetSize();
				break;
			}
			else
				Cadrs[next]->MakeHorizontal();
			break;// Next cadr found
		}
	}
	NLine Line;
	NContour DirectContour;
	if (!CreateDirection(int(SInd), DirectContour))
		return 0;
	DirectContour.GetLine(0, Line);
	// построить сетки для одного кадра
	BPoint e(0., 0., 0., 0.); // Start direction of the next cadr
	if (next < Cadrs.GetSize())
	{
		// Calculate start dir of the next cadr
		e = Cadrs.GetAt(next)->GetStartDirP();
	}
	CadrProc.m_CadrNum = int(SInd);
	CadrProc.SetInitData2d(Line.p[0], Line.p[3] - Line.p[0], e);

	for (int kc = 0; kc < DirectContour.num - 1; ++kc)// more than one part in the arc
	{
		BPoint dir = Line.GetDir(1);
		CadrProc.BuildSurf2dArcNew(Line, dir, PlaneArray);
		DirectContour.GetLine(kc + 1, Line);
	}
	CadrProc.BuildSurf2dArcNew(Line, e, PlaneArray);
	return INT_PTR();
}
void NCadrsProcessing::CrStartCont(int ChainStart)
{
	int first = ChainStart;
	for(; first < Cadrs.GetSize(); ++first)
	{
		if(Cadrs[first]->HaveGeom())
			break;
	}
	if(first == Cadrs.GetSize())
		return;

	CadrProc.m_CadrNum = first;
	CadrProc.SetInitData2d(Cadrs[first]->GetB(), Cadrs[first]->GetStartDir(), BPoint(0,0,0,0));

	CadrProc.SetInitConts();
}

void NCadrsProcessing::FindMaxChain(FChain &CurChain) const
{
	CurChain.PrevChainEndsBis = CurChain.ChainEndsBis;

	// End = Start if no cadrs can be processed
	// End = last cadr number + 1 otherwize
	CurChain.SetType(CHAIN_EMPTY);
	CurChain.End = CurChain.Start;
	NCadrGeom *pPrev = NULL;
	NCadrGeom *pCur = NULL;
	//Find first cadr with geom
	for (; CurChain.Start < Cadrs.GetSize() && CurChain.Start < CurChain.MaxNumber; ++CurChain.Start)
	{
		pCur = Cadrs[CurChain.Start];
		if(pCur->HaveGeom() || pCur->type.type == addcoord)
			break;
	}
	if (CurChain.Start == CurChain.MaxNumber || CurChain.Start == Cadrs.GetSize())
	{
		CurChain.End = CurChain.Start + 1;
		return;
	}
	if (pCur->GetType().type == addcoord)
	{
		CurChain.SetType(CHAIN_ADDCOORD);
		CurChain.End = CurChain.Start + 1;
		return;
	}
	if (pCur->IsVertical())
	{
		CurChain.SetType(CHAIN_VERTICAL);
		CurChain.End = CurChain.Start + 1;
		return;
	}
	if(pCur->IsArc() && !pComTool->IsConcave())
	{
		bool IsFull = pCur->IsFullArc(FARC);
		if(IsSmall(pCur))
		{
			CurChain.SetType(IsFull ? CHAIN_2D_SMALL_FULL_ARC
				: (IsNearSmall(pCur) ? CHAIN_2D_NEARSMALL_ARC : CHAIN_2D_SMALL_ARC));
			CurChain.End = CurChain.Start + 1;
			return;
		}
		if(IsFull)
		{
			CurChain.SetType(CHAIN_2D_FULL_ARC);
			CurChain.End = CurChain.Start + 1;
			return;
		}

	}
// pCur has geom and is not vertical or small arc or 3D arc
	if(!pCur->IsHorizontal() || pComTool->IsConcave())
	{// 3d cadr
		CurChain.SetType(CHAIN_3D);
		CurChain.ChainEndsBis = false;
		CurChain.End = CurChain.Start + 1;
		return;
	}
	
// pCur has geom is horizontal and is not small arc
	NCadrGeom *pStart = pCur;
	CurChain.SetType(CHAIN_2D);
	CurChain.ChainEndsBis = true;
	BPoint e0 = pCur->GetStartDir();
	auto i = CurChain.Start;
	for (; i < CurChain.MaxNumber;)
	{
		// find next
		NCadrGeom *pNext = NULL;
		auto k = i + 1;
		for(; k < Cadrs.GetSize(); ++k)
		{
			if(Cadrs[k]->HaveGeom())
			{
				pNext = Cadrs[k];
				break;
			}
		}
		if(pNext)
			if(!pNext->IsHorizontal())
				pNext = NULL;
		if(k >= CurChain.MaxNumber)
			CurChain.ChainEndsBis = false;
		// Radius test
		if(pNext)
		{
			if (pCur->IsFast() != pNext->IsFast())
			{
				CurChain.ChainEndsBis = false;
				CurChain.End = i + 1;
				return;
			}
			// Angle test 
			double Angle,sa,ea;
			pCur->GetArcA(&sa,&ea,&Angle);
			Angle = max(fabs(Angle), fabs(pStart->GetStartDir().Angle(pCur->GetEndDir())));
			if(Angle > 100. )
			{
				CurChain.ChainEndsBis = false;
				CurChain.End = (i == CurChain.Start) ? i + 1 : i;
				return;
			}
			if(pNext->IsArc())
			{
				
				if (fabs(pNext->GetStartDir().Angle(pCur->GetEndDir())) > MINAD)
				{
					CurChain.ChainEndsBis = false;
					CurChain.End = i + 1;
					return;
				}
				if (IsSmall(pNext))
				{
					CurChain.ChainEndsBis = false;
					CurChain.End = i + 1;
					return;
				}
				if (pNext->IsFullArc(FARC))
				{
					CurChain.ChainEndsBis = false;
					CurChain.End = i + 1;
					return;
				}
			}
		}
		if(pCur->IsArc())
		{
			CurChain.ChainEndsBis = false;
			if(pNext)
				if(fabs(pNext->GetStartDir().Angle(pCur->GetEndDir())) > MINAD)
				{
					CurChain.End = i + 1;
					return;
				}
			if(IsSmall(pCur))
			{
				CurChain.ChainEndsBis = false;
				CurChain.End = i + 1;
				return;
			}

		}
		else
		{
			BPoint P0 = pCur->GetB();
			BPoint P1 = P0 + pCur->GetStartDir();
			BPoint P2 = P1 + (P1 - P0);
			if(pNext)
				P2 = P1 + pNext->GetStartDir();
			e0 = e0 * (1./sqrt(e0.D2()));
			double MaxToolRad = MaxRad(e0,P0,P1,P2);
			if(MaxToolRad <= Rmax)
			{
				CurChain.ChainEndsBis = false;
				P2 = P1 + (P1 - P0);
				MaxToolRad = MaxRad(e0,P0,P1,P2);
				if(MaxToolRad <= Rmax)
				{
					CurChain.End = (i == CurChain.Start) ? i + 1 : i;
					return;// Current cadr can not be processed
				}
				CurChain.End = i + 1;
				return;// Current cadr can only be processed without next cadr
			}
		}
		e0 = pCur->GetEndDir();
		i = k;
		if(pNext)
		{
			pPrev = pCur;
			pCur = pNext;
		}
		else
		{
			CurChain.End = (i == CurChain.Start) ? i + 1 : i;
			return;
		}
	}
	CurChain.End = (i == CurChain.Start) ? i + 1 : i;
	return;
}
int NCadrsProcessing::GetFirstGeomCadr(int Start) const
{
	for(; Start < Cadrs.GetSize(); ++Start)
		if(Cadrs[Start]->HaveGeom() || Cadrs[Start]->type.type == addcoord)
			return Start;
	return Start;
}
bool NCadrsProcessing::CrChainMeshs(MeshsArr &PlArray, const FChain &CurChain)
{
	switch(CurChain.GetType())
	{
	case CHAIN_EMPTY:
		return false;
	case CHAIN_ADDCOORD:
		break;
	case CHAIN_3D:
		Cr3dChainMeshs(PlArray, CurChain);
		//if (CurChain.End == CurChain.Start + 1)
		//	PlArray.SetConvex(true);
		break;
	case CHAIN_VERTICAL:
		// Vertical cadr is the last cadr in the chain
		BuildSurfVert(PlArray, int(CurChain.End - 1));
		//if (CurChain.End == CurChain.Start + 1)
		//	PlArray.SetConvex(true);
		break;
	case CHAIN_2D_SMALL_ARC:
	case CHAIN_2D_NEARSMALL_ARC:
		// Small arc 2d chain processing but not full arc
		// Small arc is the last cadr in the chain
		Cr2dSArcChainMeshs(PlArray, CurChain);
//		Cr2dSArcChainMeshsNew(PlArray, CurChain);
		break;
	case CHAIN_2D_SMALL_FULL_ARC:
	default:
		//Ordinary 2d chain processing
		Cr2dChainMeshs(PlArray, CurChain);
		//if (CurChain.End == CurChain.Start + 1)
		//	PlArray.SetConvex(true);
		break;
	}
	// Set fast color if necessary
	auto k = CurChain.Start;
	for(; k < CurChain.End; ++k)
	{
		if(Cadrs[k]->IsFast())
			break;
	}
	bool IsFast = false;
	if(k == CurChain.End)
		return IsFast;
	int FastColNum = FAST_COLOR_NUM;
	for(int i = 0; i < PlArray.GetSize(); ++i)
	{
		MeshArr *pArr = PlArray[i];
		if(!pArr)
			continue;
		for(int j = 0; j < pArr->GetSize(); ++j)
		{
			NParamMesh *pMesh = pArr->GetAt(j);
			if(!pMesh)
				continue;
            ElemID ElID = pMesh->GetMeshID();
			NCadrGeom *pCadrGeom = Cadrs.GetAt(pMesh->m_CadrNum);
			IsFast = pCadrGeom->IsFast();
			bool ProcAsFast = false;
			switch (NTool::GetFastColorMode())
			{
			case FCM_ALL:
				ProcAsFast = IsFast;
				break;
			case FCM_NO:
				break;
			case FCM_ALLBUTVERT:
				ProcAsFast = IsFast && !pCadrGeom->IsVerticalUp();
				break;
			}
			if (ProcAsFast)
			{
				MElemIDProc::SetPCn(&ElID, FastColNum);
				pMesh->SetMeshID(ElID);
			}
		}
	}
	return IsFast;
}

int NCadrsProcessing::BuildSurfVert(MeshsArr &PlaneArray, int CadrNum)
{
	NCadrGeom *pCadr = Cadrs[CadrNum];
	if(!pCadr->IsVertical())
		return -1;
	CadrProc.m_CadrNum = CadrNum;
	CadrProc.SetInitData2d(pCadr->GetB(), pCadr->GetStartDir(), BPoint(0,0,0,0));
	NLine Line;
	Line.Set(pCadr->GetB(),pCadr->GetB(),pCadr->GetE(),pCadr->GetE());
	CadrProc.BuildSurfVert(Line, PlaneArray);
	return 0;
}
bool NCadrsProcessing::TrimFlatMesh(NRectMesh *pMesh, const BPoint &C, const BPoint &N)
{
	auto n = pMesh->m_arr[0].GetSize();
	if( n < 2) 
		return false;
	CUIntArray Arr;
	const BPoint *pp0 = MFOREST.GetPoint(pMesh->m_arr[0][0]);
	bool f0 = (*pp0 - C) * N < 0.;// true - keep point
	for(int i = 0; i < n; ++i)
	{
		const BPoint *pp1 = MFOREST.GetPoint(pMesh->m_arr[0][(i+1)%n]);
		bool f1 = (*pp1 - C) * N < 0.;// true - keep point
		if(f0)
			Arr.Add(pMesh->m_arr[0][i]);
		if(f0 != f1)
		{// find intersection point
			double A = (C - *pp0) * N / ((*pp1 - *pp0) * N);
			BPoint P = (*pp0) * (1. - A) + (*pp1) * A;
			Arr.Add(MFOREST.AddPoints(P));
		}
		pp0 = pp1;
		f0 = f1;
	}
	// Remove coincident points
	pp0 = MFOREST.GetPoint(pMesh->m_arr[0][0]);
	for(int i = 0, j = 0; i < Arr.GetSize(); ++i)
	{

		const BPoint *pp1 = MFOREST.GetPoint(Arr[(j+1)%Arr.GetSize()]);
		if( (*pp1 - *pp0).D2() < MIND * MIND )
			Arr.RemoveAt(j);
		else
			++j;
		pp0 = pp1;
	}
	pMesh->m_arr[0].RemoveAll();
	pMesh->m_arr[0].Append(Arr);
	return true;
}

bool NCadrsProcessing::TrimFlatMeshWPos(NRectMesh* pMesh, const BPoint& C, const BPoint& N)
{
	// Trim and ensure cutting points are the first and last points in the result mesh
	// Returns false if intersection is not found
	// 
	auto n = pMesh->m_arr[0].GetSize();
	if (n < 2)
		return false;
	// Find first extra (unnecessary) point
	int Extra = 0;
	for (; Extra < n; ++Extra)
	{
		const BPoint* pp1 = MFOREST.GetPoint(pMesh->m_arr[0][Extra]);
		bool f1 = (*pp1 - C) * N < 0.;// true - keep point
		if (!f1)
			break;
	}
	if (Extra == n)
		return false;
	// Find last extra point
	for (; Extra < n; ++Extra)
	{
		const BPoint* pp1 = MFOREST.GetPoint(pMesh->m_arr[0][(Extra + 1) % n]);
		bool f1 = (*pp1 - C) * N < 0.;// true - keep point
		if (f1)
			break;
	}
	if (Extra == n)
	{
		pMesh->m_arr[0].RemoveAll();
		return false;
	}
	CUIntArray Arr;
	const BPoint* pp0 = MFOREST.GetPoint(pMesh->m_arr[0][Extra]);
	bool f0 = false;// true - keep point
	for (int k = 0; k < n; ++k)
	{
		int i = (k + Extra + 1) % n;
		const BPoint* pp1 = MFOREST.GetPoint(pMesh->m_arr[0][i]);
		bool f1 = (*pp1 - C) * N < 0.;// true - keep point
		if (f0)
			Arr.Add(pMesh->m_arr[0][i]);
		if (f0 != f1)
		{// find intersection point
			double A = (C - *pp0) * N / ((*pp1 - *pp0) * N);
			BPoint P = (*pp0) * (1. - A) + (*pp1) * A;
			Arr.Add(MFOREST.AddPoints(P));
		}
		if (!f1)
			break;
		pp0 = pp1;
		f0 = f1;
	}
	// intersection points are first and last now
	// Remove coincident points
	pp0 = MFOREST.GetPoint(pMesh->m_arr[0][0]);
	for (int i = 0, j = 0; i < Arr.GetSize(); ++i)
	{
		const BPoint* pp1 = MFOREST.GetPoint(Arr[(j + 1) % Arr.GetSize()]);
		if ((*pp1 - *pp0).D2() < MIND * MIND)
			Arr.RemoveAt(j);
		else
			++j;
		pp0 = pp1;
	}
	pMesh->m_arr[0].RemoveAll();
	pMesh->m_arr[0].Append(Arr);
	return true;
}

bool NCadrsProcessing::TrimRotMesh(NRectMesh *pMesh, const BPoint &C, const BPoint &N)
{
	auto n = pMesh->m_arr[0].GetSize();
	if( n < 2) 
		return false;
	const BPoint *pp0 = MFOREST.GetPoint(pMesh->m_arr[0][0]);
	bool f0 = (*pp0 - C) * N < 0.;// true - keep point
	bool KeepStart = f0;
	int IntersNum = -1;
	BPoint P(0., 0., 0., 1.);
	for(int i = 0; i < n - 1; ++i)
	{
		const BPoint* pp1 = MFOREST.GetPoint(pMesh->m_arr[0][i + 1]);
		bool f1 = (*pp1 - C) * N < 0.;// true - keep point
		if(f0 != f1)
		{// find intersection point
			double A = (C - *pp0)*N / ((*pp1 - *pp0)*N);
			P = (*pp0) * (1. - A) + (*pp1) * A;
			IntersNum = i;
			break;
		}
		pp0 = pp1;
		f0 = f1;
	}
	if(IntersNum < 0)
	{
		if(KeepStart)
		{
			return true;// Mesh is unchanged
		}
		else
		{
			pMesh->m_r = 0;// Mesh is cleared
			return true;
		}
	}
	// Intersection point found
	int IntersP = MFOREST.AddPoints(P);
	if (KeepStart)
	{
		if ((P - *MFOREST.GetPoint(pMesh->m_arr[0][IntersNum])).D2() < MIND * MIND)
			IntersNum--;
		if (IntersNum < 0)
		{
			pMesh->m_r = 0;// Mesh is cleared
			return true;
		}
		for (int i = 0; i < pMesh->m_r; ++i)
		{
			pMesh->m_arr[i].RemoveAt(IntersNum + 1, pMesh->m_arr[i].GetSize() - IntersNum - 1);
			pMesh->m_arr[i].Add(IntersP);
		}
	}
	else
	{
		if ((P - *MFOREST.GetPoint(pMesh->m_arr[0][IntersNum + 1])).D2() < MIND * MIND)
			IntersNum++;
		for (int i = 0; i < pMesh->m_r; ++i)
		{
			pMesh->m_arr[i].RemoveAt(0, IntersNum);
			pMesh->m_arr[i].SetAt(0, IntersP);
		}
	}
	// Remove coincident points
/*	pp0 = MFOREST.GetPoint(pMesh->m_arr[0][0]);
	int j = 0;
	for(i = 0; i < Arr.GetSize(); ++i)
	{

		BPoint *pp1 = MFOREST.GetPoint(Arr[(j+1)%Arr.GetSize()]);
		if( (*pp1 - *pp0).D2() < MIND * MIND )
			Arr.RemoveAt(j);
		else
			++j;
		pp0 = pp1;
	}
	pMesh->m_arr[0].RemoveAll();
	pMesh->m_arr[0].Append(Arr);
	*/
	return true;
}

void NCadrsProcessing::Clear()
{
	if(IsApproximated())
	{
		for(int i = 0; i < Cadrs.GetSize(); ++i)
			delete Cadrs[i];
		ApproximatedFragment = false;
	}
	Cadrs.RemoveAll();
}

void NCadrsProcessing::Cr3dChainMeshs(MeshsArr &PlArray, const FChain &CurChain)
{//With approximation
	// Approximate ToolsContour
	CadrProc.ApproxContour();
	CadrProc.CreateWConts();
	// 3d chain processing
	CadrProc.CurChainType = CurChain.GetType();
	CrStartCont(int(CurChain.Start));

	for(auto l = CurChain.Start, d = CurChain.Start; l < CurChain.End; l = d)
	{
		if(!Cadrs[l]->HaveGeom())
		{
			d = l + 1;
			continue;
		}
		CadrProc.m_IsLastCadr = (l == CurChain.End-1);
		d = BuildSurf3d(PlArray, int(l), false);
		if(d == l)
		{
			d += 1;
			if(DEBUGMODE) LOGFILE.WriteString("Cr3dChainMeshs:\n");
		}
	}
	// Restore ToolsContour
	CadrProc.RestoreContour();
	CadrProc.CreateWConts();
	CadrProc.IsExtLeft = false;
}
int NCadrsProcessing::BuildSurf3d(MeshsArr &PlaneArray, int SInd, bool ChainEndBis)
{
//	Source copied from BuildPlane

	NLine Line, NewLine;
	bool IsFirst = true;
	NContour DirectContour;
	int first = SInd;
	for(; first < Cadrs.GetSize(); ++first)
	{
		if(Cadrs[first]->HaveGeom())
			break;
	}
	if(first == Cadrs.GetSize())
		return first;
	int next = first + 1;
	for(; next < Cadrs.GetSize(); ++next)
	{
		if(Cadrs[next]->HaveGeom())
			break;
	}
	// построить сетки для одного кадра
	if(Cadrs[first]->IsLine())
	{
		// определить траекторию для одного кадра
		if (!CreateDirection(first, DirectContour))
			return 0;
		if (IsFirst)
			DirectContour.GetLine(0, Line);
		else
			Line = NewLine;

		// построить сетки для одного кадра
		BPoint s = Line.p[3];		
		BPoint e = Line.p[0];
		s.Norm();		
		e.Norm();
		s = s - e;
		e.Set(0,0,0,0);
		IsFirst = false;
		// если есть следующий кадр
		if (next < Cadrs.GetSize())
			if (Cadrs[next]->IsLine())
			{
				if (!CreateDirection(next, DirectContour))
					return 0;
				DirectContour.GetLine(0, NewLine);
				BPoint p = NewLine.p[3];		
				e = NewLine.p[0];
				p.Norm();		
				e.Norm();
				e = p - e;
			}
		CadrProc.m_CadrNum = first;
		CadrProc.BuildSurf3d(Line, e, PlaneArray, false);
	}
//	if(DEBUGMODE)
//		SavePlanes(PlaneArray);
	return next;
}

void NCadrsProcessing::Cr2dChainMeshs(MeshsArr &PlArray, const FChain &CurChain)
{
	// Ensure horizontal
	bool StartFixed = false;
	double Zstart = 0.;
	auto StartInd = CurChain.Start;
	for(auto i = CurChain.Start; i < CurChain.End; ++i)
	{
		if(!Cadrs[i]->HaveGeom())
			continue;
		if(!StartFixed)
		{
			StartFixed = true;
			Zstart = Cadrs[i]->GetB().GetZ();
			StartInd = i;
		}
		Cadrs[i]->MakeHorizontal(Zstart);
		if (Cadrs[i]->Length() < MIND)
			Cadrs[i]->SetType(undef);
	}
	//if (!Cadrs[CurChain.Start]->HaveGeom())
	//	return; // May be if Cadrs[i]->Length() become < MIND after MakeHorizontal(Zstart);
	CadrProc.CurChainType = CurChain.GetType();
	CadrProc.FixIMStart(PlArray);
	if(CurChain.PrevChainEndsBis)
		CadrProc.CrBaseFaces(PlArray, true);
	else
	{
		CrStartCont(int(StartInd));
		if(CurChain.GetType() != CHAIN_2D_FULL_ARC && CurChain.GetType() != CHAIN_2D_SMALL_FULL_ARC)
			CadrProc.CrFaces4Trim(PlArray, true);
	}
	CadrProc.FixIMEnd(PlArray);
	if(CurChain.GetType() == CHAIN_2D_SMALL_ARC || CurChain.GetType() == CHAIN_2D_NEARSMALL_ARC || CurChain.GetType() == CHAIN_2D_SMALL_FULL_ARC)
	{
		CadrProc.IsArcExtLeft = (Cadrs[CurChain.End - 1]->IsCWArc());
		if(Cadrs[CurChain.End - 1]->GetN().GetZ() < 0.)
			CadrProc.IsArcExtLeft = !CadrProc.IsArcExtLeft;
	}

	for (auto l = StartInd, d = StartInd; l < CurChain.End; l = d)
	{
		if(!Cadrs[l]->HaveGeom())
		{
			d = l + 1;
			continue;
		}
		CadrProc.m_IsLastCadr = (l == CurChain.End - 1);
		d = BuildSurf2d(PlArray, l,	CadrProc.m_IsLastCadr ? CurChain.ChainEndsBis : true);
		if(d == l)
		{
			d += 1;
			if(DEBUGMODE) LOGFILE.WriteString(_T("Cr2dChainMeshs:\n"));
		}
	}
	if(CurChain.GetType() == CHAIN_2D_SMALL_ARC /*|| CurChain.GetType() == CHAIN_2D_NEARSMALL_ARC*/)
	{
		BPoint C = CadrProc.CurLine.GetCenter();
		BPoint N = (C - Cadrs[CurChain.End - 1]->GetB()).ProjXY();
		CadrProc.TrimStartFaces(PlArray, C, N);
	}
//	if(NCM_PROJECT.GetGlobalState().GetModelMode() != NGlobalState::DX3X)
		if(CurChain.GetType() != CHAIN_2D_FULL_ARC && CurChain.GetType() != CHAIN_2D_SMALL_FULL_ARC)
			CadrProc.CrEndFaces(PlArray);
}

void NCadrsProcessing::ApproximateFragment()
{
	NCadrGeom *pGeom = Cadrs[0];
	
	if(!pGeom->IsNotXYArc() && !pComTool->IsConcave())
		return;
	ApproximatedFragment = true;
	if (CutHelix)
	{
		NCadrGeom *pNewArc = new NCadrGeom(*pGeom);
		pNewArc->SetE(pNewArc->GetE().GetX(), pNewArc->GetE().GetY(), pNewArc->GetB().GetZ());
		Cadrs.SetSize(0);
		Cadrs.Add(pNewArc);
		NCadrGeom *pNewGeom = new NCadrGeom;
		*pNewGeom = *pGeom;
		pNewGeom->SetType(line);
		pNewGeom->SetB(pNewArc->GetE());
		pNewGeom->SetE(pGeom->GetE());
		Cadrs.Add(pNewGeom);
		pNewGeom = new NCadrGeom;
		Cadrs.Add(pNewGeom);
	}
	else
	{
		BPointsBuf Pts;
		pGeom->GetPolyLine(Pts, NSurfSettings::GetTolerance(), false);

		Cadrs.SetSize(0);
		int Num = Pts.GetSize() - 1;
		auto StartInd = Cadrs.GetSize();
		Cadrs.SetSize(StartInd + Num);
		BPoint P0 = Pts.GetPoint(0);
		for(int i = 1; i <= Num; ++i)
		{
			BPoint P1  = Pts.GetPoint(i);
			NCadrGeom *pNewGeom = new NCadrGeom;
			*pNewGeom = *pGeom;
			pNewGeom->SetType(line);
			pNewGeom->SetB(P0);
			pNewGeom->SetE(P1);
			Cadrs[i - 1 + StartInd] = pNewGeom;
			P0 = P1;
		}
		// Add one extra geom cadr
		NCadrGeom *pNewGeom = new NCadrGeom;
		Cadrs.Add(pNewGeom);// This cadr will not be used because previous cadr is 3d
	}
}

bool NCadrsProcessing::IsApproximated() const
{
	return ApproximatedFragment;
}

bool NCadrsProcessing::IsSmall(const NCadrGeom *pGeom) const
{
	return Rmax >= pGeom->GetR() - 1.e4 * DMIN;
}

bool NCadrsProcessing::IsLarge(const NCadrGeom *pGeom) const
{
	if(IsSmall(pGeom) || pGeom->IsFullArc(FARC))
		return false;
	double Angle,sa,ea;
	pGeom->GetArcA(&sa,&ea,&Angle);
	if(fabs(Angle) > 270.)
		return true;
	return false;
}

bool NCadrsProcessing::IsNearSmall(const NCadrGeom *pGeom) const
{
	return Rmax >= pGeom->GetR() - 1.e4 * DMIN && Rmax <= pGeom->GetR() + 1.e4 * DMIN;
}

void NCadrsProcessing::Cr2dSArcChainMeshs(MeshsArr &PlArray, const FChain &CurChain)
{
	// small arc is single geom cadr in this chain. It is the last one.
	// Previous chain ChainEndsBis = false
// 2D small arc processing
	if(!pComTool)
		return;
	NCadrGeom *pOrigCadr = Cadrs[CurChain.End - 1];
	if(!pOrigCadr)
		return;

	double As, Ae, Ac;
	pOrigCadr->GetArcA(&As, &Ae, &Ac);
	if(fabs(Ac) < 179.)
	{
		Cr2dChainMeshs(PlArray, CurChain);
		return;
	}
	// Divide small arc into ArcsNum segments
	NCadrGeom* pNextCadr
		= (CurChain.End >= Cadrs.GetSize()) ? NULL : Cadrs[CurChain.End];
	const int ArcsNum = int(fabs(Ac) / 110) + 1;// The arc must be less then 120 degrees to prevent its subdivision in 
	BPoint Center = pOrigCadr->GetC();
	BPoint Pb = pOrigCadr->GetB();
	NCadrGeom DummyNextCadr;
	if(pNextCadr)
		Cadrs[CurChain.End] = &DummyNextCadr;// Для предотвращения использования направления следующего кадра
	BMatr Rot;
	Rot.rotg(Ac / ArcsNum, Center, pOrigCadr->GetN());
	for(int i = 0; i < ArcsNum; ++i)
	{
		BPoint Pe = Pb * Rot;

		if(i >= ArcsNum - 1)
			if(pNextCadr)
				Cadrs[CurChain.End] = pNextCadr;
		else
			DummyNextCadr.Set(line, Pe.GetX(), Pe.GetY(), Pe.GetZ()
				, Pe.GetX(), Pe.GetY(), Pe.GetZ()+100.);

		NCadrGeom ArcPart(*pOrigCadr);
		ArcPart.SetB(Pb);
		ArcPart.SetE(Pe);
		ArcPart.SetI(Center - Pb);
		Pb = Pe;
		Cadrs[CurChain.End - 1] = &ArcPart;
		Cr2dChainMeshs(PlArray, CurChain);
		// Переписать созданные MeshArray'и в конец PlArray, 
		// чтобы освободить место для новых
		if(i < ArcsNum - 1)
		{
			for(int l = 0; l < pComTool->num_tools; ++l)
			{
				PlArray.Add(PlArray[l]);
				PlArray[l] = new MeshArr;
			}
		}
	}
	// Restore Cadrs
	Cadrs[CurChain.End - 1] = pOrigCadr;
}

void NCadrsProcessing::Cr2dSArcChainMeshsNew(MeshsArr& PlArray, const FChain& CurChain)
{
	// small arc is single geom cadr in this chain. It is the last one.
	// Previous chain ChainEndsBis = false
// 2D small arc processing
	if (!pComTool)
		return;
	NCadrGeom* pOrigCadr = Cadrs[CurChain.End - 1];
	if (!pOrigCadr)
		return;
	// Ensure horizontal
	bool StartFixed = false;
	double Zstart = 0.;
	int StartInd = int(CurChain.End - 1);
	Zstart = pOrigCadr->GetB().GetZ();
	pOrigCadr->MakeHorizontal(Zstart);
	if (pOrigCadr->Length() < MIND) // cadr is too small
		return;
	CadrProc.IsArcExtLeft = (pOrigCadr->IsCWArc());
	if (pOrigCadr->GetN().GetZ() < 0.)
		CadrProc.IsArcExtLeft = !CadrProc.IsArcExtLeft;
	CrStartCont(StartInd);
	BPoint C = pOrigCadr->GetC();
	BPoint N = (C - pOrigCadr->GetB()).ProjXY();
	CadrProc.TrimStartCont(C, N);
	CadrProc.CrFaces4Trim(PlArray, true);
	CadrProc.IsArcExtLeft = (Cadrs[CurChain.End - 1]->IsCWArc());
	CadrProc.m_IsLastCadr = true;
	BuildSurf2dArcNew(PlArray, StartInd, false);
	CadrProc.CrFaces4Trim(PlArray, false);// ClearTrim called inside
	MeshsArr AddMeshsArr(PlArray.GetSize());
	CadrProc.MakeFullTool(AddMeshsArr);
	PlArray.Append(AddMeshsArr);
	//	PlArray.SetConvex(true);
}

bool NCadrsProcessing::ReplaceRotateCadr(NCadrGeom *&pGeom)
{
	if(!pGeom->IsRotate())
		return false;
	BPoint dABC = pGeom->GetI() - pGeom->GetN();
	if(fabs(dABC.GetX()) >= MINAD 
		|| fabs(dABC.GetY()) >= MINAD 
		|| fabs(dABC.GetZ()) < MINAD
		|| fabs(dABC.GetZ()) > 360. )
		return false;

	if((pGeom->GetB() - pGeom->GetE()).ProjXY().D2() < MIND * MIND)
	{// Rotation only
		NCadrGeom *pNewGeom = new NCadrGeom;// memory leak
		pNewGeom->SetType(ccwarc);
		pNewGeom->SetB(pGeom->GetBP());
		pNewGeom->SetE(pGeom->GetEP());
		BPoint I = (BPoint(0., 0., 0., 1.) - pNewGeom->GetB()).ProjXY();
		pNewGeom->SetI(I);
		pNewGeom->SetPolar();
		// Check Arc direction
		BPoint DirArc = pNewGeom->GetStartDir();
		BPoint DirCurve = pGeom->GetStartDirP();
		if(DirArc * DirCurve < 0)
			pNewGeom->SetType(cwarc);
		pGeom = pNewGeom;
	}
	else
	{// Translation and rotation
		BCurve *pCurve = pGeom->GetCurve();
		if (pCurve == nullptr)
			return false;
		if(pCurve->GetSize() != 4)
			return false;
		NLine Line;
		BPoint *pP = pCurve->GetSegm(0);
		Line.Set(*pP, *(pP + 1), *(pP + 2), *(pP + 3));
		int Num = Line.GetNumAproxLine(NSurfSettings::GetTolerance());
		if(Num > 2)
			return false;
		NCadrGeom *pNewGeom = new NCadrGeom;
		pNewGeom->SetType(line);
		pNewGeom->SetFast(pGeom->IsFast());
		pNewGeom->SetB(Line.p[0]);
		pNewGeom->SetE(Line.p[3]);
		pNewGeom->SetPolar();
		pGeom = pNewGeom;
	}
	return true;
}

bool NCadrsProcessing::ApproxSpline(const NCadrGeom *pGeom)
{// Return true if cadr is approximated
	if(!pGeom)
		return false;
	if(!pGeom->IsSpline())
		return false;
	BPointsBuf Pts;
	int Res = pGeom->GetPolyLine(Pts, NSurfSettings::GetTolerance(), true);
	if(Res < 3)
		return false;
	ApproximatedFragment = true;
	for(int i = 0; i < Pts.GetSize() - 1; ++i)
	{
		NCadrGeom *pNewGeom = new NCadrGeom(*pGeom);
		pNewGeom->SetCurve(NULL);
		pNewGeom->SetB(Pts.GetPoint(i));
		pNewGeom->SetE(Pts.GetPoint(i + 1));
		Cadrs.Add(pNewGeom);
	}
	
	return true;
}

bool NCadrsProcessing::MayBeApproximated(NCadrGeom *pGeom, int n)
{
	if(pGeom->IsRotate() || (pGeom->IsSpline() && pGeom->GetCurve()))
	{
		BPoint dABC = pGeom->GetI() - pGeom->GetN();
		if(fabs(dABC.GetX()) >= MINAD || fabs(dABC.GetY()) >= MINAD || fabs(dABC.GetZ()) < MINAD)
			return false;
		BCurve *pCurve = pGeom->GetCurve();
		if(!pCurve)
			return false;
		if(n != 0)
			return true;
		ApproximatedFragment = true;
		Cadrs.SetSize(0);
		BPointsBuf Pts;
		pCurve->MakeAppr(NSurfSettings::GetTolerance(), Pts);
		for(int i = 0; i < Pts.GetSize() - 1; ++i)
		{
				NCadrGeom *pNewGeom = new NCadrGeom;
				pNewGeom->SetType(line);
				pNewGeom->SetFast(pGeom->IsFast());
				pNewGeom->SetB(Pts.GetPoint(i));
				pNewGeom->SetE(Pts.GetPoint(i + 1));
				Cadrs.Add(pNewGeom);
		}
		//for(int sn = 0; sn < pCurve->GetSize() - 1; sn += 3)
		//{
		//	BPoint *pP = pCurve->GetSegm(sn);
		//	NLine Line;
		//	Line.Set(*pP, *(pP + 1), *(pP + 2), *(pP + 3));
		//	if((Line.p[0] - Line.p[3]).D2() < MIND * MIND)
		//		continue;
		//	int Num = Line.GetNumAproxLine(NSurfSettings::GetTolerance());
		//	BPoint P0 = *pP;
		//	int StartInd = Cadrs.GetSize();
		//	Cadrs.SetSize(StartInd + Num);
		//	for(int i = 0; i < Num; ++i)
		//	{
		//		NCadrGeom *pNewGeom = new NCadrGeom;
		//		pNewGeom->SetType(line);
		//		pNewGeom->SetFast(pGeom->IsFast());
		//		pNewGeom->SetB(P0);
		//		P0 = Line.GetPointFromParam(double(i + 1) / Num).Norm();
		//		pNewGeom->SetE(P0);
		//		Cadrs[i + StartInd] = pNewGeom;
		//	}
		//}
		// Add one extra geom cadr
		NCadrGeom *pNewGeom = new NCadrGeom(*(Cadrs.GetAt(Cadrs.GetSize() - 1)));
		pNewGeom->Reverse();
		Cadrs.Add(pNewGeom);
		return true;
	}
	if(pGeom->IsNotXYArc() || (pGeom->IsArc() && pComTool->IsConcave()))
	{
		if(n != 0)
			return true;
		Cadrs.Add(pGeom);
		ApproximateFragment();
		return true;
	}
	if(pGeom->IsArc())
		if((NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::DX) || IsLarge(pGeom))
		{
			NCadrGeom *BufGeom = NULL;
			int PartsNum = pGeom->DivideQuad(BufGeom);
			if(PartsNum <= 1)
				return false;
			if(n != 0)
				return true;
			ApproximatedFragment = true;
			for(int k = 0; k < PartsNum; ++k)
				Cadrs.Add(new NCadrGeom(BufGeom[k]));
			delete[] BufGeom;
			// Add one extra geom cadr
			NCadrGeom *pNewGeom = new NCadrGeom(*(Cadrs.GetAt(Cadrs.GetSize() - 1)));
			pNewGeom->Reverse();
			Cadrs.Add(pNewGeom);
			return true;
		}

	return false;
}
NCadrGeom *NCadrsProcessing::GetCadr(int i) const
{
	return Cadrs[i];
}
