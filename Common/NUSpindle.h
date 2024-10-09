#pragma once
#include "UNITS_API.h"

class UNITS_API NUSpindle
{
	friend class NCUnitState;
	friend class NCUnit;
	friend class APTCLOut;
	friend class NMiSpindleSpeed;
	friend class NMiSpindleForwardReverse;
	friend class NMiCheckFeedTurnSpindleTurnOff;
	friend class NMiCheckFeedTurnSpindleMillOn;
	friend class NMiCheckFeedMillSpindleMillOff;
	friend class NMiCheckFeedMillSpindleTurnOn;
	friend class NStateUnitPane;
	friend class CNCMDoc;
public:
	NUSpindle(void);
	~NUSpindle(void);
	void Clear(void);
	double CalcSpeed(void);
protected:
	enum RotDir SpindleDir;
	double Speed;
	double MaxSpeed;
	BOOL SetMaxSpeed;
	BOOL FixCutSpeed;
	double FixedCutSpeed;// in mm/min
	enum SpeedMeasure SpeedM;
};
