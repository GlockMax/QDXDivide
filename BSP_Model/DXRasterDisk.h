#include "BPoint.h"
#include "MElemIDProc.h"
#include "DXDirection3X.h"
//������������ ����������� ������ ������

class DXRastDisk
{
public:
	///����������� ������������ ���������� ��������
	DXRastDisk(const BPoint & P0, const BPoint & P1, double Radius, DXDirection3X &DXDir);

	~DXRastDisk(void);

	///����� ������������ ������� �������� (������, ��������� ������ �����������)
	int RightCylinder(const BPoint & ArcCntr, DX_ID PairID);
	///����� ������������ �������� ������� �������� iS - ������ ������ ��������, iE - ������ ����� ��������
	int RightCylinderSegment(const BPoint & iS, const BPoint & iE, DX_ID PairID);
	///����� ������������ ���������� �������� (������� � ���������)
	int InclinedCylinder(const BPoint & N, DX_ID PairID);
	///����� ������������ ���������� ��������, ������������ � ��������� (������� � ���������)
	int DegradedCylinder(const BPoint & N, DX_ID PairID);
	///����� ������������ ����� �� ���� ����� (������� � ���������)
	int FlatDisk(const BPoint & N, DX_ID PairID);

private:

	union YSType
	{
		__int16 i;
		struct
		{//segments intersection flags
			bool RA1:1,LA1:1,RA2:1,LA2:1,S1:1,S2:1,SL:1,RA1Up:1,LA1Up:1,RA2Up:1,LA2Up:1;
		};
		enum pname {D,E,F,G,H,I,S,T,none} p;//point name flags
	};

	union XSType
	{
		char i;
		struct
		{bool S1:1,S2:1,SL:1,RA1:1,LA1:1,RA2:1,LA2:1;};
		struct
		{bool Up:1,Down:1;};
	};

	struct TYSeg
	{
		double y;
		YSType t;
	};

	static int cmpYSeg(const void* a,const void* b)
	{
		return (((TYSeg *)a)->y == ((TYSeg *)b)->y ? 0 : (((TYSeg *)a)->y > ((TYSeg *)b)->y ? 1 : -1));
	};

	struct TXSeg
	{
		double x;
		XSType t;
	};

	static int cmpXSeg(const void* a,const void* b)
	{
		return (((TXSeg *)a)->x == ((TXSeg *)b)->x ? 0 : (((TXSeg *)a)->x>((TXSeg *)b)->x ? 1 : -1));
	};

	DXDirection3X *DXD;//Pointer to caller class instance

	DX_ID ElID;

	const BPoint &A, &B;
	BPoint D,E,F,G,S,T;//Cylinder points
	double Hy,Iy;//Cylinder points, actually only Y coordinate
	bool HINeeded;//����� H � I ����������, ��� ��� ��������� �� ��������� � ������� ����
	double R,Ry;//cylinder radius and it's Y projection
	double Ymin, Ymax;
	BPoint Rv,Nv,Lv,Cv;//Rv = D-A, Nv - basement normal, Lv = B-A, Cv = Nv%Rv - from center of basement to center of arc
	BPoint Dv;//normal to plane, dividing upper and lower solutions
	BPoint Sv, Ev;//������� ������ � ����� �������� ��������
	BPoint Vertv,Horizv;
	/// Silhouette line presence
	bool SL;//������� ����� ������
	bool SLft,SRht;///����� ������ �� ����� ����/�� ������ ���� ��� �� �� �������
	bool S1up,S2up;//cylinder edge segments corresponding to upper or lower solution along Z axis
	bool S1Lft,S2Lft;//���������� ��� S1 � S2 �������� ������ ��� ������� ���������
	bool Cup;//������� ��������� ������������ ������� ��� ������ �������
	bool RA1Up,LA1Up,RA2Up,LA2Up;//left/right arc 1/2 solutions
	bool ArcDegraded;//��������� ���� ��� ���
	bool YminOnArc;//����� �� ������� �� Y �� ����

	TYSeg YSeg[8];//y segments types and coords
	TXSeg XSeg[8];//x segments types and coords

	double step0;
	int n0;

	void CommonInit();
	void CylSegmentInit();
	void DXRDProcessFlat();
	void DXRDMakeYSegments();
	void DXRDProcYLines();
	void DXRDProcYLinesFlat();
	void DXRDProcXSeg(unsigned int y, double Ycoord);
	void DXRDProcXSegFlat(unsigned int y, double Ycoord, int PairsNum);

	//find intersection points with circle with center A, radius R, normal to n with plane Y = const, P0x < P1x - solutions
	static void ArcSolver(const BPoint &A, const double R, const BPoint &n, const double Y, BPoint &P0, BPoint &P1);

	void RastDisk(const BPoint &Cp, const BPoint &N, const double Rad);

	//������ ��� ����������� ������� �� ������ ������
	void ICylSolverPrepare();
	void ICylSolverYPrepare(const double Ycoord);
	//ics for Inclined Cylinder Solver
	double icsC1,icsC2,icsA,icsB,icsC,icsD,icsE,icsE1,icsF, icsAinv;//global constants
	double icsG,icsH,icsI;//Y constants
};