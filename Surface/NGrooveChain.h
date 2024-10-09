#pragma once
#include "NCadrGeomArray.h"

class NCadrGeom;
class SMachStateFix;


class NGrooveChain
{
public:
	NGrooveChain(double Tol = 0.1);
	~NGrooveChain(void);
	void SetTol(double Tol) { Toler = Tol; }
	int GetSamplesNum(void);
	bool GetSample(int Num, SMachStateFix1 & P);
	void Clear(void);
	bool Define(void);
	bool IsDefined(void) { return Defined; };
	int SendCadrs(NCadrGeom *InCadrs, int Size, int Orient);
	int GetNCadrs(void) const { return NCadrs;}
	int GetOrient() const { return Orient;}
	bool IsFast() const { return Fast; }
protected:
	static const int MAX_CADRS = 1024;
	bool Defined;
	double Toler;
	int NCadrs;
	int SamplesNum;
	NCadrGeom *Cadrs[MAX_CADRS];
	int CadrsSamplesNum[MAX_CADRS];
	int Orient; // 0 - undefined (flat), 1 - right, -1 - left
	bool Fast;
protected:
	void CalcSamplesNum(void);


};
