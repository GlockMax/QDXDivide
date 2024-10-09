#pragma once
#include "NColor.h"

// NColorButton

class AFX_EXT_CLASS NColorButton : public CButton
{
	DECLARE_DYNAMIC(NColorButton)

public:
	NColorButton();
	virtual ~NColorButton();
	void Update();
	COLORREF GetColor(void) { return Color;}
	COLORREF Color;
	void SetColor(const NColor &Col) { Color = Col.GetRGB();}
	void Enable(bool En);

protected:
	DECLARE_MESSAGE_MAP()
	CBitmap ColorButtonBmp;
	bool Enabled;

public:
	afx_msg void OnBnClicked();
};


