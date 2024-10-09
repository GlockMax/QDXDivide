#pragma once
#include "BTimeLine.h"
class cadrID
{
public:
	static constexpr int MIX_SIZE = 17;
	static constexpr int PROG_SIZE = 8;
	union
	{
		unsigned ID;
		struct
		{
			int Prog : PROG_SIZE;
			unsigned Cadr : sizeof(ID) * 8 - PROG_SIZE;
		};
	};
	cadrID(void) {}
	cadrID(int ProgNum, int CadrNum = 0) { Prog = ProgNum; Cadr = CadrNum; }
	bool operator > (const cadrID& b) const { return ((Prog == b.Prog) && (Cadr > b.Cadr)) || (Prog > b.Prog); }
	bool operator == (const cadrID& b) const { return (ID == b.ID); }
	bool operator != (const cadrID& b) const { return !(*this == b); }
	bool operator >= (const cadrID& b) const { return (*this == b) || (*this > b); }
	bool operator < (const cadrID& b) const { return !(*this >= b); }
	bool operator <= (const cadrID& b) const { return !(*this > b); }
	const cadrID& operator -- (void) { if (Cadr > 0)  --Cadr; return *this; }
	const cadrID& operator ++ (void) { ++Cadr; return *this; }
	bool IsEmpty() const { return Prog < 0; }
	void SetEmpty() { Prog = -1; Cadr = 0; }
	void MakeMix(cadrID I0, cadrID I1) {
		Prog = I0.Prog;
		Cadr = I0.Cadr;
		if (I0.Cadr == I1.Cadr)
			return;
		auto Min = I0.Cadr < I1.Cadr ? I0.Cadr : I1.Cadr;
		Cadr = Min;
		auto Delta = I0.Cadr > I1.Cadr ? I0.Cadr - Min : I1.Cadr - Min;
		if (Delta >= (1 << (sizeof(ID) * 8 - PROG_SIZE - MIX_SIZE)) || Min >= (1 << MIX_SIZE))
			return;
		Cadr += Delta * (1 << MIX_SIZE);
		Prog = -Prog - 2;
	}
	bool SplitMix(cadrID& I0, cadrID& I1) const {
		I0 = *this;
		I1 = *this;
		if (Prog > -2)
			return false;
		I0.Prog = -I0.Prog - 2;
		I1.Prog = -I1.Prog - 2;
		const auto Delta = Cadr >> MIX_SIZE;
		I0.Cadr = Cadr - (Delta << MIX_SIZE);
		I1.Cadr = I0.Cadr + Delta;
		return true;
	}
	bool IsMix() { return Prog < -1; }
};

class ProgTPoint : public cadrID
{
public:
	ProgTPoint() : cadrID(), Time(0) {}
	ProgTPoint(const cadrID& ID, MainTime T) : cadrID(ID) { Time = T; }
	auto GetTime() const { return Time; }
	void SetTime(MainTime T) { Time = T; }
	void IncrTime(MainTime T) { Time += T; }
protected:
	MainTime Time;
};