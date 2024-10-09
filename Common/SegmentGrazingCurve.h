#pragma once
#include <math.h>
#include "BPoint.h"
#include "GPoints.h"
#include "NSurfSettings.h"

class AFX_EXT_CLASS  SegmentGrazingCurve  //—егмент граничной кривой одного элемента контура простого инструмента
{
public:
	SegmentGrazingCurve(void);
	SegmentGrazingCurve(BPoint *pt, int kol);
	void Copy(const SegmentGrazingCurve & Segment);
	void Copy(const SegmentGrazingCurve & Segment, const BMatr &M);
	SegmentGrazingCurve(const SegmentGrazingCurve & Segment);
	~SegmentGrazingCurve(void);
	void Add(const BPoint & P);					//добавить точку
	void Add(const BPoint *P, int kol);			//добавить массив точек
	void Add(const BPoint & p0, const BPoint & p1, const BPoint & p2, const BPoint & p3);
	void Add(GPoints *gp);	//добавить хранилище точек
	void Add(SegmentGrazingCurve & S, bool f = false);	//добавить сегмент точек
	void Clear(void);
	BPoint GetGP(int i) const;
	void SetColor(bool c = true);
	bool GetColor(void) const;
	int Getkolgp() const {return p.Getkol();}
	void draw(bool f = true) const;//f = true - рисование кривой отрезками, f = false - рациональными кубич. кривыми
	bool IsCurve(void) const;
private:
	GPoints p;
	bool color;//false-зеленый сегмент, true-красный сегмент
};