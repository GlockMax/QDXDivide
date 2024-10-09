// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NEdit

NEdit::NEdit()
{
	m_x=0;m_y=0;
	m_cx=0;m_cy=0;
}

NEdit::~NEdit()
{
}


BEGIN_MESSAGE_MAP(NEdit, CEdit)
	//{{AFX_MSG_MAP(NEdit)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NEdit message handlers

void NEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->x=m_x;
	lpwndpos->y=m_y;
	if (lpwndpos->cx<m_cx)lpwndpos->cx=m_cx;
	if (lpwndpos->cy<m_cy)lpwndpos->cy=m_cy;
		
  

	CEdit::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	
}
