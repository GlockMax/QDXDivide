#include "stdafx.h"
#include "NCMProject.h"
#include "NTiParams.h"
#include "NSectArrParams.h"

NSectArrParams::NSectArrParams(void) :
	Default(true)
{
	SetDefault(SAT_CIRCLE);
}

NSectArrParams::~NSectArrParams(void)
{
}

NSectArrParams &NSectArrParams::SetDefault(SectArrType iType)
{
	NTiParams &Par = NCM_PROJECT.Defaults;

	CString Key = _T("Defaults/Part/Sections/");
	Type = iType;
	double x, y, z;
	switch(Type)
	{
	case SAT_LINE:
		x = Par.GetDouble(Key + "Linear/Basepoint@X", 0.);
		y = Par.GetDouble(Key + "Linear/Basepoint@Y", 0.);
		z = Par.GetDouble(Key + "Linear/Basepoint@Z", 0.);
		P.Set(x, y, z, 1.);
		x = Par.GetDouble(Key + "Linear/Axisdirection@X", 0.);
		y = Par.GetDouble(Key + "Linear/Axisdirection@Y", 1.);
		z = Par.GetDouble(Key + "Linear/Axisdirection@Z", 0.);
		V.Set(x, y, z, 0.);
		StartVal = Par.GetDouble(Key + "Linear@Start", -200.);
		EndVal = Par.GetDouble(Key + "Linear@End", 200.);
		IntervalsNum = int(Par.GetDouble(Key + "Linear@Number", 36.));
		LineThickness = int(Par.GetDouble(Key + "Linear@Thick", 1.));
		ColorByPart = Par.GetBool(Key + "Linear@PartColor", false);
		Color.R() = float(Par.GetDouble(Key + "Linear/Color@R", 0.5));
		Color.G() = float(Par.GetDouble(Key + "Linear/Color@G", 0.5));
		Color.B() = float(Par.GetDouble(Key + "Linear/Color@B", 0.5));
		Color.A() = 1.;
		Default = true;
		break;
	case SAT_CIRCLE:
		x = Par.GetDouble(Key + "Radial/Basepoint@X", 0.);
		y = Par.GetDouble(Key + "Radial/Basepoint@Y", 0.);
		z = Par.GetDouble(Key + "Radial/Basepoint@Z", 0.);
		P.Set(x, y, z, 1.);
		x = Par.GetDouble(Key + "Radial/Axisdirection@X", 0.);
		y = Par.GetDouble(Key + "Radial/Axisdirection@Y", 0.);
		z = Par.GetDouble(Key + "Radial/Axisdirection@Z", 1.);
		V.Set(x, y, z, 0.);
		x = Par.GetDouble(Key + "Radial/Startdirection@X", 1.);
		y = Par.GetDouble(Key + "Radial/Startdirection@Y", 0.);
		z = Par.GetDouble(Key + "Radial/Startdirection@Z", 0.);
		StartDir.Set(x, y, z, 0.);
		StartVal = Par.GetDouble(Key + "Radial@StartAngle", 0.);
		EndVal = Par.GetDouble(Key + "Radial@EndAngle", 360.);
		IntervalsNum = int(Par.GetDouble(Key + "Radial@Number", 36.));
		LineThickness = int(Par.GetDouble(Key + "Radial@Thick", 2.));
		ColorByPart = Par.GetBool(Key + "Radial@PartColor", true);
		Color.R() = float(Par.GetDouble(Key + "Radial/Color@R", 0.5));
		Color.G() = float(Par.GetDouble(Key + "Radial/Color@G", 0.5));
		Color.B() = float(Par.GetDouble(Key + "Radial/Color@B", 0.5));
		Color.A() = 1.;
		Default = true;
		break;
	}
	return *this;
}
