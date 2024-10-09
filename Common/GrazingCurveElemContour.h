#pragma once
#include "NCMB_API.h"
#include "BMatrPair.h"
#include "NLine.h"
#include "NContour.h"
#include "SegmentGrazingCurve.h"

enum AvailableTrace//������� �����������
{
	COUNTLESS_TRACE = 0,//���������
	ABSENCE_TRACE = 1,	//����������
	ONE_TRACE = 2,		//����
	TWO_TRACE = 4		//���
};
enum Connect//������� ���������� ��. ������ ����� ���������� �������
{
	NOT = -1,//��������� ���, ������� ��� ���
	NOTUP = 0,//��������� ����, ������� ��� ���
	YES = 1,//��������� ����, ������� ���� ����
	YESUP = 2//��������� ���, ������� ���� ����
};

class NCMB_API GrazingCurveElemContour  //��������� ������ ������ �������� ������� �������� �����������
{
	friend class BGrazingCurveElemContour;
public:
	GrazingCurveElemContour(void);
	~GrazingCurveElemContour(void);
	bool SetSegment(unsigned int i, const SegmentGrazingCurve segment);

	/* ��� �������, ������������ � ������, ������ ���� ����������! */

	/*����� ����� ����������� ���� ������(� ������ ����������� ������������ ������� ����� ��������� ����� �� ������)*/
	static BPoint TraceOfLines(const BPoint &P0, const BPoint &P1, const BPoint &Q0, const BPoint &Q1);
	/*����� ����� ����������� ������ � ���������. NULL, ���� ������ �� ���������� ��� ����������� ���������*/
	static AvailableTrace LinePlane(const BPoint &P, const BPoint &Vector, const BPoint &Q, const BPoint &Normal, BPoint &M);
	/*����� ����� ����������� ���� ���������(�����, ������). P - ����� �����������*/
	static AvailableTrace PlanePlane(const BPoint &P1, const BPoint &Normal1, const BPoint &P2, const BPoint &Normal2, BPoint P[2]);
	/*����� ����� ����������� ���������� (O,Normal1) � ����������(P,Normal2)*/
	static AvailableTrace CirclePlane(const BPoint &O, const BPoint &Normal1, const BPoint &P, const BPoint &Normal2, const double r2, BPoint M[2]);
	/*����� ������ � ������� ����� ����������������� ������� ��� ���� ���������� ��� ��������� � �������� ����� � ������� ������� � ���*/
	static void PointsOfSegmentsCircle(const BPoint &P0, const BPoint &a, const BPoint &P3, const BPoint &b, BPoint m[2]);
	/*����� ������� �������� � ����� � �� �����������*/
	static BPoint SpeedOfPointTool(	const BMatrPair & MatrixOfTool, //������� �����������
									const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
									const BPoint & P);			//����� � �� �����������
	/*����� ��������� ����� �� ���������� ������*/
	static AvailableTrace  FindGPointOfLine(	const BMatrPair & MatrixOfTool,			//������� �����������
												const BMatr & TotalDeriv,			//������ ����������� ������� �������� �� �������
												const BPoint & A, const BPoint & B,	//����� � �� �����������
												const BPoint & Normal,				//������� � ���������� ������
												BPoint & P);	//��������� ����� �� ���������� ������
	/*����� ������� � ����� �� ����������� ������*/
	static BPoint NormalToSurfaceOfPoint(	const NLine & Line,		//����� �������� �������
											const BPoint & P);		//�����, � ������� ������� ������� � ����������� ������
	/*����� ������� � ����� ������� �������� �����������*/
	static BPoint NormalToSurfaceOfLine(const NLine & Line,	//����� � �� �����������
										const double t);	//�������� �������������� ������
	/*����� ������������ ������� � ����� ������� �������� �����������*/
	static BPoint KasatToSurfaceOfLine(	const NLine & Line,	//����� � �� �����������
										const double t);	//�������� �������������� ������
	/*����� �������� ���������� �� ����� �� ������*/
	static double PointLine2(	const BPoint & A,	//1 ����� ������
								const BPoint & B,	//2 ����� ������
								const BPoint & P);	//�����, �� ������� ���� ���������� �� ������
	/*����� �������� ����� �� ������*/
	static BPoint PointProjLine(const BPoint & A,
								const BPoint & B,
								const BPoint & P);
         
	/*����� ������ ��.����� �� ������ z �� ��� Oz*/
	AvailableTrace FindGPoints(	double z,					//���������� �� ��� Oz, ��� ���. ���� ��. ����� 
								const NLine & Line,			//����� �������� �������
								const BMatrPair & MatrixOfTool, //������� �����������
								const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
								BPoint &d,					//������ ������ �������� � ����� Q � �� �����������
								BPoint &Q,					//����� �����, �����������  ����������� �����. ����� ������� ����� ��������� z
								BPoint M[2],				//������� ��. ����� ��� ������� z
								TypeLineInContour Type);	//��� ����� �������� �������								

	/*����� ������ ��������� ��������� ������*/
	Connect FindSegmentsGCurve(	const NLine	&Line,	//����� �������� �������� �������
								const NLine	&Line1,	//����� �������� �������, ����������(���� �� ��� Oz) �� �������
								const BMatrPair &MatrixOfTool,			//������� �����������
								const BMatr &TotalDeriv,			//������ ����������� ������� �������� �� �������
								const bool  flagleft,				//���� ����� ��������� �������� �������� �������: false - ����� ������ �� �������� '�������', true - '�������'
								bool		&fl,					//���� ����� ��������� ���������� �������� �������
								AvailableTrace fgph,				//��� ����������� �� ������� �c������� �������� �������� �������
								const AvailableTrace fgpl,			//��� ����������� �� ������� �c������� �������� �������, ���. ���� �������� �� Oz
								const BPoint *gp1,					//���. � ����. ��. ����� �� ������� ��������� �������� �������� �������
								const BPoint *gp2,					//���. � ����. ��. ����� �� ������� ��������� �������� �������, ���. ���� �������� �� Oz
								TypeLineInContour Type,				//��� ����� �������� �������
								SegmentGrazingCurve **segments);	//��������� �� 2 �������� �� ������� ��������� ���������� �������� �������				

	/*����� ������ ����� �� ������: ������� �� ����� ����� ������� �� ������ ��������� �������� ��. ������� ��.������(������������ ������������� �������). true - ��, false - ���*/
	bool TransSign(	const NLine	&Line,			//����� �������� �������� �������
					const NLine	&Line1,			//����� �������� �������, ����������(���� �� ��� Oz) �� �������
					const BMatrPair & MatrixOfTool,	//������� �����������
					const BMatr &TotalDeriv,	//������ ����������� ������� �������� �� �������
					const BPoint &D,			//����� �� ���� �� 
					bool &sign);				//true - ���� ��������, false - ���

	/*����� ��������� ��. ������ �� ���� ������� ���������. ���� ����� ��������� ���������� ��.������� ���������� � fl. true - ����� ��������� �� ��������� ���������, false - ���������� ��������� ������ �����������*/
	Connect FindSegmentsOfLowBases(	const NLine	&Line,					//����� �������� �������� �������
									const NLine	&Line1,					//����� �������� �������, ����������(���� �� ��� Oz) �� �������
									const BMatrPair & MatrixOfTool,			//������� �����������
									const BMatr &TotalDeriv,			//������ ����������� ������� �������� �� �������
									const AvailableTrace fgph,			//��� ����������� �� ������ �c������� �������� �������� �������
									const AvailableTrace fgpl,			//��� ����������� �� ������� �c������� �������� �������, ���. ���� �������� �� Oz
									const BPoint gp1[2],				//���� �� �����, ��������������� �� ���������� �� ��������� 
									const BPoint gp2[2],				//����� ���������� �������� �������. 
									const bool flagleft,				//���� sgcleft(sgcleft1 ���� ����)
									bool &fl,							//���� ����� ��������� ���������� �������� �������
									SegmentGrazingCurve **segments);	//��������� �� 2 �������� �� ������� ��������� ���������� �������� �������					

	/*����� ���� ��������� ����� ��� ����������� z ��� �������� ������� ���� CONE*/
	AvailableTrace FindGPointsCone(	double z,					//���������� �� ��� Oz, ��� ���. ���� ��. ����� 
									const NLine & Line,			//����� �������� �������
									const BMatrPair & MatrixOfTool, //������� �����������
									const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
									BPoint &d,					//������ ������ �������� � ����� Q � �� �����������(������������)
									BPoint &Q,					//����� �����, �����������  ����������� �����(������������). ����� ������� ����� ��������� z
									BPoint M[2]);				//������� ��. ����� ��� ������� z

	/*����� ���������� �������� ����� "�������" � "������" ������� ��� �������� ������� ���� CONE*/
	void FindGPointsCone(	const NLine & Line,			//����� �������� �������
							const BMatrPair & MatrixOfTool, //������� �����������
							const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
							const BPoint &A,				//"�������" ����� �� �� ����������� ������
							const BPoint &B,				//"������" ����� �� �� ����������� ������
							const double eps,			//�����������
							GPoints *gp);				//��������� ����� �������� ������
	/*����� ���� ��������� ����� ��� ����������� z ��� �������� ������� ���� HORIZONTAL*/
	AvailableTrace FindGPointsHorizontal(	bool up,					//true - ���� ��. ����� �� ������� ���������, false - �� ������ ���������
											const NLine & Line,			//����� �������� �������
											const BMatrPair & MatrixOfTool, //������� �����������
											const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
											BPoint &d,					//������ ������ �������� � ����� Q � �� �����������(������������)
											BPoint &Q,					//����� �����, �����������  ����������� �����(������������). ����� ������� ����� ��������� z
											BPoint M[2]);				//������� ��. ����� 

	/*����� ���� ��������� ����� ��� ����������� z ��� �������� ������� ���� SPHERE*/
	AvailableTrace FindGPointsSphere(	double z,					//���������� �� ��� Oz, ��� ���. ���� ��. ����� 
										const NLine & Line,			//����� �������� �������
										const BMatrPair & MatrixOfTool, //������� �����������
										const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
										BPoint &d,					//������ ������ �������� � ����� Q � �� �����������(������������)
										BPoint &Q,					//����� �����, �����������  ����������� �����(������������). ����� ������� ����� ��������� z
										BPoint M[2]);				//������� ��. ����� ��� ������� z

	/*����� ���� ��������� ����� ��� ����������� z ��� �������� ������� ���� TORAHS � ������ � d ������� ������ �������� � �� �����.*/
	AvailableTrace FindGPointsTorahs(	double z,					//���������� �� ��� Oz, ��� ���. ���� ��. ����� 
										const NLine & Line,			//����� �������� �������
										const BMatrPair & MatrixOfTool, //������� �����������
										const BMatr & TotalDeriv,	//������ ����������� ������� �������� �� �������
										BPoint &d,					//������ ������ �������� � ����� Q � �� �����������(������������)
										BPoint &Q,					//����� �����, �����������  ����������� �����(������������). ����� ������� ����� ��������� z
										BPoint M[2]);				//������� ��. ����� ��� ������� z

	/*����� ������������� ����� �� ��������� ������ � ���� ����� ��� �������� ������� ���� CONE*/
	AvailableTrace FindExtremumPointCone(const NLine & Line, const BMatrPair & MatrixOfTool, const BMatr & TotalDeriv, BPoint E[2]);

	/*����� ��������� ��������� ������ �� ���������� �������� �������: true - ����� ��������� �� ��������� ���������, false - ���������� ��������� ������ �����������(�����)*/
	Connect FindSegmentsGCurveCone(	const NLine	&Line,					//����� �������� �������� �������
									const NLine	&Line1,					//����� �������� �������, ����������(���� �� ��� Oz) �� �������
									const BMatrPair & MatrixOfTool,			//������� �����������
									const BMatr &TotalDeriv,			//������ ����������� ������� �������� �� �������
									const bool  flagleft,				//���� ����� ��������� �������� �������� �������: false - ����� ������ �� �������� '�������', true - '�������'
									bool		&fl,					//���� ����� ��������� ���������� �������� �������
									const AvailableTrace fgph,			//��� ����������� ��. ������ � ������� ���������
									const AvailableTrace fgpl,			//��� ����������� �� ������� �c������� �������� �������, ���. ���� �������� �� Oz
									const BPoint gp1[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������� �������
									const BPoint gp2[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������, ���. ���� �������� �� Oz
									SegmentGrazingCurve **segments);		//��������� �� 2 �������� �� ������� ��������� ���������� �������� �������	

	/*����� ��������� ��������� ������ �� ���������� �������� �������: true - ����� ��������� �� ��������� ���������, false - ���������� ��������� ������ �����������*/
	Connect FindSegmentsGCurveHorisontal(	const NLine	&Line,					//����� �������� �������� �������
											const NLine	&Line1,					//����� �������� �������, ����������(���� �� ��� Oz) �� �������
											const BMatrPair & MatrixOfTool,			//������� �����������
											const BMatr &TotalDeriv,			//������ ����������� ������� �������� �� �������
											const bool  flagleft,				//���� ����� ��������� �������� �������� �������: false - ����� ������ �� �������� '�������', true - '�������'
											bool		&fl,					//���� ����� ��������� ���������� �������� �������
											const AvailableTrace fgph,			//��� ����������� ��. ������ � ������� ���������
											const AvailableTrace fgpl,			//��� ����������� �� ������� �c������� �������� �������, ���. ���� �������� �� Oz
											const BPoint gp1[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������� �������
											const BPoint gp2[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������, ���. ���� �������� �� Oz
											SegmentGrazingCurve **segments);	//��������� �� 2 �������� �� ������� ��������� ���������� �������� �������	

	/*����� ��������� ��������� ������ �� ����������� �������� �������: true - ����� ��������� �� ��������� ���������, false - ���������� ��������� ������ �����������*/
	Connect FindSegmentsGCurvesSphere(	const NLine	&Line,					//����� �������� �������� �������
										const NLine	&Line1,					//����� �������� �������, ����������(���� �� ��� Oz) �� �������
										const BMatrPair & MatrixOfTool,			//������� �����������
										const BMatr &TotalDeriv,			//������ ����������� ������� �������� �� �������
										const bool  flagleft,				//���� ����� ��������� �������� �������� �������: false - ����� ������ �� �������� '�������', true - '�������'
										bool		&fl,					//���� ����� ��������� ���������� �������� �������
										AvailableTrace fgph,				//��� ����������� �� ������� �c������� �������� �������� �������
										const AvailableTrace fgpl,			//��� ����������� �� ������� �c������� �������� �������, ���. ���� �������� �� Oz
										const BPoint gp1[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������� �������
										const BPoint gp2[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������, ���. ���� �������� �� Oz
										SegmentGrazingCurve **segments);	//��������� �� 2 �������� �� ������� ��������� ���������� �������� �������

	/*����� ��������� ��������� ������ �� ���������� �������� �������: true - ����� ��������� �� ��������� ���������, false - ���������� ��������� ������ �����������*/
	Connect FindSegmentsGCurvesTorahs(	const NLine	&Line,	//����� �������� �������� �������
										const NLine	&Line1,	//����� �������� �������, ����������(���� �� ��� Oz) �� �������
										const BMatrPair & MatrixOfTool,			//������� �����������
										const BMatr &TotalDeriv,			//������ ����������� ������� �������� �� �������
										const bool  flagleft,				//���� ����� ��������� �������� �������� �������: false - ����� ������ �� �������� '�������', true - '�������'
										bool		&fl,					//���� ����� ��������� ���������� �������� �������
										const AvailableTrace fgph,			//��� ����������� �� ������� �c������� �������� �������� �������
										const AvailableTrace fgpl,			//��� ����������� �� ������� �c������� �������� �������, ���. ���� �������� �� Oz
										const BPoint gp1[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������� �������
										const BPoint gp2[2],				//���. � ����. ��. ����� �� ������� ��������� �������� �������, ���. ���� �������� �� Oz
										SegmentGrazingCurve **segments);	//��������� �� 2 �������� �� ������� ��������� ���������� �������� �������
	void draw(void);
	const SegmentGrazingCurve &GetCont(bool Left, bool Top, bool Side) const;
	Connect GetConnect(void) const {return connect;}
	bool GetColor(int num) const;
	void MakeCopy(GrazingCurveElemContour &Dest, const BMatr &M);
	void MakeCopy(GrazingCurveElemContour &Dest);
	void Clear();

protected:
	SegmentGrazingCurve sgcleft,	//������� ����� ������� �������
						sgcright,	//������� ������ ������� ������� 
						sgcleft1,	//������� ����� ������ �������
						sgcright1,	//������� ������ ������ �������
						sgcldown,	//������ ����� �������
						sgcrdown,	//������ ������ �������
						sgclup,		//������� ����� �������
						sgcrup;		//������� ������ �������
	Connect connect;
public:
	static double EPS() {return NSurfSettings::GetTolerance();}
};
