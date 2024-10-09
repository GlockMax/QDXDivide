// InfoProgram.h: interface for the InfoProgram class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class AFX_EXT_CLASS InfoProgram
{
	friend class NCMData;
	friend class NProgram;
public:
	virtual void Serialize(CArchive &ar);
	InfoProgram(const InfoProgram &In);
	void Clear();
	InfoProgram();
	virtual ~InfoProgram();
	InfoProgram & operator =(const InfoProgram &In);

	double Time;
	double FastTime;
	double WorkTime;
	double FastLength;
	double WorkLength;
	double CutVolume;
	double MechEnergy;

	int StepsNumber;
	int LinesNumber;
	CString Comment;
	CString FileName;
protected:
};
