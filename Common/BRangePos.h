#pragma once
#include "UNITS_API.h"
#include "cadrID.h"
#include "BGeomArray.h"

class BProgram;

class UNITS_API BRangePos
{
public:
	BRangePos(const BProgram &Prog);
	~BRangePos(void);
	cadrID FindCurHCadr(int iRet) const;
	cadrID FindCurHistCadr(int iRet) const;
	cadrID FindCurCadr(int iRet) const;
	void IncrCadrNums(int Cadr);
	void Incrik(int Shift);
	int kStart;
	int iStart;
	int ProgNum;
	int CadrNum;
	double StepParamPerCent;
protected:
	const BProgram &Prog;
};

class UNITS_API BRangePosColl : public BRangePos
{
public:
	BRangePosColl(const BRangePos& Pos, CollLogElemType Type) : BRangePos(Pos) { CollType = Type; 	LastStartCadrNum = CadrNum;}
	CollLogElemType GetCollType() const { return CollType; }
	void operator += (size_t k) { CadrNum += int(k); LastStartCadrNum += int(k); }
	void SetCadrNums(int Cadr, int LastStartCadr);
	void IncrCadrNums(int Cadr, int LastStartCadr);
	auto GetLastStartCadrNum() const { return LastStartCadrNum; }
	ProgTPoint GetStartTPoint() const;
protected:
	int LastStartCadrNum;// used for collision detection only
	CollLogElemType CollType;
};