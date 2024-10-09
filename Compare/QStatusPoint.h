#pragma once

class QStatusPoint
{
public:
	QStatusPoint(void);
	QStatusPoint(unsigned int NumTrian, unsigned int NumPoint, unsigned int TypePoint);
	~QStatusPoint(void);
	void SetStPoint(unsigned int NumTrian, unsigned int NumPoint, unsigned int TypePoint);
	unsigned int NumTr;//номер треугольника, на кот. лежит ближ. точка 
	unsigned int NumP;//номер вершины, являющейся ближайшей точкой
	unsigned int TypeP;//тип точки (внутри тр., на ребре тр., в вершине тр.)
};
