#pragma once
#include "BAnglesPoint.h"
#include "BGrazingCurve.h"
#include "MVRDS.h"

//����� ����������� �������� ����������� � ������/����� �����
class AFX_EXT_CLASS BSurfaceElemTool//!!!
{
public:
	static double Angle(const BPoint & P);//����, ��������������� �����
	static CUIntArray * ModifyList(	
									const BMatrPair & MatrixOfTool,
									const double z,
									const double r,
									BAnglesPoint & ap0, 
									BAnglesPoint & ap1, 
									const int num1,
									const int num2 = -1);
	static bool IsVert(CUIntArray *n, const BMatrPair & MatrixOfTool, const bool f = true);
public:
	BSurfaceElemTool(void);
	~BSurfaceElemTool(void);

	int Getkol(void) const;
	//���������� ����� ��������� ����� i-������ �� BDCELP
	int Getbegin(int i) const;
	//���������� ������ ��������� ����� � i-������
	int Getbeginindex(int i) const;
	int Getb(int i) const;
	int Gete(int i) const;
	void Setkl(int k = 0){kl = k;}
	int Getkl(void) const {return kl;}

	BAnglesPoint ** GetAPoint(void) const;

	//Create lists surfaces

	//�������� ������� ������ �����
	BAnglesPoint * CreateListInLoop(	
										const NLine & Line,
										BGrazingCurveElemContour * bgc, 
										BGrazingCurveElemContour * next, 
										BAnglesPoint * p,
										const BMatrPair & MatrixOfTool,
										TypeLineInContour Type,
										bool f = true);
	//�������� ������� ��� �����
	BAnglesPoint * CreateListOutLoop(	
										const NLine & Line,
										BGrazingCurveElemContour * bgc, 
										BGrazingCurveElemContour * next, 
										BAnglesPoint * p,
										const BMatrPair & MatrixOfTool,
										TypeLineInContour Type,
										bool &modify,
										bool f = true);
	//�������� ������� ����������� �����
	BAnglesPoint * CreateListSphere(
									const NLine & Line, 
									BGrazingCurveElemContour & bgc, 
									BGrazingCurveElemContour * next,  
									BAnglesPoint * p, 
									const BMatrPair & MatrixOfTool,
									const BMatr & TotalDeriv,
									bool f = true);


	//Create lists full's surfaces

	//�������� ������� ������ ����������� �����������
	BAnglesPoint * CreateListHorizontal(const NLine & Line, BGrazingCurveElemContour & bgc, BGrazingCurveElemContour * next,  BAnglesPoint * p, const BMatrPair & MatrixOfTool);
	//�������� ������� ������ ����������� ������
	BAnglesPoint * CreateListCone(const NLine & Line, BGrazingCurveElemContour & bgc, BGrazingCurveElemContour * next,  BAnglesPoint * p, const BMatrPair & MatrixOfTool);
	

	//Create surfaces

	//�������� ������ ����������� �����������
	//void CreateSurfaceHorizontal(BDCELP & VRS, const NLine & Line) const;
	void CreateSurfaceHorizontal(NTriMesh *pMesh, const NLine & Line) const;
	//�������� ����������� ������, � �������� � ����� �� ������� ������ 1 �����(����� � ������� ����������)
	//void CreateSurfaceCone(BDCELP & VRS, const NLine & Line, const BMatrPair & MatrixOfTool) const;
	void CreateSurfaceCone(NTriMesh *pMesh, const NLine & Line) const;
	//�������� ����������� �����
	//MVRDS * CreateSurfaceSphere(BDCELP & VRS, const NLine & Line, const BMatrPair & MatrixOfTool) const;
	MVRDS * CreateSurfaceSphere(const NLine & Line, const BMatrPair & MatrixOfTool) const;

	//���������� ����� ����� � ������, ��� ������� ���������� ����������
	int UpdatingOfList(BAnglesPoint * bap, double r, const BMatrPair & MatrixOfTool, int b, int e = -1);
	//���������� ������ ����������� ������� �� ������ (��������� ����������)
	void Update(BGrazingCurve & bgc, int i, const BMatrPair & MatrixOfTool) const;
	//���������� �������� ������ ������� �� ������ (��������� ����������) 
	void Update(CUIntArray *segm, const BMatrPair & MatrixOfTool, bool f = true) const;

	//���������� ������ ������� ��������� ������, ������������ � ������� ��������� �������� �����������
	const CUIntArray * CreateSurfaceTri(MeshArr *pMeshArr, const BMatrPair & MatrixOfTool, const NLine & Line, TypeLineInContour Type, bool MakeTop, bool f) const;
	
	static int MakeToolSurface(const NLine & Line, TypeLineInContour Type, const BMatrPair & MatrixOfTool);
protected:
	BAnglesPoint ** APoint;
	int kl;		//���������� ��� �������
	int begin[4];//������ ������� ��������� ����� � ������� APoint ������� ����� �� MFOREST
	int b[4];	//������ ��������� ����� ���������� � �������, � ���. ����� ��������� �����
	int e[4];	//������ �������� ����� ���������� � �������, � ���. ����� ��������� �����
};
