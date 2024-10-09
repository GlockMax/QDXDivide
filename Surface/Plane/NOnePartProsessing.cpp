#include "stdafx.h"
#include "ConstDef.h"
#include <vector>
#include "NRectMesh.h"
#include "NOneCadrProcessing.h"
#include "NOneCadrOneTool.h"
#include "nonepartprosessing.h"
#include "SOrigSurf.h"
#include "SOSphere.h"
#include "SOPlane.h"
#include "SOCyl.h"
#include "SOECyl.h"
#include "NSurfSettings.h"
#include "SOrigSurf.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NOnePartProcessing::NOnePartProcessing(void)
{
	ClearTrimed();
}
NOnePartProcessing::NOnePartProcessing(NLine &Line, NOneCadrProcessing *pCadrInfo, NOneCadrOneTool *pCadrInfoC)
{
	ClearTrimed();
	SetLine(Line, pCadrInfo, pCadrInfoC);
}

NOnePartProcessing::~NOnePartProcessing(void)
{
}

void NOnePartProcessing::SetLine(NLine &Line, NOneCadrProcessing *pCadrInfo, NOneCadrOneTool *pCadrInfoC)
{
	m_pCInfo = pCadrInfo;
	m_pCInfoC = pCadrInfoC;
	m_Line = Line;
}
NLine &NOnePartProcessing::GetLine()
{
	return m_Line;
}
// определение контура полученного из начального на пересечении плоскости
void NOnePartProcessing::GetContByIntersecPlane(CUIntArray &arr, int St, int En, MPlane &PL1)
{
#ifdef _DEBUG
	if ((0 > St || St >= arr.GetSize()) ||
		(0 > En || En >= arr.GetSize()))
		return;
#endif
	int n;
	const BPoint *p;
	BPoint r1;
	MBSPForest *pForest = &MBSPForest::GetActive();

	for(int i=St; i<=En; i++)
	{
		p = pForest->GetPoint(arr.GetAt(i));

		PL1.IntersLine(*p, m_pCInfo->m_CurDir, r1);
		n = pForest->AddPoints(r1);
		arr[i] = n;
	}
}

// создание сетки простым соединением двух контуров
NRectMesh* NOnePartProcessing::CreateParMesh(CUIntArray &arr1, CUIntArray &arr2, bool is_pl)
{
	return NULL;
}
// создание сетки перемещением одного контура на заданный вектор
NRectMesh* NOnePartProcessing::CreateParMeshByMove(CUIntArray &arr, const BPoint &dir, SSurfType Type)
{
	return NULL;
}
// создание сетки поворотом одного контура на заданный угол вокруг оси
NRectMesh* NOnePartProcessing::CreateParMeshByRot(CUIntArray &arr, int St, int En,
												   const BPoint &c, const BPoint &t, 
												   const double &alf, int num)
{
//	if(DEBUGMODE)
	{
		if ((0 > St || St >= arr.GetSize()) ||
			(0 > En || En >= arr.GetSize()))
			return NULL;
	}
	BMatr m;
	BPoint p, a, b;
	int n;

	// вычисление матрицы поворота
	m.rotg(alf/num, c, t);

	CUIntArray i_arr, r_arr;
	NRectMesh *pParMesh = new NRectMesh;
	MBSPForest *pForest = &MBSPForest::GetActive();

	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->SetRowsNum(num+1);
	pParMesh->m_arr[0].Append(arr);
	for (int i=1; i<=num; i++)
	{
		for (int j=St; j<=En; j++)
		{
			p = *pForest->GetPoint(arr[j]);
			p = p * m;
			n = pForest->AddPoints(p);
			arr[j] = n;
		}
		pParMesh->m_arr[i].Append(arr);
	}
	p = *pForest->GetPoint(pParMesh->m_arr[0][0]);
	a = *pForest->GetPoint(pParMesh->m_arr[0][1]);
	b = *pForest->GetPoint(pParMesh->m_arr[1][1]);
	a = (a-p)%(b-p);
	pParMesh->m_sgn = ((p - c) * a > 0 ? 1: -1);
	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;

	pParMesh->m_iSurf = pForest->AddOrigSurf(new SOSphere(sqrt((p-c).D2()), c));
	return pParMesh;
}
// определение числа разбиения дуги окружности
int NOnePartProcessing::GetNumAproxLine(const NLine &Line)
{
	/*int num;
	BPoint p, O, T;
	double d, h, k, S;

	O = Line.p[0];
	T = Line.p[1] - O;
	d = sqrt(T.D2());
	T = T*(1./d);
	num = 1;
	h = k = 1.;
	// определение числа, на которое разбивается кривая
	while (h > ACCURACY) 
	{
		p = Line.GetPointFromParam(k);
		p.Norm();
		p = p - O;
		S = (T*d)*p;
		h = S / sqrt(p.D2());
		k = k/2;
		d = d/2;
		num*=2; // число плоскостей для аппроксимации этого участока
	}
	//num = 5;
	return num;*/
#ifdef _DEBUG
	if ((Line.p[0]-Line.p[3]).D2() <= 0)
		return 1;
#endif

	BPoint C, T1, T2;
	double R, alf;
/* // EK	
	C = Line.p[0];
	T1 = Line.p[1];
	T1 = T1.Norm() - C;
	T2 = Line.p[3] - C;
	R = sqrt(T1.D2());
	T1 = T1*(1./R);
	T2 = T2*(1./R);
	alf = C.Cos2(T1, T2);	// cos alf
// EK */
	{ // EK start
	double cosA2 = Line.p[1].GetH();
	alf = acos(cosA2);
	T1 = Line.p[1];
	T1.Norm();
    R = sqrt((Line.p[3] - Line.p[0]).D2())*0.5;
	R = R/sin(alf);
	alf = alf * 2.;
	} // EK end

	alf = alf*180./PI;
	int n = NSurfSettings::GetNumAproxLine(R, alf);
	return n;
}


// создание сетки поворотом одного контура на заданный угол вокруг оси Z
NRectMesh* NOnePartProcessing::CreateRotMesh(CUIntArray &arr, int St, int En,
												   const BPoint &c,  
												   double alf, int num)
{
//	if(DEBUGMODE)
	{
		if ((0 > St || St >= arr.GetSize()) ||
			(0 > En || En >= arr.GetSize()))
			return NULL;
	}

	NRectMesh *pParMesh = new NRectMesh;
	MBSPForest *pForest = &MBSPForest::GetActive();

	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->SetRowsNum(num+1);
	pParMesh->m_arr[0].Append(arr);
	for (int i=1; i<=num; i++)
	{
		BMatr m;
		m.RotZ(c, i*alf/num);
		for (int j=St; j<=En; j++)
			arr[j] = RotatePoint(pParMesh->m_arr[0][j], m);
		pParMesh->m_arr[i].Append(arr);
	}
	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	return pParMesh;
}
NRectMesh* NOnePartProcessing::CreateFullRotMesh1(const CUIntArray& Start, const CUIntArray& Mid, const BPoint& c)
{
	if (Start.GetSize() == 0)
		return nullptr;
	if (!m_pCInfoC->m_pToolsContour)
		return nullptr;
	const int Sign = m_pCInfo->IsExtLeft ? -1 : 1;
	BPoint p = m_pCInfo->CurLine.GetDir(1);
	double Angle = p.Angle(m_pCInfo->m_NextDir);
	double Rmax = m_pCInfoC->m_pToolsContour->GetMaxX();
	int nAngle = 0;
	double RestAngle = Sign * 180.;
	if (fabs(Angle) > MINAD)
	{
		nAngle = NSurfSettings::GetNumAproxLine(Rmax, Angle);
		RestAngle = (Angle > 0.) ? 180. - Angle : -180 - Angle;
	}
	int nRest = NSurfSettings::GetNumAproxLine(Rmax, RestAngle);
	int MeshSize = (nAngle + nRest) * 2; // a number of intervals in the whole mesh
	NRectMesh* pParMesh = new NRectMesh;
	MBSPForest* pForest = &MBSPForest::GetActive();

	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->SetRowsNum(MeshSize + 1);
	pParMesh->m_arr[0].Append(Start);
	pParMesh->m_arr[MeshSize].Append(Start);// last row
	for (int i = 1; i <= nAngle; i++)
	{
		pParMesh->m_arr[i].SetSize(Start.GetSize());
		BMatr m;
		m.RotZ(c, i * Angle / nAngle);
		for (int j = 0; j < Start.GetSize(); j++)
			pParMesh->m_arr[i][j] = RotatePoint(pParMesh->m_arr[0][j], m);
	}
	for (int i = 1; i <= nRest - 1; i++)
	{
		const int k = i + nAngle;
		pParMesh->m_arr[k].SetSize(Start.GetSize());
		BMatr m;
		m.RotZ(c, i * RestAngle / nRest + Angle);
		for (int j = 0; j < Start.GetSize(); j++)
			pParMesh->m_arr[k][j] = RotatePoint(pParMesh->m_arr[0][j], m);
	}
	{
		// Add Mid
		const int k = nAngle + nRest;
		pParMesh->m_arr[k].SetSize(Start.GetSize());
		for (int j = 0; j < Start.GetSize(); j++)
			pParMesh->m_arr[k][j] = Mid[j];
	}
	for (int i = 1; i <= nAngle; i++)
	{
		const int k = i + nAngle + nRest;
		pParMesh->m_arr[k].SetSize(Start.GetSize());
		BMatr m;
		m.RotZ(c, i * Angle / nAngle + Angle + RestAngle);
		for (int j = 0; j < Start.GetSize(); j++)
			pParMesh->m_arr[k][j] = RotatePoint(pParMesh->m_arr[0][j], m);
	}
	for (int i = 1; i <= nRest - 1; i++)// Except last
	{
		const int k = i + nAngle + nRest + nAngle;
		pParMesh->m_arr[k].SetSize(Start.GetSize());
		BMatr m;
		m.RotZ(c, i * RestAngle / nRest + Angle + RestAngle + Angle);
		for (int j = 0; j < Start.GetSize(); j++)
			pParMesh->m_arr[k][j] = RotatePoint(pParMesh->m_arr[0][j], m);
	}
	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pParMesh->m_sgn = Sign;
	return pParMesh;
	return nullptr;
}
const CUIntArray &NOnePartProcessing::GetArray(int i) const
{
	return m_arr[i];
}
int NOnePartProcessing::AppendTrimedArray(int i, CUIntArray& buf) const
{
	const CUIntArray& LocArr = m_arr[i];
	if (IsTrimed)
	{
		if (TrimPointForInd == -1)
			return 0;
		if (TrimPointForInd == -2)
		{
			buf.Append(LocArr);
			return int(LocArr.GetSize());
		}
		if (StartTrimed)
		{
			buf.Add(TrimPointForInd);
			for(int k = TrimPointLocInd + 1; k < LocArr.GetSize(); ++k)
				buf.Add(LocArr[k]);
			return int(LocArr.GetSize()) - TrimPointLocInd;
		}
		else
		{
			for (int k = 0; k <= TrimPointLocInd; ++k)
				buf.Add(LocArr[k]);
			buf.Add(TrimPointForInd);
			return TrimPointLocInd + 1;
		}
	}
	else
	{
		buf.Append(LocArr);
		return int(LocArr.GetSize());
	}
}
double NOnePartProcessing::CalcDelta(bool Ext)
{
	bool Left = (Ext == m_pCInfo->IsExtLeft);
	bool ccw = (m_pCInfo->CurLine.GetDir(0) % (m_pCInfo->CurLine.p[3]-m_pCInfo->CurLine.p[0])).GetZ() > 0;
	BPoint bp = m_Line.GetCenter();
	double delta = sqrt((bp.ProjXY()-BPoint(0.,0.,0.,1)).D2());
	delta *= ((Left == ccw) ? -1. : 1.);
	return delta;
}

void NOnePartProcessing::MakeFullTool(MeshArr& MeshArray, const NOnePartProcessing* pIntCont)
{
	if (!m_pCInfoC->m_pToolsContour)
		return;
	NRectMesh* pMesh = CreateFullRotMesh1(m_arr[1], pIntCont->m_arr[1], m_pCInfo->CurLine.p[3]);
	if (pMesh != nullptr)
	{
		// make surf
		BMatr m;
		m.Trans(BPoint(0, 0, 0, 1), m_pCInfo->CurLine.p[3]);
		pMesh->m_iSurf = Cr2dRotSurf(m_Line * m, m_pCInfo->CurLine.p[3], 1.);
		MeshArray.Add(pMesh);
	}
}

NRectMesh* NOnePartProcessing::CrDriveMesh()
{
	if(!m_pCInfoC->m_pToolsContour)
		return nullptr;

	// fill m_arr[1] in allcases
	MBSPForest* pForest = &MBSPForest::GetActive();
	BMatr m = m_pCInfo->CurLine.GetDriveMatr(1.);
	m_arr[1].SetSize(m_arr[0].GetSize());
	bool EqualFound = false;
	for (int j = 0; j < m_arr[0].GetSize(); j++)
	{
		bool Processed = false;
		if (IsTrimed)
		{
			if (TrimPointForInd == -2)
			{
				m_arr[1][j] = m_arr[0][j];
				EqualFound = true;
				Processed = true;
			}
			else if (StartTrimed && j == 0)
			{
				const BPoint& P0 = *MFOREST.GetPoint(m_arr[0][j]);
				const BPoint& P1 = *MFOREST.GetPoint(TrimPointForInd);
				if ((P1 - P0).D2() < MIND * MIND)
				{
					m_arr[1][j] = m_arr[0][j];
					Processed = true;
				}
			}
		}
		if(!Processed)
			m_arr[1][j] = RotatePoint(m_arr[0][j], m);
	}

	bool TrimedAway = (IsTrimed && TrimPointForInd == -1);// contour is fully trimed 
	if (TrimedAway)
		return nullptr;

	int num = GetNumAproxLine(m_pCInfo->CurLine);

	double Rmax = m_pCInfoC->m_pToolsContour->GetMaxX();
	double Rline = m_pCInfo->CurLine.GetRad();
	num = int(num * sqrt(fabs(Rmax + Rline)/Rline));
	num = min(50, abs(num));
	num = max(1, num);

	CUIntArray i_arr, r_arr;
	NRectMesh *pMesh = new NRectMesh;

	pMesh->SetMeshID(m_pCInfoC->PColorNum);
	pMesh->SetRowsNum(num + 1);
	AppendTrimedArray(0, pMesh->m_arr[0]);
	for (int i = 1; i < num; i++)
	{
		BMatr m = m_pCInfo->CurLine.GetDriveMatr((double)i / num);
		pMesh->m_arr[i].SetSize(pMesh->m_arr[0].GetSize());
		for (int j = 0; j < pMesh->m_arr[0].GetSize(); j++)
		{
			if (j == 0 && IsTrimed && StartTrimed)
			{
				pMesh->m_arr[i][j] = TrimPointForInd;
			}
			else if (j == pMesh->m_arr[0].GetSize() - 1 && IsTrimed && !StartTrimed)
			{
				pMesh->m_arr[i][j] = TrimPointForInd;
			}
			else
			{
				const BPoint& P0 = *pForest->GetPoint(pMesh->m_arr[0][j]);
				pMesh->m_arr[i][j] = pForest->AddPoints(P0 * m);
			}
		}
	}
	
	if (IsTrimed && TrimPointForInd == -2)// degraded mesh
	{
		pMesh->SetRowsNum(1);
		pMesh->m_arr[0].Append(m_arr[0]);
		m_arr[1].RemoveAll();
		m_arr[1].Append(m_arr[0]);
	}
	else
	{
		AppendTrimedArray(1, pMesh->m_arr[num]);
	}
	pMesh->m_IsDeg = 0;
	pMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pMesh->Shank = m_pCInfoC->Shank;
	pMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	return pMesh;
}
int NOnePartProcessing::RotatePoint(int PointInd, const BMatr& m)
{
	const BPoint& P0 = *MFOREST.GetPoint(PointInd);
	BPoint P1 = P0 * m;
	const double D = MIND * MIND;
	if ((P1 - P0).D2() < D)
		return PointInd;
	else
		return MFOREST.AddPoints(P1);
}
NRectMesh* NOnePartProcessing::Create2dAngleMesh()
{
	// Построение сектора "доворота"
	if (!m_pCInfoC->m_pToolsContour)
		return nullptr;
	BPoint p = m_pCInfo->CurLine.GetDir(1);
	double Angle = p.Angle(m_pCInfo->m_NextDir);
	if (fabs(Angle) < MINAD)
		return nullptr;
	double Rmax = m_pCInfoC->m_pToolsContour->GetMaxX();
	int n = NSurfSettings::GetNumAproxLine(Rmax, Angle);
	NRectMesh* pParMesh = CreateRotMesh(m_arr[1], 0, int(m_arr[1].GetSize() - 1), m_pCInfo->CurLine.p[3], Angle, n);
	pParMesh->m_IsDeg = 0;
	return pParMesh;
}
void NOnePartProcessing::TrimStartCont(const BPoint& C, const BPoint& N, const NOnePartProcessing* pNextPart)
{
	if (!m_pCInfoC->m_pToolsContour)
		return;
	int Size = int(m_arr[0].GetSize()) + ((pNextPart != nullptr) ? 1 : 0);
	std::vector<const BPoint*> Points(Size);
	if (pNextPart != nullptr)
		Points.back() = MFOREST.GetPoint(pNextPart->GetArray(0)[0]);
	for (int k = 0; k < m_arr[0].GetSize(); ++k)
		Points[k] = MFOREST.GetPoint(m_arr[0][k]);
	std::vector<int> Signes(Size);// -1 - extra point, +1 - necessary point, 0 - point on trim plane
	bool AllPlus = true;
	bool AllMinus = true;
	bool HaveZero = false;
	for (int i = 0; i < Size; ++i)
	{
		const BPoint* pp1 = Points[i];
		BPoint V = *pp1 - C;
//		V.ProjXY();
		const double D2 = V.D2();
		if (D2 < MIND * MIND)
			Signes[i] = 0;
		else
			Signes[i] = (V * N < 0.) ? 1 : -1;// true - keep point
		switch (Signes[i])
		{
		case 1:
			AllMinus = false;
			break;
		case 0:
			AllMinus = false;
			AllPlus = false;
			HaveZero = true;
			break;
		case -1:
			AllPlus = false;
		}
	}
	if (AllPlus)
	{
		IsTrimed = false;
		return;
	}
	if (AllMinus)
	{
		ClearTrimed();
		IsTrimed = true;
		return;
	}
	IsTrimed = false;
	for (int i = 0; i < Size - 1; ++i)
	{
		if (Signes[i] == 0)
		{
			IsTrimed = true;
			StartTrimed = Signes[i + 1] == 1;
			TrimPointLocInd = i - StartTrimed ? 0 : 1;
			if (Signes[i + 1] == 0)
				TrimPointForInd = -2;
			else
				TrimPointForInd = m_arr[0][i];
			break;
		}
		if (Signes[i] * Signes[i + 1] < 0)// intersection point found;
		{
			IsTrimed = true;
			TrimPointLocInd = i;
			StartTrimed = Signes[i + 1] == 1;
			const BPoint* pp0 = Points[i];
			const BPoint* pp1 = Points[i + 1];
			double A = (C - *pp0) * N / ((*pp1 - *pp0) * N);
			BPoint P = (*pp0) * (1. - A) + (*pp1) * A;
			TrimPointForInd = MFOREST.AddPoints(P);
			break;
		}
	}
}

void NOnePartProcessing::ClearTrimed()
{
	IsTrimed = false;
	StartTrimed = false;
	TrimPointForInd = -1;
	TrimPointLocInd = -1;
}
void NOnePartProcessing::BuildMeshFullRot(MeshArr &MeshArray, int AppNum)
{
	CUIntArray arr;
	arr.Append(m_arr[0]);
	NRectMesh *pMesh = CreateRotMesh(arr, 0, int(arr.GetSize() - 1), m_pCInfo->CurLine.p[0], 360., AppNum);
	//замкнуть сетку
	for(int i = 0; i < arr.GetSize(); ++i)
		pMesh->m_arr[AppNum][i] = pMesh->m_arr[0][i];
	pMesh->m_sgn = 1;

	BMatr m;
	m.Trans(BPoint(0,0,0,1), m_pCInfo->m_BPoint);
	double Sign = m_Line.p[0].GetZ() > m_Line.p[3].GetZ() ? 1. : -1.;// Nedded for plane processing only
	pMesh->m_iSurf = Cr2dRotSurf(m_Line * m, m_pCInfo->CurLine.p[3], Sign);

	MeshArray.Add(pMesh);
}

void NOnePartProcessing::Copy10()// copy m_arr[1] - > m_arr[0]
{
	m_arr[0].Copy(m_arr[1]);
}

bool NOnePartProcessing::IsLeft(void) const
{
	if(m_Line.p[3].GetX() > MIND)
		return false;
	if(m_Line.p[3].GetX() < -MIND)
		return true;
	if(m_Line.p[0].GetX() > MIND)
		return false;
	if(m_Line.p[0].GetX() < -MIND)
		return true;
	return true;
}
NRectMesh *NOnePartProcessing::CreateMeshFromArc(bool LastPart, const NOnePartProcessing *pPrevPart)
{
	if(!pPrevPart)
		return NULL;
	int Start = m_pCInfoC->BaseAngles.Find(fabs(pPrevPart->BotAngle));
	int End = m_pCInfoC->BaseAngles.Find(fabs(TopAngle));
	double ArcAngle =  pPrevPart->BotAngle - TopAngle;
	if(fabs(ArcAngle) < MINAD)
		return NULL;
	BPoint StartPoint = m_pCInfo->m_CurDir;
	StartPoint.ProjXY();
	StartPoint = StartPoint * (fabs(m_Line.p[0].GetX())/sqrt(StartPoint.D2()));
	BMatr m;
	StartPoint = StartPoint * m.RotZ(BPoint(0.,0.,0.,1.),pPrevPart->BotAngle);
	StartPoint = StartPoint + m_pCInfo->m_BPoint + BPoint(0.,0., m_Line.p[0].GetZ(),0.) ;
	BPoint Center = m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[0].GetZ(),0.);
	if(!LastPart)
	{// remove last point
		End += (End > Start) ? -1 : 1;
	}
	int IndexSign = End >= Start ? 1 : -1;
	double sign = IsLeft() ? 1. : -1.;
	m_Addarr[0].RemoveAll();
	for(int ip = 0; ip <= abs(End - Start) ; ++ip)
	{
		double a = (m_pCInfoC->BaseAngles[Start + ip * IndexSign] - m_pCInfoC->BaseAngles[Start]) * sign;
		BMatr m;
		m.RotZ(Center, a);
		BPoint tmpPoint = StartPoint * m;
		m_Addarr[0].Add(MFOREST.AddPoints(tmpPoint));
	}

// Create mesh 
	m_Addarr[1].RemoveAll();
	m_Addarr[1].Append(m_Addarr[0]);
	NRectMesh *pMesh = CreateParMeshTrans(m_Addarr[1], m_pCInfo->m_CurDir);
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOECyl(
		m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[0].GetZ(),0.),
		m_pCInfo->m_CurDir * (1./sqrt(m_pCInfo->m_CurDir.D2())),
		BPoint(0.,0.,1.,0),
		fabs(m_Line.p[0].GetX())));
	pMesh->m_sgn = 
		((TopAngle - pPrevPart->BotAngle > 0.) ? -1 : 1) *
		(m_pCInfo->m_CurDir.GetZ() > 0. ? -1 : 1);
	return pMesh;

}
// создание сетки перемещением одного контура на заданный вектор
// без определения знака и создания OrigSurf
NRectMesh* NOnePartProcessing::CreateParMeshTrans(CUIntArray &arr, const BPoint &dir)
{
	MBSPForest *pForest = &MBSPForest::GetActive();
	BPoint p, a, b;
	NRectMesh *pParMesh = new NRectMesh;
	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->SetRowsNum(2);
	pParMesh->m_arr[0].Append(arr);
	for (int i = 0; i < arr.GetSize(); ++i)
		arr[i] = pForest->AddPoints(*pForest->GetPoint(arr[i]) + dir);
	pParMesh->m_arr[1].Append(arr);

	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	return pParMesh;
}
// создание сетки поворотом по BaseAngles от Start до End включительно и далее до "-Start"
// без создания OrigSurf
NRectMesh* NOnePartProcessing::CreateMeshByBase(bool First, int Start, int End) const
{
	if(Start == End)
		return NULL;
	NRectMesh *pParMesh = new NRectMesh;
	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pParMesh->SetSize(2, abs(End - Start) * 2 + 1);
	// Поворачиваем точку, соответствующую TopAngle, поэтому Start обязан соответствовать этому же углу
	BPoint StartPoint = m_pCInfo->m_CurDir;
	StartPoint.ProjXY();
	BPoint EndPoint = StartPoint * (fabs(m_Line.p[3].GetX())/sqrt(StartPoint.D2()));
	StartPoint = StartPoint * (fabs(m_Line.p[0].GetX())/sqrt(StartPoint.D2()));
	BMatr m;
	EndPoint = EndPoint * m.RotZ(BPoint(0.,0.,0.,1.), BotAngle);
	StartPoint = StartPoint * m.RotZ(BPoint(0.,0.,0.,1.), TopAngle);
	EndPoint = EndPoint + m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[3].GetZ(),0.) ;
	StartPoint = StartPoint + m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[0].GetZ(),0.) ;
	BPoint Center = m_pCInfo->m_BPoint;
	if(!First)
	{
		Center = Center + m_pCInfo->m_CurDir;
		StartPoint = StartPoint + m_pCInfo->m_CurDir;
		EndPoint = EndPoint + m_pCInfo->m_CurDir;
	}

	int IndexSign = Start < End ? 1 : -1;
	double AngleSign = IsLeft() ? 1. : -1.;
	int Count = abs(End - Start) + 1;
	for (int i = 0; i < abs(End - Start) + 1; i++)
	{
		double Angle = (m_pCInfoC->BaseAngles[Start + i*IndexSign] - m_pCInfoC->BaseAngles[Start]) * AngleSign;
		pParMesh->m_arr[0][i] = MFOREST.AddPoints(StartPoint * m.RotZ(Center, Angle));
		pParMesh->m_arr[1][i] = MFOREST.AddPoints(EndPoint * m);
		pParMesh->m_arr[0][abs(End - Start)*2 - i] = MFOREST.AddPoints(StartPoint * m.RotZ(Center, -2. * m_pCInfoC->BaseAngles[Start] * AngleSign - Angle));
		pParMesh->m_arr[1][abs(End - Start)*2 - i] = MFOREST.AddPoints(EndPoint * m);
	}
	pParMesh->m_sgn = IsLeft() ^ First ? 1 : -1;
	if (Count == m_pCInfoC->BaseAngles.GetSize())
		pParMesh->SetClosed();
	return pParMesh;
}
NRectMesh* NOnePartProcessing::CreateMeshByBase(bool First, int Start, int End, int PointsNum/* = 2*/) const
{
	if(Start == End)
		return NULL;
	NRectMesh *pParMesh = new NRectMesh;
	pParMesh->SetMeshID(m_pCInfoC->PColorNum);
	pParMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pParMesh->Shank = m_pCInfoC->Shank;
	pParMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pParMesh->SetSize(PointsNum, abs(End - Start) * 2 + 1);
	// Поворачиваем точку, соответствующую TopAngle, поэтому Start обязан соответствовать этому же углу
	BPoint StartPoint = m_pCInfo->m_CurDir;
	StartPoint.ProjXY();
	BPoint EndPoint = StartPoint * (fabs(m_Line.p[3].GetX())/sqrt(StartPoint.D2()));
	StartPoint = StartPoint * (fabs(m_Line.p[0].GetX())/sqrt(StartPoint.D2()));
	BMatr m;
	EndPoint = EndPoint * m.RotZ(BPoint(0.,0.,0.,1.), BotAngle);
	StartPoint = StartPoint * m.RotZ(BPoint(0.,0.,0.,1.), TopAngle);
	EndPoint = EndPoint + m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[3].GetZ(),0.) ;
	StartPoint = StartPoint + m_pCInfo->m_BPoint + BPoint(0.,0.,m_Line.p[0].GetZ(),0.) ;
	BPoint Center = m_pCInfo->m_BPoint;
	if(!First)
	{
		Center = Center + m_pCInfo->m_CurDir;
		StartPoint = StartPoint + m_pCInfo->m_CurDir;
		EndPoint = EndPoint + m_pCInfo->m_CurDir;
	}

	int IndexSign = Start < End ? 1 : -1;
	double AngleSign = IsLeft() ? 1. : -1.;
	for (int i = 0; i < abs(End - Start) + 1; i++)
	{
		double Angle = (m_pCInfoC->BaseAngles[Start + i*IndexSign] - m_pCInfoC->BaseAngles[Start]) * AngleSign;
		m.RotZ(Center, Angle);
		pParMesh->m_arr[0][i] = MFOREST.AddPoints(StartPoint * m);
		pParMesh->m_arr[PointsNum - 1][i] = MFOREST.AddPoints(EndPoint * m);
		m.RotZ(Center, -2. * m_pCInfoC->BaseAngles[Start] * AngleSign - Angle);
		pParMesh->m_arr[0][abs(End - Start)*2 - i] = MFOREST.AddPoints(StartPoint * m);
		pParMesh->m_arr[PointsNum - 1][abs(End - Start)*2 - i] = MFOREST.AddPoints(EndPoint * m);
	}
	if(PointsNum > 2)
	{
		for(int l = 1; l < PointsNum - 1; ++l)
		{
			StartPoint = m_pCInfo->m_CurDir;
			StartPoint.ProjXY();
			StartPoint = StartPoint * (fabs(m_Line.GetPointFromParam(double(l) / (PointsNum - 1)).GetX())/sqrt(StartPoint.D2()));
			BMatr m;
			StartPoint = StartPoint * m.RotZ(BPoint(0.,0.,0.,1.), TopAngle);
			StartPoint = StartPoint + m_pCInfo->m_BPoint + BPoint(0., 0., m_Line.GetPointFromParam(double(l) / (PointsNum - 1)).GetZ(), 0.) ;
			if(!First)
			{
				StartPoint = StartPoint + m_pCInfo->m_CurDir;
			}
			for (int i = 0; i < abs(End - Start) + 1; i++)
			{
				double Angle = (m_pCInfoC->BaseAngles[Start + i*IndexSign] - m_pCInfoC->BaseAngles[Start]) * AngleSign;
				m.RotZ(Center, Angle);
				pParMesh->m_arr[l][i] = MFOREST.AddPoints(StartPoint * m);
				m.RotZ(Center, -2. * m_pCInfoC->BaseAngles[Start] * AngleSign - Angle);
				pParMesh->m_arr[l][abs(End - Start)*2 - i] = MFOREST.AddPoints(StartPoint * m);
			}
		}
	}
	pParMesh->m_sgn = -1;
	return pParMesh;
}
double NOnePartProcessing::GetMinAngle()
{
	return min(fabs(TopAngle), fabs(BotAngle));
}
double NOnePartProcessing::GetMaxAngle()
{
	return max(fabs(TopAngle), fabs(BotAngle));
}
double NOnePartProcessing::GetTopAngle()
{
	return TopAngle;
}
double NOnePartProcessing::GetBotAngle()
{
	return BotAngle;
}
NRectMesh *NOnePartProcessing::CreateHorCircle(BPoint Center, double Rad, double Sign) const
{
	if(Rad <= 0.)
		return 0;
	BPoint N = BPoint(0.,0.,Sign,0);
	NRectMesh *pMesh = new NRectMesh;
	pMesh->SetMeshID(m_pCInfoC->PColorNum);
	pMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pMesh->Shank = m_pCInfoC->Shank;
	pMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pMesh->SetSize(1, m_pCInfoC->BaseAngles.GetSize() * 2 - 2);
	BPoint StartPoint = m_pCInfo->m_CurDir;
	StartPoint.ProjXY();
	StartPoint = StartPoint * (Rad/sqrt(StartPoint.D2()));
	StartPoint = Center + StartPoint;
	BMatr m;
	for (int i = 0; i < m_pCInfoC->BaseAngles.GetSize(); ++i)
	{
		pMesh->m_arr[0][i] = MFOREST.AddPoints(
			StartPoint * m.RotZ(Center, m_pCInfoC->BaseAngles[i]));
	}
	for (int i = 2; i < m_pCInfoC->BaseAngles.GetSize(); ++i)
	{
		pMesh->m_arr[0][i + m_pCInfoC->BaseAngles.GetSize() - 2] = MFOREST.AddPoints(
			StartPoint * m.RotZ(Center, -m_pCInfoC->BaseAngles[m_pCInfoC->BaseAngles.GetSize() - i]));
	}
	pMesh->m_sgn = int(Sign);
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(N, Center));
	return pMesh;
}

NRectMesh* NOnePartProcessing::CreateHorRing(BPoint Center, double R0, double R1, double Sign) const
{
	if (R0 > R1)
	{
		const double b = R0; R0 = R1; R1 = b;
	}
	if (R0 < 0.)
		return nullptr;
	if (R0 < MIND)
		return CreateHorCircle(Center, R1, -Sign);

	BPoint N = BPoint(0., 0., -Sign, 0);
	NRectMesh* pMesh = new NRectMesh;
	pMesh->SetMeshID(m_pCInfoC->PColorNum);
	pMesh->CuttingTool = m_pCInfoC->CuttingTool;
	pMesh->Shank = m_pCInfoC->Shank;
	pMesh->m_CadrNum = m_pCInfo->m_CadrNum;
	pMesh->SetSize(2, m_pCInfoC->BaseAngles.GetSize() * 2 - 1);
	BPoint V = m_pCInfo->m_CurDir;
	V.ProjXY();
	V = V * (1. / sqrt(V.D2()));
	BPoint StartPoint0 = Center + V * R0;
	BPoint StartPoint1 = Center + V * R1;
	BMatr m;
	for (int i = 0; i < m_pCInfoC->BaseAngles.GetSize(); ++i)
	{
		m.RotZ(Center, m_pCInfoC->BaseAngles[i]);
		pMesh->m_arr[0][i] = MFOREST.AddPoints(StartPoint0 * m);
		pMesh->m_arr[1][i] = MFOREST.AddPoints(StartPoint1 * m);
	}
	for (int i = 1; i < m_pCInfoC->BaseAngles.GetSize(); ++i)
	{
		m.RotZ(Center, -m_pCInfoC->BaseAngles[m_pCInfoC->BaseAngles.GetSize() - i - 1]);
		pMesh->m_arr[0][i + m_pCInfoC->BaseAngles.GetSize() - 1] = MFOREST.AddPoints(StartPoint0 * m);
		pMesh->m_arr[1][i + m_pCInfoC->BaseAngles.GetSize() - 1] = MFOREST.AddPoints(StartPoint1 * m);
	}
	pMesh->m_sgn = int(Sign);
	pMesh->m_iSurf = MFOREST.AddOrigSurf(new SOPlane(N, Center));
	return pMesh;
}
