// NStateUnitCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NStateUnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NStateUnitCtrl

NStateUnitCtrl::NStateUnitCtrl()
{
}

NStateUnitCtrl::~NStateUnitCtrl()
{
}


BEGIN_MESSAGE_MAP(NStateUnitCtrl, MListCtrl)
	//{{AFX_MSG_MAP(NStateUnitCtrl)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NStateUnitCtrl message handlers


void NStateUnitCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	SetItemState(phdn->iItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
COLORREF NStateUnitCtrl::OnGetCellTextColor(int nRow, int nColum)
{
	if(nColum == 1)
	{
		COLORREF Col(COLORREF(GetItemData(nRow)));
		if(Col != 0)
			return Col;
	}
	return MListCtrl::OnGetCellTextColor( nRow, nColum); 
}
