#include <stdlib.h>
#include "stdafx.h"
#include "ConstDef.h"
#include "GrazingCurveElemContour.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const BPoint iv(1.0, 0.0, 0.0, 0.0);
const BPoint jv(0.0, 1.0, 0.0, 0.0);
const BPoint kv(0.0, 0.0, 1.0, 0.0);

int compare(const void * a, const void * b)
{
	BPoint	A = *((BPoint *)a), 
			B = *((BPoint *)b);

	double vA = A.GetY();
	double vB = B.GetY();

	double vAA = A.GetX();
	double vBB = B.GetX();

	if(vAA <= 0 && vBB <= 0)// левая полуплоскость
	{
		if(vA < vB)	// A ниже чем B по оси Oy
			return -1;//A < B
		return 1;//A > B	// A выше чем B по оси Oy
	}
	if(vAA > 0 && vBB > 0)// правая полуплоскость
	{
		if(vA < vB)	// A ниже чем B по оси Oy
			return 1;//A > B
		return -1;//A < B	// А выше чем В по оси Oy
	}
	if(vAA < 0 && vBB > 0)//A - слева, В - справа
		return -1;//A < B
	if(vAA > 0 && vBB <= 0)//В - слева, А - справа
		return 1;//A > B
	return 0;//A == B
}

GrazingCurveElemContour::GrazingCurveElemContour(void)
{
	connect = NOT;
}
bool GrazingCurveElemContour::SetSegment(unsigned int i, const SegmentGrazingCurve segment)
{
	if(i < 0 || i > 7)
		return false;
	if(i==0)
		sgcleft.Copy(segment);
	if(i==1)
		sgcright.Copy(segment);
	if(i==2)
		sgcleft1.Copy(segment);
	if(i==3)
		sgcright1.Copy(segment);
	if(i==4)
		sgcldown.Copy(segment);
	if(i==5)
		sgcrdown.Copy(segment);
	if(i==6)
		sgclup.Copy(segment);
	if(i==7)
		sgcrup.Copy(segment);
	return true;

}
GrazingCurveElemContour::~GrazingCurveElemContour(void)
{
}

BPoint GrazingCurveElemContour::TraceOfLines(const BPoint &P0, const BPoint &P1, const BPoint &Q0, const BPoint &Q1)
{
	//P0,P1 и Q0,Q1 - концы отрезков, у которых ищется точка пересечения
	double	A1 = (P1-P0).D2(),
			B1 = -((P1-P0)*(Q1-Q0)),
			H1 = (P1-P0)*(Q0-P0),
			A2 = (P1-P0)*(Q1-Q0),
			B2 = -((Q1-Q0).D2()),
			H2 = (Q1-Q0)*(Q0-P0),	
			det = A1 * B2 - B1 * A2,
			det1 = H1 * B2 - B1 * H2,
			det2 = A1 * H2 - H1 * A2;
	BPoint S(0., 0., 0., 1.);
	if(det*det > MIND*MIND*MIND*MIND)
	{
		double t1 = det1 / det;
		double t2 = det2 / det;
		S.MiddlePoint(P0 + (P1-P0)*t1, Q0 + (Q1-Q0)*t2);
		return S;
	}
	S.MiddlePoint(P0, P1);
	return S;
}

AvailableTrace GrazingCurveElemContour::LinePlane(const BPoint &P, const BPoint &Vector, const BPoint &Q, const BPoint &Normal, BPoint &M)
{
	//Поиск типа и точек пересечения отрезка линии (P,Vector) и плоскости (Q, Normal)
	double d = Vector * Normal; 
	if( d * d < MINAR * MINAR * Vector.D2())
	{
		d = (P - Q) * Normal;
		if( d * d < MINAR * MINAR * (P - Q).D2() )//плоскости совпадают
			return COUNTLESS_TRACE;
		return ABSENCE_TRACE;//Если плоскость параллельна прямой (P,Vector) и совпадения нет
	}
	M = P + Vector * ((Q - P) * Normal/(Vector * Normal));//Знаменатель не проверяем, т.к этот сл.рассм. раньше	
	return ONE_TRACE; 
}

AvailableTrace GrazingCurveElemContour::PlanePlane(const BPoint &P1, const BPoint &Normal1, const BPoint &P2, const BPoint &Normal2, BPoint P[2])
{
	if( (Normal1 % Normal2).D2() < MINAR * MINAR)//Если нормали плоскостей близки к параллельности
	{
		double	d = (P2 - P1) * Normal1;
		if(d * d <= MINAR * MINAR * (P2 - P1).D2())//если точка одной плоскости лежит при этом в другой, то плоскости совпадают
			return COUNTLESS_TRACE;//множество общих точек
		return ABSENCE_TRACE;//плоскости параллельны и не имеют общих точек
	}
	P[1] = Normal1 % Normal2;//направляющий вектор прямой пересечения плоскостей
	P[1] = P[1] * (1/sqrt(P[1].D2()));
	BPoint a = P[1] % Normal1;//вектор от точки P1 в сторону плоскости (P2,Normal2)
	LinePlane(P1, a, P2, Normal2, P[0]);//P[0] - точка пересечения прямой (P1,a) и плоскости (P2,Normal2)
	return ONE_TRACE;//есть одно пеесечение в виде прямой (P[0], P[1])
}

AvailableTrace GrazingCurveElemContour::CirclePlane(const BPoint &O, const BPoint &Normal1, const BPoint &P, const BPoint &Normal2, const double r2, BPoint M[2])
{
	BPoint m[2];
	AvailableTrace f = PlanePlane(O, Normal1, P, Normal2, m);//в 'm' точка и вектор, задающие линию пересечения плоскостей
	switch (f)
	{
	case COUNTLESS_TRACE:
		return COUNTLESS_TRACE;
	case ABSENCE_TRACE:
		return ABSENCE_TRACE;
	case ONE_TRACE:
		BPoint OO1 = (Normal1 % m[1]);
		double d = (m[0] - O) * OO1;//расстояние от центра окр. до линии пересечения
		if(d*d > pow((sqrt(fabs(r2)) + MIND),2))
			return ABSENCE_TRACE;//реально точек пересечения окружности и плоскости нет
		BPoint O1 = O + OO1 * d;

		M[0] = O1 + m[1] * sqrt(fabs(r2 - d*d));//точка левее от плоскости (Normal1,Normal2)
		M[1] = O1 - m[1] * sqrt(fabs(r2 - d*d));//точка правее от плоскости (Normal1,Normal2)
		if( ((d*d - r2) * (d*d - r2)) < MIND * MIND)//окружность касается плоскости
			return ONE_TRACE;
		return TWO_TRACE;
	}
	return TWO_TRACE;
}
void GrazingCurveElemContour::PointsOfSegmentsCircle(const BPoint &P0, const BPoint &a, const BPoint &P3, const BPoint &b, BPoint m[2])
{
	if((P0-P3).D2() < MIND * MIND)
	{
		m[0].MiddlePoint(P0, P3);
		m[1]= m[0];
		return;
	}
	m[0] = m[1] = TraceOfLines(P0, P0 + a, P3, P3 + b);
	double h = sqrt((P0 - P3).D2()) / (sqrt((P0 - m[0]).D2()) + sqrt((P3 - m[1]).D2()));
	m[0] = m[0] * h;
	m[1] = m[1] * h;
}
BPoint GrazingCurveElemContour::SpeedOfPointTool(	const BMatrPair & MatrixOfTool, //матрица инструмента
													const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
													const BPoint & P)			//точка в СК инструмента
{
	//BMatr MatrOfTool = MatrixOfTool.invr();
	//BMatr Matr = MatrixOfTool * TotalDeriv * MatrOfTool;//полная производная марицы скорости в СК инструмента
	//BPoint speed = P * Matr;//Вектор полной скороти в точке P в СК инструмента
	//return speed;
	return (((P * MatrixOfTool.Or()) * TotalDeriv) * MatrixOfTool.Inv());//Вектор полной скороти в точке P в СК инструмента
}
AvailableTrace GrazingCurveElemContour::FindGPointOfLine(	const BMatrPair & MatrixOfTool,			//матрица инструмента
															const BMatr & TotalDeriv,			//полная производная матрицы движения по времени
															const BPoint & A, const BPoint & B,	//точки в СК инструмента
															const BPoint & Normal,				//нормаль к образующей конуса
															BPoint & P)							//граничная точка на образующей конуса
{
	BPoint v0 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, A);
	BPoint v1 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, B);
	//if(pow((v0-v1)*Normal,2) < MIND * MIND)
	//{
	//	if((v0-v1).D2() < MIND * MIND)
	//	{	
	//		P = B;
	//		return COUNTLESS_TRACE;
	//	}
	//	return ABSENCE_TRACE;
	//}
	double d1 = pow(v0*Normal, 2);
	double d2 = v0.D2()*Normal.D2()*MINAR*MINAR;
	double d3 = pow(v1*Normal, 2);
	double d4 = v1.D2()*Normal.D2()*MINAR*MINAR;
	
	if(pow(v0*Normal, 2) <= v0.D2()*Normal.D2()*MINAR*MINAR && pow(v1*Normal, 2) <= v1.D2()*Normal.D2()*MINAR*MINAR)
		return COUNTLESS_TRACE;
	if(v0.D2() < MIND*MIND)
		if(pow(v1*Normal, 2) <= v1.D2()*Normal.D2()*MINAR*MINAR)
			return COUNTLESS_TRACE;
	if(v1.D2() < MIND*MIND)
		if(pow(v0*Normal, 2) <= v0.D2()*Normal.D2()*MINAR*MINAR)
			return COUNTLESS_TRACE;

	double t = v0*Normal/((v0-v1)*Normal);
	if(t < 0.0)
		return ABSENCE_TRACE;
	if(t > 1.0)
	{
		P = B;
		return ONE_TRACE;
	}
	P = A + (B - A)*t;
	return ONE_TRACE;
}
BPoint GrazingCurveElemContour::NormalToSurfaceOfPoint(	const NLine & Line,		//линия элемента контура
														const BPoint & P)		//точка, в которой находим нормаль к поверхности конуса
{
	BPoint N = NormalToSurfaceOfLine(Line, 0.0);//нормаль для конуса на образующей везде одинаковая
	BPoint E(0.0, 0.0, P.GetZ(), 1.0);
	BPoint V(P - E);
	double Ang = BPoint(1., 0., 0., 0.).Angle(V);
	BPoint n = N * BMatr().RotZ(0., 0., 0., Ang);
	return n * (1. / sqrt(n.D2()));
}
BPoint GrazingCurveElemContour::NormalToSurfaceOfLine(	const NLine & Line,		//точка в СК инструмента
														const double t)			//параметр параметризации кривой
{
	BMatr M;			//матрица поворота касательного вектора вектора r'(t) к вектору нормали в точке P(t)

	BPoint r1 = KasatToSurfaceOfLine(Line, t);//нормаль в точке r(t)
	M = M.RotY(r1, -90.0);
	BPoint norm = r1 * M;
	norm = norm * (1/sqrt(norm.D2()));
	return norm;
}
BPoint GrazingCurveElemContour::KasatToSurfaceOfLine(	const NLine & Line,	//точка в СК инструмента
														const double t)		//параметр параметризации кривой
{
	BPoint	r,		//нормализованная точка r(t) = P(t)/H(t)
			r1,		//производная нормализованной точки r'(t) 
			P,		//ненормализованная точка P(t)
			P1;		//произвольная P'(t) ненормализованной точки P(t)
	double	H,		//коордианта H(t) ненормализованной точки P(t)
			H1;		//производная координаты H(t) ненормализованной точки P(t)

	P =  Line.p[0] * pow((1-t), 3) +  Line.p[1] * 3*(t - 2*t*t + t*t*t) +  Line.p[2] * 3*(t*t - t*t*t) +  Line.p[3] * t*t*t;
	P1 = Line.p[0] * (-3)*(1-t)*(1-t) + Line.p[1] * 3*(1 - 4*t + 3*t*t) + Line.p[2] * 3*(2*t - 3*t*t) + Line.p[3] * 3*t*t;
	H = P.GetH();
	H1 = Line.p[0].GetH() * (-3)*(1-t)*(1-t) + Line.p[1].GetH() * 3*(1 - 4*t + 3*t*t) + Line.p[2].GetH() * 3*(2*t - 3*t*t) + Line.p[3].GetH() * 3*t*t;
	r = P.Norm();//тоже, что и P*(1/H);
	r1 = (P1 - r*H1) * (1/H);
	return r1;
}
double GrazingCurveElemContour::PointLine2(	const BPoint & A,	//1 точка прямой
											const BPoint & B,	//2 точка прямой
											const BPoint & P)	//точка, от которой ищем расстояние до прямой
{
	return (P - PointProjLine(A, B, P)).D2();
}
BPoint GrazingCurveElemContour::PointProjLine(	const BPoint & A,
												const BPoint & B,
												const BPoint & P)
{
	BPoint AP, AB;
	AP = P - A;
	AB = B - A;
	AB = AB * (1/sqrt(AB.D2()));
	return A + AB * (AP * AB);//проекция P на AB
}
AvailableTrace GrazingCurveElemContour::FindGPoints(double z,					//координата по оси Oz, для кот. ищем гр. точки 
													const NLine & Line,			//линия элемента контура
													const BMatrPair & MatrixOfTool, //матрица инструмента
													const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
													BPoint &d,					//вектор полной скорости в точке Q в СК инструмента
													BPoint &Q,					//центр сферы, касательной  поверхности инстр. Точки касания имеют аппликату z
													BPoint M[2],				//искомые гр. точки для данного z
													TypeLineInContour Type)		//тип линии элемента контура		
{
	AvailableTrace f = ABSENCE_TRACE;
	switch(Type)
	{
	case CONE:
		f = FindGPointsCone(z, Line, MatrixOfTool, TotalDeriv, d, Q, M);
		break;
	case SPHERE:
		f = FindGPointsSphere(z, Line, MatrixOfTool, TotalDeriv, d, Q, M);
		break;
	case TORAHS:
		f = FindGPointsTorahs(z, Line, MatrixOfTool, TotalDeriv, d, Q, M);
		break;
	case HORIZONTAL:
		f = FindGPointsHorizontal(true, Line, MatrixOfTool, TotalDeriv,	d, Q, M);
		break;
	default:
		f = ABSENCE_TRACE;
		break;
	}
	return f;
}
Connect GrazingCurveElemContour::FindSegmentsGCurve(	const NLine	&Line,				//линия текущего элемента контура
														const NLine	&Line1,				//линия элемента контура, следующего(ниже по оси Oz) за текущим
														const BMatrPair &MatrixOfTool,		//матрица инструмента
														const BMatr &TotalDeriv,		//полная производная матрицы движения по времени
														const bool  flagleft,			//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
														bool		&fl,				//цвет левых сегментов следующего элемента контура
														const AvailableTrace fgph,		//тип пересечения на верхнем оcновании текущего элемента контура
														const AvailableTrace fgpl,		//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
														const BPoint *gp1,				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
														const BPoint *gp2,				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
														TypeLineInContour Type,			//Тип линии элемента контура
														SegmentGrazingCurve **segments)	//указатель на 2 сегмента на верхнем основании следующего элемента контура

{
	switch(Type)
	{
	case CONE:
		connect = FindSegmentsGCurveCone(Line, Line1, MatrixOfTool, TotalDeriv, flagleft, fl, fgph, fgpl, gp1, gp2, segments);
		break;
	case SPHERE:
		connect = FindSegmentsGCurvesSphere(Line, Line1, MatrixOfTool, TotalDeriv, flagleft, fl, fgph, fgpl, gp1, gp2, segments);
		break;
	case HORIZONTAL:
		connect = FindSegmentsGCurveHorisontal(Line, Line1, MatrixOfTool, TotalDeriv, flagleft, fl, fgph, fgpl, gp1, gp2, segments);
		break;
	default:
		connect = NOT;
	}
	return connect;
}
bool GrazingCurveElemContour::TransSign(const NLine	&Line,			//линия текущего элемента контура
										const NLine	&Line1,			//линия элемента контура, следующего(ниже по оси Oz) за текущим
										const BMatrPair & MatrixOfTool,	//матрица инструмента
										const BMatr &TotalDeriv,	//полная производная матрицы движения по времени
										const BPoint &D,			//точка на дуге АВ 
										bool &sign)					//true - знак меняется, false - нет
{
	BPoint d =  SpeedOfPointTool(MatrixOfTool, TotalDeriv, D);
	BPoint E(0.0, 0.0, D.GetZ(), 1.0);
	BPoint ED = D - E; //ED - перпендикуляр из точки на гр.кривой к оси инстр.
	ED.ProjXY();

	BPoint N1 = NormalToSurfaceOfPoint(Line, D);
	BPoint N2 = NormalToSurfaceOfPoint(Line1, D);
	
	double	l1 = N1 * d,
			l2 = N2 * d;

	if(l1*l1 < MINAR*MINAR)
	{
		BPoint D1 = Line.GetPointFromParam(0.99);
		BPoint ED1 = D1 - E;
		ED1.ProjXY();
		ED1 = ED1*(1/sqrt(ED1.D2()));
		ED = ED*(1/sqrt(ED.D2()));
		BMatr M;
		M.e0cong(ED1, ED);
		D1 = D1 * M;
		BPoint d1 = SpeedOfPointTool(MatrixOfTool, TotalDeriv,	D1);
		l1 = N1 * d1;
	}
	if(l2*l2 < MINAR*MINAR)
	{
		BPoint D2 = Line1.GetPointFromParam(0.05);
		BPoint ED2 = D2 - E;
		ED2.ProjXY();
		ED2 = ED2*(1/sqrt(ED2.D2()));
		ED = ED*(1/sqrt(ED.D2()));
		BMatr M;
		M.e0cong(ED2, ED);
		D2 = D2 * M;
		BPoint d2 = SpeedOfPointTool(MatrixOfTool, TotalDeriv,	D2);
		l2 = N2 * d2;
	}
	sign = false;
	if(l1 * l2 < 0.0)
	{
		sign = true;//знак меняется
		if(l1 > 0)//если ограничиваем положительную область текущего элемента контура
			return true;
	}
	return false;
}

Connect GrazingCurveElemContour::FindSegmentsOfLowBases(	const NLine	&Line,				//линия текущего элемента контура
															const NLine	&Line1,				//линия элемента контура, следующего(ниже по оси Oz) за текущим
															const BMatrPair & MatrixOfTool,		//матрица инструмента
															const BMatr &TotalDeriv,		//полная производная матрицы движения по времени
															const AvailableTrace fgph,		//тип пересечения на нижнем оcновании текущего элемента контура
															const AvailableTrace fgpl,		//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
															const BPoint gp1[2],			//точки текущего элемента контура
															const BPoint gp2[2],			//точки следующего элемента контура
															const bool flagleft,			//цвет sgcleft(sgcleft1 если есть)
															bool &fl,						//цвет левых сегментов следующего элемента контура
															SegmentGrazingCurve **segments)	//указатель на 2 сегмента на верхнем основании следующего элемента контура
{
	if(fgph == ABSENCE_TRACE && fgpl == ABSENCE_TRACE || fgph == COUNTLESS_TRACE && fgpl == COUNTLESS_TRACE)
		return NOT;

	BPoint t[2];//t[2] - первая и вторая точки для образующей дуги окружности
	//NLine Ll, Lr;//левая и правая дуги на нижнем основании

	if(fgph == ABSENCE_TRACE && fgpl == COUNTLESS_TRACE)//если точек, с которыми нужно соединять множество, а у текущего - нет
	{
		/*создадим верхние сегменты в виде полуокружностей*/
		if(!(*segments))
			*segments = new SegmentGrazingCurve [2];

		(*segments)[0].SetColor(false);
		(*segments)[1].SetColor(true);

		BPoint A, B, v;
		GPoints *gp1 = new GPoints,
				*gp2 = new GPoints;

		//Записываем левый сегмент
		A.Set(0.0, -Line.p[3].GetX(), Line.p[3].GetZ(), 1.0);
		B.Set(-Line.p[3].GetX(), 0.0,  Line.p[3].GetZ(), 1.0);
		v = SpeedOfPointTool(MatrixOfTool, TotalDeriv, B);
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, iv, B, jv, t);
		gp1->Add(A);
		gp1->Add(t[0]);
		gp1->Add(t[1]);
		gp1->Add(B);
		A.SetY(Line.p[3].GetX());
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, jv, A, iv, t);
		gp1->Add(t[0]);
		gp1->Add(t[1]);
		gp1->Add(A);

		//Записываем правый сегмент
		A.SetY(-Line.p[3].GetX());
		B.SetX(Line.p[3].GetX());
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, iv, B, jv, t);
		gp2->Add(A);
		gp2->Add(t[0]);
		gp2->Add(t[1]);
		gp2->Add(B);
		A.SetY(Line.p[3].GetX());
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, jv, A, iv, t);
		gp2->Add(t[0]);
		gp2->Add(t[1]);
		gp2->Add(A);

		if(kv*v > 0.0)
		{
			(*segments)[0].Add(gp1);
			(*segments)[1].Add(gp2);
		}
		else
		{
			(*segments)[0].Add(gp2);
			(*segments)[1].Add(gp1);
		}
		return YESUP;
	}
	if(fgph == COUNTLESS_TRACE && fgpl == ABSENCE_TRACE)//если точек, с которыми нужно соединять нет, а у текущего - множество
	{
		sgcldown.SetColor(false);
		sgcrdown.SetColor(true);

		BPoint A, B, v;
		GPoints *gp1 = new GPoints,
				*gp2 = new GPoints;

		//Записываем левый сегмент
		A.Set(0.0, -Line.p[3].GetX(), Line.p[3].GetZ(), 1.0);
		B.Set(-Line.p[3].GetX(), 0.0,  Line.p[3].GetZ(), 1.0);
		v = SpeedOfPointTool(MatrixOfTool, TotalDeriv, B);
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, iv, B, jv, t);

		gp1->Add(A);
		gp1->Add(t[0]);
		gp1->Add(t[1]);
		gp1->Add(B);
		A.SetY(Line.p[3].GetX());
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, jv, A, iv, t);
		gp1->Add(t[0]);
		gp1->Add(t[1]);
		gp1->Add(A);

		//Записываем правый сегмент
		A.SetY(-Line.p[3].GetX());
		B.SetX(Line.p[3].GetX());
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, iv, B, jv, t);
		gp2->Add(A);
		gp2->Add(t[0]);
		gp2->Add(t[1]);
		gp2->Add(B);
		A.SetY(Line.p[3].GetX());
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, jv, A, iv, t);
		gp2->Add(t[0]);
		gp2->Add(t[1]);
		gp2->Add(A);

		if(kv*v > 0.0)
		{
			sgcldown.Add(gp1);
			sgcrdown.Add(gp2);
		}
		else
		{
			sgcldown.Add(gp2);
			sgcrdown.Add(gp1);
		}
		return NOT;
	}

	bool sign1;

	BPoint E;
	if(fgph == COUNTLESS_TRACE || fgph == ABSENCE_TRACE)
		E.Set(0.0, 0.0, gp2[0].GetZ(), 1.0);
	else
		E.Set(0.0, 0.0, gp1[0].GetZ(), 1.0);

	BPoint	a1 = gp1[0] - gp1[1],
			a2 = gp2[0] - gp2[1],

			q1 = (gp1[0] - E) % kv,
			q2 = (gp1[1] - E) % kv,
			q3 = (gp2[0] - E) % kv,
			q4 = (gp2[1] - E) % kv;

	q1 = q1 * (1/sqrt(q1.D2()));
	q2 = q2 * (1/sqrt(q2.D2()));
	q3 = q3 * (1/sqrt(q3.D2()));
	q4 = q4 * (1/sqrt(q4.D2()));

	a1 = a1 * (1/sqrt(a1.D2()));
	a2 = a2 * (1/sqrt(a2.D2()));

	/*Если у верхнего эл.контура нет гр. точек на осн., а у нижнего эл.конт. они точно есть*/
	if(fgph == ABSENCE_TRACE || fgph == COUNTLESS_TRACE)
	{
		bool ts = false, ts1 = false;

		BPoint T = a2 % kv;
		T = T * (Line.p[3].GetX()/sqrt(T.D2()));//вектор длины радиуса

		BPoint D = E + T;//D стала точкой между левым и правым сегментами на основании
		BPoint D1 = E - T;//D1 стала точкой между левым и правым сегментами на основании противоположно D
		bool sign = false;
		ts = TransSign(Line, Line1, MatrixOfTool, TotalDeriv, D, sign);
		ts1 = TransSign(Line, Line1, MatrixOfTool, TotalDeriv, D1, sign1);
	
		BPoint G(0., 0., 0., 1.);
		//там, где меняется знак, там и будет дуга
		if(sign)
			G = D;
		if(sign1)
			G = D1;

		fl = false;//Красим заново как условились(левые сегменты в цвет false)

		if(ts || ts1)//если есть дуги нижних сегментов, ограничивающие положительную область текущего элемента контура, то они в силу единственности пройдут через G(там где меняется знак)
		{
			PointsOfSegmentsCircle(G, a2, gp2[0], q3, t);//левый сегмент
			sgcldown.Add(G, t[0], t[1], gp2[0]);

			PointsOfSegmentsCircle(G, a2, gp2[1], q4, t);//правый сегмент
			sgcrdown.Add(G, t[0], t[1], gp2[1]);

			sgcldown.SetColor(fl);
			sgcrdown.SetColor(!fl);

			return NOTUP;//связность есть, верхних дуг нет
		}
		if(!(*segments))
			*segments = new SegmentGrazingCurve [2];

		PointsOfSegmentsCircle(G, a2, gp2[0], q3, t);//левый сегмент
		(*segments)[0].Add(G, t[0], t[1], gp2[0]);

		PointsOfSegmentsCircle(G, a2, gp2[1], q4, t);//правый сегмент
		(*segments)[1].Add(G, t[0], t[1], gp2[1]);

		(*segments)[0].SetColor(fl);
		(*segments)[1].SetColor(!fl);

		return YESUP;//связности нет, верхние дуги есть
	}

	//если точек, с которыми нужно соединять нет, а у текущего - есть
	if(fgpl == ABSENCE_TRACE || fgpl == COUNTLESS_TRACE)
	{
		if((gp1[0] - gp1[1]).D2() < MIND*MIND)//если петля лишь коснулась основания
			return NOT;//связности нет, верхних дуг нет

		bool ts = false, ts1 = false;

		BPoint T = a1 % kv;
		T = T * (Line.p[3].GetX()/sqrt(T.D2()));//вектор длины радиуса

		BPoint D = E + T;//D стала точкой между левым и правым сегментами на основании
		BPoint D1 = E - T;//D стала точкой между левым и правым сегментами на основании
		bool sign = false;
		ts = TransSign(Line, Line1, MatrixOfTool, TotalDeriv, D, sign);
		ts1 = TransSign(Line, Line1, MatrixOfTool, TotalDeriv, D1, sign1);

		BPoint G(0., 0., 0., 1.);
		//там, где меняется знак, там и будет дуга
		if(sign)
			G = D;
		if(sign1)
			G = D1;

		if(ts || ts1)//если есть дуги нижних сегментов, ограничивающие положительную область текущего элемента контура, то они в силу единственности пройдут через G(там где меняется знак)
		{
			PointsOfSegmentsCircle(gp1[0], q1, G, a1, t);//левый сегмент
			sgcldown.Add(gp1[0], t[0], t[1], G);

			PointsOfSegmentsCircle(gp1[1], q2, G, a1, t);//правый сегмент
			sgcrdown.Add(gp1[1], t[0], t[1], G);

//			fl = false;//Красим заново как условились(левые сегменты в цвет false)!!!

			sgcldown.SetColor(fl);
			sgcrdown.SetColor(!fl);

			return NOT;//связности нет, верхних дуг нет
		}

		if(!(*segments))
			*segments = new SegmentGrazingCurve [2];
		
		PointsOfSegmentsCircle(gp1[0], q1, G, a1, t);//левый сегмент
		(*segments)[0].Add(gp1[0], t[0], t[1], G);

		PointsOfSegmentsCircle(gp1[1], q2, G, a1, t);//правый сегмент
		(*segments)[1].Add(gp1[1], t[0], t[1], G);

		fl = false;//Красим заново как условились(левые сегменты в цвет false)!!!

		(*segments)[0].SetColor(fl);
		(*segments)[1].SetColor(!fl);

		return YES;//связности нет, верхние дуги есть
	}

	/*Если гр. точки на текущем и следующем элементах контура "совпали"*/
	if( ((gp1[0] - gp2[0]).D2() < MIND * MIND && (gp1[1] - gp2[1]).D2() < MIND * MIND) || ((gp1[0] - gp2[1]).D2() < MIND * MIND && (gp1[1] - gp2[0]).D2() < MIND * MIND) )
	{
		BPoint	p1 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, E),
				p2 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, E + kv), 
				p3 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, E + iv + jv);

		if(	pow(p1 * kv, 2) <  MINAR*MINAR*p1.D2() && 
			pow(p2 * kv, 2) <  MINAR*MINAR*p2.D2() &&
			pow(p3 * kv, 2) <  MINAR*MINAR*p3.D2())//если движение поступательно в направлении перпендикулярном оси инструмента
		{
			fl = flagleft;
			return NOTUP;//соединим с боковыми сегментами следующего элемента контура(связность есть), т.к. на основании дуг вообще не создали
		}

		fl = false;

		BPoint T = a1 % kv;//здесь не важно a1 или a2, т.к. точки совпали, то и a1==a2
		T = T * (Line.p[3].GetX()/sqrt(T.D2()));//вектор длины радиуса

		BPoint D = E + T;//D стала точкой между левым и правым сегментами на основании

		bool sign = false;
		if( !TransSign(Line, Line1, MatrixOfTool, TotalDeriv, D, sign) )//Если здесь не проходит кривая, ограничивающая положительную область тек.эл.конт.(нет нижних сегментов у текущего)
		{
			if(sign)//если знак все же меняется, то здесь есть граничная кривая, ограничивающая положительную область след.эл.конт.
			{
				fl = false;//так как начали новую компоненту связности гр.кривой

				if(!(*segments))
					*segments = new SegmentGrazingCurve [2];

				PointsOfSegmentsCircle(D, a2, gp2[0], q3, t);//левый сегмент
				(*segments)[0].Add(D, t[0], t[1], gp2[0]);

				PointsOfSegmentsCircle(D, a2, gp2[1], q4,  t);//правый сегмент
				(*segments)[1].Add(D, t[0], t[1], gp2[1]);

				(*segments)[0].SetColor(fl);
				(*segments)[1].SetColor(!fl);

				D = E - T;//сменим точку проверяемую на смену знаков скалярных произведений  на противоположную

				PointsOfSegmentsCircle(gp1[0], q1, D, a1, t);//левый сегмент
				sgcldown.Add(gp1[0], t[0], t[1], D);

				PointsOfSegmentsCircle(gp1[1], q2,  D, a1, t);//правый сегмент
				sgcrdown.Add(gp1[1], t[0], t[1], D);

				sgcldown.SetColor(flagleft);
				sgcrdown.SetColor(!flagleft);

				return YESUP;
			}
			//нет верхних дуг, связность есть
			fl = flagleft;
			return NOTUP;//если нет нижних сегментов для текущего и верхних для следующего, то вместе они быть на второй дуге быть не могут => соединим точки попарно
		}
		else
		{
			PointsOfSegmentsCircle(gp1[0], q1, D, a1, t);//левый сегмент
			sgcldown.Add(gp1[0], t[0], t[1], D);

			PointsOfSegmentsCircle(gp1[1], q2, D, a1, t);//правый сегмент
			sgcrdown.Add(gp1[1], t[0], t[1], D);

			sgcldown.SetColor(flagleft);
			sgcrdown.SetColor(!flagleft);

			D = E - T;//если на второй дуге есть нижние сегменты, то на первой должны быть верхние в силу замкнутости всех компонент кривой

			fl = false;//так как начали новую компоненту связности гр.кривой

			if(!(*segments))
				*segments = new SegmentGrazingCurve [2];

			PointsOfSegmentsCircle(D, a2, gp2[0], q3, t);//левый сегмент
			(*segments)[0].Add(D, t[0], t[1], gp2[0]);

			PointsOfSegmentsCircle(D, a2, gp2[1], q4,  t);//правый сегмент
			(*segments)[1].Add(D, t[0], t[1], gp2[1]);
			 
			(*segments)[0].SetColor(fl);
			(*segments)[1].SetColor(!fl);

			return YESUP;	
		}
	}

	BPoint n[4];//точки гр.кривой на основании
	n[0] = gp1[0];
	n[0].SetH(0.0);//запомним вектор, соответствующий левой гр.точке для текущ.эл.контура

	n[1] = gp1[1];
	n[1].SetH(1.0);//запомним вектор, соответствующий правой гр.точке для текущ.эл.контура

	n[2] = gp2[0];
	n[2].SetH(2.0);//запомним вектор, соответствующий левой гр.точке для ниж.эл.контура

	n[3] = gp2[1];
	n[3].SetH(3.0);//запомним вектор, соответствующий правой гр.точке для ниж.эл.контура
	
	qsort((void *)n, 4, sizeof(BPoint), &compare);


	Connect connect = NOT;
	bool sign[4];
	bool q[4];
	for(int i = 0; i < 4; i++)
	{
		BPoint a = n[i] - n[(i+1)%4];
		a.SetH(0.0);
		BPoint T = a % kv;
		T = T * (Line.p[3].GetX()/sqrt(T.D2()));//вектор длины радиуса
		BPoint D = E + T;//D стала точкой между левым и правым сегментами на основании
		D.SetH(1.0);

		sign[i] = false;
		q[i] = TransSign(Line, Line1, MatrixOfTool, TotalDeriv, D, sign[i]);
	}
	// если 3 последовательно true - убрать средний
	for(int i = 0; i < 4; i++)
	{
		if(sign[i] && sign[(i + 1) % 4] && sign[(i + 2) % 4])
			sign[(i + 1) % 4] = false;
	}
	for(int i = 0; i < 4; i++)
	{
		BPoint s1 = n[i], s2 = n[(i+1)%4];

		if(sign[i])//если меняется знак
		{
			//соединили попарно точки на нижнем основании текущего элемента контура
			if( (s1.GetH() == 0 && s2.GetH() == 1) || 
				(s1.GetH() == 1 && s2.GetH() == 0) ||
				(s1.GetH() == 2 && s2.GetH() == 3) ||
				(s1.GetH() == 3 && s2.GetH() == 2) )
			{
				auto bi = i;
				if(s1.GetH() > 1.5)
					bi = (bi + 2) % 4;

				s1 = n[bi];
				s2 = n[(bi + 1) % 4];
				BPoint a = s1 - s2;
				a.SetH(0.0);
				BPoint T = a % kv;
				T = T * (Line.p[3].GetX()/sqrt(T.D2()));//вектор длины радиуса
				BPoint D = E + T;//D стала точкой между левым и правым сегментами на основании
				D.SetH(1.0);

				PointsOfSegmentsCircle(gp1[0], q1, D, a, t);//левый сегмент
				sgcldown.Add(gp1[0], t[0], t[1], D);

				PointsOfSegmentsCircle(gp1[1], q2, D, a, t);//правый сегмент
				sgcrdown.Add(gp1[1], t[0], t[1], D);

				sgcldown.SetColor(flagleft);
				sgcrdown.SetColor(!flagleft);

				s1 = n[(bi+2)%4];
				s2 = n[(bi+3)%4];

				a = s1 - s2;
				a.SetH(0.0);
				T = a % kv;
				T = T * (Line.p[3].GetX()/sqrt(T.D2()));//вектор длины радиуса
				D = E + T;//D стала точкой между левым и правым сегментами на основании
				D.SetH(1.0);

				if(!(*segments))
					*segments = new SegmentGrazingCurve [2];

				PointsOfSegmentsCircle(D, a, gp2[0], q3, t);//левый сегмент
				(*segments)[0].Add(D, t[0], t[1], gp2[0]);

				PointsOfSegmentsCircle(D, a, gp2[1], q4, t);//правый сегмент
				(*segments)[1].Add(D, t[0], t[1], gp2[1]);

				fl = false;//так как начали новую компоненту связности гр.кривой

				(*segments)[0].SetColor(fl);
				(*segments)[1].SetColor(!fl);

				connect = YESUP;
				break;
			}
			

			BPoint temp1, temp2;
			//temp1, temp2 - точки соединяющиеся дугами с левой и правой точками текущего контура соответственно
			//С ЧЕМ соединили
			if((s1.GetH() == 2 || s2.GetH() == 2))//соединили с левой точкой следующего элемента
			{
				temp2 = gp2[0];
				if((s1.GetH() == 0 || s2.GetH() == 0))//соединили левую точку текущего элемента
				{
					temp1 = gp1[0];
					fl = flagleft;
				}
				else
				{
					temp1 = gp1[1];
					fl = !flagleft;
				}
			}
			else//соединили с правой точкой следующего элемента
			{
				temp2 = gp2[1];
				if((s1.GetH() == 0 || s2.GetH() == 0))//соединили левую точку текущего элемента
				{
					temp1 = gp1[0];
					fl = !flagleft;
				}
				else
				{
					temp1 = gp1[1];
					fl = flagleft;
				}
			}

			BPoint	q10 = (temp1 - E) % kv,
					q20 = (temp2 - E) % kv;

			q10 = q10 * (1. / sqrt(q10.D2()));
			q20 = q20 * (1. / sqrt(q20.D2()));

			PointsOfSegmentsCircle(temp1, q10, temp2, q20, t);//левый сегмент
			if(q[i])//нижний сегмент текущего элемента
			{
				if((s1.GetH() == 0 || s2.GetH() == 0))//соединили левую точку текущего элемента
					sgcldown.Add(temp1, t[0], t[1], temp2);
				else//соединили правую точку текущего элемента
					sgcrdown.Add(temp1, t[0], t[1], temp2);

				sgcldown.SetColor(flagleft);
				sgcrdown.SetColor(!flagleft);

				if(connect < NOTUP)
					connect = NOTUP;//связность есть, верхних дуг нет
			}
			else//верхний сегмент следующего контура
			{
				if(!(*segments))
					*segments = new SegmentGrazingCurve [2];

				if((s1.GetH() == 2 || s2.GetH() == 2))//соединили c левой точкой следующего элемента //!!! 0 или 2 ???
					(*segments)[0].Add(temp1, t[0], t[1], temp2);
				else
					(*segments)[1].Add(temp1, t[0], t[1], temp2);

				(*segments)[0].SetColor(fl);
				(*segments)[1].SetColor(!fl);

				connect = YES;//связность есть, верхние дуги есть
			}
		}
		
	}
	return connect;
}
AvailableTrace GrazingCurveElemContour::FindGPointsCone(double z,					//координата по оси Oz, для кот. ищем гр. точки 
														const NLine & Line,			//линия элемента контура
														const BMatrPair & MatrixOfTool, //матрица инструмента
														const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
														BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
														BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
														BPoint M[2])				//искомые гр. точки для данного z
{

	BPoint E(0.0, 0.0, z, 1.0);
	BPoint A = Line.p[0];
	BPoint B = Line.p[3];
	BPoint AB = B - A;
	BPoint A1 = E + kv * ((A - E) * kv);
	BPoint B1 = E + kv * ((B - E) * kv);
	BPoint P = A + AB * sqrt( (E - A1).D2() / (B1 - A1).D2() );
	double R2 = (P - E).D2();
	double tgq = (A.GetX() - B.GetX())/(A.GetZ() - B.GetZ());
	if(R2 < MIND * MIND)
		Q = E;
	else
	{
		//tgq = (A.GetX() - B.GetX())/(A.GetZ() - B.GetZ());
		Q = E + kv * (sqrt(R2) * tgq);
	}

	//d - вектор полной скороти в точке Q в СК инструмента
	d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q);

	BPoint speedXY = d;
	speedXY.ProjXY();
	if(speedXY.D2() < MIND * MIND)
	{
		if(fabs(A.GetZ()-B.GetZ())< MIND)
			d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q+BPoint(-(B-A).GetZ(), 0, (B-A).GetX(), 0.0));
		else
		{
			if( ((Q+kv) - (A+B)*0.5).D2() < ((Q-kv) - (A+B)*0.5).D2() )
				d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q+kv);
			else
				d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q-kv);
		}
	}
	//если вектор скорости лежит внутри конусности конуса, то точек нет.
	double l1 = sqrt(speedXY.D2());
	double l2 = fabs(d.GetZ());
	double l = l1/l2;
	if( (l - fabs(tgq)) < -MINAR)
		return ABSENCE_TRACE;
		//return COUNTLESS_TRACE;

	return CirclePlane(E, kv, Q, d * (1/sqrt(d.D2())), R2, M);//M[0] - левая точка, M[1] - правая точка
}
void GrazingCurveElemContour::FindGPointsCone(	const NLine & Line,			//линия элемента контура
												const BMatrPair & MatrixOfTool, //матрица инструмента
												const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
												const BPoint &A,				//"верхняя" точка по на поверхности конуса
												const BPoint &B,				//"нижняя" точка по на поверхности конуса
												const double eps,			//погрешность
												GPoints *gp)				//хранилище точек сегмента кривой
{
	BPoint	E1, E2;	

	E1.Set(0.0, 0.0, A.GetZ(), 1.0);
	E2.Set(0.0, 0.0, B.GetZ(), 1.0);

	double a1 = iv.Angle(A-E1);
	double a2 = iv.Angle(B-E2);
	double alfa = fabs(a1 - a2);//угол поворота дуги на поверхности конуса
	if(alfa > 180.)
		alfa = 360. - alfa;
	if(alfa < MINAD)
	{
		gp->Add(A);
		gp->Add(B);
		return;
	}

	double r = sqrt((A - E1).D2());
	double r1 = sqrt((B - E2).D2());

	BPoint A1 = A;//начальная точка дуги граничной кривой
	BPoint B1 = B;//конечная точка дуги граничной кривой

	NLine L1, L2;//дуга большего диаметра
	BPoint t[2];//t[2] - первая и вторая точки для образующей дуги окружности
	PointsOfSegmentsCircle(A1, (A1 - E1) % kv, (E1 + (B1 - E2)*(r/r1)), (B1 - E2) % kv, t);
	L1.Set(A1, t[0], t[1], E1 + (B1 - E2)*(r/r1));

	PointsOfSegmentsCircle((E2 + (A1 - E1)*(r1/r)), (A1 - E1) % kv, B1, (B1 - E2) % kv, t);
	L2.Set(E2 + (A1 - E1)*(r1/r), t[0], t[1], B1);
	
	if(r < r1)
		r = r1;

	int kol = NSurfSettings::GetNumAproxLine(r, alfa);//количество отрезков дуги на поверхности конуса

	gp->Add(A);
	for(int i = 1; i < kol; i++)
	{
		A1 = L1.GetPointFromParam(double(i)/double(kol));
		B1 = L2.GetPointFromParam(double(i)/double(kol));
		A1.Norm();
		B1.Norm();
		BPoint N = NormalToSurfaceOfPoint(Line, A1);
		//f = FindGPointOfLine(MatrixOfTool, TotalDeriv, A1, B1, N, A1);
		FindGPointOfLine(MatrixOfTool, TotalDeriv, A1, B1, N, A1);
		gp->Add(A1);
	}
	gp->Add(B);
}

AvailableTrace GrazingCurveElemContour::FindGPointsHorizontal(	bool up,					//true - ищет гр. точки с верхним основанием, false - с нижним основанием
																const NLine & Line,			//линия элемента контура
																const BMatrPair & MatrixOfTool, //матрица инструмента
																const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
																BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
																BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
																BPoint M[2])				//искомые гр. точки
{
	BMatr Matr = MatrixOfTool.Or() * TotalDeriv * MatrixOfTool.Inv();
	double kZ = Line.p[3].GetX() > Line.p[0].GetX() ? -1.0 : 1.0;
	BPoint k(0.0, 0.0, kZ, 0.0);

	BPoint A;
	if(up)//если ищем гр. точки на верхнем основании
		A = Line.p[0];
	else//если ищем гр. точки на нижнем основании
		A = Line.p[3];

	Q.Set(0.0, 0.0, Line.p[0].GetZ(), 1.0);
	d = Q * Matr;
	BPoint i(1.0, 0.0, 0.0, 0.0);
	BPoint j(0.0, 1.0, 0.0, 0.0);
	
	double a = i * Matr * k;//орт k в данном случае в роли нормали к элементу контура HORIZONTAL в СК инструмента
	double b = j * Matr * k;
	double c = d * k;

	double tau, t1, t2;

	if (a*a < MINZ*MINZ && b*b < MINZ*MINZ)//поступательное движение
	{
		//	pow((d1 * k), 2) < MINAR*MINAR*d1.D2() &&	//d1 перпендикулярен k
		//	((d%d1).D2() < MINAR*MINAR*d.D2()*d1.D2() || A.GetX() < MIND)) 	//d и d1 параллельны (сонаправлены - дв. поступательно, противоположно напрвлены - дв. чисто вращательное) 
		if (pow((d * k), 2) < MINAR*MINAR*d.D2())	//d перпендикулярен k (horizontal plane movement)
		{
			BPoint d1 = (Q + k) * Matr;
			BPoint left = k % d;
			if (d.D2() < MIND*MIND)
				left = k % d1;
			left = left * (A.GetX() / sqrt(left.D2()));
			M[0] = Q - left;
			M[1] = Q + left;
			return TWO_TRACE;//Хотя реально все основание, но условились, что прямая, проходящая посередине диска перпендикулярно направлению движения
		}
		// non horizontal plane movement
		bool Internal = (kZ > 0) == up;
		if (Internal)
			return ABSENCE_TRACE;
		d.ProjXY();
		double dd2 = d.D2();
		if (dd2 < MIND*MIND)
			return ABSENCE_TRACE; // движение вдоль нормали, т.е. в направлении оси Z должно быть обработано раньше

		M[0] = Q + c * d * (A.GetX() / (fabs(c) * sqrt(dd2)));
		M[1] = M[0];
		return TWO_TRACE;
	}
//		return ABSENCE_TRACE;

	BPoint n(a, b, 0, 0);
	BPoint e(-b, a, 0, 0);
	tau = -c/(a*a + b*b);
	
	BPoint P0 = n * tau;
	P0.SetZ(A.GetZ());
	P0.SetH(1.0);

	double	AA = e.GetX()*e.GetX() + e.GetY()*e.GetY(),
			BB = 2*(P0.GetX()*e.GetX() + P0.GetY()*e.GetY()),
			CC = P0.GetX()*P0.GetX() + P0.GetY()*P0.GetY() - A.GetX()*A.GetX();

	if (AA < MINZ * MINZ)//просто защита. Такого быть не может. ex и ey сразу не могут быть равны 0
		return ABSENCE_TRACE;

	double D = BB * BB - 4 * AA * CC;

	if(D >= 0 /*|| D*D < MINZ*MINZ */)//есть пересечение с основанием
	{
		t1 = (-BB - sqrt(fabs(D)))/(2*AA);
		t2 = (-BB + sqrt(fabs(D)))/(2*AA);
		M[0] = P0 + e*t1;
		M[1] = P0 + e*t2;

		BPoint dn = SpeedOfPointTool(MatrixOfTool, TotalDeriv, M[0]);
		double m = (dn % k) * (M[1] - M[0]);
		if( m > 0 )//если M[0] не левая точка
		{
			BPoint temp = M[0];
			M[0] = M[1];
			M[1] = temp;
		}
		return TWO_TRACE;
	}
	return ABSENCE_TRACE;
}

AvailableTrace GrazingCurveElemContour::FindGPointsSphere(	double z,					//координата по оси Oz, для кот. ищем гр. точки 
															const NLine & Line,			//линия элемента контура
															const BMatrPair & MatrixOfTool, //матрица инструмента
															const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
															BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
															BPoint &Q,					//центр сферы, касательной к поверхности инстр(возвращается). Точки касания имеют аппликату z
															BPoint M[2])				//искомые гр. точки для данного z
{
	Q = Line.GetCenter();
	d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q);
	BPoint k(0.0, 0.0, 1.0, 0.0);
	BPoint E(0.0, 0.0, z, 1.0);
	AvailableTrace f = CirclePlane(E, k, Q, d * (1/sqrt(d.D2())), (Line.p[0] - Q).D2() - (Q - E).D2(), M);
	switch(f)
	{
	case COUNTLESS_TRACE:
		return COUNTLESS_TRACE;
	case ABSENCE_TRACE:
		return ABSENCE_TRACE;
	case ONE_TRACE:
		return ONE_TRACE;
	case TWO_TRACE:
		return TWO_TRACE;
	}
	return TWO_TRACE;
}
AvailableTrace GrazingCurveElemContour::FindGPointsTorahs(	double z,					//координата по оси Oz, для кот. ищем гр. точки 
															const NLine & Line,			//линия элемента контура
															const BMatrPair & MatrixOfTool, //матрица инструмента
															const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
															BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
															BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
															BPoint M[2])				//искомые гр. точки для данного z
{
	BPoint E(0.0, 0.0, z, 1.0);//точка на оси
	BPoint i(1.0, 0.0, 0.0, 0.0);//орт оси Ox
	BPoint k(0.0, 0.0, 1.0, 0.0);//орт оси Oz
	if(pow((z - Line.p[0].GetZ()),2) < MIND * MIND)//Если ищем гр. точки на высоте == коор. z начала кривой элемента контура
	{
		BPoint kasat = Line.p[0] - Line.p[1] * (1/Line.p[1].GetH());
		if(pow((kasat * k), 2) < MINAR*MINAR*kasat.D2())//Если касательная ломаной в начале кривой перпендикулярна к оси инструмента
		{
			NLine L;
			BPoint P = Line.p[0];
			P.SetX(0.0);
			L.Set(Line.p[0], (Line.p[0]+P)*0.5, (Line.p[0]+P)*0.5, P);

			AvailableTrace f = FindGPointsHorizontal(true, L, MatrixOfTool, TotalDeriv, d,	Q, M);

			return f;
		}
	}
	if(pow((z - Line.p[3].GetZ()),2) < MIND * MIND)//Если ищем гр. точки на высоте == коор. z конца кривой элемента контура
	{
		BPoint kasat = Line.p[3] - Line.p[1]*(1/Line.p[1].GetH());
		if(pow((kasat * k), 2) < MINAR*MINAR*kasat.D2())//Если касательная ломаной в конце кривой перпендикулярна к оси инструмента
		{
			NLine L;
			BPoint P = Line.p[3];
			P.SetX(0.0);
			L.Set(Line.p[3], (Line.p[3]+P)*0.5, (Line.p[3]+P)*0.5, P);

			AvailableTrace f = FindGPointsHorizontal(true, L, MatrixOfTool, TotalDeriv, d,	Q, M);

			return f;	
		}
	}

	double r, EF, FP, EP, PQ;

	r = Line.GetRad();
	BPoint O = Line.GetCenter();

	BPoint EO = O - E;
	EF = fabs(EO * i);
	auto Val = (r*r - pow((EO * k), 2));
	if( Val < 0.0)
		return ABSENCE_TRACE;
	FP = sqrt(Val);//Если подкоренное выражение отрицательно, то мы наткнулись на точку P, нормаль в которой параллельна оси Oz. А этот случай обработан выше
	if(FP*FP < MIND*MIND)//Неизвестная ситуация
		return ABSENCE_TRACE;//Вроде, если до сюда дошли, и скорость направлена на всей оси инструмента не перпендикулярно ей, то гр. точек быть вообще не может быть
	BPoint temp = Line.p[1];
	temp = temp.Norm();
	if((temp - O) * i > 0)//тор выпуклый к оси Oz
	{
		EP = EF + FP;//радиус окружности
		BPoint P = E + i*EP;
		BPoint PO = O - P;//O и P никогда не совпадут => (PO != 0.0) => у следующего выражения знаменатель всегда != 0.0
		PO = PO * (1/sqrt(PO.D2()));
		PQ = EP * r / FP;
		Q = P + PO * PQ;
	}
	//если тор вогнутый к оси Oz
	else// == 0 быть не может, иначе одному z соответствовало бы два значения x
	{
		EP = EF - FP;//радиус окружности
		BPoint P = E + i*EP;
		BPoint PO = O - P;
		PO = PO * (1/sqrt(PO.D2()));
		PQ = EP * r / FP;
		Q = P - PO * PQ;
	}
	d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q);
	AvailableTrace f = CirclePlane(E, k, Q, d * (1/sqrt(d.D2())), EP*EP, M);
	return f;
}

AvailableTrace GrazingCurveElemContour::FindExtremumPointCone(const NLine & Line, const BMatrPair & MatrixOfTool, const BMatr & TotalDeriv, BPoint E[2])
{
	BPoint	v0, v1, 
			k(0.0, 0.0, 1.0, 0.0), 
			A = Line.p[3], 
			B = Line.p[0], 
			Q0, Q1, 
			M[2];//пара граничных точек на некоторой высоте
	
	double	R0 = A.GetX(),
			h = B.GetZ() - A.GetZ();

	//FindGPointsCone(A.GetZ(), Line, MatrixOfTool, TotalDeriv, v0, Q0, M);
	//FindGPointsCone(B.GetZ(), Line, MatrixOfTool, TotalDeriv, v1, Q1, M);
	v0 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, BPoint(0.0, 0.0, A.GetZ(), 1.0));
	v1 = SpeedOfPointTool(MatrixOfTool, TotalDeriv, BPoint(0.0, 0.0, B.GetZ(), 1.0));

	double cos2q = h * h / (A - B).D2();

	double	a = ((v1-v0)*k) * ((v1-v0)*k) - (v1-v0) * (v1-v0) * cos2q,
			b = 2*( (v0*k) * ((v1-v0)*k) - v0 * (v1-v0) * cos2q ),
			c = (v0*k)*(v0*k)-v0*v0*cos2q;

	double t1, t2, D = b*b - 4*a*c;
	
	if(a*a/(a*a + b*b + c*c) < MINZ*MINZ*1.e-4)//Если уравнение линейное(случай с поворотом циллиндра)
		return ABSENCE_TRACE;
	else
	{
		if(D < MIND)//корней нет(циллиндр)???
		{
			double t;
			t = -v0.GetY()/(v1.GetY() - v0.GetY());
			if(t * (v1.GetX() + v0.GetX()) - v0.GetX() == 0)
			{
				if(t >= 0. && t <= 1.)
				{
					A.SetX(0.0);
					B.SetX(0.0);
					E[0] = A * (1 - t) + B * t;
					E[1] = E[0];
					return COUNTLESS_TRACE;
				}
			}
			return ABSENCE_TRACE;
		}
		t1 = (-b + sqrt(fabs(D))) / (2*a);
		t2 = (-b - sqrt(fabs(D))) / (2*a);//используется модуль для случая, когда под корнем из-за погрешности зн. чуть < 0
	}
	double	z1 = t1 * h + A.GetZ();//высота точки Q экстремума
	double	z2 = t2 * h + A.GetZ();

	z1 = PointProjLine(A, B, BPoint(0.0, 0.0, z1, 1.0)).GetZ();//высота точек экстремума
	z2 = PointProjLine(A, B, BPoint(0.0, 0.0, z2, 1.0)).GetZ();
	
	if(z1 >= Line.p[3].GetZ() && z1 <= Line.p[0].GetZ() && z2 >= Line.p[3].GetZ() && z2 <= Line.p[0].GetZ())
	{
		if(pow((z1 - z2), 2) < MIND * MIND)//Если координаты z очень близки друг к другу(почти равны). Характерно только для циллиндра или конуса при вращении вокруг оси пересекающей его ось
		{
			double r = t1*(B.GetX() - A.GetX()) + R0;
			AvailableTrace f = CirclePlane(BPoint(0.0, 0.0, z1), k, BPoint(0.0, 0.0, z1), v1 * (1/sqrt(v1.D2())), r*r, M);//M[0] - левая точка, M[1] - правая точка
			E[0].Set(M[0].GetY(), -M[0].GetX(), (z1+z2)/2, 1.0);
			E[1].Set(-M[0].GetY(), M[0].GetX(), (z1+z2)/2, 1.0);
			return ONE_TRACE;
		}
		if(z1 > z2)
		{
			FindGPointsCone(z1, Line, MatrixOfTool, TotalDeriv, v0, Q0, M);
			E[0] = M[1];
			FindGPointsCone(z2, Line, MatrixOfTool, TotalDeriv, v1, Q1, M);
			E[1] = M[0];
		}
		else
		{
			/*Для случая с циллиндром(z1==z2) левая и правая точки соответствуют левой и правой точкам верхних боковых сегментов sgcleft и sgcright, 
			т.к. скорость v1 соответствует высоте == верхнему основанию конуса*/
			FindGPointsCone(z2, Line, MatrixOfTool, TotalDeriv, v1, Q1, M);
			E[0] = M[0];
			FindGPointsCone(z1, Line, MatrixOfTool, TotalDeriv, v0, Q0, M);
			E[1] = M[1];
		}
		return TWO_TRACE;
	}
	else
	{
		if(z1 >= Line.p[3].GetZ() && z1 <= Line.p[0].GetZ())
		{
			FindGPointsCone(z1, Line, MatrixOfTool, TotalDeriv, v0, Q0, M);
			E[0] = E[1] = M[0];
			return ONE_TRACE;
		}
		if(z2 >= Line.p[3].GetZ() && z2 <= Line.p[0].GetZ())
		{
			FindGPointsCone(z2, Line, MatrixOfTool, TotalDeriv, v1, Q1, M);
			E[0] = E[1] = M[1];
			return ONE_TRACE;
		}
		return ABSENCE_TRACE;
	}
	return ABSENCE_TRACE;
}
Connect GrazingCurveElemContour::FindSegmentsGCurveCone(	const NLine	&Line,					//линия текущего элемента контура
															const NLine	&Line1,					//линия элемента контура, следующего(ниже по оси Oz) за текущим
															const BMatrPair & MatrixOfTool,			//матрица инструмента
															const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
															const bool  flagleft,				//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
															bool		&fl,					//цвет левых сегментов следующего элемента контура
															const AvailableTrace fgph,			//тип пересечения гр. кривой с верхним онованием
															const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
															const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
															const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
															SegmentGrazingCurve **segments)		//указатель на 2 сегмента на верхнем основании следующего элемента контура	
{
	if(*segments)
	{
		sgclup.Add((*segments)[0]);
		sgcrup.Add((*segments)[1]);
	}
	BPoint m1[2], m2[2], d, Q, k;
	GPoints *gp = new GPoints;
	k.Set(0.0, 0.0, 1.0, 0.0);
	AvailableTrace f1 = ABSENCE_TRACE;//количество экстремумов
	AvailableTrace f = FindGPointsCone(Line.p[3].GetZ(), Line, MatrixOfTool, TotalDeriv, d, Q, m2);
	int kol = 0;
	BPoint *p = NULL;

	if((fgph==TWO_TRACE||fgph==ONE_TRACE) && (f==ABSENCE_TRACE || f==COUNTLESS_TRACE))
	{
		//Петля экстремумом вниз
		f1 = FindExtremumPointCone(Line, MatrixOfTool, TotalDeriv, m2);
		if (f1 != ABSENCE_TRACE)
		{
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, gp1[0], m2[0], EPS(), gp);
			sgcleft.Add(gp);
			sgcleft.SetColor(flagleft);
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, gp1[1], m2[1], EPS(), gp);
			sgcright.Add(gp);
			sgcright.SetColor(!flagleft);
		}
	}
	if((fgph==ABSENCE_TRACE || fgph==COUNTLESS_TRACE) && (f==TWO_TRACE||f==ONE_TRACE))
	{
		//Петля экстремумом вверх
		f1 = FindExtremumPointCone(Line, MatrixOfTool, TotalDeriv, m1);
		if (f1 != ABSENCE_TRACE)
		{
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, m1[0], m2[0], EPS(), gp);
			sgcleft1.Add(gp);
			sgcleft1.SetColor(false);
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, m1[1], m2[1], EPS(), gp);
			sgcright1.Add(gp);
			sgcright1.SetColor(true);
		}
	}
	if((fgph==TWO_TRACE||fgph==ONE_TRACE) && (f==TWO_TRACE||f==ONE_TRACE))
	{
		//считаем экстремумы. В зависимости от попадания их в диапазон их 2 или нет вообще, т.к. случай с 1 экстемумом рассмотрен выше
		f1 = FindExtremumPointCone(Line, MatrixOfTool, TotalDeriv, m1);
		if(f1 == ABSENCE_TRACE)//нет экстремумов вообще
		{
			//просто соединяем попарно точки на основаниях сегментами кривых
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, gp1[0], m2[0], EPS(), gp);
			sgcleft.Add(gp);
			sgcleft.SetColor(flagleft);
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, gp1[1], m2[1], EPS(), gp);
			sgcright.Add(gp);
			sgcright.SetColor(!flagleft);
		}
		else
		{
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, gp1[0], m1[0], EPS(), gp);
			sgcleft.Add(gp);
			sgcleft.SetColor(flagleft);
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, gp1[1], m1[0], EPS(), gp);
			sgcright.Add(gp);
			sgcright.SetColor(!flagleft);

			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, m1[1], m2[0], EPS(), gp);
			sgcleft1.Add(gp);
			sgcleft1.SetColor(false);
			FindGPointsCone(Line, MatrixOfTool, TotalDeriv, m1[1], m2[1], EPS(), gp);
			sgcright1.Add(gp);
			sgcright1.SetColor(true);
		}
	}
	delete gp;
	if(sgcleft1.Getkolgp())
		return FindSegmentsOfLowBases(Line, Line1, MatrixOfTool, TotalDeriv, f, fgpl, m2, gp2, sgcleft1.GetColor(), fl, segments);
	return FindSegmentsOfLowBases(Line, Line1, MatrixOfTool, TotalDeriv, f, fgpl, m2, gp2, flagleft, fl, segments);
}
Connect GrazingCurveElemContour::FindSegmentsGCurveHorisontal(	const NLine	&Line,	//линия текущего элемента контура
																const NLine	&Line1,	//линия элемента контура, следующего(ниже по оси Oz) за текущим
																const BMatrPair & MatrixOfTool,		//матрица инструмента
																const BMatr &TotalDeriv,		//полная производная матрицы движения по времени
																const bool  flagleft,			//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
																bool		&fl,				//цвет левых сегментов следующего элемента контура
																const AvailableTrace fgph,		//тип пересечения гр. кривой с верхним онованием
																const AvailableTrace fgpl,		//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
																const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
																const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
																SegmentGrazingCurve **segments)//указатель на 2 сегмента на верхнем основании следующего элемента контура	
{
	if(*segments)
	{
		sgclup.Copy((*segments)[0]);
		sgcrup.Copy((*segments)[1]);
	}
	BPoint M[2], d, Q;
	AvailableTrace f = FindGPointsHorizontal(false, Line, MatrixOfTool, TotalDeriv, d, Q, M);
	if(f != ABSENCE_TRACE)//если есть гр. точки на нижнем основании
	{
		if(fgph != ABSENCE_TRACE)//если есть гр. точки на верхнем основании
		{
			sgcleft.SetColor(flagleft);
			sgcright.SetColor(!flagleft);

			sgcleft.Add(gp1[0]);
			sgcleft.Add(M[0]);

			sgcright.Add(gp1[1]);
			sgcright.Add(M[1]);
		}
		else//если нет гр. точек на верхнем основании(значит, нижнее большего радиуса)
		{
			sgcleft1.SetColor(false);
			sgcright1.SetColor(true);

			sgcleft1.Add((M[0] + M[1])*0.5);
			sgcleft1.Add(M[0]);

			sgcright1.Add((M[0] + M[1])*0.5);
			sgcright1.Add(M[1]);		
		}
	}
	else//если нет гр. точек на нижнем основании
	{
		if(fgph != ABSENCE_TRACE)//если есть гр. точки на верхнем основании(значит, верхнее большего радиуса)
		{
			sgcleft.SetColor(flagleft);
			sgcright.SetColor(!flagleft);

			sgcleft.Add(gp1[0]);
			sgcleft.Add((gp1[0] + gp1[1])*0.5);

			sgcright.Add(gp1[1]);
			sgcright.Add((gp1[0] + gp1[1])*0.5);
		}
	}
	if(sgcleft1.Getkolgp())
	{
		Connect h = FindSegmentsOfLowBases(Line, Line1, MatrixOfTool, TotalDeriv, f, fgpl, M, gp2, sgcleft1.GetColor(), fl, segments);
		return h;
	}
	Connect h = FindSegmentsOfLowBases(Line, Line1, MatrixOfTool, TotalDeriv, f, fgpl, M, gp2, flagleft, fl, segments);
	return h;
}
Connect GrazingCurveElemContour::FindSegmentsGCurvesSphere(	const NLine	&Line,	//линия текущего элемента контура
															const NLine	&Line1,	//линия элемента контура, следующего(ниже по оси Oz) за текущим
															const BMatrPair & MatrixOfTool,		//матрица инструмента
															const BMatr &TotalDeriv,		//полная производная матрицы движения по времени
															const bool  flagleft,			//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
															bool		&fl,				//цвет левых сегментов следующего элемента контура
															AvailableTrace fgph,			//тип пересечения на верхнем оcновании текущего элемента контура
															const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
															const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
															const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
															SegmentGrazingCurve **segments)//указатель на 2 сегмента на верхнем основании следующего элемента контура
{
	if(*segments)
	{
		sgclup.Copy((*segments)[0]);
		sgcrup.Copy((*segments)[1]);
	}
	BPoint m1[2], m2[2], d, Q;//d, Q узнали в след.строчке., т.к. они одни для сферы
	BPoint k(0.0, 0.0, 1.0, 0.0);
	m1[0] = gp1[0];//т.к. на верхнем основании есть точки, то перезапишем их в массив верхних точек 
	m1[1] = gp1[1];
	AvailableTrace f = FindGPointsSphere(Line.p[3].GetZ(), Line, MatrixOfTool, TotalDeriv, d, Q, m2);

	if(d.D2() <= MIND*MIND)
	{
		BPoint E(0.0, 0.0, (Line.p[0].GetZ() + Line.p[3].GetZ())/2, 1.0);//Некая точка внутри части сферы на её оси(никогда не совпадет с центром сферы!)
		d = SpeedOfPointTool(MatrixOfTool, TotalDeriv, E);
		fgph = TWO_TRACE;
		f = TWO_TRACE;
		E.SetZ(Line.p[0].GetZ());
		CirclePlane(E, k, Q, d * (1/sqrt(d.D2())), pow(Line.p[0].GetX(), 2), m1);
		E.SetZ(Line.p[3].GetZ());
		CirclePlane(E, k, Q, d * (1/sqrt(d.D2())), pow(Line.p[3].GetX(), 2), m2);
	}
	double r = Line.GetRad();
	BPoint a = d % (k % d);//вектор (k % d) направлен в левую сторону 
	a = a * (r / sqrt(a.D2()));
	if((fgph == ABSENCE_TRACE || fgph == COUNTLESS_TRACE) && f != ABSENCE_TRACE)//если граничных точек на верхнем основании нет(или множество), а на нижнем есть
		m1[1] = m1[0] = Q + a;
	if((fgph != ABSENCE_TRACE || fgph != COUNTLESS_TRACE) && f == ABSENCE_TRACE)//если граничные точки на верхнем основании есть, а на нижнем нет//ERROR
		m2[1] = m2[0] = Q - a;

	NLine L;
	BPoint t[2];
	PointsOfSegmentsCircle(m1[0], (m1[0] - Q) % d, m2[0], (m2[0] - Q) % d, t);
	L.Set(m1[0], t[0], t[1], m2[0]);

	double alfa = (m1[0] - Q)*(m2[0] - Q)/(r*r);
	alfa = acos(alfa);
	alfa = alfa * 180/PI;
	int kol = NSurfSettings::GetNumAproxLine(r, alfa);//количество отрезков
	kol++;//количество точек

	BPoint	*p1 = new BPoint[kol],
			*p2 = new BPoint[kol];
	p1[0] = m1[0];
	p1[kol-1] = m2[0];
	p2[0] = m1[1];
	p2[kol-1] = m2[1];
	for(int i = 1; i < kol-1; i++)
	{
		BPoint A = L.GetPointFromParam(double(i)/double(kol-1));
		A.Norm();
		f = FindGPointsSphere(A.GetZ(), Line, MatrixOfTool, TotalDeriv, d, Q, t);
		p1[i] = t[0];
		p2[i] = t[1];		
	}
	sgcleft.Add(p1, kol);
	sgcleft.SetColor(flagleft);
	sgcright.Add(p2, kol);
	sgcright.SetColor(!flagleft);

	if(sgcleft1.Getkolgp())
		return FindSegmentsOfLowBases(Line, Line1, MatrixOfTool, TotalDeriv, f, fgpl, m2, gp2, sgcleft1.GetColor(), fl, segments);
	return FindSegmentsOfLowBases(Line, Line1, MatrixOfTool, TotalDeriv, f, fgpl, m2, gp2, flagleft, fl, segments);
}
void GrazingCurveElemContour::draw(void)
{
	sgcleft.draw(true);		//рисуем боковой левый верхний сегмент
	sgcright.draw(true);	//рисуем боковой правый верхний сегмент 
	sgcleft1.draw(true);	//рисуем боковой левый нижний сегмент
	sgcright1.draw(true);	//рисуем боковой правый нижний сегмент
	sgcldown.draw(false);	//рисуем нижний левый сегмент
	sgcrdown.draw(false);	//рисуем нижний правый сегмент
	sgclup.draw(false);		//рисуем верхний левый сегмент
	sgcrup.draw(false);		//рисуем верхний правый сегмент
}
const SegmentGrazingCurve &GrazingCurveElemContour::GetCont(bool Left, bool Top, bool Side) const
{
	int Key = (Left ? 4 : 0) + (Top ? 2 : 0) +  (Side ? 1 : 0);
	switch(Key)
	{
	case 0:
		return sgcrdown;
	case 1:
		return sgcright1;
	case 2:
		return sgcrup;
	case 3:
		return sgcright;
	case 4:
		return sgcldown;
	case 5:
		return sgcleft1;
	case 6:
		return sgclup;
	case 7:
		return sgcleft;
	}
	return sgcrdown;
}
bool GrazingCurveElemContour::GetColor(int num) const
{
	switch(num)
	{
	case 0:
		return sgcrdown.GetColor();
	case 1:
		return sgcright1.GetColor();
	case 2:
		return sgcrup.GetColor();
	case 3:
		return sgcright.GetColor();
	case 4:
		return sgcldown.GetColor();
	case 5:
		return sgcleft1.GetColor();
	case 6:
		return sgclup.GetColor();
	case 7:
		return sgcleft.GetColor();
	}
	return sgcrdown.GetColor();
}

void GrazingCurveElemContour::MakeCopy(GrazingCurveElemContour &Dest, const BMatr &M)
{
#define CopyI(Name) Name.Copy(Dest.Name, M);
	CopyI(sgcleft);
	CopyI(sgcright);
	CopyI(sgcleft1);
	CopyI(sgcright1);
	CopyI(sgcldown);
	CopyI(sgcrdown);
	CopyI(sgclup);
	CopyI(sgcrup);
	Dest.connect = connect;
}

void GrazingCurveElemContour::MakeCopy(GrazingCurveElemContour &Dest)
{
#define CopyP(Name) Dest.Name.Copy(Name);
	CopyP(sgcleft);
	CopyP(sgcright);
	CopyP(sgcleft1);
	CopyP(sgcright1);
	CopyP(sgcldown);
	CopyP(sgcrdown);
	CopyP(sgclup);
	CopyP(sgcrup);
	Dest.connect = connect;
}

void GrazingCurveElemContour::Clear()
{
	sgcleft.Clear();
	sgcright.Clear();
	sgcleft1.Clear();
	sgcright1.Clear();
	sgcldown.Clear();
	sgcrdown.Clear();
	sgclup.Clear();
	sgcrup.Clear();
}