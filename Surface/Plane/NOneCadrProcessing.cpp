// NOneCadrProcessing.cpp: implementation of the NOneCadrProcessing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NOneCadrOneTool.h"
#include "NOneCadrProcessing.h"
#include "NCadrsProcessing.h"
#include "NMeshArray.h"
#include "NRectMesh.h"
#include "SOPlane.h"
#include "SOECyl.h"
#include "FChain.h"
#include "ConstDef.h"
#include "NSortedArray.h"
#include "math.h"
#include "NOneCadrProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NOneCadrProcessing::NOneCadrProcessing()
{
	m_IsFirstCadr = true;
	m_IsLastCadr = false;
	m_IsTurnCW = false;
	m_IsMovInVertPl = false;
	IsExtLeft = false;
	IsArcExtLeft = false;

	m_Alf = 0;
	m_BAlf = 0;
	m_GAlf = 0;
	m_endAlf = 0;
	m_endBAlf = 0;
	m_endGAlf = 0;

	CurChainType = CHAIN_EMPTY;

	ToolsNum = 0;

	m_CadrNum = 0;
	m_ChainEndBis = false;
}

NOneCadrProcessing::~NOneCadrProcessing()
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].Clear();
	ToolSpecArr.RemoveAll();
}

// ЗАДАНИЕ КОНТУРА СОСТАВНОГО ИНСТРУМЕНТА (плоскость XOZ)
void NOneCadrProcessing::SetToolsContour(int iTool, NContour *pToolsContour, bool IsCutting, bool IsShank, const NColor& ToolColor, bool IsConcave)
{
	ElemID ElID = ID_EMPTY + SURF_NUM_SHIFT;
	MElemIDProc::SetPCn(&ElID, MElemIDProc::AddPC(MElemIDProc::MakePC(ToolColor)) + RESERVED_PCS);
	ToolSpecArr[iTool].SetPColorNum(ElID);
	ToolSpecArr[iTool].SetToolContour(pToolsContour, IsCutting, IsShank, IsConcave);
}

void NOneCadrProcessing::SetDataConcave(int iTool, double CRad)
{
	ToolSpecArr[iTool].SetDataConcave(CRad);
}

// ИНИЦИАЛИЗАЦИЯ НАЧАЛЬНЫХ ДАННЫХ
void NOneCadrProcessing::SetInitData2d(const BPoint &bPoint, BPoint &curDir, BPoint &nextDir)
{
	m_BPoint = bPoint;
	m_CurDir = curDir;
	m_NextDir = nextDir;
	m_IsTurnCW = false;
}

// передаётся:
// curDir	- вектор направления для текущего кадра (не единичный !!!)
// nextDir	- вектор направления для следующего кадра (не единичный !!!)
void NOneCadrProcessing::BuildSurf2d(const NLine &bLine, BPoint &nextDir, MeshsArr &MeshsArray, bool ChainEndBis)
{
	// задание начальных данных
	SetInitData2d(bLine.p[0], bLine.p[3] - bLine.p[0], nextDir);
	CurLine = bLine;
	if (nextDir.D2() > 0.)
	{
		BPoint v1(CurLine.GetDir(1)), v2(nextDir);
		BPoint n = v1.ProjXY() % v2.ProjXY();

		if (n.GetZ() > 0)	// против часовой
			m_IsTurnCW = false;
		else				// по часовой
			m_IsTurnCW = true;
	}
	else
	{
		m_IsLastCadr = true;	// последний
		m_IsTurnCW = false;
	}

	m_ChainEndBis = ChainEndBis;
	NLine Line;
	Line.Set(BPoint(0,0,0,1),BPoint(0,0,0,1),BPoint(0,0,1,1),BPoint(0,0,1,1));
	BPoint StartDir = bLine.GetDir(0);
	ComputeAngles(Line, CONE, StartDir, m_Alf, m_BAlf);
	ComputeAngles(Line, CONE, m_NextDir, m_endAlf, m_endBAlf);
	//определение базовых проскостей для построения
	SetBasePlanes2d();

	// если произошла смена в поворотах траектории
	if (IsExtLeft != m_IsTurnCW )
	{
		SwapConts();
		IsExtLeft = !IsExtLeft;
	}
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].BuildSurf2d(*MeshsArray[iTool]);
}

void NOneCadrProcessing::SetBasePlanes2d()
{
	BPoint n;
	// ПЛОСКОСТЬ 4 
	{// нормаль к проекции кадра на XY в конце
		n = CurLine.GetDir(1);
		n = n*(1./sqrt(n.D2()));
		m_P4.Set(n, CurLine.p[3]);
	}

	// ПЛОСКОСТЬ 2 биссектриса угла между кадрами в конце
	n = CurLine.GetDir(0);
	n = n*(1./sqrt(n.D2()));
	if(!m_IsLastCadr)
	{
		n = n + m_NextDir*(1./sqrt(m_NextDir.D2()));
		n = n*(1./sqrt(n.D2()));
	}
	m_P2.Set(n, CurLine.p[3]);
}


// ВЫЧИСЛЕНИЕ УГЛОВ ДЛЯ УЧАТСКА ОБРАЗУЮЩЕЙ НА ЗАДАННОМ КАДРЕ
// параметры:
// Line		- линия образующей (плоскость XOZ)
// type		- тип участка образующей
// dir		- направляющий вектор
// alf		- угол Alfa
// beta		- угол Beta
// maxR		- максимальный радиус у данной образующей
// is_top	- какой точке этот радиус соответствует (первая, последняя)
BOOL NOneCadrProcessing::ComputeAngles(const NLine &Line, TypeLineInContour type,
									  const BPoint &dir,
									  double &alf, double &beta, 
									  double &maxR, bool &is_top)
{
	BOOL res = FALSE;
	
	double DirXYD2 = sqrt(dir.GetX() * dir.GetX() + dir.GetY() * dir.GetY());
	switch(type)
	{
	case CONE:
		// alf - удвоенный угол от направления dir в плоск. XY до искомой точки
		alf = dir.GetZ() * (Line.p[0].GetX()-Line.p[3].GetX());
		beta = DirXYD2 * (Line.p[0].GetZ()-Line.p[3].GetZ());

		if (fabs(alf) < fabs(beta))
		{
			alf = -alf/beta;
			alf = min(1.,alf);
			alf = max(-1.,alf);
			alf = acos(alf);
			alf = 2*180.*alf/PI;
		}
		else
		{
			if (alf * beta < 0.)
				alf = 0.;
			else
				alf = 360.;
		}

		if (0. < alf  && alf < 360.)
		{
			beta = dir.GetX() / DirXYD2;
			beta = acos(beta);
			if ((dir.GetX() < 0. && dir.GetY() < 0.) ||
				dir.GetY() < 0.)
				beta = -beta;				
			beta = 180.*beta/PI;
			beta = beta + 180.-alf/2.;
		}
		else
			beta = 0.;
		// определение максимального радиуса в текущем инструменте
		if (Line.p[0].GetX() > Line.p[3].GetX())
		{
			is_top = true;	maxR = Line.p[0].GetX();
		}
		else
		{
			is_top = false;	maxR = Line.p[3].GetX();
		}
		res = TRUE;
		break;
	case SPHERE:
		{
		// alf - угол от направления dir в плоск. XY до искомой точки
		double h = Line.GetCenter().GetZ() - Line.p[0].GetZ();
		h = h / Line.GetRad();
		double TgA = dir.GetZ() / sqrt(dir.GetX() * dir.GetX() + dir.GetY() * dir.GetY());
		double d = 1. - h * h;
		d = TgA * h / sqrt( d );
		alf = 0.;
		if( d >= 1. || d <= -1.)
			return TRUE;
		alf = asin(d);
		alf = 90. - 180. * alf / PI; 
		alf = (Line.p[0].GetX() < 0.) ? 2.*alf : -2.*(alf-180.);
		res = TRUE;
		}
		break;
	case HORIZONTAL:
		res = FALSE;
		break;
	case TORAHS:
		res = FALSE;
		break;
	case NOTKNOWN:
		res = FALSE;
		break;
	}
	return res;
}

// ВЫЧИСЛЕНИЕ УГЛОВ ДЛЯ УЧАСТКА ОБРАЗУЮЩЕЙ НА ЗАДАННОМ КАДРЕ
BOOL NOneCadrProcessing::ComputeAngles(NLine &Line, TypeLineInContour type,
									  BPoint &dir,
									  double &alf, double &beta)
{
	double t=0;
	bool f=false;
	return ComputeAngles(Line, type, dir, alf, beta, t, f);
}

bool NOneCadrProcessing::IsLast(void)
{
	return m_IsLastCadr;
}
bool NOneCadrProcessing::CompareSECont(void)
{
	return true;
}
bool NOneCadrProcessing::CheckMesh(NRectMesh *pMesh)
{
	if(pMesh->m_r < 2)
		return false;
	for(int i = 0; i < pMesh->m_r-1; ++i)
		for(int j = 0; j < pMesh->m_arr[0].GetSize(); ++j)
		{
			BPoint v =	*MFOREST.GetPoint(pMesh->m_arr[i+1][j]) -
						*MFOREST.GetPoint(pMesh->m_arr[i][j]);
			if(v * m_CurDir < 0.)
				return false;
		}
	return true;
}
bool NOneCadrProcessing::CreateWConts()
{// Create Int and Ext contours
	bool flag = true;
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		flag &= ToolSpecArr[iTool].CreateWConts(this);
	return true;
}
void NOneCadrProcessing::CrFaces4Trim(MeshsArr& MeshsArray, bool StartFace)
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].CrFaces4Trim(*MeshsArray[iTool], StartFace);
}
void NOneCadrProcessing::BuildAddTurnMesh(MeshsArr& PlaneArray)
{
	for (int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].BuildAddTurnMesh(*PlaneArray[iTool]);
}
void NOneCadrProcessing::MakeFullTool(MeshsArr& PlaneArray)
{
	for (int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].MakeFullTool(*PlaneArray[iTool]);
}
bool NOneCadrProcessing::TrimStartCont(const BPoint& C, const BPoint& N)
{
	for (int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].TrimStartCont(C, N);
	return false;
}
void NOneCadrProcessing::CrBaseFaces(MeshsArr &MeshsArray, bool StartFace)
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].CrBaseFaces(*MeshsArray[iTool], StartFace);
}
bool NOneCadrProcessing::SetInitConts()
{
	// определение параметров кадра (углов и точек)
	NLine Line;
	Line.Set(BPoint(0,0,0,1),BPoint(0,0,0,1),BPoint(0,0,1,1),BPoint(0,0,1,1));
	ComputeAngles(Line, CONE, m_CurDir, m_Alf, m_BAlf);
	m_Alf = 180.;
//	m_BAlf = 0.;
	// Set Int and Ext contours
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].SetInitConts();

	return true;
}
// ПОСТРОЕНИЕ СЕТОК ОБРАЗОВАННЫХ ОДНИМ КАДРОМ ПРОГРАММЫ (ДУГА)
// передаётся:
// bLine	- траектория движения 
// nextDir	- вектор направления для следующего кадра (не единичный !!!)
void NOneCadrProcessing::BuildSurf2dArc(const NLine &bLine, BPoint &nextDir, MeshsArr &MeshsArray, bool ChainEndBis, bool LastArcPart)
{
// ОБЩАЯ ИНФОРМАЦИЯ 	
	// задание начальных данных
	BPoint LocNextDir = (CurChainType == CHAIN_2D_FULL_ARC || CurChainType == CHAIN_2D_SMALL_FULL_ARC) 
		? BPoint(0.,0.,0.,0.) : nextDir;
	SetInitData2d(bLine.p[0], bLine.p[3] - bLine.p[0], LocNextDir);
	CurLine = bLine;

	if (LocNextDir.D2() > 0)
	{
		BPoint v1(CurLine.GetDir(1)), v2(nextDir);
		BPoint n = v1.ProjXY() % v2.ProjXY();

		if (n.GetZ() > 0)	// против часовой
			m_IsTurnCW = false;
		else				// по часовой
			m_IsTurnCW = true;
	}
	else
	{
		m_IsLastCadr = true;	// последний
		m_IsTurnCW = false;
	}

	m_ChainEndBis = ChainEndBis;
	NLine Line;
	Line.Set(BPoint(0,0,0,1),BPoint(0,0,0,1),BPoint(0,0,1,1),BPoint(0,0,1,1));
	BPoint StartDir = bLine.GetDir(0);
	ComputeAngles(Line, CONE, StartDir, m_Alf, m_BAlf);
	//определение базовых проскостей для построения
	SetBasePlanes2d();

	// если произошла смена в поворотах траектории
	if (IsExtLeft != m_IsTurnCW )
	{
		SwapConts();
		IsExtLeft = !IsExtLeft;
	}
	if((CurChainType == CHAIN_2D_SMALL_ARC || CurChainType == CHAIN_2D_NEARSMALL_ARC || CurChainType == CHAIN_2D_SMALL_FULL_ARC))
	{
		for(int iTool = 0; iTool < ToolsNum; ++iTool)
			ToolSpecArr[iTool].BuildSurf2dArcSmall(*MeshsArray[iTool], LastArcPart);
	}
	else
	{
		for(int iTool = 0; iTool < ToolsNum; ++iTool)
			ToolSpecArr[iTool].BuildSurf2dArc(*MeshsArray[iTool]);
	}
}

void NOneCadrProcessing::BuildSurf2dArcNew(const NLine& iLine, BPoint& nextDir, MeshsArr& MeshsArray)
{
	CurLine = iLine;
	BPoint LocNextDir = nextDir;
	m_IsLastCadr = true;	// последний
	if (LocNextDir.D2() > 0)
	{
		BPoint v1(CurLine.GetDir(1)), v2(nextDir);
		BPoint n = v1.ProjXY() % v2.ProjXY();

		if (n.GetZ() > 0)	// против часовой
			m_IsTurnCW = false;
		else				// по часовой
			m_IsTurnCW = true;
	}
	else
	{
		m_IsLastCadr = true;	// последний
		m_IsTurnCW = false;
	}
	m_ChainEndBis = false;
	// если произошла смена в поворотах траектории
	if (IsExtLeft != m_IsTurnCW )
	{
		SwapConts();
		IsExtLeft = !IsExtLeft;
	}
	for (int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].BuildSurf2dArcNew(*MeshsArray[iTool]);
}

void NOneCadrProcessing::BuildSurfVert(const NLine &iLine, MeshsArr &MeshsArray)
{
	CurLine = iLine;
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].BuildSurfVert(*MeshsArray[iTool]);
}

void NOneCadrProcessing::CrEndFaces(MeshsArr &MeshsArray)
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].CrEndFaces(*MeshsArray[iTool]);
}

// передаётся:
// DirLine	- траектория движения 
// curDir	- вектор направления взгляда для текущего кадра (не единичный !!!)
// nextDir	- вектор направления взгляда для следующего кадра (не единичный !!!)
void NOneCadrProcessing::BuildSurf3d(const NLine &bLine, BPoint &nextDir, MeshsArr &MeshsArray, bool ChainEndBis)
{
	// задание начальных данных
	SetInitData2d(bLine.p[0], bLine.p[3] - bLine.p[0], nextDir);
	CurLine = bLine;

	for (int iTool = 0; iTool < ToolsNum; ++iTool)
	{
		NOneCadrOneTool& OCOT = ToolSpecArr[iTool];
		if(OCOT.IsConcaveTool())
		{
			OCOT.BuildSurf3dCv(MeshsArray);
			// move last to iTool position
			MeshsArray[iTool] = MeshsArray[MeshsArray.GetSize() - 1];
			MeshsArray.SetSize(MeshsArray.GetSize() - 1);
		}
		else
			OCOT.BuildSurf3d(*MeshsArray[iTool]);
	}
}
int NOneCadrProcessing::DefBaseAngles()
{
	int flag = 0;
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		flag = min(ToolSpecArr[iTool].DefBaseAngles(), flag);
	return flag;
}
void NOneCadrProcessing::SetToolsNum(int num)
{
	ToolsNum = num;
	ToolSpecArr.SetSize(num);
}
void NOneCadrProcessing::SwapConts()
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].SwapConts();
}
void NOneCadrProcessing::FixIMStart(const MeshsArr &MeshsArray)
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].FixIMStart(*MeshsArray[iTool]);
}
void NOneCadrProcessing::FixIMEnd(const MeshsArr &MeshsArray)
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].FixIMEnd(*MeshsArray[iTool]);
}
void NOneCadrProcessing::TrimStartFaces(MeshsArr &MeshsArray, const BPoint &C, const BPoint &N)
{// Works with small arc only
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].TrimStartFaces(*MeshsArray[iTool], C, N);
}
void NOneCadrProcessing::TrimAllFaces(MeshsArr& MeshsArray, const BPoint& C, const BPoint& N)
{// Works with small arc only
	for (int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].TrimAllFaces(*MeshsArray[iTool], C, N);
}
void NOneCadrProcessing::ApproxContour()
{
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].ApproxContour();
}
void NOneCadrProcessing::RestoreContour()
{// After ApproxContour
	for(int iTool = 0; iTool < ToolsNum; ++iTool)
		ToolSpecArr[iTool].RestoreContour();
}
