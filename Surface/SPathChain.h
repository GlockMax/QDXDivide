#pragma once
#include "NCadrGeomArray.h"
#include "BScrewMotion.h"
#include "BMTLinkage.h"

class NCadrGeom;
class SMachStateFix1;
class MTMachineTool;

class SPathChain
{
public:
	friend class SProcChain;
	SPathChain(class NMachUnitPair& MU, double Tol = 0.1);
	~SPathChain(void);
	void SetTol(double Tol) { Toler = Tol; }; 
	size_t GetSamplesNum(void);
	bool GetSample(MTMachineTool &Mach, size_t Num, SMachState & P, BMTLinkage&ConfNum, const BStockToolInds& STInds) const;
	void Clear(void);
	bool Define(void);
	bool IsDefined(void) { return Defined; };
	int SendCadrs(NCadrGeom *InCadrs, int Size, int Turret, int Attachment);
	int GetNCadrs(void) const { return NCadrs;}
	bool IsMotionFast(int IM, bool &IsFast) const;
protected:
	bool Defined;
	double Toler;
	int NCadrs;
	size_t SamplesNum;
	NCadrGeom ** Cadrs;
	size_t * CadrsSamplesNum;
	std::vector <BScrewMotion> Motions;
protected:
	size_t GetSamplesNum(size_t i) const;
	void CalcCurState(MTMachineTool &Mach, int CadrNum, double t, SMachState & P, BMTLinkage& ConfNum, const BStockToolInds& STInds) const;

	NMachUnitPair& MUPair;
};
