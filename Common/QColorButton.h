#pragma once
#include "COMPARE_API.h"


// QColorButton

class COMPARE_API QColorButton : public CButton
{
	DECLARE_DYNAMIC(QColorButton)

public:
	QColorButton();
	virtual ~QColorButton();
	void SetColor(COLORREF NewColor);
	COLORREF GetColor(void);//взять текущий цвет кнопки

protected:
	COLORREF Color;
	CBitmap ColorButtonBmp;
	DECLARE_MESSAGE_MAP()
};


