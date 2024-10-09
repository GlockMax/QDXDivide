#pragma once
#include "BAnglesArray.h"
#include "MBSPForest.h"
#include "BMatrPair.h"
//#include "BDCELP.h"

class AFX_EXT_CLASS BAnglesPoint ://!!!
	public BAnglesArray
{
public:
	static BPoint RotatePoint(double angle, const BPoint & B);
public:
	BAnglesPoint(void);
	BAnglesPoint(const BAnglesPoint & ap);
	virtual ~BAnglesPoint(void);

	void Copy(const BAnglesPoint * ap);
	void Copy(const CUIntArray * n);
	const CUIntArray * GetList(void) const {return &nlist;};

	//b, e - ������ ����� � MFOREST
	int		Insert(const BPoint & P, const BMatrPair & MatrixOfTool);							//������ �����
	int		Insert(const BPoint & P, const BMatrPair & MatrixOfTool, int b, int e = -1);		//������ ����� � ��������
	int		Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool, double angle);					//������� �������� ������� �����
	int		Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool);					//������� �������� ������� �����
	void	Insert(CUIntArray *n, const BMatrPair & MatrixOfTool);							//������� ������� �����
	void	Insert(CUIntArray *n, const BMatrPair & MatrixOfTool, int b, int e = -1);			//������� ������� ����� � ��������
	int		Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool, int b, int e = -1);	//������� �������� ������� ����� � ��������
	int		Insert(int num, const BMatrPair & MatrixOfTool);									//������� ����� �� ������ � MFOREST
	int		Insert(int num, double angle, const BMatrPair & MatrixOfTool);									//������� ����� �� ������ � MFOREST
	int		Insert(CUIntArray *n, INT_PTR ind1, INT_PTR ind2, const BMatrPair & MatrixOfTool);		//������� ��������� ��������� ������� (ind1,ind2 - ������� ��������� �������)

	static bool IsVert(CUIntArray *n, const BMatrPair & MatrixOfTool, const bool f = true);//���������� �� �� ���� �������

	int GetNumBDCELPPoint(int index) const;
	int GetNumForestPoint(int index) const;
	double GetAt(int index, bool flag = false) const;
	//���������� �����. index - ������ � ������ nlist
	BPoint GetPoint(int index, const BMatrPair & MatrixOfTool) const ;
	CUIntArray * Getlist(void) const;
	CUIntArray * Getlist(CUIntArray &list) const;
	CUIntArray * Getlist(int indb, int inde) const;
	//���������� ������ nlist � ���� ��������(������ ������� �����) � BGrazingCurveElemContour
	void Getsegm(CUIntArray * n, const BMatrPair & MatrixOfTool);
	//������ ������� ������� ����� � ������ ������� nlist
	void Setlist(CUIntArray *n, const BMatrPair & MatrixOfTool, INT_PTR n1 = 0, INT_PTR n2 = 0);
	void SetPoint(const BPoint & P, const BMatrPair & MatrixOfTool, int index = 0);
	//�������� �������� ���� � �������� �� -180 �� 180 ��������
	static double Normalize(double angle);
	//return index in list 'nlist'. num - number in MFOREST
	int Find(int num) const;
	INT_PTR GetCount(void) const {return nlist.GetCount();}
	bool IsEmpty(void) const;
protected:
	CUIntArray nlist;//������ ������� ����� � MFOREST, ������������� �� ����
};
