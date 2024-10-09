#include "stdafx.h"
#include "MTpalette.h"

MTPalette::MTPalette(void) : NAutoPalette(1, 16)
{
	SetColor(RGB(255, 204, 51),0, 0);
	SetColor(RGB(215, 215, 215), 1, 0);
	SetColor(RGB(0, 255, 255), 2, 0);
	SetColor(RGB(255, 0, 255), 3, 0);
	SetColor(RGB(0, 0, 255), 4, 0);
	SetColor(RGB(255, 255, 0), 5, 0);
	SetColor(RGB(0, 255, 0), 6, 0);
	SetColor(RGB(247, 154, 190), 7, 0);
	SetColor(RGB(153, 153, 255), 8, 0);
	SetColor(RGB(153, 204, 51), 9, 0);
	SetColor(RGB(255, 102, 51), 10, 0);
	SetColor(RGB(128, 128, 128), 11, 0);
	SetColor(RGB(51, 153, 255), 12, 0);
	SetColor(RGB(255, 153, 51), 13, 0);
	SetColor(RGB(204, 255, 204), 14, 0);
	SetColor(RGB(0, 204, 204), 15, 0);
}

MTPalette::~MTPalette(void)
{
}
