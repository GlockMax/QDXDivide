// NToolPRect.h: interface for the NToolPRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTOOLPRECT_H__77837768_3AB2_11D4_8F8C_000021667618__INCLUDED_)
#define AFX_NTOOLPRECT_H__77837768_3AB2_11D4_8F8C_000021667618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NToolPunch.h"

class AFX_EXT_CLASS NToolPRect : public NToolPunch  
{
public:
	void Draw(RenderMode GlobRMode, int step) override;
	NToolPRect();
	virtual ~NToolPRect();

protected:
	GLUquadricObj* QObj;
	double X;
	double Y;
	double H;
	double R;
	double Angle;
public:
	NToolPRect(double X, double Y, double H, double R, double Angle);
};

#endif // !defined(AFX_NTOOLPRECT_H__77837768_3AB2_11D4_8F8C_000021667618__INCLUDED_)
