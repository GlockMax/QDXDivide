#include "stdafx.h"
#include "NSetToolBar.h"
int NSetToolBar::BSize = 16;
HMODULE NSetToolBar::HIns = NULL;
NSetToolBar::NSetToolBar(CMFCToolBar &TB, UINT *IDsArray, int IDsArraySize)
: ToolBar(TB)
{
	IDsSize = IDsArraySize;
	IDs = IDsArray;
	pToolBarImages = CMFCToolBar::GetImages();
	pToolBarImagesDisabled = CMFCToolBar::GetDisabledImages();
	pToolLargeBarImages = CMFCToolBar::GetLargeImages();
	pToolLargeBarImagesDisabled = CMFCToolBar::GetLargeDisabledImages();
	pMenuBarImages = CMFCToolBar::GetMenuImages();
	ToolBar.SetButtons(IDs, IDsSize);
}

NSetToolBar::~NSetToolBar(void)
{
	ToolBar.AdjustLayout();
}

bool NSetToolBar::SetButton(int IDInd, int IconID, int IconID_H)
{
	int ImageIndex = AddIconTo(IconID, pToolBarImages, 16);
	ImageIndex = AddIconTo(IconID, pToolLargeBarImages, 24);
	ImageIndex = AddIconTo(IconID_H < 0 ? IconID : IconID_H, pToolBarImagesDisabled, 16);
	ImageIndex = AddIconTo(IconID_H < 0 ? IconID : IconID_H, pToolLargeBarImagesDisabled, 24);
	ToolBar.SetButtonInfo(IDInd, IDs[IDInd], TBBS_BUTTON, ImageIndex); 
	ImageIndex = AddIconTo(IconID, pMenuBarImages, 16);
	return true;
}

int NSetToolBar::AddIconTo(int IconID, CMFCToolBarImages *pImages, int Size)
{
	HICON tmp = (HICON)LoadImage(HIns, MAKEINTRESOURCE(IconID), IMAGE_ICON, Size, Size, LR_DEFAULTCOLOR);
	int ImageIndex = pImages->AddIcon(tmp);
	BOOL res = DestroyIcon(tmp);
	return ImageIndex;
}

bool NSetToolBar::SetButton(CMFCToolBarButton *pButton, int IconID, int IconID_H)

{
	int ImageIndex = AddIconTo(IconID, pToolBarImages, 16);
	ImageIndex = AddIconTo(IconID, pToolLargeBarImages, 24);
	ImageIndex = AddIconTo(IconID_H < 0 ? IconID : IconID_H, pToolBarImagesDisabled, 16);
	ImageIndex = AddIconTo(IconID_H < 0 ? IconID : IconID_H, pToolLargeBarImagesDisabled, 24);
	if(pButton)
		pButton->SetImage(ImageIndex);
	ImageIndex = AddIconTo(IconID, pMenuBarImages, 16);
	return true;
}
