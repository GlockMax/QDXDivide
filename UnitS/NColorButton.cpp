// NColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NColorButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// NColorButton

IMPLEMENT_DYNAMIC(NColorButton, CButton)

NColorButton::NColorButton()
{
	Enabled = true;
}

NColorButton::~NColorButton()
{
}


BEGIN_MESSAGE_MAP(NColorButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &NColorButton::OnBnClicked)
END_MESSAGE_MAP()



// NColorButton message handlers



void NColorButton::Update()
{
	COLORMAP Map;
	Map.from = RGB(255, 255, 255);
	Map.to = Color;
	ColorButtonBmp.DeleteObject();
	ColorButtonBmp.LoadMappedBitmap(IDB_COLOR_BUTTON_BMP, 0, &Map, 1);
	SetBitmap(HBITMAP(ColorButtonBmp));
}

void NColorButton::OnBnClicked()
{
	if(!Enabled)
		return;
	CColorDialog ColorDialog(Color, 0, this);
	ColorDialog.m_cc.Flags |= CC_FULLOPEN;
	if(ColorDialog.DoModal() != IDOK)
		return;
	Color = ColorDialog.GetColor();
	Update();
}
void NColorButton::Enable(bool En)
{
	Enabled = En;
}
