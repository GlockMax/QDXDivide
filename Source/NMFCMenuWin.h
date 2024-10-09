#pragma once

// NMFCMenuWin command target

class NMFCMenuWin : public CMFCToolBarMenuButton
{
	DECLARE_DYNCREATE(NMFCMenuWin)
public:
	NMFCMenuWin();
	~NMFCMenuWin();
	NMFCMenuWin(UINT uiID, HMENU hMenu, int iImage) : CMFCToolBarMenuButton(uiID, hMenu, iImage){};
	SIZE OnCalculateSize(
		CDC* pDC,
		const CSize& sizeDefault,
		BOOL bHorz
		) override;
protected:
	static const int BUTTON_NWIDTH = 100;
};


