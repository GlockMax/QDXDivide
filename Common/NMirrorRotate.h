// NMirrorRotate.h: interface for the NMirrorRotate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMIRRORROTATE_H__99E34F93_A963_11D4_8EA9_00C0DFF66F9B__INCLUDED_)
#define AFX_NMIRRORROTATE_H__99E34F93_A963_11D4_8EA9_00C0DFF66F9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BMatr.h"
class AFX_EXT_CLASS NMirrorRotate  
{
public:
	void SetAngle(double a);
	BOOL IsMirror();
	void SetXY(double m00,double m01,double m10,double m11);
	void Reset();
	BOOL IsDefined();
	double GetY(double x,double y);
	double GetX(double x,double y);
	NMirrorRotate();
	virtual ~NMirrorRotate();

protected:
	BMatr Matr;
	BOOL Defined;
};

#endif // !defined(AFX_NMIRRORROTATE_H__99E34F93_A963_11D4_8EA9_00C0DFF66F9B__INCLUDED_)
