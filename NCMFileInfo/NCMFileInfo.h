// NCMFileInfo.h : main header file for the NCMFILEINFO application
//

#if !defined(AFX_NCMFILEINFO_H__679A9CCC_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_)
#define AFX_NCMFILEINFO_H__679A9CCC_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNCMFileInfoApp:
// See NCMFileInfo.cpp for the implementation of this class
//

class CNCMFileInfoApp : public CWinApp
{
public:
	CNCMFileInfoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCMFileInfoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNCMFileInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCMFILEINFO_H__679A9CCC_C2D0_11D5_B0AC_0050BF4A8273__INCLUDED_)
