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
	// Создать объект из STL файла (ASCII или двоичного)
	bool create(/*[in]*/const char* filePath); // здесь нужно выбрать форму представления строки
	// Создать объект из данных в памяти
	// facesCount - количество треугольников
	// vert - массив координат. Формат: xN, yN, zN, x, y, z. Координаты единичной нормали и координаты точки, соответственно.
	// размер vert должен быть не меньше 3 * 6 * facesCount (не проверяется)
	bool create(/*[in]*/int facesCount, /*[in]*/const float* vert);
	// Получить сетку
	// minAngle - минимальный угол между смежными с ребром гранями, при котором ребро считается "острым" 
	// points - данные о вершинах сетки. Формат: xN, yN, zN, x, y, z. Координаты единичной нормали и координаты точки, соответственно.
	// facesCount - количество граней
	// stride - количество элементов массива points между двумя последовательными вершинами
	// sharpInd - индексы вершин "острых" ребер. sharpInd[0] - размер массива
	bool getMesh(/*[in]*/double minAngle, /*[out]*/int& facesCount, /*[out]*/int& stride
		, /*[out]*/std::vector<float>& points, /*[out]*/int*& sharpInd);
private:
	class BStl* pStl;
};
// Пример OpenGL отрисовки по такой структуре данных (для точного понимания смысла всех параметров)
/*
	// отрисовка поверхности
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, stride * sizeof(points[0]), points.data() + 3);
	glNormalPointer(GL_FLOAT, stride * sizeof(points[0]), points.data());
	glDrawArrays(GL_TRIANGLES, 0, facesCount * 3);
	// отрисовка "острых" ребер
	glDrawElements(GL_LINES, sharpInd[0] - 1, GL_UNSIGNED_INT, sharpInd + 1);
*/
