#include "stdafx.h"
#include <crtdbg.h>
#include <math.h>
#include "BAnglesPoint.h"
#include "ConstDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const BPoint i(1.0, 0.0, 0.0, 0.0);

BAnglesPoint::BAnglesPoint(void)
{
}
BAnglesPoint::BAnglesPoint(const BAnglesPoint & ap)
{
	Copy(&ap);
}
BAnglesPoint::~BAnglesPoint(void)
{
}
void BAnglesPoint::Copy(const BAnglesPoint * ap)
{
	if(ap)
	{
		const CUIntArray * list = ap->GetList();
		INT_PTR kol = list->GetSize();
		for(int i = 0; i < kol; i++)
		{
			nlist.Add(list->GetAt(i));
			Add(ap->GetAt(i));
		}
	}
}
void BAnglesPoint::Copy(const CUIntArray * n)
{
	if(n)
	{
		INT_PTR kol = n->GetSize();
		for(int i = 0; i < kol; i++)
			nlist.Add(n->GetAt(i));
	}
}
BPoint BAnglesPoint::RotatePoint(double angle, const BPoint & B)
{
	BMatr R;
	R.rotg(angle, BPoint(0.0, 0.0, 0.0, 1.0), BPoint(0.0, 0.0, 1.0, 0.0));
	return (B * R).Norm();
}
int BAnglesPoint::Insert(const BPoint & P, const BMatrPair & MatrixOfTool)//??? �������� ��:  ����, z, R
{
	BPoint v = P;
	v.Norm();
	v.SetH(0.0);
	v.ProjXY();
	double angle = Normalize(i.Angle(v));
	int index = 0;
	if(GetSize() > 0)
	{
		index = NSortedArray::Find(angle);
		if(index > -1)//���� ����� ���� ��� ����
			return index;
	}
	index = NSortedArray::Insert(angle);
	SetPoint(P, MatrixOfTool, index);
	return index;
}
int BAnglesPoint::Insert(const BPoint & P, const BMatrPair & MatrixOfTool, int b, int e)
{
	BPoint i, v = P;
	i.Set(1.0, 0.0, 0.0, 0.0);
	v.Norm();
	v.SetH(0.0);
	v.ProjXY();
	double angle = 0.0;
	if (P.D2() > 1.e-12)
	{
		angle = i.Angle(v);
		angle = Normalize(angle);
	}
	INT_PTR kol = GetSize();
	if(b < 0)
	{
		if(e < 0)
			return -1;
		return Insert(P, MatrixOfTool);
	}
	b = Find(b);
	if(b < 0)
		return -1;//�� ����� ������ ���������
	if(e < 0)
		return Insert(P, MatrixOfTool);
	else
	{
		e = Find(e);
		if(e < 0)
			return -1;//�� ����� ����� ���������
	}
	if(IsBetween(angle, GetAt(b)-MINAD, GetAt(e)+MINAD))
		return Insert(P, MatrixOfTool);
	return -1;//�� ������ � ��������
}
int BAnglesPoint::Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool, double angle)
{
	int num = n->GetAt(in);//����� ����� � BDCELP
	int index = Insert(num, angle, MatrixOfTool);
	if (nlist.GetAt(index) != num)//���� ����� ����������� ����� �� ��������� � ������� ����������� ����� c ��� �� �����
		n->SetAt(in, nlist.GetAt(index));//�������� ����� � ������� �������
	return index;
}
int BAnglesPoint::Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool)
{
	int num = n->GetAt(in);//����� ����� � BDCELP
	int index = Insert(num, MatrixOfTool);
	if (nlist.GetAt(index) != num)//���� ����� ����������� ����� �� ��������� � ������� ����������� ����� c ��� �� �����
		n->SetAt(in, nlist.GetAt(index));//�������� ����� � ������� �������
	return index;
}
void BAnglesPoint::Insert(CUIntArray *n, const BMatrPair & MatrixOfTool)
{
	if(n)
	{
		INT_PTR kol = n->GetSize();
		for(int i = 0; i < kol; i++)
			Insert(n, i, MatrixOfTool);
	}
}
void BAnglesPoint::Insert(CUIntArray *n, const BMatrPair & MatrixOfTool, int b, int e)
{
	if(n)
	{
		INT_PTR kol = n->GetSize();
		for(int i = 0; i < kol; i++)
			Insert(n, i, MatrixOfTool, b, e);
	}
}
int BAnglesPoint::Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool, int b, int e)
{
	if(!n)
		return -1;
	if(b < 0)
	{
		if(e < 0)
			return -1;
		return Insert(n, in, MatrixOfTool);
	}
	b = Find(b);
	if(b < 0)
		return -1;//�� ����� ������ ���������
	if(e < 0)
		e = (b+GetSize()-1) % GetSize();
	else
	{
		e = Find(e);
		if(e < 0)
			return -1;//�� ����� ����� ���������
	}
	BPoint i(1.0, 0.0, 0.0, 0.0), P;
	P = (*MFOREST.GetPoint(n->GetAt(in))) * MatrixOfTool.Inv();
	P.Norm();
	P.SetH(0.0);
	P.ProjXY();
	double angle = 0.0;
	if (P.D2() > 1.e-12)
	{
		angle = i.Angle(P);
		angle = Normalize(angle);
	}
	if (IsBetween(angle, GetAt(b) - MINAD, GetAt(e) + MINAD))
		return Insert(n, in, MatrixOfTool, angle);
	return -1;//�� ������ � ��������
}
int BAnglesPoint::Insert(int num, const BMatrPair & MatrixOfTool)
{
	//num - ����� � BDCELP
	BPoint i(1.0, 0.0, 0.0, 0.0), P;
	if(num < 0)//����� � MFOREST
		return -1;//����� �� ����� ���� �������������
	P = (*MFOREST.GetPoint(num)) * MatrixOfTool.Inv();
	P.Norm();
	P.SetH(0.0);
	P.ProjXY();
	double angle = 0.0;
	if(P.D2() > 1.e-12)
	{
		angle = i.Angle(P);
		angle = Normalize(angle);
	}
	int index = NSortedArray::Find(angle);
	if(index > -1)//���� ����� ���� ��� ����
		return index;

	index = NSortedArray::Insert(angle);
	nlist.InsertAt(index, num);
	return index;
}
int BAnglesPoint::Insert(int num, double angle, const BMatrPair & MatrixOfTool)
{
	//num - ����� � BDCELP
	BPoint i(1.0, 0.0, 0.0, 0.0), P;
	int index = NSortedArray::Find(angle);
	if(index > -1)//���� ����� ���� ��� ����
		return index;

	index = NSortedArray::Insert(angle);
	nlist.InsertAt(index, num);
	return index;
}
int BAnglesPoint::Insert(CUIntArray *n, INT_PTR ind1, INT_PTR ind2, const BMatrPair & MatrixOfTool)
{
	//n - ������ BDCELP-�������. 
	//ind1, ind2 - ������� � ������� n, �������� ����� �������� ����������� � ������, ������������� �� ����
	if(n)
	{
		INT_PTR kol = n->GetSize();
		if(ind1 > -1 && ind1 < kol && ind2 > -1 && ind2 < kol )
		{
			for(auto i = ind1; i%kol != (ind2+1)%kol; i++)
				Insert(n->GetAt(i), MatrixOfTool);
			return 1;
		}
		return -1;//������������ ������
	}
	return -2;//�������� ������ ������
}
bool BAnglesPoint::IsVert(CUIntArray *n, const BMatrPair & MatrixOfTool, const bool f)
{
	//���� f=true, �� n - ������ "��������" ��������
	//f=false - "�������"
	double a, b;
	BPoint A, B;
	if(f)
	{
		A = (*MFOREST.GetPoint(n->GetAt(0))) * MatrixOfTool.Inv();
		B = (*MFOREST.GetPoint(n->GetAt(1))) * MatrixOfTool.Inv();
	}
	else
	{
		A = (*MFOREST.GetPoint(n->GetAt(n->GetUpperBound() - 1))) * MatrixOfTool.Inv();
		B = (*MFOREST.GetPoint(n->GetAt(n->GetUpperBound()))) * MatrixOfTool.Inv();
	}
	A.SetH(0.0);
	B.SetH(0.0);
	A.ProjXY();
	B.ProjXY();
	if(A.D2() < 1.e-12 || B.D2() < 1.e-12)//���� ���� �� ����� �� ������� ���������
		return true;
	a = i.Angle(A);
	b = i.Angle(B);
	if(fabs(Normalize(GetDelta(a, b))) < MINAD)
		return true;
	return false;
}
int BAnglesPoint::GetNumBDCELPPoint(int index) const
{
	//index - ������ � ������ nlist
	return nlist.GetAt(index);
}
int BAnglesPoint::GetNumForestPoint(int index) const
{
	//���������� MFOREST-�����
	//index - ���������� ����� � ������ nlist
	int NumVRS = nlist.GetAt(index);	//����� BDCELP-����� � ������
	return NumVRS;//VRS.GetNvInForest(NumVRS);	//����� MFOREST-����� � BDCELP
}
double BAnglesPoint::GetAt(int index, bool flag) const
{
	//index - ������ � ������ nlist
	//flag = false - ������� ���� �� ��������������� ������
	if(flag)
		index = int(nlist.GetSize()) - index - 1;
	return CArray::GetAt(index);
}
BPoint BAnglesPoint::GetPoint(int index, const BMatrPair & MatrixOfTool) const
{
	//���������� ����� � ������� ��������� �����������
	//int num = GetNumForestPoint(VRS, index);
	int num = GetNumForestPoint(index);
	return (*MFOREST.GetPoint(num)) * MatrixOfTool.Inv();
}
CUIntArray * BAnglesPoint::Getlist(void) const
{
	CUIntArray *list = new CUIntArray;
	if(nlist.GetSize() <= 0)
		return NULL;
	list->Copy(nlist);
	return list;
}
CUIntArray * BAnglesPoint::Getlist(CUIntArray &list) const
{
	if(nlist.GetSize() <= 0)
		return NULL;
	list.Copy(nlist);
	return &list;
}
CUIntArray * BAnglesPoint::Getlist(int indb, int inde) const
{
	//int indb, int inde - ������� ��������� � ������
	INT_PTR kol = nlist.GetSize();
	if(indb < 0 || indb > kol-1)
		return NULL;
	if(inde < 0 || inde > kol-1)
		return NULL;
	CUIntArray * list = new CUIntArray;
	int i = indb;
	do
	{
		list->Add(nlist.GetAt(i%kol));
		i++;
	}
	while(i%kol != inde);
	list->Add(nlist.GetAt(i%kol));

	return list;
}
void BAnglesPoint::Getsegm(CUIntArray * segm, const BMatrPair & MatrixOfTool)
{
	//(EK) ���������� ��� �����, ������������� segm, �� ����� ������ � segm � ������� ������ ������� �������
	//left=true - ��������� ����� �������
	if(nlist.GetSize() < 1)//���� ���������� ������
		return;
	if(!segm)//���� ���������� ������
		return;

	double	bangle,//angles of segments begin and end
			eangle;

	int b = segm->GetAt(0);
	BPoint P = (*MFOREST.GetPoint(b)) * MatrixOfTool.Inv();
	P.Norm();
	P.SetH(0.0);
	P.ProjXY();
	bangle = i.Angle(P);
	bangle = Normalize(bangle);
	b = NSortedArray::Find(bangle);
//	b = Insert(b, MatrixOfTool);
	if(b<0)
		return;

	int e = segm->GetAt(segm->GetUpperBound());
	P = (*MFOREST.GetPoint(e)) * MatrixOfTool.Inv();
	P.Norm();
	P.SetH(0.0);
	P.ProjXY();
	eangle = i.Angle(P);
	eangle = Normalize(eangle);
	e = NSortedArray::Find(eangle);
//	e = Insert(e, MatrixOfTool);
	if(e<0)
		return;
	//(EK) b - ����� ��������� ����� �� segm � ���� ������, bangle - � ����
	//(EK) e - ����� �������� ����� �� segm � ���� ������, eangle - � ����
	INT_PTR k = GetSize();
	
	int j = b;

	double lam = BAnglesArray::GetDelta(eangle, bangle);
	if(lam < (180. + MINAD) && lam > (180. - MINAD))//���� �������� ����� ������ � 180 ��������, �� ��������� � ������ �����������, �.�. ��. ������ - ��� ���������
	{
		P = (*MFOREST.GetPoint(segm->GetAt(1))) * MatrixOfTool.Inv();
		P.Norm();
		P.SetH(0.0);
		P.ProjXY();
		lam = i.Angle(P);
		lam = Normalize(lam);
		segm->SetSize(0);
		if(BAnglesArray::GetDelta(lam, bangle) < 180.)
		{
			do
			{
				segm->Add(nlist.GetAt(j%k));
				j++;
			}
			while(j%k != e);	
			segm->Add(nlist.GetAt(e));
		}
		else
		{
			if(b < e)
			{
				for(INT_PTR ind = b; ind > -1; ind--)
					segm->Add(nlist.GetAt(ind));
				for(INT_PTR ind = k - 1; ind > e - 1; ind--)
					segm->Add(nlist.GetAt(ind));
			}
			else
			{
				for(int ind = b; ind > e - 1; ind--)
					segm->Add(nlist.GetAt(ind));
			}
		}
	}
	else
	{
		segm->SetSize(0);
		if(lam < 180)//���� ������� ������ ������� �������(������������� �����������)
		{
			do
			{
				segm->Add(nlist.GetAt(j%k));
				j++;
			}
			while(j%k != e);	
			segm->Add(nlist.GetAt(e));
		}
		else
		{
			if(b < e)
			{
				for(INT_PTR ind = b; ind > -1; ind--)
					segm->Add(nlist.GetAt(ind));
				for(INT_PTR ind = k - 1; ind > e - 1; ind--)
					segm->Add(nlist.GetAt(ind));
			}
			else
			{
				for(int ind = b; ind > e - 1; ind--)
					segm->Add(nlist.GetAt(ind));
			}
		}
	}
	return;
}
void BAnglesPoint::Setlist(CUIntArray *n, const BMatrPair & MatrixOfTool, INT_PTR n1, INT_PTR n2)
{
	INT_PTR kol = n->GetSize();
	if(n1 < 0 || n2 < 0 || n1 > kol-1 || n2 > kol-1)
		return;
	if(n2 < n1)
		return;
	for(auto i = n1; i < n2+1; i++)
	{
		auto num = n->GetAt(i);
		nlist.Add(num);
	}
}
void BAnglesPoint::SetPoint(const BPoint & P, const BMatrPair & MatrixOfTool, int index)
{
	int num = MFOREST.AddPoints(P * MatrixOfTool.Or());
//	num = VRS.AddVertex(num);//��������� ����� ����� �� MFOREST
	nlist.InsertAt(index, num);//�������� ����� ����� �� MFOREST
}
double BAnglesPoint::Normalize(double angle)
{
	if(angle < -180.0)
		angle += 360.0;
	if(angle > 180.0)
		angle -=360.0;
	return angle;
}
int BAnglesPoint::Find(int num) const
{
	//����� ����� � ������ �� �������� � ���-������ 
	if(num < 0)
		return -1;
	INT_PTR kol = nlist.GetSize();
	for(int i = 0; i < kol; i++)
	{
		int n = nlist.GetAt(i);
		if(n == num)
			return i;
	}
	return -1;
}
bool BAnglesPoint::IsEmpty(void) const
{
	if(GetCount() == 0)
		return true;
	return false;
}