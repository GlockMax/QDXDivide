#include "stdafx.h"
#include "BConst.h"
#include "NTiParams.h"
#include "NCadrGeom.h"
#include "NCadrSpeedPar.h"

NSpeedPar NSpeedPar::Extern;
//NSpeedPar NSpeedPar::Intern;

NCadrSpeedPar::NCadrSpeedPar() noexcept
{
	SpeedScale = 1.;
	LengthLimit = 49.;
	FastScale4Long = 1.;
	FastScale4Short = 1.;
}

double NCadrSpeedPar::GetLengthScale(const NCadrGeom& Geom) const
{
	double Res = SpeedScale;
	if (!Geom.HaveGeom())
		Res *= FastScale4Long;
	else if (Geom.IsFast())
	{
		Res *= (Geom.Length() > LengthLimit) ?
			FastScale4Long :
			FastScale4Short;
	}
	return Res;
}

double NCadrSpeedPar::GetScaledCadrLength(const NCadrGeom& Geom) const
{
	double Scale = GetLengthScale(Geom);
	Scale = Scale == 0. ? 1. : 1. / Scale;
	return Geom.IsRotate() ? Geom.GetRotSpecLength() : Geom.Length() * Scale;
}

NSpeedPar::NSpeedPar() noexcept
{
	BeepOnProbe = false;
	LengthStep = 1.;// in length units
	TrackFeed = false;// move with real feed
	TrackFeedScale = 1.;
	TimeStep = 40;// in milliseconds
}

bool NSpeedPar::IsTrackFeed(void) const
{
	return TrackFeed;
}

void NSpeedPar::SetTrackFeed(bool Val)
{
	TrackFeed = Val;
}

void NSpeedPar::SetLengthStep(double s)
{
	LengthStep = s;
}

double NSpeedPar::GetLengthStep(void) const
{
	return LengthStep;
}

double NSpeedPar::GetTrackFeedScale(void) const
{
	return TrackFeedScale;
}

bool NSpeedPar::IsBeepOnProbe() const
{
	return BeepOnProbe;
}

void NSpeedPar::SetTrackFeedScale(double s)
{
	TrackFeedScale = s;
}

int NSpeedPar::GetTimeStep(void) const
{
	return TimeStep;
}

double NSpeedPar::GetCurLengthStepMill(const NCadrGeom& Geom) const
{
	double Res = 0;
	if (IsTrackFeed())
	{
		if (Geom.HaveGeom())
			Res = Geom.Length() * GetTimeStep() * 1.15 * TrackFeedScale / (1000. * Geom.GetWorkTimeS());// 1.15 is a magic number to compensate timer inaccuracy
		else if (Geom.IsRotate())
			Res = Geom.GetRotSpecLength() * GetTimeStep() * 1.15 * TrackFeedScale / (1000. * Geom.GetWorkTimeS());// 1.15 is a magic number to compensate timer inaccuracy
		else
			Res = GetTimeStep() * 1.15 * TrackFeedScale / (1000. * Geom.GetWorkTimeS());// 1.15 is a magic number to compensate timer inaccuracy
	}
	else
		Res = GetLengthStep() * ParMill.GetLengthScale(Geom);
	return Res;
}

double NSpeedPar::GetCurLengthStepRot(const NCadrGeom& Geom) const
{
	double Res = 0;
	if (IsTrackFeed())
	{
		if (Geom.HaveGeom() || Geom.IsRotate())
			Res = Geom.Length() * GetTimeStep() * 1.15 * TrackFeedScale / (1000. * Geom.GetWorkTimeS());// 1.15 is a magic number to compensate timer inaccuracy
		else
			Res = GetTimeStep() * 1.15 * TrackFeedScale / (1000. * Geom.GetWorkTimeS());// 1.15 is a magic number to compensate timer inaccuracy
	}
	else
		Res = GetLengthStep() * ParRot.GetLengthScale(Geom);
	return Res;
}

void NSpeedPar::SetDefaults(NTiParams& Par)
{
	CString Key = "Defaults/Speed/Smooth/Mill/Scale";
	ParMill.SpeedScale = Par.GetDouble((Key + "@Gen").GetBuffer(), 1.);
	ParMill.LengthLimit = Par.GetDouble((Key + "@Length").GetBuffer(), 49.) * MM_I_SC;
	ParMill.FastScale4Long = Par.GetDouble((Key + "@Long").GetBuffer(), 10.);
	ParMill.FastScale4Short = Par.GetDouble((Key + "@Short").GetBuffer(), 3.);
	Key = "Defaults/Speed/Smooth/Mill/Timer";
	TimeStep = int(Par.GetDouble((Key + "@Step").GetBuffer(), 40.));
	Key = "Defaults/Speed/Smooth/Turn/Scale";
	ParRot.SpeedScale = Par.GetDouble((Key + "@Gen").GetBuffer(), 1.);
	ParRot.LengthLimit = Par.GetDouble((Key + "@Length").GetBuffer(), 49.) * MM_I_SC;
	ParRot.FastScale4Long = Par.GetDouble((Key + "@Long").GetBuffer(), 10.);
	ParRot.FastScale4Short = Par.GetDouble((Key + "@Short").GetBuffer(), 3.);
}

