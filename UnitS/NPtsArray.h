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
	double Angle; // ���� �������� ������ ������� �����
	int N1; // ����� ����� ������������ X ( > 0); 
	int N2; // ����� ����� ������������ Y ( > 0); 
	double Step1; // ����� X (����� ���� �������������);
	double Step2; // ����� Y (����� ���� �������������);
	double Retr; // �������� �������
	int IStartP; // ����� ��������� �����
	int IPoint1;
	int IPoint2;
	int IPointG;
	bool Active;
};
