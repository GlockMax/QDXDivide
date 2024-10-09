#pragma once
#include <vector>

#ifdef MACHINE_EXPORTS
#define MACHINE_API __declspec(dllexport)
#else
#define MACHINE_API __declspec(dllimport)
#endif

class MACHINE_API StlRend
{
public:
	StlRend();
	~StlRend();
	// ������� ������ �� STL ����� (ASCII ��� ���������)
	bool create(/*[in]*/const char* filePath); // ����� ����� ������� ����� ������������� ������
	// ������� ������ �� ������ � ������
	// facesCount - ���������� �������������
	// vert - ������ ���������. ������: xN, yN, zN, x, y, z. ���������� ��������� ������� � ���������� �����, ��������������.
	// ������ vert ������ ���� �� ������ 3 * 6 * facesCount (�� �����������)
	bool create(/*[in]*/int facesCount, /*[in]*/const float* vert);
	// �������� �����
	// minAngle - ����������� ���� ����� �������� � ������ �������, ��� ������� ����� ��������� "������" 
	// points - ������ � �������� �����. ������: xN, yN, zN, x, y, z. ���������� ��������� ������� � ���������� �����, ��������������.
	// facesCount - ���������� ������
	// stride - ���������� ��������� ������� points ����� ����� ����������������� ���������
	// sharpInd - ������� ������ "������" �����. sharpInd[0] - ������ �������
	bool getMesh(/*[in]*/double minAngle, /*[out]*/int& facesCount, /*[out]*/int& stride
		, /*[out]*/std::vector<float>& points, /*[out]*/int*& sharpInd);
private:
	class BStl* pStl;
};
// ������ OpenGL ��������� �� ����� ��������� ������ (��� ������� ��������� ������ ���� ����������)
/*
	// ��������� �����������
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride * sizeof(points[0]), points.data() + 3);
	glNormalPointer(GL_FLOAT, stride * sizeof(points[0]), points.data());
	glDrawArrays(GL_TRIANGLES, 0, facesCount * 3);
	// ��������� "������" �����
	glDrawElements(GL_LINES, sharpInd[0] - 1, GL_UNSIGNED_INT, sharpInd + 1);
*/
