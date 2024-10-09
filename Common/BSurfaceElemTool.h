#pragma once
#include "BAnglesPoint.h"
#include "BGrazingCurve.h"
#include "MVRDS.h"

//класс поверхности элемента инструмента в начале/конце кадра
class AFX_EXT_CLASS BSurfaceElemTool//!!!
{
public:
	static double Angle(const BPoint & P);//угол, соответствующий точке
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
	//возвращает номер начальной точки i-списка из BDCELP
	int Getbegin(int i) const;
	//возвращает индекс начальной точки в i-списке
	int Getbeginindex(int i) const;
	int Getb(int i) const;
	int Gete(int i) const;
	void Setkl(int k = 0){kl = k;}
	int Getkl(void) const {return kl;}

	BAnglesPoint ** GetAPoint(void) const;

	//Create lists surfaces

	//создание списков внутри петли
	BAnglesPoint * CreateListInLoop(	
										const NLine & Line,
										BGrazingCurveElemContour * bgc, 
										BGrazingCurveElemContour * next, 
										BAnglesPoint * p,
										const BMatrPair & MatrixOfTool,
										TypeLineInContour Type,
										bool f = true);
	//создание списков вне петли
	BAnglesPoint * CreateListOutLoop(	
										const NLine & Line,
										BGrazingCurveElemContour * bgc, 
										BGrazingCurveElemContour * next, 
										BAnglesPoint * p,
										const BMatrPair & MatrixOfTool,
										TypeLineInContour Type,
										bool &modify,
										bool f = true);
	//создание списков поверхности сферы
	BAnglesPoint * CreateListSphere(
									const NLine & Line, 
									BGrazingCurveElemContour & bgc, 
									BGrazingCurveElemContour * next,  
									BAnglesPoint * p, 
									const BMatrPair & MatrixOfTool,
									const BMatr & TotalDeriv,
									bool f = true);


	//Create lists full's surfaces

	//создание списков полной поверхности горизонтали
	BAnglesPoint * CreateListHorizontal(const NLine & Line, BGrazingCurveElemContour & bgc, BGrazingCurveElemContour * next,  BAnglesPoint * p, const BMatrPair & MatrixOfTool);
	//создание списков полной поверхности конуса
	BAnglesPoint * CreateListCone(const NLine & Line, BGrazingCurveElemContour & bgc, BGrazingCurveElemContour * next,  BAnglesPoint * p, const BMatrPair & MatrixOfTool);
	

	//Create surfaces

	//создание полной поверхности горизонтали
	//void CreateSurfaceHorizontal(BDCELP & VRS, const NLine & Line) const;
	void CreateSurfaceHorizontal(NTriMesh *pMesh, const NLine & Line) const;
	//создание поверхности конуса, у которого в одном из списков только 1 точка(конус с нулевым основанием)
	//void CreateSurfaceCone(BDCELP & VRS, const NLine & Line, const BMatrPair & MatrixOfTool) const;
	void CreateSurfaceCone(NTriMesh *pMesh, const NLine & Line) const;
	//создание поверхности сферы
	//MVRDS * CreateSurfaceSphere(BDCELP & VRS, const NLine & Line, const BMatrPair & MatrixOfTool) const;
	MVRDS * CreateSurfaceSphere(const NLine & Line, const BMatrPair & MatrixOfTool) const;

	//Добавление новых точек в список, при условии превышения отклонения
	int UpdatingOfList(BAnglesPoint * bap, double r, const BMatrPair & MatrixOfTool, int b, int e = -1);
	//Обновление кривой инструмента точками из списка (полностью заменяются)
	void Update(BGrazingCurve & bgc, int i, const BMatrPair & MatrixOfTool) const;
	//Обновление сегмента кривой точками из списка (полностью заменяются) 
	void Update(CUIntArray *segm, const BMatrPair & MatrixOfTool, bool f = true) const;

	//Возвращает массив номеров полуребер списка, примыкающего к нижнему основанию элемента инструмента
	const CUIntArray * CreateSurfaceTri(MeshArr *pMeshArr, const BMatrPair & MatrixOfTool, const NLine & Line, TypeLineInContour Type, bool MakeTop, bool f) const;
	
	static int MakeToolSurface(const NLine & Line, TypeLineInContour Type, const BMatrPair & MatrixOfTool);
protected:
	BAnglesPoint ** APoint;
	int kl;		//количество пар списков
	int begin[4];//список номеров начальных точек в списках APoint номеров точек из MFOREST
	int b[4];	//номера начальных точек интервалов в списках, в кот. можно добавлять точки
	int e[4];	//номера конечных точек интервалов в списках, в кот. можно добавлять точки
};
