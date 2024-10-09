#pragma once
#include "NCMB_API.h"
#include "GrazingCurveElemContour.h"

class NCMB_API GrazingCurve  //Граничная кривая всего простого инструмента
{
public:
	GrazingCurve(void);
	GrazingCurve(const int k);//kol - количество элементов контура в простом инструменте
	virtual ~GrazingCurve(void);
	/*Поиск граничной кривой для одного простого инструмента*/
	bool FindGPNTool(const BMatrPair & MatrixOfTool, const BMatr & TotalDeriv, const NContour &Contour);
	int Getkol(void) const {return kol;}
	void draw(void);
	bool GetColor(int iElem, int num) const;
	void MakeCopy(GrazingCurve &Dest, const BMatr &M) const;
	void MakeCopy(GrazingCurve &Dest) const;
	INT_PTR GetConCompNum(void); // Get a number of connected components
	GrazingCurveElemContour *GetGrCurve(int i) const { return &GCurve[i]; }//указатель на массив граничных кривых элемента контура
protected:
	GrazingCurveElemContour *GCurve;//указатель на массив граничных кривых элемента контура
	int kol;//количество граничных кривых
};
