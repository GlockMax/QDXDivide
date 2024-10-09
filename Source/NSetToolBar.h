#pragma once

class NSetToolBar
{
public:
	NSetToolBar(CMFCToolBar &ToolBar, UINT *IDsArray, int IDsArraySize);
	~NSetToolBar(void);
	CMFCToolBar &ToolBar;
	static int BSize;
	CMFCToolBarImages *pToolBarImages;
	CMFCToolBarImages *pMenuBarImages;
	CMFCToolBarImages *pToolBarImagesDisabled;
	CMFCToolBarImages *pToolLargeBarImages;
	CMFCToolBarImages *pToolLargeBarImagesDisabled;
	UINT *IDs;
	int IDsSize;
	static HMODULE HIns;
	bool SetButton(int IDInd, int IconID, int IconID_H = -1);
	bool SetButton(CMFCToolBarButton *pButton, int IconID, int IconID_H = -1);
	static int AddIconTo(int IconID, CMFCToolBarImages *pImages, int Size);

};
