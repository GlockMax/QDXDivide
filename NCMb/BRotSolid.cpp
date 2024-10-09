#include "stdafx.h"
#include <algorithm>
#include "ConstDef.h"
#include "BMatr.h"
#include "ClrPoint.h"
#include "BDCEL.h"
#include "NCadrGeomArray.h"
#include "NCadrGeom.h"
#include "BRotSolid.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BRotSolid::BRotSolid()
{
}
BRotSolid::~BRotSolid()
{
}

bool BRotSolid::GetFaceOrient(const BDCEL& DCEL, int FaceInd)
{
	if (FaceInd < 0)
		return false;
	const std::vector<Face>& Faces = DCEL.GetFaces();
	const std::vector<Edge>& Edges = DCEL.GetEdges();
	int tenpind = Edges[Faces[FaceInd][0]][1];//Дорисовка отрезков при условии, что горизонатльная грань пересекает ось
	if (tenpind < 0)
		return false;
	BPoint Norm = DCEL.GetNormByNum(Faces[FaceInd][1]);//нормаль грани
	BPoint FacePoint1(DCEL.Get3fvByVn(DCEL.GetESVn(tenpind))[0],DCEL.Get3fvByVn(DCEL.GetESVn(tenpind))[1],DCEL.Get3fvByVn(DCEL.GetESVn(tenpind))[2], 1.);
	BPoint FacePoint2(DCEL.Get3fvByVn(DCEL.GetEEVn(tenpind))[0],DCEL.Get3fvByVn(DCEL.GetEEVn(tenpind))[1],DCEL.Get3fvByVn(DCEL.GetEEVn(tenpind))[2], 1.);
	if (!(FacePoint1 == FacePoint2))
	{
		tenpind = Edges[tenpind][1];
		BPoint FacePoint3(DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[0],DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[1],DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[2], 1.);
		if (FacePoint3 == FacePoint1 || FacePoint3 == FacePoint2)
		{
			FacePoint3.Set(DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[0],DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[1],DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[2], 1.);
		}
		BPoint Ve1(FacePoint1.ProjXY() - BPoint(0., 0., 0., 1));
		if (Ve1 * Norm <= -epsilon)
			return true;
		BPoint Ve2(FacePoint2.ProjXY() - BPoint(0., 0., 0., 1));
		if (Ve2 * Norm <= -epsilon)
			return true;
		BPoint Ve3(FacePoint3.ProjXY() - BPoint(0., 0., 0., 1));
		if (Ve3 * Norm <= -epsilon)
			return true;
	}
	return false;
}


BPoint BRotSolid::GetVel(const BPoint& P)
{
	// returns unit velocity vector in P
	BPoint V(P.GetY(), -P.GetX(), 0., 0.);
	double D2 = V.D2();
	if (D2 > 0.)
		V = V * (1. / sqrt(D2));
	else
		V.Set(0., 0., 0., -1.);
	return V;
}

void BRotSolid::Filter(const BDCEL& DCEL, bool IgnoreVoids) //построение следа вращения по исходным отрезкам
{
	const std::vector<Face>& Faces = DCEL.GetFaces();
	const std::vector<Edge>& Edges = DCEL.GetEdges();
	std::vector<int> SignPointsInd;
	SignPointsInd.resize(Edges.size());
	std::vector<ClrPoint> NewPoints;
	for (int i = 0; i < Edges.size(); i++)//перебор по полуребрам
	{
		SignPointsInd[i] = -1;
		if (DCEL.IsDummyEdge(i))
			continue;
		if (DCEL.IsMarkedEdge(i))
			continue;
		if (!GetFaceOrient(DCEL, Edges[i][2]))
			continue;
		const ClrPoint SP(DCEL.GetClrPByVn(DCEL.GetESVn(i)));//координаты первой точки отрезка
		const ClrPoint EP(DCEL.GetClrPByVn(DCEL.GetEEVn(i)));
		BPoint Norm = DCEL.GetNormByNum(Faces[Edges[i][2]][1]);
		BPoint Os(0., 0., 1., 0.);//ось Z (ось вращения)
		ClrPoint NE = intersection3d(Os, Norm, SP, EP);
		if (NE[0] != INFINITY)
		{
			SignPointsInd[i] = int(NewPoints.size());
			NewPoints.push_back(NE);
		}
	}
	for (int e = 0; e < Edges.size(); e = DCEL.NextEdge(e)) // перебор по ребрам
	{
		if (DCEL.IsDummyEdge(e))
			continue;
		if (DCEL.IsMarkedEdge(e))
			continue;
		const ClrPoint SP(DCEL.GetClrPByVn(DCEL.GetESVn(e)));//координаты первой точки отрезка
		const ClrPoint EP(DCEL.GetClrPByVn(DCEL.GetEEVn(e)));
		// i - индекс полуребра, имеющего ТР, если такое есть
		// j - второго полуребра
		int i = e;
		int j = DCEL.PairedHalfEdge(i);
		if (SignPointsInd[i] == -1 && j != -1)
			if (SignPointsInd[j] != -1)
			{// i <-> j
				auto buf = i; i = j; j = buf;
			}
		if (j == -1)
		{
			if (SignPointsInd[i] == -1)
				out.push_back({ SP, EP });
			else
			{
				out.push_back({ SP, NewPoints[SignPointsInd[i]] });//отрезок до смены знака
				out.push_back({ EP, NewPoints[SignPointsInd[i]] });//отрезок после смены знака
			}
		}
		else
		{// both half edges exist
			if (SignPointsInd[i] == -1)
			{ // sign is constant
				BPoint Ni = DCEL.GetNormByNum(Faces[Edges[i][2]][1]);
				BPoint Nj = DCEL.GetNormByNum(Faces[Edges[j][2]][1]);
				BPoint Vs = GetVel(SP);
				BPoint Ve = GetVel(EP);
				if (!Vs.IsVector())
					Vs = Ve;
				else if (!Ve.IsVector())
					Ve = Vs;
				if (!Vs.IsVector()) // both vectors are invalid
				{
					out.push_back({ SP, EP });
				}
				else
				{
					const double Signis = Vs * Ni;
					const double Signjs = Vs * Nj;
					const double Signie = Ve * Ni;
					const double Signje = Ve * Nj;
					const double Signi = fabs(Signis) > fabs(Signie) ? Signis : Signie;
					const double Signj = fabs(Signjs) > fabs(Signje) ? Signjs : Signje;
					if (fabs(Signi) < epsilonf || fabs(Signj) < epsilonf)
						out.push_back({ SP, EP });
					else if (Signi * Signj < 0.)
						out.push_back({ SP, EP });
				}
			}
			else
			{ // sign changes
				if (SignPointsInd[j] == -1)
				{
					out.push_back({ SP, NewPoints[SignPointsInd[i]] });//отрезок до смены знака
					out.push_back({ EP, NewPoints[SignPointsInd[i]] });//отрезок после смены знака
					continue;
				}
				// 
				int k = i; // index of first point of change sign (nearest to SP)
				int l = j; // index of other point (nearest to EP)
				if ((NewPoints[SignPointsInd[k]] - SP).D2() > (NewPoints[SignPointsInd[l]] - SP).D2())
				{
					auto buf = k; k = l; l = buf;
				}
				BPoint Nk = DCEL.GetNormByNum(Faces[Edges[k][2]][1]);
				BPoint Nl = DCEL.GetNormByNum(Faces[Edges[l][2]][1]);
				BPoint Vs = GetVel(SP);
				BPoint Ve = GetVel(EP);
				if (!Vs.IsVector() || !Ve.IsVector())
					continue;// не может быть смены знака на таком ребре (одна из вершин на оси вращения)
				double Signk = Vs * Nk;
				double Signl = Vs * Nl;
				if (Signk * Signl < 0.)
				{
					out.push_back({ SP, NewPoints[SignPointsInd[k]] });//отрезок до первой смены знака
					out.push_back({ EP, NewPoints[SignPointsInd[l]] });//отрезок после второй смены знака
				}
				else
					out.push_back({ NewPoints[SignPointsInd[k]], NewPoints[SignPointsInd[l]] });//отрезок между двумя нулевыми точками на ребре
			}
		}
	}
	for (int i = 0; i < Faces.size(); i++)//перебор по граням
	{
		if (DCEL.IsDummyFace(i))
			continue;
		if (DCEL.IsMarkedFace(i))
			continue;
		BPoint norm = DCEL.GetNormByNum(Faces[i][1]);//нормаль грани
		int tenpind = Edges[Faces[i][0]][1];//Дорисовка отрезков при условии, что горизонатльная грань пересекает ось
		if (tenpind < 0)
			continue;
		ClrPoint FacePoint1(DCEL.GetClrPByVn(DCEL.GetESVn(tenpind)));
		ClrPoint FacePoint2(DCEL.GetClrPByVn(DCEL.GetEEVn(tenpind)));;
		bool FrontFace = false;
		if (FacePoint1.GetX() != FacePoint2.GetX() || FacePoint1.GetY() != FacePoint2.GetY() || FacePoint1.GetZ() != FacePoint2.GetZ())
		{
			tenpind = Edges[tenpind][1];
			BPoint FacePoint3 = { DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[0],DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[1],DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[2] };
			if (FacePoint3 == FacePoint1 || FacePoint3 == FacePoint2)
			{
				FacePoint3 = { DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[0],DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[1],DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[2] };
			}
			BPoint Ve1(-FacePoint1[0], -FacePoint1[1], 0);
			if (Ve1[1] != 0 || Ve1[0] != 0)
				Normalize(Ve1);
			BPoint Ve2(-FacePoint2[0], -FacePoint2[1], 0);
			if (Ve2[1] != 0 || Ve2[0] != 0)
				Normalize(Ve2);
			BPoint Ve3(-FacePoint3[0], -FacePoint3[1], 0);
			if (Ve3[1] != 0 || Ve3[0] != 0)
				Normalize(Ve3);
			if (((Ve1[0] * norm[0]) + (Ve1[1] * norm[1])) >= epsilonf ||
				((Ve2[0] * norm[0]) + (Ve2[1] * norm[1])) >= epsilonf ||
				((Ve3[0] * norm[0]) + (Ve3[1] * norm[1])) >= epsilonf)
				FrontFace = true;
			if (FacePoint3.GetZ() == FacePoint1.GetZ() && FacePoint3.GetZ() == FacePoint2.GetZ())
			{
				ClrPoint ZeroPoint(BPoint(0., 0., FacePoint3.GetZ(), 1.));
				ZeroPoint.SetColor(FacePoint1.GetColor());
				double alpha = ((FacePoint2.GetY() - FacePoint3.GetY()) * (ZeroPoint.GetX() - FacePoint3.GetX()) + (FacePoint3.GetX() - FacePoint2.GetX()) * (ZeroPoint.GetY() - FacePoint3.GetY())) /
					((FacePoint2.GetY() - FacePoint3.GetY()) * (FacePoint1.GetX() - FacePoint3.GetX()) + (FacePoint3.GetX() - FacePoint2.GetX()) * (FacePoint1.GetY() - FacePoint3.GetY()));
				double beta = ((FacePoint3.GetY() - FacePoint1.GetY()) * (ZeroPoint.GetX() - FacePoint3.GetX()) + (FacePoint1.GetX() - FacePoint3.GetX()) * (ZeroPoint.GetY() - FacePoint3.GetY())) /
					((FacePoint2.GetY() - FacePoint3.GetY()) * (FacePoint1.GetX() - FacePoint3.GetX()) + (FacePoint3.GetX() - FacePoint2.GetX()) * (FacePoint1.GetY() - FacePoint3.GetY()));
				double gamma = 1.0 - alpha - beta;
				if (alpha >= 0 && beta >= 0 && gamma >= 0)
				{
					out.push_back({ FacePoint1 ,ZeroPoint });
				}
			}
		}
		bool KP1 = false; bool KP2 = false;//переменные, обозначающеи наличие точек в рамках грани, лежащих на ребре(точки со сменой знака)
		ClrPoint NE1(BPoint(0., 0., 0., 1.));
		ClrPoint NE2(BPoint(0., 0., 0., 1.));
		int ECS = -1; // edge of change sign
		for (int EI = (Edges[Faces[i][0]][1]); EI >= 0; EI = (Edges[EI][1]))//перебор по ребрам в грани, перебор ребер в грани нужен для соединения найденных точек в которых меняется знак
		{

			const ClrPoint SP(DCEL.Get3fvByVn(DCEL.GetESVn(EI)));//координаты первой точки отрезка
			const ClrPoint EP(DCEL.Get3fvByVn(DCEL.GetEEVn(EI)));
			BPoint V1(GetVel(SP));//вектор скорости
			BPoint V2(GetVel(EP));
			const double RezultS = norm * V1;//произведение вектора скорости на нормаль 
			const double RezultE = norm * V2;
			int PairedEI = DCEL.PairedHalfEdge(EI);
			if (PairedEI >= 0)
			{
				BPoint innorm = DCEL.GetNormByNum(Faces[Edges[PairedEI][2]][1]);//нормаль смежной отрезку грани
				const double inRezultS = (innorm[0] * V1[0] + innorm[1] * V1[1] + innorm[2] * 0);//произведение вектора скорости на смежную отрезку нормаль
				const double inRezultE = (innorm[0] * V2[0] + innorm[1] * V2[1] + innorm[2] * 0);
				if (FrontFace &&
					(RezultS >= epsilonf &&//и произведение вектора скорости в первой точки отрезка на нормаль положительна
						RezultE <= -epsilonf) //а во второй точке отрезка произведение отрицательно
					||
					(RezultS <= -epsilonf &&
						RezultE >= epsilonf)
					)
				{
					bool& KP = KP1 ? KP2 : KP1;
					ClrPoint& NE = KP1 ? NE2 : NE1;
					KP = false;
					if (SignPointsInd[EI] >= 0)
					{
						NE = NewPoints[SignPointsInd[EI]];//находим пересечение отрезка и плоскости, проходящей через ось Z и нормаль грани
						KP = true;
						ECS = EI;
					}
				}
			}
			if (EI == Faces[i][0])
				break;
		}
		if (KP1 && KP2)// если найдены обе точки на грани в которых меняется произведение, забираем отрезок в выходной ммассив
		{
			out.push_back({ NE1,NE2 });
			KP1 = false; KP2 = false;
		}
		else if (KP1 || KP2)
		{
			auto Eind = (Edges[ECS][1]);
			const ClrPoint P(DCEL.GetClrPByVn(DCEL.GetEEVn(Eind)));// take third point of this triangle
			if (KP1)
				out.push_back({ NE1, P });
			else
				out.push_back({ NE2, P });
		}

	}
	//std::vector<ClrPoint> Pts;
	//auto Num = DCEL.RayShreddingTri(BPoint(0.001, 0.001, -1.e6, 1.), BPoint(0., 0., 1., 0.), 1.e-6, Pts);
	//for (int i = 0; i < Pts.size(); ++i)
	//{
	//	Pts.at(i).SetX(0.);
	//	Pts.at(i).SetY(0.);
	//}
	//if (IgnoreVoids)
	//{
	//	if(!Pts.empty())
	//		out.push_back({ Pts.front(), Pts.back() });// first and last points only
	//}
	//else
	//{
	//	for (int i = 1; i < Num; i += 2)
	//		out.push_back({ Pts[i - 1], Pts[i] });
	//}
}

void BRotSolid::FilterSoup(const BDCEL& DCEL)
{
	const std::vector<Face>& Faces = DCEL.GetFaces();
	const std::vector<Edge>& Edges = DCEL.GetEdges();
	std::vector<int> SignPointsInd;
	SignPointsInd.resize(Edges.size());
	std::vector<ClrPoint> NewPoints;
	for (int i = 0; i < Edges.size(); i++)//перебор по полуребрам
	{
		SignPointsInd[i] = -1;
		if (DCEL.IsDummyEdge(i))
			continue;
		if (DCEL.IsMarkedEdge(i))
			continue;
		if (!GetFaceOrient(DCEL, Edges[i][2]))
			continue;
		const ClrPoint SP(DCEL.GetClrPByVn(DCEL.GetESVn(i)));//координаты первой точки отрезка
		const ClrPoint EP(DCEL.GetClrPByVn(DCEL.GetEEVn(i)));
		BPoint Norm = DCEL.GetNormByNum(Faces[Edges[i][2]][1]);
		BPoint Os(0., 0., 1., 0.);//ось Z (ось вращения)
		ClrPoint NE = intersection3d(Os, Norm, SP, EP);
		if (NE[0] != INFINITY)
		{
			SignPointsInd[i] = int(NewPoints.size());
			NewPoints.push_back(NE);
		}
	}
	for (int e = 0; e < Edges.size(); e = DCEL.NextEdge(e)) // перебор по ребрам
	{
		if (DCEL.IsDummyEdge(e))
			continue;
		if (DCEL.IsMarkedEdge(e))
			continue;
		const ClrPoint SP(DCEL.GetClrPByVn(DCEL.GetESVn(e)));//координаты первой точки отрезка
		const ClrPoint EP(DCEL.GetClrPByVn(DCEL.GetEEVn(e)));
		// i - индекс полуребра, имеющего ТР, если такое есть
		// j - второго полуребра
		int i = e;
		int j = DCEL.PairedHalfEdge(i);
		if (SignPointsInd[i] == -1 && j != -1)
			if (SignPointsInd[j] != -1)
			{// i <-> j
				auto buf = i; i = j; j = buf;
			}
		if (j == -1)
		{
			if (SignPointsInd[i] == -1)
				out.push_back({ SP, EP });
			else
			{
				out.push_back({ SP, NewPoints[SignPointsInd[i]] });//отрезок до смены знака
				out.push_back({ EP, NewPoints[SignPointsInd[i]] });//отрезок после смены знака
			}
		}
		else
		{// both half edges exist
			if (SignPointsInd[i] == -1)
			{ // sign is constant
				BPoint Ni = DCEL.GetNormByNum(Faces[Edges[i][2]][1]);
				BPoint Nj = DCEL.GetNormByNum(Faces[Edges[j][2]][1]);
				BPoint Vs = GetVel(SP);
				BPoint Ve = GetVel(EP);
				if (!Vs.IsVector())
					Vs = Ve;
				else if (!Ve.IsVector())
					Ve = Vs;
				if (!Vs.IsVector()) // both vectors are invalid
				{
					out.push_back({ SP, EP });
				}
				else
				{
					const double Signis = Vs * Ni;
					const double Signjs = Vs * Nj;
					const double Signie = Ve * Ni;
					const double Signje = Ve * Nj;
					const double Signi = fabs(Signis) > fabs(Signie) ? Signis : Signie;
					const double Signj = fabs(Signjs) > fabs(Signje) ? Signjs : Signje;
					if (fabs(Signi) < epsilonf || fabs(Signj) < epsilonf)
						out.push_back({ SP, EP });
					else if (Signi * Signj < 0.)
						out.push_back({ SP, EP });
				}
			}
			else
			{ // sign changes
				if (SignPointsInd[j] == -1)
				{
					out.push_back({ SP, NewPoints[SignPointsInd[i]] });//отрезок до смены знака
					out.push_back({ EP, NewPoints[SignPointsInd[i]] });//отрезок после смены знака
					continue;
				}
				// 
				int k = i; // index of first point of change sign (nearest to SP)
				int l = j; // index of other point (nearest to EP)
				if ((NewPoints[SignPointsInd[k]] - SP).D2() > (NewPoints[SignPointsInd[l]] - SP).D2())
				{
					auto buf = k; k = l; l = buf;
				}
				BPoint Nk = DCEL.GetNormByNum(Faces[Edges[k][2]][1]);
				BPoint Nl = DCEL.GetNormByNum(Faces[Edges[l][2]][1]);
				BPoint Vs = GetVel(SP);
				BPoint Ve = GetVel(EP);
				if (!Vs.IsVector() || !Ve.IsVector())
					continue;// не может быть смены знака на таком ребре (одна из вершин на оси вращения)
				double Signk = Vs * Nk;
				double Signl = Vs * Nl;
				if (Signk * Signl < 0.)
				{
					out.push_back({ SP, NewPoints[SignPointsInd[k]] });//отрезок до первой смены знака
					out.push_back({ EP, NewPoints[SignPointsInd[l]] });//отрезок после второй смены знака
				}
				else
					out.push_back({ NewPoints[SignPointsInd[k]], NewPoints[SignPointsInd[l]] });//отрезок между двумя нулевыми точками на ребре
			}
		}
	}
	for (int i = 0; i < Faces.size(); i++)//перебор по граням
	{
		if (DCEL.IsDummyFace(i))
			continue;
		if (DCEL.IsMarkedFace(i))
			continue;
		BPoint norm = DCEL.GetNormByNum(Faces[i][1]);//нормаль грани
		int tenpind = Edges[Faces[i][0]][1];//Дорисовка отрезков при условии, что горизонатльная грань пересекает ось
		if (tenpind < 0)
			continue;
		ClrPoint FacePoint1(DCEL.GetClrPByVn(DCEL.GetESVn(tenpind)));
		ClrPoint FacePoint2(DCEL.GetClrPByVn(DCEL.GetEEVn(tenpind)));;
		bool FrontFace = false;
		if (FacePoint1.GetX() != FacePoint2.GetX() || FacePoint1.GetY() != FacePoint2.GetY() || FacePoint1.GetZ() != FacePoint2.GetZ())
		{
			tenpind = Edges[tenpind][1];
			BPoint FacePoint3 = { DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[0],DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[1],DCEL.Get3fvByVn(DCEL.GetESVn(Edges[tenpind][1]))[2] };
			if (FacePoint3 == FacePoint1 || FacePoint3 == FacePoint2)
			{
				FacePoint3 = { DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[0],DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[1],DCEL.Get3fvByVn(DCEL.GetEEVn(Edges[tenpind][1]))[2] };
			}
			BPoint Ve1(-FacePoint1[0], -FacePoint1[1], 0);
			if (Ve1[1] != 0 || Ve1[0] != 0)
				Normalize(Ve1);
			BPoint Ve2(-FacePoint2[0], -FacePoint2[1], 0);
			if (Ve2[1] != 0 || Ve2[0] != 0)
				Normalize(Ve2);
			BPoint Ve3(-FacePoint3[0], -FacePoint3[1], 0);
			if (Ve3[1] != 0 || Ve3[0] != 0)
				Normalize(Ve3);
			if (((Ve1[0] * norm[0]) + (Ve1[1] * norm[1])) >= epsilonf ||
				((Ve2[0] * norm[0]) + (Ve2[1] * norm[1])) >= epsilonf ||
				((Ve3[0] * norm[0]) + (Ve3[1] * norm[1])) >= epsilonf)
				FrontFace = true;
			if (FacePoint3.GetZ() == FacePoint1.GetZ() && FacePoint3.GetZ() == FacePoint2.GetZ())
			{
				ClrPoint ZeroPoint(BPoint(0., 0., FacePoint3.GetZ(), 1.));
				ZeroPoint.SetColor(FacePoint1.GetColor());
				double alpha = ((FacePoint2.GetY() - FacePoint3.GetY()) * (ZeroPoint.GetX() - FacePoint3.GetX()) + (FacePoint3.GetX() - FacePoint2.GetX()) * (ZeroPoint.GetY() - FacePoint3.GetY())) /
					((FacePoint2.GetY() - FacePoint3.GetY()) * (FacePoint1.GetX() - FacePoint3.GetX()) + (FacePoint3.GetX() - FacePoint2.GetX()) * (FacePoint1.GetY() - FacePoint3.GetY()));
				double beta = ((FacePoint3.GetY() - FacePoint1.GetY()) * (ZeroPoint.GetX() - FacePoint3.GetX()) + (FacePoint1.GetX() - FacePoint3.GetX()) * (ZeroPoint.GetY() - FacePoint3.GetY())) /
					((FacePoint2.GetY() - FacePoint3.GetY()) * (FacePoint1.GetX() - FacePoint3.GetX()) + (FacePoint3.GetX() - FacePoint2.GetX()) * (FacePoint1.GetY() - FacePoint3.GetY()));
				double gamma = 1.0 - alpha - beta;
				if (alpha >= 0 && beta >= 0 && gamma >= 0)
				{
					out.push_back({ FacePoint1 ,ZeroPoint });
				}
			}
		}
		bool KP1 = false; bool KP2 = false;//переменные, обозначающеи наличие точек в рамках грани, лежащих на ребре(точки со сменой знака)
		ClrPoint NE1(BPoint(0., 0., 0., 1.));
		ClrPoint NE2(BPoint(0., 0., 0., 1.));
		int ECS = -1; // edge of change sign
		for (int EI = (Edges[Faces[i][0]][1]); EI >= 0; EI = (Edges[EI][1]))//перебор по ребрам в грани, перебор ребер в грани нужен для соединения найденных точек в которых меняется знак
		{

			const ClrPoint SP(DCEL.Get3fvByVn(DCEL.GetESVn(EI)));//координаты первой точки отрезка
			const ClrPoint EP(DCEL.Get3fvByVn(DCEL.GetEEVn(EI)));
			BPoint V1(GetVel(SP));//вектор скорости
			BPoint V2(GetVel(EP));
			const double RezultS = norm * V1;//произведение вектора скорости на нормаль 
			const double RezultE = norm * V2;
			int PairedEI = DCEL.PairedHalfEdge(EI);
			if (PairedEI >= 0)
			{
				BPoint innorm = DCEL.GetNormByNum(Faces[Edges[PairedEI][2]][1]);//нормаль смежной отрезку грани
				const double inRezultS = (innorm[0] * V1[0] + innorm[1] * V1[1] + innorm[2] * 0);//произведение вектора скорости на смежную отрезку нормаль
				const double inRezultE = (innorm[0] * V2[0] + innorm[1] * V2[1] + innorm[2] * 0);
				if (FrontFace &&
					(RezultS >= epsilonf &&//и произведение вектора скорости в первой точки отрезка на нормаль положительна
						RezultE <= -epsilonf) //а во второй точке отрезка произведение отрицательно
					||
					(RezultS <= -epsilonf &&
						RezultE >= epsilonf)
					)
				{
					bool& KP = KP1 ? KP2 : KP1;
					ClrPoint& NE = KP1 ? NE2 : NE1;
					KP = false;
					if (SignPointsInd[EI] >= 0)
					{
						NE = NewPoints[SignPointsInd[EI]];//находим пересечение отрезка и плоскости, проходящей через ось Z и нормаль грани
						KP = true;
						ECS = EI;
					}
				}
			}
			if (EI == Faces[i][0])
				break;
		}
		if (KP1 && KP2)// если найдены обе точки на грани в которых меняется произведение, забираем отрезок в выходной ммассив
		{
			out.push_back({ NE1,NE2 });
			KP1 = false; KP2 = false;
		}
		else if (KP1 || KP2)
		{
			auto Eind = (Edges[ECS][1]);
			const ClrPoint P(DCEL.GetClrPByVn(DCEL.GetEEVn(Eind)));// take third point of this triangle
			if (KP1)
				out.push_back({ NE1, P });
			else
				out.push_back({ NE2, P });
		}

	}
	std::vector<ClrPoint> Pts;
	auto Num = DCEL.RayShreddingTri(BPoint(0., 0., -1.e6, 1.), BPoint(0., 0., 1., 0.), 1.e-6, Pts);
	for (int i = 1; i < Num; i += 2)
	{
		out.push_back({ Pts[i - 1], Pts[i] });
	}
}
void BRotSolid::Sled() //построение следа вращения по исходным отрезкам
{
	sled.resize(out.size()); //массив с точками следа вращения			
	for (int k = 0; k < out.size(); k++)
	{
		int Aff, Bff;
		//точка 1 всегда по координате Z больше точки 2
		if (out[k][0].GetZ() > out[k][1].GetZ())
		{
			Aff = 0; Bff = 1;
		}
		else
		{
			Aff = 1; Bff = 0;
		}
		const ClrPoint& Dot_1 = out[k][Aff]; //исходная точка 1
		const ClrPoint& Dot_2 = out[k][Bff]; //исходная точка 2

		///////
		//находим координаты точки с минимальным растоянием до оси z
		////		
		//направляющий вектор по двум исходным точкам 
		BPoint V_original(Dot_2 - Dot_1);
		//векторное произведения единичного вектора оси и вектора исходных точек	
		BPoint V_crossProduct{ V_original[1], -V_original[0], 0., 0. }; //yV*1-z0*0  //-(xV*1-zV*0)   //xV*0-yV*0
		//единичный вектор оси z
		BPoint V_axisZ(0., 0., 1., 0.);
		//координаты точки с минимальным растоянием до оси z
		BPoint D_Near = crossroad(V_axisZ, V_crossProduct, Dot_1, Dot_2);

		///////
		//определение значений параметров
		////
		//находим параметр b (значение координаты y для ближайшей точки следа)          
		const double b = sqrt(D_Near[0] * D_Near[0] + D_Near[1] * D_Near[1]);//координата Y следа для экстремума гиперболы
		//расстояние от точки 1 и 2 до оси z
		const double rass1 = sqrt((Dot_1[0] * Dot_1[0]) + (Dot_1[1] * Dot_1[1]));//координаты Y следа для исходных точек 1 и 2
		const double rass2 = sqrt((Dot_2[0] * Dot_2[0]) + (Dot_2[1] * Dot_2[1]));

		////////                
		// заполнение массива с точками следа / начало
		////

		if (((D_Near[0] == 0) && (D_Near[1] == 0)) || (Dot_1[2] == Dot_2[2])
			|| ((Dot_1[0] == Dot_2[0]) && (Dot_1[1] == Dot_2[1])))
		{

			//если кратчайшее растояние от следа до оси z равно нулю
			//то след будет прямой линией, при пересечении оси как две прямые	
			if (D_Near[2] <= Dot_1[2] && Dot_2[2] <= D_Near[2] && Dot_1[2] != Dot_2[2])
			{
				AllPoints.push_back(BPoint(0., rass1, Dot_1[2], 1.));
				AllPoints.back().SetColor(Dot_1.GetColor());
				sled[k].push_back(int(AllPoints.size() - 1));
				AllPoints.push_back(BPoint(0., b, D_Near[2], 1.));
				AllPoints.back().SetColor(Dot_1.GetColor());
				sled[k].push_back(int(AllPoints.size() - 1));
				AllPoints.push_back(BPoint(0., rass2, Dot_2[2], 1.));
				AllPoints.back().SetColor(Dot_1.GetColor());
				sled[k].push_back(int(AllPoints.size() - 1));
			}
			else
			{
				AllPoints.push_back(BPoint(0., rass1, Dot_1[2], 1.));
				AllPoints.back().SetColor(Dot_1.GetColor());
				sled[k].push_back(int(AllPoints.size() - 1));
				AllPoints.push_back(BPoint(0., rass2, Dot_2[2], 1.));
				AllPoints.back().SetColor(Dot_1.GetColor());
				sled[k].push_back(int(AllPoints.size() - 1));
			}
		}

		//////
		//определение точек гиперболы
		else
		{
			//назначение границ по оси y для следа
			double dia_y1, dia_y2; //диапазон значений расстояний следа
			if (rass2 >= rass1)
			{
				dia_y2 = rass2; dia_y1 = -rass2;
			}
			else
			{
				dia_y2 = rass1; dia_y1 = -rass1;
			}

			//разделим участок от dia_y1 до dia_y2 на равные промежутки
			int n = 200;
			double deg = abs(dia_y1 - dia_y2) / n;

			double c; //параметр из формулы второго порядка для построения гиперболы следа
			if ((int)b != (int)rass1)
			{
				c = (b * (Dot_1[2] - D_Near[2])) / (sqrt(abs(rass1 * rass1 - b * b)));
			}
			else
			{
				c = (b * (Dot_2[2] - D_Near[2])) / (sqrt(abs(rass2 * rass2 - b * b)));
			}

			//первая точка гиперболы, с минимальным значением z					
			AllPoints.push_back(BPoint(0., rass1, Dot_1[2], 1.));
			AllPoints.back().SetColor(Dot_1.GetColor());
			sled[k].push_back(int(AllPoints.size() - 1));

			//нахождение точек для построения гиперболы положительной части
			for (int i = 1; i < n + 1; i++)
			{
				double Y = dia_y2 - (deg * i);
				if (Y >= 0)
				{
					double Z = (sqrt(-(c * c) * (b * b - Y * Y))) / b; //уравнение гиперболы
					Z = Z + D_Near[2];

					if (Z <= Dot_1[2] && Dot_2[2] <= Z)
					{
						AllPoints.push_back(BPoint(0., Y, Z, 1.));
						AllPoints.back().SetColor(Dot_1.GetColor());
						sled[k].push_back(int(AllPoints.size() - 1));
					}
				}
			}

			//нахождение точки горлового эллипса 
			double Y = b; double Z = D_Near[2];
			if ((Dot_1[2] > Z) && (Dot_2[2] < Z))
			{
				AllPoints.push_back(BPoint(0., Y, Z, 1.));
				AllPoints.back().SetColor(Dot_1.GetColor());
				sled[k].push_back(int(AllPoints.size() - 1));
			}

			//нахождение точек для построения гиперболы отрицательной части
			for (int i = 1; i < n + 1; i++)
			{
				Y = dia_y1 + (deg * i);
				if (Y >= 0)
				{
					Z = sqrt(((c * c * Y * Y) / (b * b)) - (c * c)); //уравнение гиперболы
					Z = -Z + D_Near[2];

					if (Z <= Dot_1[2] && Dot_2[2] <= Z)
					{
						AllPoints.push_back(BPoint(0., Y, Z, 1.));
						AllPoints.back().SetColor(Dot_1.GetColor());
						sled[k].push_back(int(AllPoints.size() - 1));
					}
				}
			}

			//последняя точка гиперболы, с минимальным значением z
			AllPoints.push_back(BPoint(0., rass2, Dot_2[2], 1.));
			AllPoints.back().SetColor(Dot_1.GetColor());
			sled[k].push_back(int(AllPoints.size() - 1));
			////
			//заполнение массива с точками следа / конец
		}
	}
}


void BRotSolid::IndexPointSled() //индексация точек следа ///////////////////////////////////////////
{
	/// 
	/// Индексация точек следа
	/// 
	std::vector<int> EqNums;
	EqNums.resize(AllPoints.size());
	for (int k = 0; k < AllPoints.size(); ++k)
		EqNums[k] = -1;

	BDCEL::IdentifyPointsOc(AllPoints, EqNums);

	// set new indexes
	for (int m = 0; m < sled.size(); m++) //для каждого участка
		for (int mi = 0; mi < sled[m].size(); ++mi)
			sled[m][mi] = EqNums[sled[m][mi]];

	//запись кривых как отрезки в один массив с отрезками
	//запись всех точек из массива кривых для отрисовки
	for (int u = 0; u < sled.size(); u++) //для каждого участка
	{
		for (int p = 0; p < sled[u].size() - 1; p++) //для каждой точки участка
		{
			lene temp;
			temp.start = sled[u][p];
			temp.end = sled[u][p + 1];
			Lines.push_back(temp);
		}
	}
}

bool BRotSolid::CompareZ(int i0, int i1)
{
	return AllPoints.at(i0).GetZ() > AllPoints.at(i1).GetZ();
}

void BRotSolid::ZAxisLines(bool IgnoreVoids)
{
	std::vector<int> inds;
	for each(auto el in Lines)
	{
		if (AllPoints.at(el.start).GetY() < MIND)
			inds.push_back(el.start);
		if (AllPoints.at(el.end).GetY() < MIND)
			inds.push_back(el.end);
	}
	if (!inds.empty())
	{
		std::sort(inds.begin(), inds.end());
		// remove duplicate numbers
		std::vector<int> indsClear;
		indsClear.push_back(inds.at(0));
		int j = 0;
		for (int i = 1; i < inds.size(); ++i)
		{
			if (indsClear.at(j) != inds.at(i))
			{
				indsClear.push_back(inds.at(i));
				j++;
			}
		}
		std::sort(indsClear.begin(), indsClear.end(), [this](int l, int r) {return CompareZ(l, r); });
		if (IgnoreVoids)
		{
			lene temp;
			temp.start = indsClear.front();
			temp.end = indsClear.back();
			Lines.push_back(temp);
		}
		else
		{
			for (int i = 0; i < indsClear.size(); i += 2)
			{
				lene temp;
				temp.start = indsClear.at(i);
				temp.end = indsClear.at(i + 1);
				Lines.push_back(temp);
			}
		}
	}
}

double BRotSolid::LineArea(const BClrContour& Cont)
{
	if (Cont.size() < 4)
		return 0.;
	const BPoint& B = Cont.front();
	auto it = Cont.begin();
	BPoint Prev = *(++it);
	Prev -= B;
	BPoint Sv(0., 0., 0., 0.);
	for (++it; it != Cont.end(); ++it)
	{
		BPoint Cur = *it;
		Cur -= B;
		Sv += Prev % Cur;
		Prev = Cur;
	}
	return Sv.D2() * 0.25;
}

void BRotSolid::PlaceXY(BClrContour& Cont)
{
	for (auto it = Cont.begin(); it != Cont.end(); ++it)
	{
		double b = it->GetZ();
		it->SetX(it->GetY());
		it->SetZ(0.);
		it->SetY(b);
	}
	Reverse(Cont);
}

void BRotSolid::Reverse(BClrContour& Cont)
{
	if (Cont.size() < 2)
		return;
	for (auto itn = Cont.rbegin(), it = itn++; itn != Cont.rend(); ++it, ++itn)
		it->SetColor(itn->GetColor());// it - current, itn - next
	Cont.reverse();
}

BPoint BRotSolid::crossroad(const BPoint& vec1, const BPoint& vec2, const BPoint& SPoint, const BPoint& EPoint)
{   //поиск пересечения плоскости и отрезка 
	//для нахождения близжайшей точки прямой к оси
	BPoint N = vec1 % vec2;
	BPoint V(-SPoint[0], -SPoint[1], -SPoint[2], 0.);
	double d = N * V;
	BPoint W = EPoint - SPoint;
	double e = N * W;
	BPoint rv = SPoint + W * (d / e);
	return rv;
}

ClrPoint BRotSolid::intersection3d(const BPoint& vec1, const BPoint& vec2, const ClrPoint& SPoint, const ClrPoint& EPoint)
{   //поиск пересечения плоскости и отрезка 
	BPoint N = vec1 % vec2;
	Normalize(N);
	BPoint V(-SPoint[0],-SPoint[1],-SPoint[2], 0.);
	const double d = N * V;
	BPoint W = EPoint - SPoint;
	const double e = N * W;
	if (e == 0)
		return ClrPoint(BPoint( INFINITY,INFINITY,INFINITY, -1.));
	const double de = d / e;
	ClrPoint rv = SPoint + W * de;
	if (((SPoint[0] <= rv[0] && rv[0] <= EPoint[0]) || (SPoint[0] >= rv[0] && rv[0] >= EPoint[0])) &&
		((SPoint[1] <= rv[1] && rv[1] <= EPoint[1]) || (SPoint[1] >= rv[1] && rv[1] >= EPoint[1])) &&
		((SPoint[2] <= rv[2] && rv[2] <= EPoint[2]) || (SPoint[2] >= rv[2] && rv[2] >= EPoint[2]))
		)
	{
		rv.SetColor(SPoint.GetColor());
		return rv;
	}
	else return ClrPoint(BPoint(INFINITY, INFINITY, INFINITY, -1.));
}

bool BRotSolid::CrKontour(BClrContour& Contour)
{
	// returns false if Contour is empty or has dummy elements
	// Create first closed contour
	BClrContour Contour1;
	if (!CrEnvelope(AllPoints, Lines, Contour))
		return false;
	for (int i = 0; i < AllPoints.size(); ++i)
		AllPoints.at(i).ChYSign();
	if (!CrEnvelope(AllPoints, Lines, Contour1))
		return false;
	for (int i = 0; i < AllPoints.size(); ++i)
		AllPoints.at(i).ChYSign();
	Reverse(Contour1);
	for (auto it = Contour1.begin(); it != Contour1.end(); ++it)
	{
		it->ChYSign();
		Contour.push_back(*it);
	}

	if (Contour.empty())
		return false;
	Contour.push_back(*Contour.begin());
	// END:Create first closed contour
	bool Gaps = MarkGaps(Contour);
	if (Gaps)
	{
		auto k0 = Contour.begin();
		auto k1 = k0;
		for (k1++; k1 != Contour.end(); ++k0, ++k1)
		{// for each vertical "gap"
			if (k0->GetH() > 0.5)// gap is marked by H < 0
				continue;
			// Gap found
			const double LocRotAngle = -k0->GetH();
			RotatePoints(AllPoints, LocRotAngle);
			BClrContour ContourBuf;
			ContourBuf.push_back(*k0);
			ContourBuf.push_back(*k1);
			RotateCont(ContourBuf, LocRotAngle);
			const double GapXmin = ContourBuf.front().GetZ();
			const double GapXmax = ContourBuf.back().GetZ();
			const double GapYmin = ContourBuf.front().GetY();
			if (!CrPartEnvelope(AllPoints, Lines, Contour1, GapXmin, GapXmax, GapYmin))
				return false;
			if (Contour1.size() > 1)
			{
				MarkGaps(Contour1);
				RotateCont(Contour1, -LocRotAngle);
				Contour1.back().SetColor(k1->GetColor());
				Contour.insert(k1, Contour1.begin(), Contour1.end());
				auto buf = k0;
				++buf;
				Contour.erase(k0);
				Contour.erase(k1);
				k0 = buf;
				k1 = k0;
				++k1;
			}
			RotatePoints(AllPoints, -LocRotAngle);
		}
	}
	// check rotation axis points
	// "right" points are ordered left to right
	for (auto itNext = Contour.begin(), itPrev = itNext++; 0, itNext != Contour.end(); ++itPrev, ++itNext)
	{
		if (itPrev->GetY() < MIND && itNext->GetY() < MIND)
		{
			if (itPrev->GetZ() > itNext->GetZ())
				return false;
		}

			
	}
	return true;
}

bool BRotSolid::CrRotKontour(const BDCEL& DCEL, bool IgnoreVoids, BClrContour& Contour)
{
	Filter(DCEL, IgnoreVoids);
	Sled();
	IndexPointSled();
	ZAxisLines(IgnoreVoids);
	return CrKontour(Contour);
}

void BRotSolid::Contour2GeomArray(const BClrContour& Contour, NCadrGeomArray& GeomArray)
{
	// Make GeomArray from Contour
	auto k0 = Contour.begin();
	auto k1 = k0;
	for (k1++; k1 != Contour.end(); ++k0, ++k1)
	{
		if ((*k1 - *k0).D2() > MIND * MIND)
		{
			NCadrGeom* pCadr = new NCadrGeom;
			pCadr->Set(line, k0->GetZ(), k0->GetY(), 0., k1->GetZ(), k1->GetY(), 0.);
			GeomArray.Add(pCadr);
		}
	}
}

bool BRotSolid::CrPartEnvelope(const std::vector<ClrPoint>& AllPoints, const std::vector<lene>& Lines, BClrContour& Contour, double Xmin, double Xmax, double Ymin)
{
	std::vector<lene> NewLines;
	std::vector<ClrPoint> NewPoints;
	bool Over = false;
	for each (auto Line in Lines)
	{
		ClrPoint P0 = AllPoints[Line.start];
		ClrPoint P1 = AllPoints[Line.end];
		if (P1.GetZ() == P0.GetZ())
			continue;
		ClipRes Res = BPoint::ClipLine(P0, P1, Xmin, Xmax, Ymin);
		if (Res == ClipRes::CR_OVER)
		{
			Contour.clear();
			Over = true;
			ClrPoint BP(BPoint(0., Ymin, Xmin, 1.), P0.GetColor());
			ClrPoint EP(BPoint(0., Ymin, Xmax, 1.), P0.GetColor());
			Contour.push_front(BP);
			Contour.push_back(EP);
			break;
		}
		if (Res == ClipRes::CR_CLIP || Res == ClipRes::CR_UNCHANGED || Res == ClipRes::CR_OVER)
		{
			lene L;
			L.start = int(NewPoints.size());
			NewPoints.push_back(P0);
			L.end = int(NewPoints.size());
			NewPoints.push_back(P1);
			NewLines.push_back(L);
		}
	}
	if (!Over)
	{
		if (!CrEnvelope(NewPoints, NewLines, Contour))
			return false;

		// Add start and end points if neccessary
		BPoint BP(BPoint(0., Ymin, Xmin, 1.));
		BPoint EP(0., Ymin, Xmax, 1.);
		if (Contour.empty())
		{
			Contour.push_front(BP);
			Contour.push_back(EP);
		}
		else
		{
			if ((BP - Contour.front()).D2() > MIND * MIND)
				Contour.push_front(BP);
			if ((EP - Contour.back()).D2() > MIND * MIND)
				Contour.push_back(EP);
		}
	}
	return true;
}

bool BRotSolid::CrEnvelope(const std::vector<ClrPoint>& AllPoints, const std::vector<lene>& Lines, BClrContour& Contour)
{
	// returns false if Contour has dummy elements and true otherwise
	std::list<Envelope> Queue;
	// place all non vertical lines to queue
	for each (auto Line in Lines)
	{
		const ClrPoint* pP0 = &AllPoints[Line.start];
		const ClrPoint* pP1 = &AllPoints[Line.end];
		if (pP0->GetZ() == pP1->GetZ())
			continue;
		Queue.emplace_back();
		NVLine* pLine = new NVLine(pP0->GetZ(), pP0->GetY(), pP1->GetZ(), pP1->GetY(), pP0->GetColor());
		Queue.back().List.push_back(std::pair<NVLine*, NVLine*>(nullptr, pLine));
		Queue.back().List.push_back(std::pair<NVLine*, NVLine*>(pLine, nullptr));
	}
	Contour.clear();
	// Merge all envelopes
	if (Queue.empty())
		return true;
	while (&Queue.front() != &Queue.back())
	{
		Queue.emplace_front();
		reverse_iterator rit = Queue.rbegin();
		reverse_iterator last = rit;
		++rit;
		Envelope::Merge(*last, *rit, Queue.front());
		last->FreeMem();
		rit->FreeMem();
		Queue.pop_back();
		Queue.pop_back();
	}
	// Fill Contour
	bool HasDummy = false;
	for each (auto Node in Queue.back().List)
	{
		auto pPf = Node.first;
		if (pPf != nullptr)
		{
			if (pPf->Dummy)
				HasDummy = true;
			Contour.push_back(ClrPoint(BPoint(0., pPf->y0, pPf->x0, 1.), pPf->Color));
			auto pPs = Node.second;
			if(pPs == nullptr)
				Contour.push_back(ClrPoint(BPoint(0., pPf->y1, pPf->x1, 1.), pPf->Color));
			else if(pPs->y0 != pPf->y1)
				Contour.push_back(ClrPoint(BPoint(0., pPf->y1, pPf->x1, 1.), pPf->Color));
		}
	}
	Queue.back().FreeMem();
	return !HasDummy;
}

bool BRotSolid::MarkGaps(BClrContour& Contour)
{
	bool Res = false;
	bool PreviousMarked = false;
	for (auto k1 = Contour.begin(), k0 = k1++; k1 != Contour.end(); ++k0, ++k1)
	{
		if (fabs(k0->GetZ() - k1->GetZ()) > MIND)
		{
			PreviousMarked = false;
			continue;
		}
		// gap found
		Res = true;
		if (PreviousMarked)
		{// erase first (k0 is not begin)
			auto b = k0;
			k0--;
			Contour.erase(b);
		}
		else
		{// mark
			k0->SetH((k0->GetY() < k1->GetY()) ? -270. : -90.);
		}
		PreviousMarked = true;
	}
	// remove marks on short gaps
	for (auto k1 = Contour.begin(), k0 = k1++; k1 != Contour.end(); ++k0, ++k1)
	{
		if (k0->GetH() > 0.5)
			continue;
		// marked
		if (fabs(k0->GetY() - k1->GetY()) < MIND * 10000)
			k0->SetH(1.);// remove mark
	}
	return Res;
}

bool BRotSolid::RotateCont(BClrContour& Contour, double Angle)
{
	// Angle may be 0, -90, -180, -270
	BMatr Rot;
	Rot.RotX(0., 0., 0., double(-Angle));

	for (auto it = Contour.begin(); it != Contour.end(); ++it)
	{
		if (it->GetH() < 0.) // gap
		{
			double NewAngle = -it->GetH() - Angle;
			if (NewAngle >= 360)
				NewAngle -= 360.;
			it->SetH( -NewAngle);
		}
		*it *= Rot;
	}
	return true;
}

bool BRotSolid::RotatePoints(std::vector<ClrPoint>& AllPoints, double Angle)
{
	// Angle may be 90, -90, 180
	BMatr Rot;
	Rot.RotX(0., 0., 0., double(-Angle));

	for (int i = 0; i < AllPoints.size(); ++i)
		AllPoints.at(i) *= Rot;
	return true;
}

void BRotSolid::Normalize(BPoint& N)
{
	double mlr;
	mlr = sqrt(pow(N[0], 2) + pow(N[1], 2) + pow(N[2], 2));
	N[0] = N[0] / mlr;
	N[1] = N[1] / mlr;
	N[2] = N[2] / mlr;
}

BRotSolid::NVLine::NVLine(double xS, double yS, double xE, double yE, PackedColor Col)
{
	if (xS < xE)
	{
		x0 = xS;
		y0 = yS;
		x1 = xE;
		y1 = yE;
	}
	else
	{
		x1 = xS;
		y1 = yS;
		x0 = xE;
		y0 = yE;
	}
	Dummy = y0 == Y_DUMMY;
	Color = Col;
}

BRotSolid::NVLine::NVLine(double xS, double xE)
{
	if (xS < xE)
	{
		x0 = xS;
		x1 = xE;
	}
	else
	{
		x1 = xS;
		x0 = xE;
	}
	y0 = Y_DUMMY;
	y1 = Y_DUMMY;
	Dummy = true;
	Color.ival = 0;
}

void BRotSolid::NVLine::SetStart(double xS, double yS)
{
	x0 = xS;
	y0 = yS;
}

void BRotSolid::NVLine::SetEnd(double xE, double yE)
{
	x1 = xE;
	y1 = yE;
}

double BRotSolid::NVLine::GetY(double X) const
{
	if (Dummy)
		return y0;
	if (X == x0)
		return y0;
	if (X == x1)
		return y1;
	double dX = x1 - x0;
	if (dX == 0.)
		return y0;
	double t = (X - x0) / dX;
	double y = y0 * (1. - t) + y1 * t;
	double ymin = fmin(y0, y1);
	double ymax = fmax(y0, y1);
	if (y < ymin)
		return ymin;
	if (y > ymax)
		return ymax;
	return y;
}

bool BRotSolid::Envelope::Merge(Envelope& In0, Envelope& In1, Envelope& Out)
{
	// A. add first and last lines if needed
	double Xstart = min(In0.List.front().second->x0, In1.List.front().second->x0);
	double Xend = max(In0.List.back().first->x1, In1.List.back().first->x1);
	In0.Expand(Xstart, Xend);
	In1.Expand(Xstart, Xend);
	// B. Find active in Xstart
	Envelope* pActiveEnv = nullptr;
	Envelope* pPassiveEnv = nullptr;
	if (In0.List.front().second->y0 > In1.List.front().second->y0)
	{
		pActiveEnv = &In1;
		pPassiveEnv = &In0;
	}
	else if (In0.List.front().second->y0 < In1.List.front().second->y0)
	{
		pActiveEnv = &In0;
		pPassiveEnv = &In1;
	}
	else // is equal
	{
		double Tan0 = (In0.List.front().second->y1 - In0.List.front().second->y0) / (In0.List.front().second->x1 - In0.List.front().second->x0);
		double Tan1 = (In1.List.front().second->y1 - In1.List.front().second->y0) / (In1.List.front().second->x1 - In1.List.front().second->x0);
		pActiveEnv = Tan0 > Tan1 ? &In1 : &In0;
		pPassiveEnv = Tan0 <= Tan1 ? &In1 : &In0;
	}
	// C. Set start as current
	auto ActIt = pActiveEnv->List.begin();
	auto PasIt = pPassiveEnv->List.begin();
	//D. until the end
	double XCur0 = Xstart;
	bool Continue = true;
	while (Continue)
	{
		NVLine* pActNVL = ActIt->second;
		NVLine* pPasNVL = PasIt->second;
		if (pActNVL == nullptr || pPasNVL == nullptr)
		{
			Continue = false;
			continue;
		}
		double Yact0 = pActNVL->GetY(XCur0);
		double Ypas0 = pPasNVL->GetY(XCur0);
		// D.1
		const NVLine* pFound = pActNVL->x1 < pPasNVL->x1 ? pActNVL : pPasNVL;
		// D.2
		const double XCur1 = pFound->x1;
		double Yact = pActNVL->GetY(XCur1);
		double Ypas = pPasNVL->GetY(XCur1);
		bool ChangeAct = false;
		if (Yact0 > Ypas0)
		{
			ChangeAct = true;
		}
		else if (Yact0 == Ypas0)
		{
			if (Yact > Ypas)
			{
				ChangeAct = true;
			}
		}
		if (ChangeAct)
		{
			if(pActNVL->x0 != XCur0)
				Out.Add(new NVLine(pActNVL->x0, pActNVL->y0, XCur0, Yact0, pActNVL->Color));
			pPasNVL->x0 = XCur0;
			pPasNVL->y0 = Ypas0;
			auto It = ActIt; ActIt = PasIt; PasIt = It;
			pActNVL = ActIt->second;
			pPasNVL = PasIt->second;
			double buf = Yact0; Yact0 = Ypas0; Ypas0 = buf;
			buf = Yact; Yact = Ypas; Ypas = buf;
		}
		// D.3
		if (Ypas < Yact)
		{
			// D.3.A
			const double d0 = Ypas0 - Yact0;
			if (d0 <= 0.)
				return false;// internal error
			const double t = d0 / (d0 + Yact - Ypas);
			const double X = XCur0 * (1. - t) + XCur1 * t;
			const double Y = pActNVL->GetY(X);
			// D.3.B
			Out.Add(new NVLine(pActNVL->x0, pActNVL->y0, X, Y, pActNVL->Color));
			// D.3.D change passive start point to IP 
			pPasNVL->x0 = X;
			pPasNVL->y0 = Y;
			// D.3.C Change active envelope
			auto buf = pActNVL; pActNVL = pPasNVL; pPasNVL = buf;
			auto It = ActIt; ActIt = PasIt; PasIt = It;
		}
		// D.4
		if (pActNVL->x1 == XCur1)
		{// active line ended
			Out.Add(pActNVL);
			ActIt->second = nullptr;// to clear memory at the end
			ActIt++;
			pActNVL = ActIt->second;
		}
		// D.5
		if (pPasNVL->x1 == XCur1)
		{// passive line ended
			PasIt++;
			pPasNVL = PasIt->second;
		}
		XCur0 = XCur1;
	}

	return true;
}

void BRotSolid::Envelope::Expand(double Xstart, double Xend)
{
	if (List.front().second->x0 != Xstart)
	{
		NVLine* pNew = new NVLine(Xstart, List.front().second->x0);// Dummy
		List.front().first = pNew;
		List.push_front(std::pair<NVLine*, NVLine*>(nullptr, pNew));
	}
	if (List.back().first->x1 != Xend)
	{
		NVLine* pNew = new NVLine(Xend, List.back().first->x1);// Dummy
		List.back().second = pNew;
		List.push_back(std::pair<NVLine*, NVLine*>(pNew, nullptr));
	}
}

void BRotSolid::Envelope::Add(NVLine* pLine)
{
	if (List.empty())
	{
		List.emplace_back();
		List.back().first = nullptr;
	}
	List.back().second = pLine;
	List.emplace_back();
	List.back().first = pLine;
	List.back().second = nullptr;
}

void BRotSolid::Envelope::FreeMem()
{
	for each (auto Node in List)
	{
		delete Node.second;
	}
}

bool BRotSolid::Make1EnvCont(const BDCEL& DCEL, bool IgnoreVoids, std::list<BClrContour>& Contours)
{
	BRotSolid RotSolid;
	bool res = true;
	Contours.emplace_back();
	if (RotSolid.CrRotKontour(DCEL, IgnoreVoids, Contours.back()))
	{
		if (Contours.back().size() > 2)
		{
			BRotSolid::PlaceXY(Contours.back());
			double S = BRotSolid::LineArea(Contours.back());
			if (S < 1.e-2)
				Contours.pop_back();
		}
	}
	else
	{
		Contours.pop_back();
		res = false;
	}

	return res;
}

bool BRotSolid::MakeEnvConts(BDCEL& DCEL, std::list<BClrContour>& Contours, TurnGenMode TGMode)
{
	bool res = true;
	if (TGMode == TGM_CONLY)
	{
		res = Make1EnvCont(DCEL, false, Contours);
	}
	else if (TGMode == TGM_FULL)
	{
		std::vector<int> ExtrEdges;
		for (int FreeEdge = DCEL.FindUnmarkedEdge(); FreeEdge >= 0; FreeEdge = DCEL.FindUnmarkedEdge())
		{
			DCEL.MarkConnected(FreeEdge);
			ExtrEdges.push_back(FreeEdge);
		}
		DCEL.UnmarkAll();
		for each (int ExtremeEdge in ExtrEdges)
		{
			DCEL.MarkConnected(ExtremeEdge);
			DCEL.InvertMarks();
			res &= Make1EnvCont(DCEL, false, Contours);
			DCEL.UnmarkAll();
		}
	}
	return res;
}

