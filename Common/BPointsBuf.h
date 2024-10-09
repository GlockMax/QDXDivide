#pragma once

#include "NCMB_API.h"
#include "BPoint.h"
#include <vector>

const int MainSize = 12;

class NCMB_API BPointsBuf
{
public:
	BPointsBuf(void);
	~BPointsBuf(void);
	int GetSize(void) const { return Size; };
	const BPoint & GetPoint(int i) const;
	void SetSize(int n);
	bool SetPoint(int i, const BPoint &P);
	void AddPoint(const BPoint &P);
	void Move(const class BMatr &Matr);
	static int GetMainSize(void) { return MainSize;}
protected:
	BPoint Main[MainSize];
	int Size;
	std::vector <BPoint> *pExtra;
};
