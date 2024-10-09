#pragma once

class NWeldParams
{
public:
	NWeldParams(void);
	~NWeldParams(void);
	double *GetParPtr(int Ind);
	void EnsureActual();
	void ClearActual(void) { Actual = false;}
	void Define(class NCUnit *pUn);
	class NCUnit *GetUnit(void) { return pUnit;}
protected:
	bool Actual;
	static const int PARAMS_NUM = 19;
	double Values[PARAMS_NUM]; // FirstL, SecondL, RootL, FirstV, SecondV, RootV
						// MachXYZABC, MachSpeedXYZABC, Penetration depth
	class NCUnit *pUnit;
public:
	void Clear(void);
};
