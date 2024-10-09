#pragma once
const int LIMITS_NUM = 6;

enum TravelLimit
{
	TL_EMPTY,
	TL_POSITIVE,
	TL_NEGATIVE,
	TL_ACTIVATE
};
class AFX_EXT_CLASS NCoordLimits
{
public:
	NCoordLimits(void);
	~NCoordLimits(void);
	bool IsValid(int Num, double Val); 
	void SetActive(int Num, bool bVal = true) { Active[Num] = bVal;}
	void SetInactive(void);
	void SetMin(int Num, double Val) { Min[Num] = Val;}
	double GetMin(int Num) { return Min[Num];}
	void SetMax(int Num, double Val) { Max[Num] = Val;}
	double GetMax(int Num) { return Max[Num];}
	void Clear(void);
	void SetMax(const NCoordLimits &InLimits);
	void SetMin(const NCoordLimits &InLimits);
protected:
	bool Active[LIMITS_NUM];
	double Min[LIMITS_NUM];
	double Max[LIMITS_NUM];

};
