// NxyzImage.h: interface for the NxyzImage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "OPENGLRENDER_API.h"
enum AxesNumber
{
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8,
	NINE = 9,
	NIL = 0, 
	MINUS = -1
};

class BPoint;

class OPENGLRENDER_API NxyzImage
{
protected:	
	void DrawAxesZero(double s, char *string = NULL, bool highlight = false);

public:
	double Axes1Size,Axes2_xSize,Axes2_ySize,Axes2_zSizePl,Axes2_zSizeMin;
	BOOL IsVisibleZeroDash; 
	BOOL IsVisibleZeroXYZ;
	BOOL IsVisibleBase;
	BOOL AbsoluteSize;
	BOOL IsVisible;

	double s;
	double s1;
	double x, y, z;

	int num_x;
	int num_y;
	int num_zpl;
	int num_zmin;
	int ZeroPos;

	void SetParam(BOOL is_vis = TRUE,
						  BOOL is_visbase = TRUE,
						  int ZeroPos = TRUE,
						  BOOL abs_size = FALSE,
						  BOOL is_viszer_xy = FALSE);
	void SetSizes(double s1,double s2,double s3,double s4,double s5);
	void Draw(bool DrawStockMarker = false, double Brightness = 1.);
	void DrawBase(bool DrawStockMarker = false, double Brightness = 1., char *string = NULL, bool highlight = false);
	void CalcSValue();
	void DrawZero(bool DrawStockMarker = false, double Brightness = 1., char *string = NULL, bool highlight = false);
	NxyzImage();
	virtual ~NxyzImage();

	static void DrawLetter(char  Letter, const BPoint & P0, const BPoint & Ex, const BPoint & Ey);
	void DrawStockMarker(double x, double y, double z) const;
};
