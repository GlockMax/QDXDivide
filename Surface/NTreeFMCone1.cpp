// NTreeFMCone.cpp: implementation of the NTreeFMCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMatr.h"
#include "NTreeFMCone.h"
#include "MFace.h"
#include "MPlane.h"
#include "MSimpleFace.h"
#include "MRefinedFace.h"
#include "MDisjoinFace.h"
#include "MBSPNode.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NTreeFMCone::NTreeFMCone()
{

}

NTreeFMCone::~NTreeFMCone()
{

}

// ПОСТРОЕНИЕ ТОЧЕК 
int NTreeFMCone::ComputePoints(BuildingType BuildType, TopBotType top_and_bot, 
							   NCadrParam *pArrITop, NCadrParam *pArrIBot, 
							   int &indStart, int &indEnd, 
							   int indTopLineDj, int indBotLineDj)
{
/*	if (BuildType == surFront ||
		BuildType == surLateral ||
		BuildType == surBack)
		return -1;*/

	double Gamma, Gamma1;
	NLine L0, L1, L3, Lm0, Lm1, Lr;
	BMatr m;
	BPoint st, st1, pt, *pts = NULL, direct, cent;
	int i, num =0, k=0, n=0, l, t;
	double length = sqrt(pt.Dist2(m_pDirLine->p[3], m_pDirLine->p[0]));
	bool bTop = false, bBot = false;

	Gamma = m_Alf/2;
	st.Set(0.,0.,0.,1.);

	m.RotZ(pt, m_BAlf);
	L0 = m_Line * m;
	m.RotZ(pt, m_endBAlf);
	Lr = m_Line * m;
	m.RotZ(pt, Gamma);

	direct = m_pDirLine->p[3] - m_pDirLine->p[0];
	direct = direct*(1/length);
	st1 = pt + direct*length;
	cent =  pt - m_pDirLine->p[0];
	cent = m_Center + cent;

	L1 = L0 * m;
	L3 = L1 * m;

	m.Trans(st, st1);
	Lm0 = L0 * m;
	Lm1 = L3 * m;
	Lr = Lr * m;
	
	if (m_IsComplete)
		Gamma1 = m_endAlf/2;
	else
		Gamma1 = m_endAlf;
	m.RotZ(st1, Gamma1);
	Lr = Lr * m;
	//	НОМЕРАЦИЯ ТОЧЕК
	//
	//				1
	//	     	  /	|	\
	//		 	2	.		\
	//		  / |	|			0
	//		3	|  	.			|	\
	//		|	\	|			|		\
	//	L1--|---|.	\			|			10 
	//		|	|	|	4		|			|	\
	//		|	.	6	|		|-----L0	|		\
	//		|	| /		|\		|			|	   		14
	//		|	7 		|	\	|	   Lm0--|   	    |	
	//		| /		    |		5	11		|	  	  /	|
	//		8 	     	|			|\		|      15	|
	//			\		|----L3		|	\	|	  /	|	|
	//				\	|			|		12 16	|	|
	//					9 	   Lm1--|   		|	|----Lr
	//						\		|    		|	|	|
	//							\	|			|   |	17
	//								13		 	|   |  /
	//									\		|   18
	//										\	|  /
	//											19
	//	
	
	num = 0;
	// определение того, для каких частей (верхней или нижней) строятся точки
	switch(top_and_bot)
	{
	case surTOP_BOTTOM:
		bTop = true;
		bBot = true;
		break;
	case surTOP:
		bTop = true;
		break;
	case surBOTTOM:
		bBot = true;
	}
	// ПОДСЧЁТ ЧИСЛА ВНОВЬ ДОБАВЛЯЕМЫХ ТОЧЕК
	switch(BuildType) 
	{
	case surBuildAll:		// передняя часть + средняя часть + задняя часть
		if (top_and_bot == surTOP_BOTTOM)
		{
			num = 10 + 4;
			num += m_IsComplete ? 6 : 4;
			t = m_IsComplete ? 3 : 2;
		}
		else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
		{
			num = 5 + 2;
			num += m_IsComplete ? 3 : 2;
			t = m_IsComplete ? 3 : 2;
		}
		break;
	case surFront:			// передняя часть
		if (top_and_bot == surTOP_BOTTOM)
			num += 10;
		else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
			num += 5;
		break;
	case surFrontALateral:	// передняя часть + средняя часть 
		if (top_and_bot == surTOP_BOTTOM)
			num = 10 + 4;
		else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
			num = 5 + 2;
		break;
	case surLateral:		// средняя часть
		if (top_and_bot == surTOP_BOTTOM)
			num += 4 + 4;
		else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
			num += 2 + 2;
		break;
	case surLateralABack: // средняя часть + задняя часть
		if (top_and_bot == surTOP_BOTTOM)
		{
			num = 4;
			num += m_IsComplete ? 6 : 4;
			t = m_IsComplete ? 3 : 2;
		}
		else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
		{
			num = 2;
			num += m_IsComplete ? 3 : 2;
			t = m_IsComplete ? 3 : 2;
		}
		break;
/*	case surBack:		// задняя часть
		if (m_IsComplete)
		{
			if (top_and_bot == surTOP_BOTTOM)
				num += 8; //num += 10;
			else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
				num += 3; //num += 5;
			t = 5;
		}
		else
		{
			if (top_and_bot == surTOP_BOTTOM)
				num += 4; //num += 6;
			else if (top_and_bot == surTOP || top_and_bot == surBOTTOM)
				num += 1; //num += 3;
			t = 3;
		}*/
		break;
	}
	//-----------ВЫДЕЛЕНИЕ ПАМЯТИ-----------
	if (!pts)
		pts = new BPoint[num];
	//-----------ЗАДАНИЕ ТОЧЕК--------------
	// ПЕРЕДНЯЯ ЧАСТЬ
	n = 0;
	if (BuildType == surBuildAll	|| 
		BuildType == surFront		||
		BuildType == surFrontALateral)
	{
		for (i=0, k=0; i<4; i+=3)
		{
			if (i == 0 && !bTop)
				continue;
		// 0 - 5
			pts[k*5].Set(L0.p[i].GetX(), L0.p[i].GetY(), L0.p[i].GetZ(), L0.p[i].GetH());
		// 1 - 6
			pt.Set(st.GetX(), st.GetY(), L0.p[i].GetZ(), 1);
			GetPointOnHArc(pts[k*5+1], pt, L0.p[i], L1.p[i], Gamma, m_Line.p[i].GetX());
		// 2 - 7
			pts[k*5+2].Set(L1.p[i].GetX(), L1.p[i].GetY(), L1.p[i].GetZ(), L1.p[i].GetH());
		// 3 - 8	
			pt.Set(st.GetX(), st.GetY(), L3.p[i].GetZ(), 1);
			GetPointOnHArc(pts[k*5+3], pt, L1.p[i], L3.p[i], Gamma, m_Line.p[i].GetX());
		// 4 - 9
			pts[k*5+4].Set(L3.p[i].GetX(), L3.p[i].GetY(), L3.p[i].GetZ(), L3.p[i].GetH());
			k++;
		}
		n += bTop ? 5 : 0;
		n += bBot ? 5 : 0;
	}
	// СРЕДНЯЯ ЧАСТЬ
	if (BuildType == surBuildAll	|| 
		BuildType == surFrontALateral ||
		BuildType == surLateral		||
		BuildType == surLateralABack ||
		BuildType == surBack)
	{
		// определение точек пересечения граней кадров
		// для горизонтального случая
		if (!m_IsComplete &&
			fabs((m_pDirLine->p[0]-m_pDirLine->p[3])*BPoint(0,0,1,0)) <= ACCURACY)
		{
			for (i=0; i<4; i+=3)
			{
				pt.Set(st1.GetX(), st1.GetY(), Lm0.p[i].GetZ(), 1);
				if (m_IsClockwise)
					GetPointOnHArc(Lm0.p[i], pt, Lm1.p[i], Lr.p[i], Gamma1, m_Line.p[i].GetX(), false);
				else
					GetPointOnHArc(Lm1.p[i], pt, Lm0.p[i], Lr.p[i], Gamma1, m_Line.p[i].GetX(), false);
			}
		}
		if (bTop)
		{
			pts[n++].Set(Lm0.p[0].GetX(), Lm0.p[0].GetY(), Lm0.p[0].GetZ(), Lm0.p[0].GetH());
			pts[n++].Set(Lm1.p[0].GetX(), Lm1.p[0].GetY(), Lm1.p[0].GetZ(), Lm1.p[0].GetH());
		}
		if (bBot)
		{
			pts[n++].Set(Lm0.p[3].GetX(), Lm0.p[3].GetY(), Lm0.p[3].GetZ(), Lm0.p[3].GetH());
			pts[n++].Set(Lm1.p[3].GetX(), Lm1.p[3].GetY(), Lm1.p[3].GetZ(), Lm1.p[3].GetH());
		}
	}
	// ЗАДНЯЯ ЧАСТЬ
	if (BuildType == surBuildAll	|| 
		BuildType == surLateralABack ||
		BuildType == surBack)
	{
		for (i=0, k=0, l=0; i<4; i+=3, l=0)
		{
			if (i == 0 && !bTop)
				continue;
		// 15 - 18
			pt.Set(st1.GetX(), st1.GetY(), Lm0.p[i].GetZ(), 1);
			if (m_IsClockwise)
				GetPointOnHArc(pts[n+k*t+l++], pt, Lm1.p[i], Lr.p[i], Gamma1, m_Line.p[i].GetX());
			else
				GetPointOnHArc(pts[n+k*t+l++], pt, Lm0.p[i], Lr.p[i], Gamma1, m_Line.p[i].GetX());
		// 16 - 19
			pts[n+k*t+l++].Set(Lr.p[i].GetX(), Lr.p[i].GetY(), Lr.p[i].GetZ(), Lr.p[i].GetH());
			if (m_IsComplete)
			{
			// 17 - 20	
				pt.Set(st1.GetX(), st1.GetY(), Lm0.p[i].GetZ(), 1);
				if (m_IsClockwise)
					GetPointOnHArc(pts[n+k*t+l++], pt, Lm0.p[i], Lr.p[i], Gamma1, m_Line.p[i].GetX());
				else
					GetPointOnHArc(pts[n+k*t+l++], pt, Lm1.p[i], Lr.p[i], Gamma1, m_Line.p[i].GetX());
			}
			k++;
		}
	}

	//-----------СОХРАНЕНИЕ ТОЧЕК В ЛЕС-----------
	m_pAForest->AddPoints(pts, num, indStart, indEnd);
	
	//-----------РАСПРЕДЕЛЕНИЕ И СОХРАНЕНИЕ НОМЕРОВ ТОЧЕК-----------
	// назначение переменных 
	// n - общий счётчик точек
	// k - индекс последней построенной точки для вехней части
	// t - индекс последней построенной точки для нижней части
	// l - локальная вспомогательная переменная
	// передняя часть
	n = 0;	
	k = 0;
	t = 0;
	if (BuildType == surBuildAll	|| 
		BuildType == surFront		||
		BuildType == surFrontALateral)
	{
		if (bTop)
		{
			for (n=0; n<=4; n++)
				pArrITop->arrIndex[0].arrIPoint.Add(indStart+n);
			k = 4;
		}
		if (bBot)
		{
			for (; n<=k+5; n++)
				pArrIBot->arrIndex[0].arrIPoint.Add(indStart+n);
			t = n - 1;
		}
	}
	// средняя часть
	if (BuildType == surBuildAll	|| 
		BuildType == surFrontALateral )
	{
		if (bTop)
		{
			pArrITop->arrIndex[1].arrIPoint.Add(indStart+0);
			pArrITop->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrITop->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrITop->arrIndex[1].arrIPoint.Add(indStart+k);
			k = n - 1;
		}
		if (bBot)
		{
			pArrIBot->arrIndex[1].arrIPoint.Add(indStart+t-4);
			pArrIBot->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrIBot->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrIBot->arrIndex[1].arrIPoint.Add(indStart+t);
			t = n - 1;
		}
	}
	else if (//	BuildType == surLateral		||
		BuildType == surLateralABack)
	{
		n = 0;
		if (bTop)
		{
			pArrITop->arrIndex[1].arrIPoint.Add(
				m_pDisjFace->GetIndPoint(2*indTopLineDj));
			pArrITop->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrITop->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrITop->arrIndex[1].arrIPoint.Add(
				m_pDisjFace->GetIndPoint(2*indTopLineDj+1));
			k = 1;
			pt = *m_pDisjFace->GetPoint(2*indTopLineDj);
			pt = *m_pDisjFace->GetPoint(2*indTopLineDj+1);
		}
		if (bBot)
		{
			pArrIBot->arrIndex[1].arrIPoint.Add(
				m_pDisjFace->GetIndPoint(2*indBotLineDj+1));
			pArrIBot->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrIBot->arrIndex[1].arrIPoint.Add(indStart+n++);
			pArrIBot->arrIndex[1].arrIPoint.Add(
				m_pDisjFace->GetIndPoint(2*indBotLineDj));
			t = n - 1;
			pt = *m_pDisjFace->GetPoint(2*indTopLineDj+1);
			pt = *m_pDisjFace->GetPoint(2*indTopLineDj);
		}
	}
	// задняя часть
	l = 0;
	if (BuildType == surBuildAll	|| 
		BuildType == surBack		||
		BuildType == surLateralABack)
	{
		if (bTop)
		{
			if (m_IsComplete)
				l = n + 3;
			else
				l = n + 2;
			
			pArrITop->arrIndex[2].arrIPoint.Add(indStart+k-(m_IsClockwise ? 0: 1));
			for (; n<l; n++)
				pArrITop->arrIndex[2].arrIPoint.Add(indStart+n);
			pArrITop->arrIndex[2].arrIPoint.Add(indStart+k-(m_IsClockwise ? 1: 0));
		}
		if (bBot)
		{
			if (m_IsComplete)
				l = n + 3;
			else
				l = n + 2;
			
			pArrIBot->arrIndex[2].arrIPoint.Add(indStart+t - (m_IsClockwise ? 0 : 1));
			for (; n<l; n++)
				pArrIBot->arrIndex[2].arrIPoint.Add(indStart+n);
			pArrIBot->arrIndex[2].arrIPoint.Add(indStart+t - (m_IsClockwise ? 1 : 0));
		}
	}
	delete []pts;

	return 0;
}

// определяет точку на характеристической ломанной для дуги
void NTreeFMCone::GetPointOnHArc(BPoint& res, BPoint &cent, BPoint &p1, BPoint &p2, double alf, double r, bool f)
{
	double x;
	BPoint q, q1, q2;
	q1 = p1-cent;
	q2 = p2-cent;
	if (alf == 0)
	{
		alf = cent.Cos2(q1, q2);
		alf = alf *sqrt(q1.D2())*sqrt(q2.D2());	// cos alf
	}
	else
	{
		alf = alf*PI/180.;
	}
	if (r == 0)
		r = sqrt(cent.Dist2(cent, p1));

	q = q1 + q2;
	if (q.D2() > 0.)
		q = q*(1/sqrt(q.D2()));
	x = r*(1/cos(alf/2.));
	if (f) 
		res = cent + q*fabs(x);
	else
		res = cent + q*-fabs(x);
}

int NTreeFMCone::BuildTree(MBSPTree *pTree, MBSPNode *pRoot, BuildingType BuildType, bool first_cadr,
						   NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	if (m_Alf < 0.)
		return -1;
	if (BuildType == surFront)
	{
		// построить соединяющую грань ВНУТРИ/СНАРУЖИ кадра
		SetFronDisjoinFace(pTree, pRoot, pArrITop, pArrIBot);
		// формирование дерева
		BuildFrontTree(pTree, pArrITop,pArrIBot);
	}
	if (BuildType == surLateral)
	{
		if (0 < m_Alf && m_Alf < 360.)
		{
			// построить соединяющую грань ВНУТРИ/СНАРУЖИ кадра
			SetLateralDisjoinFace(pTree, pRoot, first_cadr, pArrITop, pArrIBot);
			// формирование дерева
			BuildLateralTree(pTree, pArrITop, pArrIBot);
		}
	}
	if (BuildType == surBack)
	{
		if (m_IsFirstCont)
			m_pBotNode = pRoot;
		// достроить соединяющую грань СНАРУЖИ кадра
		SetBackDisjoinFace(pTree, pArrITop, pArrIBot);
		// формирование дерева
		BuildBackTree(pTree, pArrITop, pArrIBot);
	}
	return 0;
}

void NTreeFMCone::SetFronDisjoinFace(MBSPTree *pTree, MBSPNode *pRoot,
						NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	MSimpleFace *face;
	MDisjoinFace *disj_face;
	int l, k=1;
	char str[10];
	MPlane Plane;

	// объединяющая грань  - переднюю и боковую
	face = new MSimpleFace;
	face->RemoveAll();
	face->AddPoint(pArrITop->arrIndex[0].arrIPoint[4]);
	face->AddPoint(pArrITop->arrIndex[0].arrIPoint[0]);
	face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[0]);
	face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[4]);
	Plane.SetNormal(GetNByIndex( 
		pArrITop->arrIndex[0].arrIPoint[0],
		pArrIBot->arrIndex[0].arrIPoint[4],
		pArrIBot->arrIndex[0].arrIPoint[0]));
	Plane.SetPointN(pArrIBot->arrIndex[0].arrIPoint[0]);

	if (!m_pDisjFace)
	{
		disj_face = new MDisjoinFace;
		disj_face->RemoveAll();
		disj_face->AddFirst((MFace*) face);
		disj_face->SetPlane(Plane);

		l = m_pAForest->AddFaces(disj_face);
		sprintf(str, "V:%d", 1);
		pTree->AddNode(pRoot, l, false, str);

		// сохранение первого узла
		m_pDisjFace = disj_face;
		m_pDisjNode = pRoot;
		m_pBotNode = pRoot;
	}
	else
	{
		face->SetPlane(Plane);
		if (m_pDisjFace->GetCount() == 0)
			m_pDisjFace->AddFirst((MFace*) face);
		else
			m_pDisjFace->AddFollow((MFace*) face);
	}
	delete face;
	// отрезать верхнюю часть
	if (m_IsNeedTop)
	{
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[0]);
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[1]);
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[3]);
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[4]);
		Plane.SetNormal(GetNByIndex( 
			pArrITop->arrIndex[0].arrIPoint[1],
			pArrITop->arrIndex[0].arrIPoint[3],
			pArrITop->arrIndex[0].arrIPoint[4]));
		Plane.SetPointN(pArrITop->arrIndex[0].arrIPoint[0]);
		face->SetPlane(Plane);
		sprintf(str, "T1:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(m_pBotNode, l, !m_IsFirstCont, str);
	}
	// отрезать нижнюю часть 
	if (m_IsNeedBot)
	{
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[4]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[3]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[1]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[0]);
		Plane.SetNormal(GetNByIndex( 
			pArrIBot->arrIndex[0].arrIPoint[4],
			pArrIBot->arrIndex[0].arrIPoint[3],
			pArrIBot->arrIndex[0].arrIPoint[1]));
		Plane.SetPointN(pArrIBot->arrIndex[0].arrIPoint[4]);
		face->SetPlane(Plane);
		sprintf(str, "T1:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(m_pBotNode, l, !m_IsFirstCont, str);
	}
}

void NTreeFMCone::SetLateralDisjoinFace(MBSPTree *pTree, MBSPNode *pRoot, bool first_cadr,
			   				NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	MSimpleFace *face;
	MDisjoinFace *disj_face;
	MPlane Plane;
	int l, k=1;
	char str[10];
	
	face = new MSimpleFace;
	face->RemoveAll();

	face->AddPoint(pArrITop->arrIndex[1].arrIPoint[2]);
	face->AddPoint(pArrITop->arrIndex[1].arrIPoint[1]);
	face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[1]);
	face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[2]);
	Plane.SetNormal(GetNByIndex( 
		pArrITop->arrIndex[1].arrIPoint[1],
		pArrIBot->arrIndex[1].arrIPoint[2],
		pArrIBot->arrIndex[1].arrIPoint[1]));
	Plane.SetPointN(pArrITop->arrIndex[1].arrIPoint[2]);

	// дополнительная объединяющая грань
	if (!m_pDisjFace)
	{
		disj_face = new MDisjoinFace;
		disj_face->RemoveAll();
		disj_face->AddFirst((MFace*) face);
		disj_face->SetPlane(Plane);
	
		l = m_pAForest->AddFaces(disj_face);
		sprintf(str, "V:%d", 1);
		pTree->AddNode(pRoot, l, first_cadr, str);
		
		// сохранение первого узла
		m_pDisjFace = disj_face;
		m_pDisjNode = pRoot;
		m_pBotNode = pRoot;
	}
	else
	{
		face->SetPlane(Plane);
		if (m_pDisjFace->GetCount() == 0)
			m_pDisjFace->AddFirst((MFace*) face);
		else
			m_pDisjFace->AddFollow((MFace*) face);
	}
	delete face;	
	// отрезать верхнюю часть
	if (m_IsNeedTop)
	{
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[0]);
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[1]);
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[2]);
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[3]);
		Plane.SetNormal(GetNByIndex( 
			pArrITop->arrIndex[1].arrIPoint[0],
			pArrITop->arrIndex[1].arrIPoint[1],
			pArrITop->arrIndex[1].arrIPoint[2]));
		Plane.SetPointN(pArrITop->arrIndex[1].arrIPoint[0]);
		face->SetPlane(Plane);
		sprintf(str, "T2:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(m_pBotNode, l, !m_IsFirstCont, str);
	}
	// отрезать нижнюю часть 
	if (m_IsNeedBot)
	{
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[3]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[2]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[1]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[0]);
		Plane.SetNormal(GetNByIndex( 
			pArrIBot->arrIndex[1].arrIPoint[3],
			pArrIBot->arrIndex[1].arrIPoint[2],
			pArrIBot->arrIndex[1].arrIPoint[1]));
		Plane.SetPointN(pArrIBot->arrIndex[1].arrIPoint[3]);
		face->SetPlane(Plane);
		sprintf(str, "T2:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(m_pBotNode, l, !m_IsFirstCont, str);
	}
}

void NTreeFMCone::SetBackDisjoinFace(MBSPTree *pTree, 
								NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	// грань строится между двумя кадрами
	// вершина в дереве для этой грани уже есть
	if (m_IsComplete)
		return;

	MPlane Plane;
	MSimpleFace *face = NULL;

	face = new MSimpleFace;
	face->RemoveAll();
	// объединяющая грань  - между двумя кадрами
	if (m_IsClockwise)
	{
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[3]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[2]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[2]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[3]);
		Plane.SetNormal(GetNByIndex( 
			pArrIBot->arrIndex[2].arrIPoint[2],
			pArrIBot->arrIndex[2].arrIPoint[3],
			pArrITop->arrIndex[2].arrIPoint[2]));
		Plane.SetPointN(pArrITop->arrIndex[2].arrIPoint[2]);		
	}
	else
	{
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[2]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[3]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[3]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[2]);
		Plane.SetNormal(GetNByIndex( 
			pArrIBot->arrIndex[2].arrIPoint[3],
			pArrIBot->arrIndex[2].arrIPoint[2],
			pArrITop->arrIndex[2].arrIPoint[3]));
		Plane.SetPointN(pArrITop->arrIndex[2].arrIPoint[2]);
	}
	face->SetPlane(Plane);
	if (!m_pDisjFace)
	{
		m_pDisjFace = new MDisjoinFace;
		m_pDisjFace->AddFirst((MFace*) face);
	}
	else
	{
		if (m_pDisjFace->GetCount() == 0)
			m_pDisjFace->AddFirst((MFace*) face);
		else
			m_pDisjFace->AddFollow((MFace*) face);
	}
	delete face;
}

void NTreeFMCone::BuildFrontTree(MBSPTree *pTree, 
								 NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	MSimpleFace *face;
	MRefinedFace *ref_face;
	MBSPNode *pNode1 = NULL, 
			  *pNode2 = NULL,
			  *pNode3 = NULL;
	int i, l, k = 1;
	char str[10];
	MPlane Plane;

	//	НОМЕРАЦИЯ ТОЧЕК
	//
	//				1t
	//	       	  /	|	\
	//		  	2t	.		\
	//		  / |	|	  		0t
	//		3t	|   .			|
	//		|	\	|			|
	//		|	.	\			|			| Z
	//		|	|	|	4t		|			|
	//		|	.	1b	|		|			|
	//		|	|  /	|		|			|
	//		|	2b		|	\	|			|	
	//		| /		    |		0b			|	
	//		3b	     	|					\
	//			\		|				   /  \
	//				\	|				 /	  	\
	//					4b			   / X		Y \
	
	k=1;
	i=0;
	pNode1 = m_pBotNode;
	for (i=0; i<3; i+=2)
	{
	// PLANE 1 - 4
		ref_face = new MRefinedFace;
		ref_face->RemoveAll();
		ref_face->AddPoint(pArrITop->arrIndex[0].arrIPoint[0+i]);
		ref_face->AddPoint(pArrITop->arrIndex[0].arrIPoint[2+i]);
		ref_face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[2+i]);
		ref_face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[0+i]);
		Plane.SetNormal(GetNByIndex( 
						pArrIBot->arrIndex[0].arrIPoint[0+i],
						pArrITop->arrIndex[0].arrIPoint[2+i],
						pArrITop->arrIndex[0].arrIPoint[0+i]));
		Plane.SetPointN(pArrITop->arrIndex[0].arrIPoint[0+i]);
		ref_face->SetPlane(Plane);
		sprintf(str, "R:%d", k++);
		l = m_pAForest->AddFaces(ref_face);
		pTree->AddNode(pNode1, l, i==0 && m_IsLeftBranch, str);
		pNode2 = pNode1;
	// PLANE 2 - 5
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[0+i]);
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[1+i]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[1+i]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[0+i]);
		Plane.SetNormal(GetNByIndex( 
						pArrITop->arrIndex[0].arrIPoint[1+i],
						pArrITop->arrIndex[0].arrIPoint[0+i],
						pArrIBot->arrIndex[0].arrIPoint[1+i]));
		Plane.SetPointN(pArrITop->arrIndex[0].arrIPoint[0+i]);
		face->SetPlane(Plane);
		sprintf(str, "C:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(pNode2, l, true, str);
	// PLANE 3 - 6
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[1+i]);
		face->AddPoint(pArrITop->arrIndex[0].arrIPoint[2+i]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[2+i]);
		face->AddPoint(pArrIBot->arrIndex[0].arrIPoint[1+i]);
		Plane.SetNormal(GetNByIndex( 
						pArrITop->arrIndex[0].arrIPoint[2+i],
						pArrITop->arrIndex[0].arrIPoint[1+i],
						pArrIBot->arrIndex[0].arrIPoint[2+i]));
		Plane.SetPointN(pArrITop->arrIndex[0].arrIPoint[1+i]);
		face->SetPlane(Plane);
		sprintf(str, "C:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(pNode2, l, false, str);
	}
}

void NTreeFMCone::BuildLateralTree(MBSPTree *pTree, 
								   NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	MSimpleFace *face;
	MBSPNode *pNode;
	MPlane Plane;
	int l;
	char str[10];

	//	НОМЕРАЦИЯ ТОЧЕК
	//
	//				0t
	//			  	|	\	->
	//		 		.		\
	//			 	|			1t
	//		3t	 	.			|
	//		|	\	|			|
	//		|		\			|			| Z
	//		|		|	2t		|			|
	//		|		0b	|		|			|
	//		|	  	    |		|			|
	//		|			|	\	|			|	
	//		| 		    |		1b			|	
	//		3b	   		|					\
	//			\		|				   /  \
	//			<-	\	|				 /	  	\
	//					2b			   / X		Y \
	
	pNode = m_pBotNode;
	switch(m_DirType)
	{
	case surLINE:
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[1]);
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[0]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[0]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[1]);
		Plane.SetNormal(GetNByIndex( 
						pArrITop->arrIndex[1].arrIPoint[0],
						pArrITop->arrIndex[1].arrIPoint[1],
						pArrIBot->arrIndex[1].arrIPoint[1]));
		Plane.SetPointN(pArrITop->arrIndex[1].arrIPoint[1]);
		face->SetPlane(Plane);
		sprintf(str, "C:%d", 2);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(pNode, l, m_IsLeftBranch, str);
	//-----------------------------
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[3]);
		face->AddPoint(pArrITop->arrIndex[1].arrIPoint[2]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[2]);
		face->AddPoint(pArrIBot->arrIndex[1].arrIPoint[3]);
		Plane.SetNormal(GetNByIndex( 
						pArrITop->arrIndex[1].arrIPoint[2],
						pArrITop->arrIndex[1].arrIPoint[3],
						pArrIBot->arrIndex[1].arrIPoint[2]));
		Plane.SetPointN(pArrITop->arrIndex[1].arrIPoint[3]);
		face->SetPlane(Plane);
		sprintf(str, "C:%d", 3);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(pNode, l, false, str);
		break;
	case surCWARC: case surCCWARC:
		break;
	}
}

void NTreeFMCone::BuildBackTree(MBSPTree *pTree, 
								NCadrParam *pArrITop, NCadrParam *pArrIBot)
{
	MSimpleFace *face;
	MRefinedFace *ref_face;
	MBSPNode *pNode1 = NULL, 
			  *pNode2 = NULL,
			  *pNode3 = NULL;
	int i, l, k, n;
	MPlane Plane;
	char str[10];

	//	НОМЕРАЦИЯ ТОЧЕК
	//
	//				0t
	//	       	  	|	\
	//		  		.		\
	//		   		|	  		1t
	//		4t	    .		   /|
	//		|	\	|		2t	|
	//		|		\	   /|	|			| Z
	//		|		|	3t	|	|			|
	//		|		0b	|	|	|			|
	//		|	  		| \	|	|			|
	//		|			| 	| \	|			|	
	//		| 		    |	| 	1b			|	
	//		4b	     	|	| /				\
	//			\		|	2b			   /  \
	//				\	| /				 /	  	\
	//					3b			   / X		Y \
	
	
	// флаг говорящий о том, два участка в задней поверхности или нет
	if (m_IsComplete)
		n = 1;
	else
		n = 0;
	// отрезать верхнюю часть
	if (m_IsNeedTop)
	{
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[3+n]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[2+n]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[1]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[0]);
		Plane.SetNormal(GetNByIndex( 
			pArrITop->arrIndex[2].arrIPoint[2+n],
			pArrITop->arrIndex[2].arrIPoint[1],
			pArrITop->arrIndex[2].arrIPoint[0]));
		Plane.SetPointN(pArrITop->arrIndex[2].arrIPoint[1]);
		face->SetPlane(Plane);
		sprintf(str, "T1:%d", 1);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(m_pBotNode, l, true, str);
	}
// отрезать нижнюю часть 
	if (m_IsNeedBot)
	{
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[0]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[1]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[2+n]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[3+n]);
		Plane.SetNormal(GetNByIndex( 
			pArrIBot->arrIndex[2].arrIPoint[3+n],
			pArrIBot->arrIndex[2].arrIPoint[2+n],
			pArrIBot->arrIndex[2].arrIPoint[1]));
		Plane.SetPointN(pArrIBot->arrIndex[2].arrIPoint[1]);
		face->SetPlane(Plane);
		sprintf(str, "T1:%d", 2);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(m_pBotNode, l, m_IsNeedTop ? false : true, str);
	}
	// флаг говорящий о том, два участка в задней поверхности или нет
	if (m_IsComplete)
		n = 3;
	else
		n = 1;
// остальные грани дерева
	k=1;
	pNode1 = m_pBotNode;
	for (i=0; i<n; i+=2)
	{
	// PLANE 1 - 4
		ref_face = new MRefinedFace;
		ref_face->RemoveAll();
		ref_face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[0+i]);
		ref_face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[2+i]);
		ref_face->AddPoint(pArrITop->arrIndex[2].arrIPoint[2+i]);
		ref_face->AddPoint(pArrITop->arrIndex[2].arrIPoint[0+i]);
		Plane.SetNormal(GetNByIndex( 
						pArrIBot->arrIndex[2].arrIPoint[0+i],
						pArrITop->arrIndex[2].arrIPoint[2+i],
						pArrITop->arrIndex[2].arrIPoint[0+i])*
						(m_IsClockwise ? 1: -1));
		Plane.SetPointN(pArrITop->arrIndex[2].arrIPoint[2+i]);
		ref_face->SetPlane(Plane);
		sprintf(str, "R:%d", k++);
		l = m_pAForest->AddFaces(ref_face);
		pTree->AddNode(pNode1, l, i==0 && m_IsLeftBranch, str);
		pNode2 = pNode1;
	// PLANE 2 - 5
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[0+i]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[1+i]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[1+i]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[0+i]);
		Plane.SetNormal(GetNByIndex( 
						pArrIBot->arrIndex[2].arrIPoint[1+i],
						pArrITop->arrIndex[2].arrIPoint[1+i],
						pArrITop->arrIndex[2].arrIPoint[0+i])*
						(m_IsClockwise ? 1: -1));
		Plane.SetPointN(pArrITop->arrIndex[2].arrIPoint[0+i]);
		face->SetPlane(Plane);
		sprintf(str, "C:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(pNode2, l, true, str);
	// PLANE 3 - 6
		face = new MSimpleFace;
		face->RemoveAll();
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[1+i]);
		face->AddPoint(pArrIBot->arrIndex[2].arrIPoint[2+i]);
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[2+i]);		
		face->AddPoint(pArrITop->arrIndex[2].arrIPoint[1+i]);
		Plane.SetNormal(GetNByIndex( 
						pArrIBot->arrIndex[2].arrIPoint[2+i],
						pArrITop->arrIndex[2].arrIPoint[2+i],
						pArrITop->arrIndex[2].arrIPoint[1+i])*
						(m_IsClockwise ? 1: -1));
		Plane.SetPointN(pArrITop->arrIndex[2].arrIPoint[1+i]);
		face->SetPlane(Plane);
		sprintf(str, "C:%d", k++);
		l = m_pAForest->AddFaces(face);
		pTree->AddNode(pNode2, l, false, str);
	}
}