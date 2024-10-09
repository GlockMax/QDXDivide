#pragma once

enum PAType
{
	PA_RECT,
	PA_CIRC,
	PA_EMPTY
};
class NPtsArray
{
	friend class NCUnitState;
public:
	NPtsArray(void);
	virtual ~NPtsArray(void);
	int GetNextPoint(double *x, double *y);
	void Restart(void);
protected:
	enum PAType Type;
	double Angle; // Угол поворота вокруг базовой точки
	int N1; // Число строк параллельных X ( > 0); 
	int N2; // Число строк параллельных Y ( > 0); 
	double Step1; // Вдоль X (может быть отрицательным);
	double Step2; // Вдоль Y (может быть отрицательным);
	double Retr; // Величина отскока
	int IStartP; // Номер начальной точки
	int IPoint1;
	int IPoint2;
	int IPointG;
	bool Active;
};
