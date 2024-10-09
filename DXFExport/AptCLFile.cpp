// AptCLFile.cpp: implementation of the AptCLFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResInv.h"
#include "math.h"
#include "AptCLFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AptCLFile::AptCLFile()
{

}

AptCLFile::~AptCLFile()
{

}

BOOL AptCLFile::Create(const CString &FileName)
{
	file.open(FileName);
	if(!file.is_open())
		return FALSE;

	CString str;
	str.LoadString(IDS_TITLE);
	file << str << std::endl;

	return TRUE;
}

BOOL AptCLFile::Close()
{
	file.close();
	return TRUE;
}

void AptCLFile::Fast(double x, double y, double z)
{
// x,y,z - end point
	CString str;
	str.LoadString(IDS_RAPID);
	file << str << std::endl;
	str.Format(IDS_GOTO , x,y,z);
	file << str << std::endl;

}

void AptCLFile::Line(double x, double y, double z)
{
// x,y,z - end point
	CString str;
	str.Format(IDS_GOTO , x,y,z);
	file << str << std::endl;
}

void AptCLFile::Arc(int type,double xb, double yb, double zb, double xe, double ye, double ze, double i, double j, double k, double nx, double ny, double nz)
{
// type: 1 - cwarc | 0 - ccwarc
// xb,yb,zb - start point
// xe,ye,ze - end point
// i,k,k 
// xn,yn,zn - normal
	double r = i*i + j*j + k*k;
	if(r > 0.)
		r =sqrt(r);
	int sign = type == 0 ? 1 : -1;
	CString str;
	str.Format(IDS_CIRCLE ,	xb + i,yb + j,zb + k,
							sign*nx, sign*ny, sign*nz, //The circle is always counter clockwise about the normal
							r );
	file << str << std::endl;
	str.Format(IDS_GOTO ,  xe,ye,ze);
	file << str << std::endl;
}

void AptCLFile::Feed(int value, int fm)
{
// value - feedrate value
// fm - units: 0 - empty, 1 - MMPM, 2 - MMPR, 3 - IPM, 4 - IPR, 5 - FPM, 6 - FPR
	CString units;
	switch (fm)
	{
	case 0:
		units = "";
		break;
	case 1:
		units = ",MMPM";
		break;
	case 2:
		units = ",MMPR";
		break;
	case 3:
		units = ",IPM";
		break;
	case 4:
		units = ",IPR";
		break;
	case 5:
		units = ",FPM";
		break;
	case 6:
		units = ",FPR";
		break;
	}
	CString str;
	str.Format(IDS_FEDRAT , value, units);
	file << str << std::endl;

}

void AptCLFile::CutComOn(BOOL side, int num)
{
// side - TRUE - left, FALSE - right
	CString str;
	if(side)
		str.Format(IDS_COMLEFT, num);
	else
		str.Format(IDS_COMRIGHT, num);
	file << str << std::endl;
}

void AptCLFile::CutComOf()
{
// 
	CString str;
	str.LoadString(IDS_COMOFF);
	file << str << std::endl;
}

void AptCLFile::SpindlOff()
{
	CString str;
	str.LoadString(IDS_SPINDLOFF);
	file << str << std::endl;
}

void AptCLFile::Spindl(BOOL dir, int speed)
{
// dir - TRUE - cclw, FALSE - ccw
	CString Sdir;
	if(dir)
		Sdir.LoadString(IDS_SPINCCW);
	else
		Sdir.LoadString(IDS_SPINCLW);

	CString str;
	str.Format(IDS_SPINDL , speed, Sdir);
	file << str << std::endl;
}

void AptCLFile::Spindl(int speed)
{
	CString str;
	str.Format(IDS_SPINDL , speed,"");
	file << str << std::endl;
}

void AptCLFile::SpindlOn()
{
	CString str;
	str.LoadString(IDS_SPINDLON);
	file << str << std::endl;
}

void AptCLFile::LoadTl(int num)
{
// Tool change
	CString str;
	str.Format(IDS_LOADTL , num);
	file << str << std::endl;
}

void AptCLFile::Remark(const CString &remstr)
{
	CString str;
	str.Format(IDS_REMARK , remstr);
	file << str << std::endl;
}

void AptCLFile::CycleOff()
{
	CString str;
	str.LoadString(IDS_CYCLEOFF);
	file << str << std::endl;
}

void AptCLFile::CycleOn(int type, double depth, int feed, int fm, double clear, double rapdepth)
{
	CString str;
	//str.LoadString(IDS_CYCLETYPE);
	//CString SType("");
	//int pos = 0;
	//for(int i = 0; i < type ; ++i)
	//{
	//	pos = str.Find('/',pos);
	//	++pos;
	//	if(pos >= str.GetLength())
	//		break;
	//}
	//if(pos < str.GetLength())
	//{
	//	str.Delete(0,pos);
	//	SType = str.SpanExcluding("/");
	//}

	str.Format(IDS_CYCLE, GetSubstr(IDS_CYCLETYPE, type),
		depth,_T("MMPM"), feed, clear, _T("RAPTO"), rapdepth);
	file << str << std::endl;
}

void AptCLFile::CoolantOff()
{
	CString str;
	str.LoadString(IDS_COOLANTOFF);
	file << str << std::endl;
}

void AptCLFile::Coolant(int type)
{
	CString str;
	str.Format(IDS_COOLANT , GetSubstr(IDS_COOLANTTYPE,type));
	file << str << std::endl;
}

CString AptCLFile::GetSubstr(int StrID, int n)
{
// Extract substring followed by n's symbol '/' from StrID string

	CString sub("");
	CString str;
	str.LoadString(StrID);
	int pos = 0;
	for(int i = 0; i < n ; ++i)
	{
		pos = str.Find('/',pos);
		++pos;
		if(pos >= str.GetLength())
			break;
	}
	if(pos < str.GetLength())
	{
		str.Delete(0,pos);
		sub = str.SpanExcluding("/");
	}
	return sub;
}
