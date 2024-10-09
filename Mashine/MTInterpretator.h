// MTInterpretator.h: interface for the MTInterpretator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MTINTERPRETATOR_H__35CAD465_43C0_4CB8_8176_E912B87D7C8B__INCLUDED_)
#define AFX_MTINTERPRETATOR_H__35CAD465_43C0_4CB8_8176_E912B87D7C8B__INCLUDED_

#include "NCEnums.h"
#include "UserEnum.h"
#include "MTCommand.h"	// Added by ClassView
#include "MTGOTOCommand.h"
#include "MTCIRCLECommand.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BPoint.h"


class MTInterpretator  
{
public:
	BOOL GetNewPosition(int steps,int step,MTGOTOCommand &coord,MTGOTOCommand Start);
	void GetArcPoint(double u, int nknots, float *ctlarray, float *knot, double &x, double &y, double &z);
	int CreateSimpleArc(BPoint *ControlP);
	int CreateArc(BPoint pb, BPoint pe, BPoint i, BPoint n,int type,float *&ctlarray,float *&knot,double eps = 1.e-5,enum Plane pl = XY);
	int GetCount();
	BOOL NextCommand(/*MTCommand *retCommand*/);
	void SetParametrs(CString &Names,int Num);
	
	BOOL FirstCommand(/*MTCommand  *retCommand*/);
	BOOL NewProgram(CString File);
	MTInterpretator();
	virtual ~MTInterpretator();

protected:
	MTCIRCLECommand Circle;
	int Flag;
	CStringArray CoorN;
	BOOL Interpretate(CString str);
	int Counter;
	CString ProgramFile;
	MTGOTOCommand Command;
	int NumCoor;
public:
	void EraseAll();
	int GetNumberOfCadr();
	CString GetProgrammName();
	void Remote();
};

#endif // !defined(AFX_MTINTERPRETATOR_H__35CAD465_43C0_4CB8_8176_E912B87D7C8B__INCLUDED_)
