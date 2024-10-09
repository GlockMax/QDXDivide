// onfigList.cpp : implementation file
//

#include "stdafx.h"
#include "NetConfigEdit.h"
#include "ConfigList.h"


// ConfigList

IMPLEMENT_DYNAMIC(ConfigList, CListBox)

ConfigList::ConfigList()
{
	Config = 0;
}

ConfigList::~ConfigList()
{
}


BEGIN_MESSAGE_MAP(ConfigList, CListBox)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// ConfigList message handlers



void ConfigList::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL Outside;
	int Ind = ItemFromPoint(point, Outside);
	if(Outside)
		return;

	SetSel(Ind);
	CString Text;
	GetText(Ind, Text);
	if(Text == Enabled)
		Text = Disabled;
	else
		Text = Enabled;

	DeleteString(Ind);
	InsertString(Ind, Text);
	CListBox::OnLButtonDown(nFlags, point);
}
