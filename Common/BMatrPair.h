#pragma once

#include "NCMB_API.h"
#include "BMatr.h"

class NCMB_API BMatrPair
{
public:
	BMatrPair(const BMatr &Matr);
	BMatrPair();
	~BMatrPair();
	const BMatr &Or() const { return MOrig; }
	const BMatr &Inv() const { return MInvr; }
	const BMatrPair& operator = (const BMatr& Matr);
	const BMatrPair& operator = (const BMatrPair& Matr);
	void SetE() { MOrig.SetE(); MInvr.SetE(); }
protected:
	BMatr MOrig;
	BMatr MInvr;
};

