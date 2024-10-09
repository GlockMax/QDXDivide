// Macro.h : main header file for the Macro DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMacroApp
// See Macro.cpp for the implementation of this class
//

class CMacroApp : public CWinApp
{
public:
	CMacroApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
