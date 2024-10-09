// NCStatic.cpp : implementation file
//

#include "stdafx.h"
#include "NCStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NCStatic

NCStatic::NCStatic()
{
	m_clrText = RGB( 0, 0, 255 );
	m_clrBkgnd = GetSysColor(COLOR_BTNFACE);    // text bkgnd
	m_brBkgnd.CreateSolidBrush( m_clrBkgnd );
}

NCStatic::~NCStatic()
{
}


BEGIN_MESSAGE_MAP(NCStatic, CStatic)
	//{{AFX_MSG_MAP(NCStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NCStatic message handlers

HBRUSH NCStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// Change any attributes of the DC here
	pDC->SetTextColor( m_clrText );    // text
	pDC->SetBkColor( m_clrBkgnd );    //bkground
	return m_brBkgnd;                // ctl bkgnd

	
	// Return a non-NULL brush if the parent's handler should not be called
	//return NULL;
}
