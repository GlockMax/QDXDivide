#define USEFONTBITMAPS 1
#define TEXTALIGNLEFT 3
#define TEXTALIGNRIGHT 4
#define TEXTALIGNCENTER 5

#pragma once
#include "OPENGLRENDER_API.h"

class OPENGLRENDER_API Text
{
public:
	int mode;
	int align;
	UINT texID;
	int height;
	int width;
	UINT listBase;
private:
	SIZE strwidth;
	HDC m_hDC;
public:
	bool Init(HDC hDC, int pmode, char* filename);

	void Draw3D(double x, double y, double z, const char* string) const;
	void Draw2D(double x, double y, int halign, double wndWidth,
		double wndHeight, char* string);
//************************************************************************
	Text(UINT base)
	{
		mode = USEFONTBITMAPS;
		height = 14;
		width = 0;
		align = TEXTALIGNLEFT;
		listBase = base;
	}

//************************************************************************
	virtual ~Text()
	{
		glDeleteLists(listBase, 256);
	}
};


