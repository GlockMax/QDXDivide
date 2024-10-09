#pragma once
#include "NCMB_API.h"
#include "BMatrPair.h"
#include "NLine.h"
#include "NContour.h"
#include "SegmentGrazingCurve.h"

enum AvailableTrace//наличие пересечений
{
	COUNTLESS_TRACE = 0,//множество
	ABSENCE_TRACE = 1,	//отсутствие
	ONE_TRACE = 2,		//одно
	TWO_TRACE = 4		//два
};
enum Connect//наличие соединения гр. кривой между элементами контура
{
	NOT = -1,//связности нет, верхних дуг нет
	NOTUP = 0,//связность есть, верхних дуг нет
	YES = 1,//связность есть, верхние дуги есть
	YESUP = 2//связности нет, верхние дуги есть
};

class NCMB_API GrazingCurveElemContour  //Граничная кривая одного элемента контура простого инструмента
{
	friend class BGrazingCurveElemContour;
public:
	GrazingCurveElemContour(void);
	~GrazingCurveElemContour(void);
	bool SetSegment(unsigned int i, const SegmentGrazingCurve segment);

	/* Все нормали, передаваемые в методы, должны быть единичными! */

	/*Поиск точки пересечения двух прямых(в случае скрещивания возвращается средняя точка ближайших точек на прямых)*/
	static BPoint TraceOfLines(const BPoint &P0, const BPoint &P1, const BPoint &Q0, const BPoint &Q1);
	/*Поиск точки пересечения прямой и плоскости. NULL, если прямая не пересекает или принадлежит плоскости*/
	static AvailableTrace LinePlane(const BPoint &P, const BPoint &Vector, const BPoint &Q, const BPoint &Normal, BPoint &M);
	/*Поиск линии пересечения двух плоскотей(точка, вектор). P - точки пересечения*/
	static AvailableTrace PlanePlane(const BPoint &P1, const BPoint &Normal1, const BPoint &P2, const BPoint &Normal2, BPoint P[2]);
	/*Поиск точек пересечения окружности (O,Normal1) с плоскостью(P,Normal2)*/
	static AvailableTrace CirclePlane(const BPoint &O, const BPoint &Normal1, const BPoint &P, const BPoint &Normal2, const double r2, BPoint M[2]);
	/*Поиск второй и третьей точек харакеристической ломаной для дуги окружности для начальной и конечной точек и веторов ломаной в них*/
	static void PointsOfSegmentsCircle(const BPoint &P0, const BPoint &a, const BPoint &P3, const BPoint &b, BPoint m[2]);
	/*Поиск вектора скорости в точке в СК инструмента*/
	static BPoint SpeedOfPointTool(	const BMatrPair & MatrixOfTool, //матрица инструмента
									const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
									const BPoint & P);			//точка в СК инструмента
	/*Поиск граничной точки на образующей конуса*/
	static AvailableTrace  FindGPointOfLine(	const BMatrPair & MatrixOfTool,			//матрица инструмента
												const BMatr & TotalDeriv,			//полная производная матрицы движения по времени
												const BPoint & A, const BPoint & B,	//точки в СК инструмента
												const BPoint & Normal,				//нормаль к образующей конуса
												BPoint & P);	//граничная точка на образующей конуса
	/*поиск нормали в точке на поверхности конуса*/
	static BPoint NormalToSurfaceOfPoint(	const NLine & Line,		//линия элемента контура
											const BPoint & P);		//точка, в которой находим нормаль к поверхности конуса
	/*поиск нормали к линии контура элемента инструмента*/
	static BPoint NormalToSurfaceOfLine(const NLine & Line,	//точка в СК инструмента
										const double t);	//параметр параметризации кривой
	/*поиск касательного вектора к линии контура элемента инструмента*/
	static BPoint KasatToSurfaceOfLine(	const NLine & Line,	//точка в СК инструмента
										const double t);	//параметр параметризации кривой
	/*Поиск квадрата расстояния от точки до прямой*/
	static double PointLine2(	const BPoint & A,	//1 точка прямой
								const BPoint & B,	//2 точка прямой
								const BPoint & P);	//точка, от которой ищем расстояние до прямой
	/*Поиск проекции точки на прямую*/
	static BPoint PointProjLine(const BPoint & A,
								const BPoint & B,
								const BPoint & P);
         
	/*Метод поиска гр.точек на высоте z по оси Oz*/
	AvailableTrace FindGPoints(	double z,					//координата по оси Oz, для кот. ищем гр. точки 
								const NLine & Line,			//линия элемента контура
								const BMatrPair & MatrixOfTool, //матрица инструмента
								const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
								BPoint &d,					//вектор полной скорости в точке Q в СК инструмента
								BPoint &Q,					//центр сферы, касательной  поверхности инстр. Точки касания имеют аппликату z
								BPoint M[2],				//искомые гр. точки для данного z
								TypeLineInContour Type);	//тип линии элемента контура								

	/*Метод поиска сегментов граничной кривой*/
	Connect FindSegmentsGCurve(	const NLine	&Line,	//линия текущего элемента контура
								const NLine	&Line1,	//линия элемента контура, следующего(ниже по оси Oz) за текущим
								const BMatrPair &MatrixOfTool,			//матрица инструмента
								const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
								const bool  flagleft,				//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
								bool		&fl,					//цвет левых сегментов следующего элемента контура
								AvailableTrace fgph,				//тип пересечения на верхнем оcновании текущего элемента контура
								const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
								const BPoint *gp1,					//лев. и прав. гр. точки на верхнем основании текущего элемента контура
								const BPoint *gp2,					//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
								TypeLineInContour Type,				//Тип линии элемента контура
								SegmentGrazingCurve **segments);	//указатель на 2 сегмента на верхнем основании следующего элемента контура				

	/*Метод дающий ответ на вопрос: имеется ли между двумя точками на нижнем основании текущего эл. контура гр.кривая(ограничивает положительную область). true - да, false - нет*/
	bool TransSign(	const NLine	&Line,			//линия текущего элемента контура
					const NLine	&Line1,			//линия элемента контура, следующего(ниже по оси Oz) за текущим
					const BMatrPair & MatrixOfTool,	//матрица инструмента
					const BMatr &TotalDeriv,	//полная производная матрицы движения по времени
					const BPoint &D,			//точка на дуге АВ 
					bool &sign);				//true - знак меняется, false - нет

	/*Поиск сегментов гр. кривой на дуге нижнего основания. Цвет левых сегментов следующего эл.контура записывает в fl. true - нужно соединять со следующим элементом, false - компонента связности кривой закончилась*/
	Connect FindSegmentsOfLowBases(	const NLine	&Line,					//линия текущего элемента контура
									const NLine	&Line1,					//линия элемента контура, следующего(ниже по оси Oz) за текущим
									const BMatrPair & MatrixOfTool,			//матрица инструмента
									const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
									const AvailableTrace fgph,			//тип пересечения на нижнем оcновании текущего элемента контура
									const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
									const BPoint gp1[2],				//одна из точек, просматриваемых на соединение на основании 
									const BPoint gp2[2],				//точки следующего элемента контура. 
									const bool flagleft,				//цвет sgcleft(sgcleft1 если есть)
									bool &fl,							//цвет левых сегментов следующего элемента контура
									SegmentGrazingCurve **segments);	//указатель на 2 сегмента на верхнем основании следующего элемента контура					

	/*Поиск пары граничных точек для конкретного z для элемента контура типа CONE*/
	AvailableTrace FindGPointsCone(	double z,					//координата по оси Oz, для кот. ищем гр. точки 
									const NLine & Line,			//линия элемента контура
									const BMatrPair & MatrixOfTool, //матрица инструмента
									const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
									BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
									BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
									BPoint M[2]);				//искомые гр. точки для данного z

	/*Поиск граничного сегмента между "верхней" и "нижней" точками для элемента контура типа CONE*/
	void FindGPointsCone(	const NLine & Line,			//линия элемента контура
							const BMatrPair & MatrixOfTool, //матрица инструмента
							const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
							const BPoint &A,				//"верхняя" точка по на поверхности конуса
							const BPoint &B,				//"нижняя" точка по на поверхности конуса
							const double eps,			//погрешность
							GPoints *gp);				//хранилище точек сегмента кривой
	/*Поиск пары граничных точек для конкретного z для элемента контура типа HORIZONTAL*/
	AvailableTrace FindGPointsHorizontal(	bool up,					//true - ищет гр. точки на верхнем основании, false - на нижнем основании
											const NLine & Line,			//линия элемента контура
											const BMatrPair & MatrixOfTool, //матрица инструмента
											const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
											BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
											BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
											BPoint M[2]);				//искомые гр. точки 

	/*Поиск пары граничных точек для конкретного z для элемента контура типа SPHERE*/
	AvailableTrace FindGPointsSphere(	double z,					//координата по оси Oz, для кот. ищем гр. точки 
										const NLine & Line,			//линия элемента контура
										const BMatrPair & MatrixOfTool, //матрица инструмента
										const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
										BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
										BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
										BPoint M[2]);				//искомые гр. точки для данного z

	/*Поиск пары граничных точек для конкретного z для элемента контура типа TORAHS и запись в d вектора полной скорости в СК инстр.*/
	AvailableTrace FindGPointsTorahs(	double z,					//координата по оси Oz, для кот. ищем гр. точки 
										const NLine & Line,			//линия элемента контура
										const BMatrPair & MatrixOfTool, //матрица инструмента
										const BMatr & TotalDeriv,	//полная производная матрицы движения по времени
										BPoint &d,					//вектор полной скорости в точке Q в СК инструмента(возвращается)
										BPoint &Q,					//центр сферы, касательной  поверхности инстр(возвращается). Точки касания имеют аппликату z
										BPoint M[2]);				//искомые гр. точки для данного z

	/*Поиск экстримальной точки на граничной кривой в виде петли для элемента контура типа CONE*/
	AvailableTrace FindExtremumPointCone(const NLine & Line, const BMatrPair & MatrixOfTool, const BMatr & TotalDeriv, BPoint E[2]);

	/*Поиск сегментов граничной кривой на коническом элементе контура: true - нужно соединять со следующим элементом, false - компонента связности кривой закончилась(нужен)*/
	Connect FindSegmentsGCurveCone(	const NLine	&Line,					//линия текущего элемента контура
									const NLine	&Line1,					//линия элемента контура, следующего(ниже по оси Oz) за текущим
									const BMatrPair & MatrixOfTool,			//матрица инструмента
									const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
									const bool  flagleft,				//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
									bool		&fl,					//цвет левых сегментов следующего элемента контура
									const AvailableTrace fgph,			//тип пересечения гр. кривой с верхним онованием
									const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
									const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
									const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
									SegmentGrazingCurve **segments);		//указатель на 2 сегмента на верхнем основании следующего элемента контура	

	/*Поиск сегментов граничной кривой на коническом элементе контура: true - нужно соединять со следующим элементом, false - компонента связности кривой закончилась*/
	Connect FindSegmentsGCurveHorisontal(	const NLine	&Line,					//линия текущего элемента контура
											const NLine	&Line1,					//линия элемента контура, следующего(ниже по оси Oz) за текущим
											const BMatrPair & MatrixOfTool,			//матрица инструмента
											const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
											const bool  flagleft,				//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
											bool		&fl,					//цвет левых сегментов следующего элемента контура
											const AvailableTrace fgph,			//тип пересечения гр. кривой с верхним онованием
											const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
											const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
											const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
											SegmentGrazingCurve **segments);	//указатель на 2 сегмента на верхнем основании следующего элемента контура	

	/*Поиск сегментов граничной кривой на сферическом элементе контура: true - нужно соединять со следующим элементом, false - компонента связности кривой закончилась*/
	Connect FindSegmentsGCurvesSphere(	const NLine	&Line,					//линия текущего элемента контура
										const NLine	&Line1,					//линия элемента контура, следующего(ниже по оси Oz) за текущим
										const BMatrPair & MatrixOfTool,			//матрица инструмента
										const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
										const bool  flagleft,				//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
										bool		&fl,					//цвет левых сегментов следующего элемента контура
										AvailableTrace fgph,				//тип пересечения на верхнем оcновании текущего элемента контура
										const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
										const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
										const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
										SegmentGrazingCurve **segments);	//указатель на 2 сегмента на верхнем основании следующего элемента контура

	/*Поиск сегментов граничной кривой на торическом элементе контура: true - нужно соединять со следующим элементом, false - компонента связности кривой закончилась*/
	Connect FindSegmentsGCurvesTorahs(	const NLine	&Line,	//линия текущего элемента контура
										const NLine	&Line1,	//линия элемента контура, следующего(ниже по оси Oz) за текущим
										const BMatrPair & MatrixOfTool,			//матрица инструмента
										const BMatr &TotalDeriv,			//полная производная матрицы движения по времени
										const bool  flagleft,				//цвет левых сегментов текущего элемента контура: false - левая кривая на сегменте 'зеленая', true - 'красная'
										bool		&fl,					//цвет левых сегментов следующего элемента контура
										const AvailableTrace fgph,			//тип пересечения на верхнем оcновании текущего элемента контура
										const AvailableTrace fgpl,			//тип пересечения на верхнем оcновании элемента контура, кот. ниже текущего по Oz
										const BPoint gp1[2],				//лев. и прав. гр. точки на верхнем основании текущего элемента контура
										const BPoint gp2[2],				//лев. и прав. гр. точки на верхнем основании элемента контура, кот. ниже текущего по Oz
										SegmentGrazingCurve **segments);	//указатель на 2 сегмента на верхнем основании следующего элемента контура
	void draw(void);
	const SegmentGrazingCurve &GetCont(bool Left, bool Top, bool Side) const;
	Connect GetConnect(void) const {return connect;}
	bool GetColor(int num) const;
	void MakeCopy(GrazingCurveElemContour &Dest, const BMatr &M);
	void MakeCopy(GrazingCurveElemContour &Dest);
	void Clear();

protected:
	SegmentGrazingCurve sgcleft,	//боковой левый верхний сегмент
						sgcright,	//боковой правый верхний сегмент 
						sgcleft1,	//боковой левый нижний сегмент
						sgcright1,	//боковой правый нижний сегмент
						sgcldown,	//нижний левый сегмент
						sgcrdown,	//нижний правый сегмент
						sgclup,		//верхний левый сегмент
						sgcrup;		//верхний правый сегмент
	Connect connect;
public:
	static double EPS() {return NSurfSettings::GetTolerance();}
};
