#pragma once
#include "BMatr.h"
#include "NCMB_API.h"

class NCMB_API BToolPos5x
{
public:
	BToolPos5x(void);
	BToolPos5x(const BPoint &Pi, const BPoint &Ni) { Set(Pi, Ni);}
	void Set(const BPoint &Pi, const BPoint &Ni) { P = Pi; N = Ni;}
	const BToolPos5x &Transform(const BMatr &M) { P = P * M; N = N * M; return *this;}
	const BPoint &GetP() const { return P;}
	const BPoint &GetN() const { return N;}
	BToolPos5x operator *(const BMatr &m) const { return BToolPos5x(P * m, N * m); }
	~BToolPos5x(void);
private:
	BPoint P; // Position
	BPoint N; // Axis direction
};

