// MTEditPane.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "MTEditPane.h"
#include "MTAxis.h"
#include "MTForwardAxis.h"
#include "MTRotaryAxis.h"
#include "MTPartAxis.h"
#include "SPropChange.h"
#include "MTConfig.h"
#include "MTEditSliderButton.h"
#include "MTEditPane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

// MTEditPane

IMPLEMENT_DYNAMIC(MTEditPane, CDockablePane)

MTEditPane::MTEditPane()
{
	Deactivate();
}

MTEditPane::~MTEditPane()
{
}

void MTEditPane::Deactivate()
{
	EditingObject = 1;
	fSlider = false;
	EditingMT = nullptr;
	EditingAxisIndex = -1;
	EditingObject = -1;
	EditingColIndex = -1;
}


BEGIN_MESSAGE_MAP(MTEditPane, CDockablePane)
	ON_COMMAND(ID_PROP_TOOLBAR_EXPAND, OnToolBarButtonExpand)
	ON_COMMAND(ID_PROP_TOOLBAR_APPLY, OnToolBarButtonApply)
	ON_COMMAND(ID_PROP_TOOLBAR_ADDSP, OnToolBarButtonAddsp)
	ON_COMMAND(ID_SLIDER, OnSlider)
	ON_UPDATE_COMMAND_UI(ID_SLIDER, OnUpdateStateTBSlider)
	ON_UPDATE_COMMAND_UI(ID_PROP_TOOLBAR_APPLY, OnUpdateStateTBApply)
	ON_UPDATE_COMMAND_UI(ID_PROP_TOOLBAR_ADDSP, OnUpdateToolBarButtonAddsp)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void MTEditPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient, rectPropList, rectPropDlg;
	GetClientRect(rectClient);

	int cyTlb = mtPropToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//mtPropCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	mtPropToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	mtPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb , SWP_NOACTIVATE | SWP_NOZORDER);

	
}
//void MTEditPane::AdjustLayout()
bool MTEditPane::IsClosingAllowed(void)
{
	if(mtPropList.IsPropertyGridModified())
	{
		int res = AfxMessageBox(IDS_EDITOR_LEAVE_PROP, MB_YESNOCANCEL);
		switch(res)
		{
		case IDYES:
			ApplyChanges();
			break;
		case IDNO:
			RevertChanges();
			break;
		case IDCANCEL:
			return false;
		}
	}
	return true;
}

void MTEditPane::OnPressCloseButton()
{
	if(!IsClosingAllowed())
		return;
	CDockablePane::OnPressCloseButton();
	fSlider = false;
}

void MTEditPane::OnToolBarButtonExpand(void)
{
	mtPropList.ExpandCollapse();
	return;
}//void MTEditPane::OnToolBarButtonExpand(void)

void MTEditPane::OnToolBarButtonApply(void)
{
	ApplyChanges();
}//void MTEditPane::OnToolBarButtonApply(void);

void MTEditPane::OnToolBarButtonAddsp(void)
{
	AddSpecPoint();
}//void MTEditPane::OnToolBarButtonApply(void);

BOOL MTEditPane::AllowShowOnPaneMenu() const
{
	return false;
}

void MTEditPane::OnSlider()
{
	MoveAxis(MTEditSliderButton::GetPos(ID_SLIDER)/100.);
}

void MTEditPane::OnUpdateStateTBSlider(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(fSlider);
}

void MTEditPane::OnUpdateStateTBApply(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(mtPropList.IsPropertyGridModified());
}//void MTEditPane::OnUpdateStateTBApply(CCmdUI *pCmdUI)

void MTEditPane::OnUpdateToolBarButtonAddsp(CCmdUI *pCmdUI)
{

	bool Enable = (EditingMT != nullptr) && (EditingAxisIndex >= 0) && (EditingObject == 1);
	if (Enable)
		Enable &= (dynamic_cast<MTRelocateAxis *>(EditingMT->GetCurConfig()[EditingAxisIndex]) != nullptr);
	pCmdUI->Enable(Enable);
}

void MTEditPane::SetMTEdit(MTEdit *MTE, const CString &MachinePath)
{
	MTEditPtr = MTE;
	mtPropList.SetMachinePath(MachinePath);
}//void MTEditPane::SetMTEdit(MTEdit &MTE)

void MTEditPane::SetMachineTool(MTMachineTool *MT)
{
	EditingMT = MT;
	mtPropList.SetMachineTool(MT);

	EditingAxisIndex = -1;
	fSlider = false;
}//void MTEditPane::SetMachineTool(MTMachineTool *MT)

void MTEditPane::ShowAxisProp(int index)
{
	if(index < 0)
		return;
	EditingObject = 1;
	MTEditSliderButton::SetPos(ID_SLIDER,50);
	fSlider = true;

	EditingAxisIndex = index;
	mtPropList.ShowAxisProp(index);

	return;
}

void MTEditPane::ShowColProp(int index)
{
	EditingObject = 0;
	fSlider = false;

	EditingColIndex = index;
	
	mtPropList.ShowColProp(index);
}//void MTEditPane::ShowColProp(int index)

void MTEditPane::ShowEmptyProp()
{
	EditingObject = 2;
	fSlider = false;
	mtPropList.ShowEmptyProp();
}//void MTEditPane::ShowEmptyProp()
	
void MTEditPane::MoveAxis(double MovePortion)
{
	double Min = EditingMT->GetCurConfig()[EditingAxisIndex]->GetMin();
	double Max = EditingMT->GetCurConfig()[EditingAxisIndex]->GetMax();
	if (Min == 0 && Max == 0)
	{
		Min = -720.;
		Max = 720.;
	}

	double Move = Min + (Max - Min) * MovePortion;

	EditingMT->GetCurConfig()[EditingAxisIndex]->Reset();

	MTEditPtr->SetCoordValue(EditingMT->GetCurConfig()[EditingAxisIndex]->GetName(), Move);

	MTEditPtr->UpdatePicture();

}

void MTEditPane::ApplyChanges()
{
	SPropChange *Change = new SPropChange;

	mtPropList.ReadChanges(Change);

	switch (EditingObject)
	{
	case 1:
		//Ось
		MTEditPtr->ChangeAxisProp(Change, EditingAxisIndex);
		break;
	case 0:
		//объект столкновения
		MTEditPtr->ChangeColProp(Change, EditingColIndex);
		break;
	}
	delete Change;
//	mtPropList.MakeAxisList();
	ShowAxisProp(EditingAxisIndex);
}//void MTEditPane::ApplyChanges()

void MTEditPane::AddSpecPoint()
{
	MTRelocateAxis *pRelocateAxis = dynamic_cast<MTRelocateAxis *>(EditingMT->GetCurConfig()[EditingAxisIndex]);
	if (pRelocateAxis == nullptr)
		return;
	auto MaxSize = EditingMT->GetMaxSpecPtsSize();
	pRelocateAxis->SpecialPts.resize(MaxSize + 1);
	mtPropList.ChangeSpecPosList();
//	ShowAxisProp(EditingAxisIndex);
}//void MTEditPane::ApplyChanges()

void MTEditPane::RevertChanges()
{
	mtPropList.RevertChanges();
}

bool MTEditPane::IsChangingNodeAllowed()
{
	mtPropList.SetCurSel(NULL);// To apply last change
	if(mtPropList.IsPropertyGridModified())
	{
		int res = AfxMessageBox(IDS_EDITOR_CHANGE_PROP, MB_OKCANCEL);
		switch(res)
		{
		case IDOK:
			RevertChanges();
			break;
		case IDCANCEL:
			return false;
		}
	}//if
	return true;
}//bool MTEditPane::IsChangingNodeAllowed()

int MTEditPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

		//Create proplist
	if (!mtPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, IDC_PROPLIST))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

//	mtPropList.MakeAxisList();
	
	mtPropToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	mtPropToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);

	//OnChangeVisualStyle();
	//Прикручиваем slider к тулбару
	MTEditSliderButton btnSlider (ID_SLIDER);
	btnSlider.SetRange (0, 100);
	mtPropToolBar.ReplaceButton (ID_SLIDER, btnSlider);

	mtPropToolBar.SetPaneStyle(mtPropToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	mtPropToolBar.SetPaneStyle(mtPropToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	mtPropToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	mtPropToolBar.SetRouteCommandsViaFrame(FALSE);
	
	AdjustLayout();

	this->SetMinSize(CSize(180, 30));

	return 0;
}//int MTEditPane::OnCreate(LPCREATESTRUCT lpCreateStruct)

void MTEditPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}//int MTEditPane::OnSize(UINT nType, int cx, int cy)
