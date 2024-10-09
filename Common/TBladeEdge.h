#pragma once
#include "BPoint.h"
typedef unsigned int BladeEdgeAttr;
const BladeEdgeAttr	BE_CUT = 0x01;
const BladeEdgeAttr	BE_NOTCUT = 0x02;
const BladeEdgeAttr	BE_DEGR = 0x04;

class AFX_EXT_CLASS TBladeEdge
{
public:
	TBladeEdge(void);
	~TBladeEdge(void);
protected:
	TBladeEdge *pNext;
	TBladeEdge *pPrev;
	BPoint Pb;
	BladeEdgeAttr Attrib;
	BPoint CircCenter;
public:
	TBladeEdge *GetNext() const{ return pNext;}
	TBladeEdge *GetPrev() const{ return pPrev;}
	void SetPrev(TBladeEdge *p){ pPrev = p;}
	void SetNext(TBladeEdge *p){ pNext = p;}
	void InsertAfter(TBladeEdge * pEdge);
	BPoint GetVec() const;
	void SetAttr(BladeEdgeAttr Attr){ Attrib = Attr;}
	void Trans(const BMatr &M);
	const BPoint &GetPoint() const;
	void SetPoint(const BPoint& P);
	void SetPointH(double h);
	const BPoint &GetCircCenter() const;
	void SetCircCenter(const BPoint &P);
	bool IsArc() const {return CircCenter.GetH() >= 0.;}
	double GetRad() const;
	BladeEdgeAttr GetAttr() const { return Attrib;}
	int InsertTangRad(double Rad);
	void Serialize(CArchive & ar);
};
