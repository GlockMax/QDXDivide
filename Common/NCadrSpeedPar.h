#pragma once
#include "NCTOOL_API.h"

class NCTOOL_API NCadrSpeedPar
{
public:
	NCadrSpeedPar() noexcept;
	double GetLengthScale(const class NCadrGeom& Geom) const;
	double GetScaledCadrLength(const class NCadrGeom& Geom) const;
public:
	double SpeedScale;
	double LengthLimit;
	double FastScale4Long;
	double FastScale4Short;
};

class NCTOOL_API NSpeedPar
{
public:
	NSpeedPar() noexcept;
	double GetCurLengthStepMill(const NCadrGeom& Geom) const;
	double GetCurLengthStepRot(const NCadrGeom& Geom) const;
	void SetLengthStep(double s);
	double GetLengthStep(void) const;
	bool IsTrackFeed(void) const;
	void SetTrackFeed(bool Val);
	void SetTrackFeedScale(double s);
	double GetTrackFeedScale(void) const;
	bool IsBeepOnProbe() const;
	void SetDefaults(class NTiParams& Par);
	int GetTimeStep(void) const;
public:
	NCadrSpeedPar ParMill;
	NCadrSpeedPar ParRot;
protected:
	bool BeepOnProbe;
	double LengthStep;// in length units
	bool TrackFeed;// move with real feed
	double TrackFeedScale;
	int TimeStep;// in milliseconds
public:
	static NSpeedPar Extern;
//	static NSpeedPar Intern;
};
