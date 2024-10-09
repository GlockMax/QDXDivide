// DDimEntCrSheet.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "DDimEntCrSheet.h"
#include "DPrimPoint.h"
#include "DPrimSegment.h"
#include "DPrimArc.h"
#include "DPrimPlane.h"
#include "DPrimAutofix.h"
#include "DPrimDimens.h"


// DDimEntCrSheet

IMPLEMENT_DYNAMIC(DDimEntCrSheet, CPropertySheet)

DDimEntCrSheet::DDimEntCrSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

DDimEntCrSheet::DDimEntCrSheet()
	:CMFCPropertySheet()
{

}

DDimEntCrSheet::~DDimEntCrSheet()
{
}


BEGIN_MESSAGE_MAP(DDimEntCrSheet, CPropertySheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// DDimEntCrSheet message handlers

void DDimEntCrSheet::OnClose()
{
	((CMainFrame *)AfxGetMainWnd())->ClearDimEntCrSheet();
	ShowWindow(SW_HIDE);
}


void DDimEntCrSheet::OnDestroy()
{
	CPropertySheet::OnDestroy();

	for(int i = 0; i < GetPageCount(); ++i)
		delete GetPage(i);
}

void DDimEntCrSheet::PostNcDestroy()
{
	CPropertySheet::PostNcDestroy();
	delete this;
}

