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
	void Clear(void);//удаление содержимого
	Elem *Next;
protected:
	BPoint *Val;
};
class List
{
public:
	List(void);
	~List(void);//удаление только самой очереди без содержимого
	void Clear(void);//удаление очереди: 0-без содержимого, 1-с содержимым
	void PushLast(Elem *Elem);//вносим элемент как Last в очередь
	Elem* Pop(void);//извлекаем элемент First очереди
	bool IsEmpty(void) const;//1-очередь пуста, 0-не пуста
	Elem *First, *Last;//First-извлекаем из очереди, Last-заносим в очередь
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
