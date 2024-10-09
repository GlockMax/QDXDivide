#pragma once

// BFullScreen command target

class AFX_EXT_CLASS BFullScreen : public CObject
{
public:
	BFullScreen(void);
	virtual ~BFullScreen();
	bool Activate(CFrameWnd *pParent);
	bool DeActivate(void);
	bool IsActive(void) const { return Active; };
protected:
	bool Active;
	CFrameWnd *pParent;
	HMENU HMenu;
};


