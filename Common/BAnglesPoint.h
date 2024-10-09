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

	//b, e - номера точек в MFOREST
	int		Insert(const BPoint & P, const BMatrPair & MatrixOfTool);							//Вствка точки
	int		Insert(const BPoint & P, const BMatrPair & MatrixOfTool, int b, int e = -1);		//Вствка точки в интервал
	int		Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool, double angle);					//Вставка элемента массива точек
	int		Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool);					//Вставка элемента массива точек
	void	Insert(CUIntArray *n, const BMatrPair & MatrixOfTool);							//Вставка массива точек
	void	Insert(CUIntArray *n, const BMatrPair & MatrixOfTool, int b, int e = -1);			//Вставка массива точек в интервал
	int		Insert(CUIntArray *n, INT_PTR in, const BMatrPair & MatrixOfTool, int b, int e = -1);	//Вставка элемента массива точек в интервал
	int		Insert(int num, const BMatrPair & MatrixOfTool);									//Вставка точки по номеру в MFOREST
	int		Insert(int num, double angle, const BMatrPair & MatrixOfTool);									//Вставка точки по номеру в MFOREST
	int		Insert(CUIntArray *n, INT_PTR ind1, INT_PTR ind2, const BMatrPair & MatrixOfTool);		//Вставка диапазона элементов массива (ind1,ind2 - индексы элементов массива)

	static bool IsVert(CUIntArray *n, const BMatrPair & MatrixOfTool, const bool f = true);//вертикален ли по углу сегмент

	int GetNumBDCELPPoint(int index) const;
	int GetNumForestPoint(int index) const;
	double GetAt(int index, bool flag = false) const;
	//Возвращает точку. index - индекс в списке nlist
	BPoint GetPoint(int index, const BMatrPair & MatrixOfTool) const ;
	CUIntArray * Getlist(void) const;
	CUIntArray * Getlist(CUIntArray &list) const;
	CUIntArray * Getlist(int indb, int inde) const;
	//возвращает список nlist в виде сегмента(список номеров точек) у BGrazingCurveElemContour
	void Getsegm(CUIntArray * n, const BMatrPair & MatrixOfTool);
	//запись массива номеров точек в список номеров nlist
	void Setlist(CUIntArray *n, const BMatrPair & MatrixOfTool, INT_PTR n1 = 0, INT_PTR n2 = 0);
	void SetPoint(const BPoint & P, const BMatrPair & MatrixOfTool, int index = 0);
	//приводит значение угла в диапазон от -180 до 180 градусов
	static double Normalize(double angle);
	//return index in list 'nlist'. num - number in MFOREST
	int Find(int num) const;
	INT_PTR GetCount(void) const {return nlist.GetCount();}
	bool IsEmpty(void) const;
protected:
	CUIntArray nlist;//список номеров точек в MFOREST, упорядоченных по углу
};
