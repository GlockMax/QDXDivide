// InfoTool.h: interface for the InfoTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class AFX_EXT_CLASS InfoTool  
{
public:
	void Clear();
	void Serialize(CArchive &ar);

	double Time;
	double FastTime;
	double WorkTime;
	double FastLength;
	double WorkLength;
	double CutVolume;
	double MechEnergy;

	int Pos;
	int Turret;
	CString Name;
	CString Type;
	InfoTool();
	virtual ~InfoTool();

};
