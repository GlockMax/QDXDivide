#pragma once
#include "PropPane.h"

// NPropPane

class NPropPane : public PropPane
{
	DECLARE_DYNAMIC(NPropPane)

public:
	NPropPane();
	virtual ~NPropPane();

protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


