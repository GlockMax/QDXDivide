#pragma once

class QStatusPoint
{
public:
	QStatusPoint(void);
	QStatusPoint(unsigned int NumTrian, unsigned int NumPoint, unsigned int TypePoint);
	~QStatusPoint(void);
	void SetStPoint(unsigned int NumTrian, unsigned int NumPoint, unsigned int TypePoint);
	unsigned int NumTr;//����� ������������, �� ���. ����� ����. ����� 
	unsigned int NumP;//����� �������, ���������� ��������� ������
	unsigned int TypeP;//��� ����� (������ ��., �� ����� ��., � ������� ��.)
};
