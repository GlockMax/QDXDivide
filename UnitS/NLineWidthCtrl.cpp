// NLineWidthCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NLineWidthCtrl.h"

const int PIXW = 107;
const int WIDTHNUM = 4;
// NLineWidthCtrl

IMPLEMENT_DYNAMIC(NLineWidthCtrl, CComboBoxEx)

NLineWidthCtrl::NLineWidthCtrl()
{
}

NLineWidthCtrl::~NLineWidthCtrl()
{
}


BEGIN_MESSAGE_MAP(NLineWidthCtrl, CComboBoxEx)
END_MESSAGE_MAP()



// NLineWidthCtrl message handlers

void NLineWidthCtrl::Init()
{
	IList.Create(IDB_LINE_THICK, PIXW, 0, RGB(1, 1, 1));
	SetImageList(&IList);
	COMBOBOXEXITEM Item;
	memset(&Item, 0, sizeof(Item));
	Item.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	for(int i = WIDTHNUM; i >= 0; --i)
	{
		Item.iSelectedImage = Item.iImage = i;
		InsertItem(&Item);
	}
}
