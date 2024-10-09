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

	bool	flagleft = false,	//���� ����� ��������� �������� �������� �������: false - ����� ������ �� �������� '�������', true - '�������'
			up = true,			//true - ����� ������� �������� �� ������� ���������
			fl = false;			//���� ����� ��������� ���������� �������� �������
	BPoint	a, j, k;			//���� ���� �����������
	a.Set(1.0, 0.0, 0.0, 0.0);
	j.Set(0.0, 1.0, 0.0, 0.0);
	k.Set(0.0, 0.0, 1.0, 0.0);
	
	delete[] GCurve;
	GCurve = new GrazingCurveElemContour [Contour.num];
	kol = Contour.num;
	NLine Line, Line1;
	Contour.GetLine(0, Line);
	Contour.GetLine(kol - 1, Line1);

	BPoint Q = Line.p[0];//����� �����, �����������  ����������� �����. � �������(������ ?) �������� �������
	Q.SetX(0.0);
	BPoint q = Line1.p[3];//����� �����, �����������  ����������� �����. � ������(���������)(��������� ?) �������� �������
	q.SetX(0.0);
	BPoint d = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, Q);//������ �������� � ����� Q
	BPoint sp = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, q);//������ �������� � ����� q
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

		if(d.GetZ() < 0.0)//���� �������� � �� ����������� ��������� "����" �� z
		{
			z = Line1.p[3].GetZ();
			for(int i = Contour.num-1; i > -1; i--)//��� ������� �������� ������� �������� �����������
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
			for(int i = 0; i < Contour.num; i++)//��� ������� �������� ������� �������� �����������
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

		//���������� ����� �������
		A.Set(0.0, r, z, 1.0);
		B.Set(-r, 0.0,  z, 1.0);
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, a, B, j, temp);
		segments[0].Add(A, temp[0], temp[1], B);

		A.SetY(-r);
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, j, A, a, temp);
		segments[0].Add(B, temp[0], temp[1], A);

		//���������� ������ �������
		A.SetY(r);
		B.SetX(r);
		GrazingCurveElemContour::PointsOfSegmentsCircle(A, a, B, j, temp);
		segments[1].Add(A, temp[0], temp[1], B);

		A.SetY(-r);
		GrazingCurveElemContour::PointsOfSegmentsCircle(B, j, A, a, temp);
		segments[1].Add(B, temp[0], temp[1], A);

		if(d.GetZ() < 0.0)//���� ��������� - ������� � ������� �������� �������
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

	BPoint	gp1[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������� �������
			gp2[2];				//���. � ����. ��. ����� �� ������� ��������� �������� �������, ���. ���� �������� �� Oz
	AvailableTrace fgph = GCurve[0].FindGPoints(Line.p[0].GetZ(), Line, MatrixOfTool, TotalDeriv, d, Q, gp1, Contour.Types[0]);
	AvailableTrace fgpl = ABSENCE_TRACE;
	for(int i = 0; i < Contour.num; i++)//��� ������� �������� ������� �������� �����������
	{
		if(Contour.GetLine(i+1, Line1))
			fgpl = GCurve[i+1].FindGPoints(Line1.p[0].GetZ(), Line1, MatrixOfTool, TotalDeriv, sp, q, gp2, Contour.Types[i+1]);

		Connect flagup = GCurve[i].FindSegmentsGCurve(Line, Line1, MatrixOfTool, TotalDeriv, flagleft, fl, fgph, fgpl, gp1, gp2, Contour.Types[i]
			, static_cast<SegmentGrazingCurve**>(&buf));

		Line = Line1;

		switch(flagup)
		{
		case NOTUP://��� ������� ��� ����. ��. ����. �� ���������, �� ��������� ����
			flagleft = fl;//�������� ���� ����� ��������� ���������� �������� ������� 
			break;
		case NOT://��������� ���, ������� ��� ���
			flagleft = false;
			break;
		case YES://��������� ����, ������� ���� ����
			if(fgpl == ABSENCE_TRACE)
				flagleft = false;
			else
				flagleft = fl;//�������� ���� ����� ��������� ���������� �������� ������� 
			break;
		case YESUP://��������� ���, �� ������� ���� ����
			flagleft = false;
			break;
		};
		gp1[0] = gp2[0];//������� ��.����� ����.���.����.��.������� ��.������� �������� ��������� ���������� �������� ��.�������, �.�. ��� ��
		gp1[1] = gp2[1];
		fgph = fgpl;	//�������� ��� ����������� �� ������ ��������� ��� ���������� �������� �������
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
