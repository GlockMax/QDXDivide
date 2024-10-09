#pragma once
#include "NCMB_API.h"
#include "NColor.h"
class NCMB_API BEdgesAttrs
{
public:
	BEdgesAttrs();
	virtual ~BEdgesAttrs();
protected:
	NColor EdgesColor;
	double EdgesWidth;
	double SmoothAngle;
public:
	const NColor &GetEdgesColor(void) const { return EdgesColor; }
	void SetEdgesColor(const NColor & Col);
	double GetEdgesWidth(void) const { return EdgesWidth; }
	void SetEdgesWidth(double Width);
	double GetSmoothAngle(void) const { return SmoothAngle; }
	void SetSmoothAngle(double Angle);
	void Serialize(CArchive &ar);

};

