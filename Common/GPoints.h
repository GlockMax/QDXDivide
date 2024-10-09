#pragma once
#include "BPoint.h"


class AFX_EXT_CLASS GPoints
{
class Elem
{
public:
	Elem(void);
	Elem(BPoint *V);
	~Elem(void);
	BPoint* GetVal(void);
	void SetVal(BPoint *V);
	void Clear(void);//�������� �����������
	Elem *Next;
protected:
	BPoint *Val;
};
class List
{
public:
	List(void);
	~List(void);//�������� ������ ����� ������� ��� �����������
	void Clear(void);//�������� �������: 0-��� �����������, 1-� ����������
	void PushLast(Elem *Elem);//������ ������� ��� Last � �������
	Elem* Pop(void);//��������� ������� First �������
	bool IsEmpty(void) const;//1-������� �����, 0-�� �����
	Elem *First, *Last;//First-��������� �� �������, Last-������� � �������
	unsigned int kol;
};
public:
	GPoints(void);
	GPoints(const GPoints & P);
	~GPoints(void);
	void Clear();
	void Add(const BPoint &P);
	void Add(const BPoint *P, int kol);
	void GetPoints(BPoint **points);
	int Getkol(void) const;
	BPoint GetPoint(int i) const;
protected:
	static const int n = 100;
	BPoint p[n];
	List *list;
	int kol;
};
