#include "stdafx.h"
#include "GPoints.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

GPoints::Elem::Elem(void)
{
	Next = NULL;
}
GPoints::Elem::Elem(BPoint *V)
{
	Next = NULL;
	Val = V;
}
GPoints::Elem::~Elem(void)
{
	delete [] Val;
}
BPoint* GPoints::Elem::GetVal(void)
{
	return Val;
}
void GPoints::Elem::SetVal(BPoint *V)
{
	Val = V;
}
void GPoints::Elem::Clear(void)
{
	delete Val;
}
GPoints::List::List(void)
{
	First = Last = NULL;//������� �����
	kol = 0;
}
GPoints::List::~List(void)
{
	Clear();
}
void GPoints::List::Clear(void)
{
	if(First!=NULL)//���� ������ �� ����
	{
		Elem *Temp;
		while(First!=NULL)//���� �� �������� � ��������������� ��������
		{
			Temp = First;
			First = First->Next;
			delete Temp;//������ ��� �������
		}
		Last = NULL;
	}
}
void GPoints::List::PushLast(Elem *Elem)
{
	if(Last!=NULL)//���� ������ �� ����
	{
		Last->Next = Elem;
		Last = Elem;
	}
	else //���� ������ ����
		First = Last = Elem; 
	Elem = NULL;//������� ��� ����������� ������������� 
	kol++;
}
GPoints::Elem* GPoints::List::Pop(void)
{
	Elem* Out = NULL;
	if(First!=NULL)//���� ������� �� �����
	{
		Out = First;
		if(First->Next==NULL)//���� ������� ������������ �������
			First = Last = NULL;
		else
			First = First->Next;
		kol--;
	}
	return Out;
}

bool GPoints::List::IsEmpty(void) const
{
	if(First == NULL)//���� ������� �����
		return true;
	return false;
}
GPoints::GPoints(void)
{
	kol = 0;
	list = NULL;
}
GPoints::GPoints(const GPoints & P)
{
	int kol = P.Getkol();
	for(int i = 0; i < kol; i++)
		Add(P.GetPoint(i));
}
GPoints::~GPoints(void)
{
	Clear();
}
void GPoints::Clear()
{
	if(list)
	{
		delete list;
		list = NULL;
	}
	kol = 0;
}
int GPoints::Getkol(void) const
{
	return kol;
}
BPoint GPoints::GetPoint(int i) const
{
	if(i >= 0 && i < kol)
	{
		if(i < n)
			return p[i];
		Elem *E = list->First;
		int mn = int(i / n);//����� �������(� ������ �������� � 1)
		for(int j = 1; j < mn; j++)
			E = E->Next;
		int num = i % n;
		BPoint * pt = (BPoint *)E->GetVal();
		return pt[num];
	}
	return BPoint(0., 0., 0., -1.);
}
void GPoints::Add(const BPoint &P)
{
	if(kol<n)
		p[kol] = P;
	else
	{
		int num = kol%n;
		Elem *E = NULL;
		BPoint *points = NULL;
		if( num == 0)
		{
			points = new BPoint [n];
			points[0] = P;
			E = new Elem(points);
			if(!list)
				list = new List;
			list->PushLast(E);
		}
		else
		{
			points = (BPoint *)list->Last->GetVal();
			points[num] = P;
		}
	}
	kol++;
}
void GPoints::Add(const BPoint *P, int kol)
{
	for(int i = 0; i < kol; i++)
	{
		Add(P[i]);
	}
}
void GPoints::GetPoints(BPoint **points)
{

	BPoint  *temp = NULL;
	Elem *E = NULL;

	*points = new BPoint [kol];
	int i = 0;
	for(; i < n && i<kol; i++)
		(*points)[i] = p[i];

	int num;
	//if(n == 1)
	//	num = 1;
	//else
		num = kol%n;//���������� ����� � ��������� ������������� �� ����� �������

	if(list)
	{
		while(!(list->IsEmpty()))
		{
			E = list->Pop();
			temp = (BPoint *)E->GetVal();
			if( list->Last )//���� ������� �������� �� �����, �� ����� ���������� ������ ���������
				for(int j = 0; j < n; j++, i++)
					(*points)[i] = temp[j];
			else
				for(int j = 0; j < num; j++, i++)
					(*points)[i] = temp[j];
		}
	}
	kol = 0;
}
