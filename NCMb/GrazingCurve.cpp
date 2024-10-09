#include "stdafx.h"
#include "ConstDef.h"
#include "grazingcurve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

GrazingCurve::GrazingCurve(void)
: GCurve(NULL)
{
}
GrazingCurve::GrazingCurve(const int k)
{
	if(k > 0)
	{
		GCurve = new GrazingCurveElemContour [k];
		kol = k;
	}
	else
	{
		GCurve = NULL;
		kol = 0;
	}
}
GrazingCurve::~GrazingCurve(void)
{
	if(GCurve)
		delete [] GCurve;
}
bool GrazingCurve::FindGPNTool(const BMatrPair & MatrixOfTool, const BMatr & TotalDeriv, const NContour &Contour)
{	
	if(Contour.num <= 0)
		return false;

	bool	flagleft = false,	//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
			up = true,			//true - нужно создать сегменты на верхнем основании
			fl = false;			//цвет левых сегментов следующего элемента контура
	BPoint	a, j, k;			//орты осей инструмента
	a.Set(1.0, 0.0, 0.0, 0.0);
	j.Set(0.0, 1.0, 0.0, 0.0);
	k.Set(0.0, 0.0, 1.0, 0.0);
	
	delete[] GCurve;
	GCurve = new GrazingCurveElemContour [Contour.num];
	kol = Contour.num;
	NLine Line, Line1;
	Contour.GetLine(0, Line);
	Contour.GetLine(kol - 1, Line1);

	BPoint Q = Line.p[0];//центр сферы, касательной  поверхности инстр. в текущем(первом ?) элементе контура
	Q.SetX(0.0);
	BPoint q = Line1.p[3];//центр сферы, касательной  поверхности инстр. в нижнем(следующем)(последнем ?) элементе контура
	q.SetX(0.0);
	BPoint d = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q);//вектор скорости в точке Q
	BPoint sp = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, q);//вектор скорости в точке q
	double dD2 = d.D2();
	double spD2 = sp.D2();
	if (dD2 < MIND * MIND && spD2 < MIND * MIND)
		return false; // tool does not move
	SegmentGrazingCurve segments[2];
	SegmentGrazingCurve* buf = segments;// for compatibility with FindSegmentsGCurve
	if((d % k).D2() < MINAR * MINAR * dD2 && (sp % k).D2() < MINAR * MINAR * spD2)
	{
		double r = 0, z;
		int num = 0;

		segments[0].SetColor(false);
		segments[1].SetColor(true);

		BPoint temp[2], A, B;

		if(d.GetZ() < 0.0)//если скорость в СК инструмента направена "вниз" по z
		{
			z = Line1.p[3].GetZ();
			for(int i = Contour.num-1; i > -1; i--)//для каждого элемента контура простого инструмента
			{
				Contour.GetLine(i, Line1);
				if(Line1.p[0].GetX() > r+MIND)
				{
					r = Line1.p[0].GetX();
					z = Line1.p[0].GetZ();
					num = i;
				}
			}
		}
		else
		{
			z = Line.p[0].GetZ();
			for(int i = 0; i < Contour.num; i++)//для каждого элемента контура простого инструмента
			{
				Contour.GetLine(i, Line);
				if(Line.p[3].GetX() > r+MIND)
				{
					r = Line.p[3].GetX();
					z = Line.p[3].GetZ();
					num = i;
				}
			}
		}

		//Записываем левый сегмент
		A.Set(0.0, r, z, 1.0);
		B.Set(-r, 0.0,  z, 1.0);
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, a, B, j, temp);
		segments[0].Add(A, temp[0], temp[1], B);

		A.SetY(-r);
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, j, A, a, temp);
		segments[0].Add(B, temp[0], temp[1], A);

		//Записываем правый сегмент
		A.SetY(r);
		B.SetX(r);
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, a, B, j, temp);
		segments[1].Add(A, temp[0], temp[1], B);

		A.SetY(-r);
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, j, A, a, temp);
		segments[1].Add(B, temp[0], temp[1], A);

		if(d.GetZ() < 0.0)//если основание - верхнее у первого элемента контура
		{
			GCurve[num].SetSegment(6, segments[0]);//sgclup
			GCurve[num].SetSegment(7, segments[1]);//sgcrup
		}
		else
		{
			GCurve[num].SetSegment(5, segments[0]);//sgcrdown
			GCurve[num].SetSegment(4, segments[1]);//sgcldown
		}
		return true;
	}

	BPoint	gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
			gp2[2];				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
	AvailableTrace fgph = GCurve[0].FindGPoints(Line.p[0].GetZ(), Line, MatrixOfTool, TotalDeriv, d, Q, gp1, Contour.Types[0]);
	AvailableTrace fgpl = ABSENCE_TRACE;
	for(int i = 0; i < Contour.num; i++)//для каждого элемента контура простого инструмента
	{
		if(Contour.GetLine(i+1, Line1))
			fgpl = GCurve[i+1].FindGPoints(Line1.p[0].GetZ(), Line1, MatrixOfTool, TotalDeriv, sp, q, gp2, Contour.Types[i+1]);

		Connect flagup = GCurve[i].FindSegmentsGCurve(Line, Line1, MatrixOfTool, TotalDeriv, flagleft, fl, fgph, fgpl, gp1, gp2, Contour.Types[i]
			, static_cast<SegmentGrazingCurve**>(&buf));

		Line = Line1;

		switch(flagup)
		{
		case NOTUP://нет верхних дуг след. эл. конт. на основании, но связность есть
			flagleft = fl;//запомним цвет левых сегментов следующего элемента контура 
			break;
		case NOT://связности нет, верхних дуг нет
			flagleft = false;
			break;
		case YES://связность есть, верхние дуги есть
			if(fgpl == ABSENCE_TRACE)
				flagleft = false;
			else
				flagleft = fl;//запомним цвет левых сегментов следующего элемента контура 
			break;
		case YESUP://связности нет, но верхние дуги есть
			flagleft = false;
			break;
		};
		gp1[0] = gp2[0];//сделаем гр.точки верх.осн.след.эл.контура гр.точками верхнего основания следующего текущего эл.контура, т.е. его же
		gp1[1] = gp2[1];
		fgph = fgpl;	//запомним тип пересечения на вехнем основании для следующего элемента контура
		fgpl = ABSENCE_TRACE;
	}
	return true;
}
void GrazingCurve::draw(void)
{
	if(GCurve)
		for(int i = 0; i < kol; i++)
			GCurve[i].draw();
}
bool GrazingCurve::GetColor(int iElem, int num) const
{
	if(iElem < 0 || iElem > kol-1)
		return false;
	return GCurve[iElem].GetColor(num);
}

void GrazingCurve::MakeCopy(GrazingCurve &Dest, const BMatr &M) const
{
	for(int i = 0; i < kol; i++)
		GCurve[i].MakeCopy(Dest.GCurve[i], M);
}

void GrazingCurve::MakeCopy(GrazingCurve &Dest) const
{
	for(int i = 0; i < kol; i++)
		GCurve[i].MakeCopy(Dest.GCurve[i]);
}

INT_PTR GrazingCurve::GetConCompNum(void)
{
	// Get a number of connected components
	INT_PTR CCNum = 1;
	for (int i = 0; i < kol - 1; ++i)
		if (GCurve[i].GetConnect() == NOT || GCurve[i].GetConnect() == YESUP)
			++CCNum;
	return CCNum;
}
