#pragma once
#include "OPENGLRENDER_API.h"
#include "BMatr.h"

class OPENGLRENDER_API RDynViewChange
{
public:
	RDynViewChange(const BMatr &M0, const BMatr &M1);
	~RDynViewChange(void);
	bool GetNextMatr(BMatr &M);
protected:
	void CalcCurMatr(BMatr &M);
	double t;
	int CurStep;
	int StepsNum;
	unsigned int CurTime;
	int MinTimePeriod;
	int StartTime;
	BMatr StartM;
	BPoint Dir1;
	BPoint Dir2;
	BPoint P1;
	BPoint P2;
	double Alf1;
	double Alf2;
	double Scale;
public:
	static unsigned int TimePeriod; // The time of view changing

};
