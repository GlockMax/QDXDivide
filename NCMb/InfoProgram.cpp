// InfoProgram.cpp: implementation of the InfoProgram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoProgram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InfoProgram::InfoProgram()
{
	Clear();
	Comment = "";
}

InfoProgram::~InfoProgram()
{

}

void InfoProgram::Clear()
{
	WorkLength = 0.;
	FastLength = 0.;
	FastTime = 0.;
	WorkTime = 0.;
	Time = 0.;
	CutVolume = 0.;
	MechEnergy = 0.;
	LinesNumber = 0;
	StepsNumber = 0;
//	FileName = "";
}
InfoProgram & InfoProgram::operator =(const InfoProgram &In)
{
	StepsNumber=	In.StepsNumber;
	Time = In.Time;
	Comment = In.Comment;
	WorkLength = In.WorkLength;
	FastLength = In.FastLength;
	LinesNumber = In.LinesNumber;
	FileName = In.FileName;
	FastTime = In.FastTime;
	WorkTime = In.WorkTime;
	CutVolume = In.CutVolume;
	MechEnergy = In.MechEnergy;
	return *this;
}


InfoProgram::InfoProgram(const InfoProgram &In)
{
	*this = In;
}



void InfoProgram::Serialize(CArchive &ar)
{
	SerializeElements(ar,&WorkLength,1);
	SerializeElements(ar,&FastLength,1);
	SerializeElements(ar,&LinesNumber,1);
	SerializeElements(ar,&StepsNumber,1);
	SerializeElements(ar,&Time,1);
	SerializeElements(ar,&FileName,1);
	SerializeElements(ar,&Comment,1);
}


