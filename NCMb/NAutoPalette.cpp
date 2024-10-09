#include "StdAfx.h"
#include "nautopalette.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NAutoPalette::NAutoPalette(int PalettesNum, int PaletteSize)
{
	NColors = PaletteSize;
	NPalettes = PalettesNum;
	Colors = new COLORREF[NColors * NPalettes];
	CurNum = 0;
}

NAutoPalette::~NAutoPalette(void)
{
	delete Colors;
}
bool NAutoPalette::SetColor(COLORREF Col, int ColNum, int PalNum)
{
	if(ColNum < 0 || ColNum >= NColors || PalNum < 0 || PalNum >= NPalettes)
		return false;

	Colors[PalNum * NColors + ColNum] = Col;
	return true;
}
COLORREF NAutoPalette::GetColor(int ColNum, int PalNum)
{
	if(ColNum < 0 || ColNum >= NColors || PalNum < 0 || PalNum >= NPalettes)
		return RGB(0,0,0);

	return Colors[PalNum * NColors + ColNum];
}
COLORREF *NAutoPalette::GetPalPtr(int PalNum)
{
	return Colors + NColors * PalNum;
}
COLORREF NAutoPalette::GetRecurColor(int PalNum)
{
	if(PalNum == 0)
	{
		CurNum = (CurNum + 1) % NColors;
		return Colors[CurNum];
	}

	if(PalNum < 0 || PalNum >= NPalettes)
		return RGB(0,0,0);

	return Colors[PalNum * NColors + CurNum];
}
