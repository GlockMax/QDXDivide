#pragma once

class AFX_EXT_CLASS NDraftCode
{
public:
	NDraftCode(void);
	~NDraftCode(void);
	void Clear(void);
	bool IsAngleDefined(void) const { return AngleDefined;}
	bool IsChamfDefined(void) const { return ChamfDefined;}
	bool IsRadDefined(void) const { return RadDefined;}
	bool IsCornerDefined(void) const { return RadDefined || ChamfDefined;}
	void SetAngle(double A);
	void SetChamf(double A);
	void SetRad(double A);
	double GetAngle(void) { return Angle;}
	void ClearAngle(void) {AngleDefined = false; AngleActive = false;}
	void ClearRad(void) {RadDefined = false;}
	void ClearChamf(void) {ChamfDefined = false;}
	void SetAngleActive(void) { AngleActive = true; }
	void ClearAngleActive(void) { AngleActive = false; }
	bool IsAngleActive(void) const { return AngleActive;}
	
protected:
	double Angle;
	double Chamf;
	double Rad;
	bool AngleDefined;
	bool ChamfDefined;
	bool RadDefined;
	bool AngleActive;
};
