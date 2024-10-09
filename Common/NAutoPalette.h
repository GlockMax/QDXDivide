#pragma once
#include "Stdafx.h"

class AFX_EXT_CLASS NAutoPalette
{
public:
	NAutoPalette(int PalettesNum, int PaletteSize);
	~NAutoPalette(void);
	bool SetColor(COLORREF Col, int ColNum, int PalNum = 0);
	COLORREF GetColor(int ColNum, int PalNum = 0);
	COLORREF GetRecurColor(int PalNum = 0);
	COLORREF *GetPalPtr(int PalNum = 0);
	int GetColorsCount() const { return NColors; }
protected:
	COLORREF *Colors;
	int NColors;
	int NPalettes;
	int CurNum;
};
