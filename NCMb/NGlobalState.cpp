#include "stdafx.h"
#include "..\NCMExt\NExtEnums.h"
#include "NCMProject.h"

NGlobalState::NGlobalState(void)
{
	Reset();
	AnimateMode = Animation;
	ModelMode = BSP;
	HostMode = false;
	RibbonInterface = false;
	SubStProgChange.Wnd = NULL;
	SubStProgChange.Pass = 0;
	SubStColl.Wnd = NULL;
	SubStColl.Pass = 0;
}

NGlobalState::~NGlobalState(void)
{
}
bool NGlobalState::GetCuttingMode(void) const
{ return CuttingMode;}
bool NGlobalState::GetAnimateMode(void) const
{ return (GetAnimMode() == Animation);}
bool NGlobalState::GetResultMode(void) const
{ return (GetAnimMode() == Result);}
void NGlobalState::SetCuttingMode(bool in)
{ CuttingMode = in;}
void NGlobalState::SetAnimateMode()
{ 
	AnimateMode = Animation;
}
void NGlobalState::SetResultMode()
{ 
	AnimateMode = Result;
}
void NGlobalState::SetEditMachMode(bool in)
{ EditMode = in ? Edit_Mach : Edit_no;}
NGlobalState::EditModes NGlobalState::GetEditMode() const
{ return EditMode; }

void NGlobalState::Reset()
{
	CuttingMode = false;
	AnimateMode = Animation;
	EditMode = Edit_no;
}

NGlobalState::AnimModes NGlobalState::GetAnimMode(void)const
{
	return AnimateMode;
}

bool NGlobalState::GetHostMode(void) const
{
	return HostMode;
}

void NGlobalState::SetHostMode(bool Val)
{
	HostMode = Val;
}

void NGlobalState::SetViewInterface(const CStringA& Style)
{
	if (Style == _T("GibbsCAM"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_CONTROL, VConditions(), VA_PREP_PAN, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, VConditions(), VA_PREP_ZOOMW, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_ROT, WM_MBUTTONUP);
	}
	else if (Style == _T("CATIA"))
	{
		ClearViewRules();
		//		AddViewRule(WM_MBUTTONDOWN, MK_MBUTTON, VConditions(), VA_PREP_PAN, WM_LBUTTONDOWN);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		//AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, VConditions(1, VK_MBUTTON), VA_PREP_ROT, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_LBUTTON, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_RBUTTON, VConditions(), VA_PREP_ROT, WM_RBUTTONUP);
		AddViewRule(WM_LBUTTONUP, MK_MBUTTON, VConditions(), VA_PREP_SCALE, WM_MBUTTONUP);
		AddViewRule(WM_RBUTTONUP, MK_MBUTTON, VConditions(), VA_PREP_SCALE, WM_MBUTTONUP);
	}
	else if (Style == _T("EdgeCAM"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON, VConditions(), VA_PREP_ROT, WM_RBUTTONUP);
	}
	else if (Style == _T("hyperMILL"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_CONTROL, VConditions(), VA_PREP_PAN, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_SHIFT, VConditions(), VA_PREP_SCALE, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON, VConditions(), VA_PREP_ROT, WM_RBUTTONUP);
	}
	else if (Style == _T("NX"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_SHIFT, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_RBUTTON, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_LBUTTON, VConditions(), VA_PREP_SCALE, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_ROT, WM_MBUTTONUP);
	}
	else if (Style == _T("PowerMILL"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_SHIFT, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_ROT, WM_MBUTTONUP);
	}
	else if (Style == _T("Pro/E"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_CONTROL, VConditions(), VA_PREP_PAN, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_CONTROL, VConditions(), VA_PREP_ROT, WM_MBUTTONUP);
	}
	else if (Style == _T("TopSolid"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_CONTROL, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);
	}
	else if (Style == _T("VERICUT"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_CONTROL | MK_SHIFT, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_CONTROL | MK_SHIFT, VConditions(), VA_PREP_ROT, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_CONTROL | MK_SHIFT, VConditions(), VA_PREP_ROT, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_SHIFT, VConditions(), VA_PREP_PAN, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_SHIFT, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_SHIFT, VConditions(), VA_PREP_PAN, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON, VConditions(), VA_PREP_PAN, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_RBUTTONUP);
	}
	else if (Style == _T("Wildfire"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_SHIFT, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_ROT, WM_MBUTTONUP);
	}
	else if (Style == _T("Cimatron"))
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON | MK_CONTROL, VConditions(), VA_PREP_PAN, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON | MK_CONTROL, VConditions(), VA_PREP_SCALE, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON | MK_CONTROL, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);
	}
	else
	{
		ClearViewRules();
		AddViewRule(WM_MOUSEMOVE, MK_RBUTTON, VConditions(), VA_PREP_PAN, WM_RBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_MBUTTON, VConditions(), VA_PREP_ZOOMW, WM_MBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, VConditions(1, 0x58), VA_PREP_ROTX, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, VConditions(1, 0x59), VA_PREP_ROTY, WM_LBUTTONUP);
		VConditions Conditions = { 0x5A};
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, Conditions, VA_PREP_ROTZ, WM_LBUTTONUP);
		AddViewRule(WM_MOUSEMOVE, MK_LBUTTON, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);

		AddViewRule(WM_LBUTTONDOWN, MK_CONTROL | MK_LBUTTON, VConditions(), VA_PREP_ROT, WM_LBUTTONUP);
		AddViewRule(WM_LBUTTONDOWN, MK_SHIFT | MK_LBUTTON, VConditions(), VA_PREP_PAN, WM_LBUTTONUP);
		AddViewRule(WM_LBUTTONDOWN, MK_LBUTTON, VConditions(1, VK_MENU), VA_PREP_SCALE, WM_LBUTTONUP);

		//AddViewRule(WM_RBUTTONDOWN, MK_LBUTTON | MK_RBUTTON, VConditions(), VA_DO_EMPTY, 0);
		//AddViewRule(WM_RBUTTONDOWN, MK_MBUTTON | MK_RBUTTON, VConditions(), VA_DO_EMPTY, 0);
		//AddViewRule(WM_RBUTTONDOWN, MK_LBUTTON | MK_MBUTTON | MK_RBUTTON, VConditions(), VA_DO_EMPTY, 0);

//		AddViewRule(WM_RBUTTONUP, MK_LBUTTON | MK_RBUTTON, VConditions(), VA_DO_EMPTY, 0);
		AddViewRule(WM_RBUTTONUP, MK_MBUTTON, VConditions(), VA_ZOOM_ALL, 0);
//		AddViewRule(WM_RBUTTONUP, MK_LBUTTON | MK_MBUTTON | MK_RBUTTON, VConditions(), VA_DO_EMPTY, 0);
	}
}

bool NGlobalState::ProcProgChangeSubscr(const CString &Name) const
{
	HWND Wnd = GetSubStProgChange().Wnd;
	if (Wnd != NULL)
	{
		if (IsWindow(Wnd))
		{
			CString BufName = Name;
			COPYDATASTRUCT Data;
			Data.dwData = GetSubStProgChange().Pass;
			Data.cbData = Name.GetLength();
			Data.lpData = BufName.GetBuffer();
			return (TRUE == ::SendMessage(Wnd, WM_COPYDATA, NULL, (LPARAM)&Data));
		}
	}
	return false;
}

bool NGlobalState::ProcCollSubscr(const CollisionData & CollD) const
{
	HWND Wnd = GetSubStColl().Wnd;
	if (Wnd != NULL)
	{
		if (IsWindow(Wnd))
		{
			COPYDATASTRUCT Data;
			Data.dwData = GetSubStColl().Pass;
			Data.cbData = sizeof(CollD);
			Data.lpData = (void *)(&CollD);
			return (TRUE == ::SendMessage(Wnd, WM_COPYDATA, NULL, (LPARAM)&Data));
		}
	}
	return false;
}

void NGlobalState::AddViewRule(WORD Message, UINT nFlag, const VConditions & Conditions, int ViewActionInd, int StopMessage)
{
	VRules.AddRule(Message, nFlag, Conditions, ViewActionInd, StopMessage);
}

std::pair<int, WORD> NGlobalState::GetViewActionInd(WORD Message, UINT nFlags) const
{
	return VRules.GetActionInd(Message, nFlags);
}
