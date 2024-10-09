// QColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "QColorButton.h"


// QColorButton

IMPLEMENT_DYNAMIC(QColorButton, CButton)
QColorButton::QColorButton()
{
}

QColorButton::~QColorButton()
{
}


BEGIN_MESSAGE_MAP(QColorButton, CButton)
END_MESSAGE_MAP()



void QColorButton::SetColor(COLORREF NewColor)
{
	COLORMAP Map;
	Map.from = RGB(255, 255, 255);
	Map.to = NewColor;
	ColorButtonBmp.DeleteObject();
	ColorButtonBmp.LoadMappedBitmap(IDB_COLOR_BUTTON_BMP, 0, &Map, 1);
	SetBitmap(HBITMAP(ColorButtonBmp));
	Color = NewColor;
}
COLORREF QColorButton::GetColor(void)
{
	return Color;
}

