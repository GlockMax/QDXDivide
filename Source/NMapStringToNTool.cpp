#include "stdafx.h"
#include <algorithm>
#include "NCMProject.h"
#include "NPassiveObject.h"
#include "NMapStringToNTool.h"
#include "NTool.h"
#include "NToolID.h"
#include "ResLocal.h"
#include "NToolCombined.h"
#include "PropList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NMapStringToNTool::NMapStringToNTool(void)
{
	NeedProgRegen = false;
}

NMapStringToNTool::~NMapStringToNTool(void)
{
}
void NMapStringToNTool::Serialize(CArchive &ar)
{
	NMapStringToOb::Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		SerializeElements(ar, &NTool::ShapeVisibleTool, 1);
		if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() >= atof("4.4") && NCM_PROJECT.GetInFileVersion() < atof("4.5"))
		{
			int Tmp;
			SerializeElements(ar, &Tmp, 1);
			if (!(Tmp == FALSE || Tmp == TRUE))
				NTool::FastColorMode = (NFastColorMode)Tmp;
		}
		else
			SerializeElements(ar, &NTool::ShapeVisibleHolder, 1);

		SerializeElements(ar, &NTool::RMTool, 1);
		SerializeElements(ar, &NTool::RMHolder, 1);
		SerializeElements(ar, &NTool::Transparency, 1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.5")))
	{
		SerializeElements(ar, &NTool::FastColorMode, 1);
	}
	if (ar.IsLoading())
	{
		for each(const auto& el in *this)
		{
			NToolCombined* pTool = static_cast<NToolCombined*>(el.second);
			pTool->SetGroup(OG_TOOLS);
		}
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.0")))
	{
		SerializeElements(ar, &NTool::ToolStockCollision, 1);
		if(ar.IsLoading())
			NCMProject::GetProject().GetGlobalState().SetMultiThreadDX_And(NTool::GetToolStockCollision() == CWS_OFF);
		SerializeElements(ar, &NTool::XYOffset, 1);
		SerializeElements(ar, &NTool::ZOffset, 1);
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("2025")))
		{
			SerializeElements(ar, &NTool::XYOffsetSh, 1);
			SerializeElements(ar, &NTool::ZOffsetSh, 1);
			SerializeElements(ar, &NTool::TurnOffset, 1);
		}
	}
}

PropList * NMapStringToNTool::Create(PropList * List)
{
	List->Clear();
	// Tool visibility
	PropSingle * h = new PropSingle();
	h->ID=1;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_VISIBILITY_COMTOOL;
	h->Group=IDS_PROP_TOOLS;
	h->ST_Value= (NTool::ShapeVisibleTool ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	List->Add(h);
	// Tool render mode
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_DISPLAY_MODE;
	h->Type=1;
	h->Comment=IDS_COMM_DISPLAY_MODE_COMTOOL;
	h->Group=IDS_PROP_TOOLS;
	h->ST_Value = NTool::RMTool;
	h->For_Enum.Add(IDS_RM_SHADE);
	h->For_Enum.Add(IDS_RM_GLITTER);
	h->For_Enum.Add(IDS_RM_TRANSLUCENT);
	h->Color = 1;
	List->Add(h);
	// Transparency
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_TRANSPARENCY_DEGREE;
	h->Type=5;
	h->Comment=IDS_COMM_MACH_TRANSP;
	h->Group=IDS_PROP_TOOLS;
	h->Max=100;
	h->Min=0;
	h->V_Value=int((1. - NTool::Transparency) * 100. + 0.5);
	List->Add(h);
	// Holder visibility
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_VISIBILITY_HOLDER;
	h->Group=IDS_PROP_VISIBILITY_HOLDER;
	h->ST_Value= (NTool::ShapeVisibleHolder ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	List->Add(h);
	// Holder render mode
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_DISPLAY_MODE;
	h->Type=1;
	h->Comment=IDS_COMM_DISPLAY_MODE_HOLDER;
	h->Group=IDS_PROP_VISIBILITY_HOLDER;
	h->ST_Value = NTool::RMHolder;
	h->For_Enum.Add(IDS_RM_SHADE);
	h->For_Enum.Add(IDS_RM_GLITTER);
	h->For_Enum.Add(IDS_RM_TRANSLUCENT);
	h->Color = 1;
	List->Add(h);

//	// Collision with stock ON/OFF
//	if(NCMComData::GetType() == AT_TURN)
//	{
//		h = new PropSingle();
//		h->ID=6;
//		h->Name=IDS_TOOLSTOCK_ONOFF;
//		h->Type=1;
//		h->Comment=IDS_COMM_TOOLSTOCK_ONOFF;
//		h->Group=IDS_PROP_COLLISION_TOOLSTOCK;
//		h->ST_Value= NTool::GetToolStockCollision();
//		h->For_Enum.Add(IDS_TSC_CUT);
//		h->For_Enum.Add(IDS_FALSE);
//		h->Color = 1;
//		List->Add(h);
//		// IgnoreUpToolStockColl
//		h = new PropSingle();
//		h->ID=10;
//		h->Name=IDS_TSC_UPSTOP;
//		h->Type=1;
//		h->Comment=IDS_COMM_TSC_UPSTOP;
//		h->Group=IDS_PROP_COLLISION_TOOLSTOCK;
//		h->ST_Value= (NTool::IgnoreUpToolStockColl ? IDS_TRUE : IDS_FALSE);
//		h->For_Enum.Add(IDS_FALSE);
//		h->For_Enum.Add(IDS_TRUE);
//		h->Color = 1;
//		List->Add(h);
//	}
//	else
//	{		
//		h = new PropSingle();
//		h->ID=6;
//		h->Name=IDS_TOOLSTOCK_ONOFF;
//		h->Type=1;
//		h->Comment=IDS_COMM_TOOLSTOCK_ONOFF;
//		h->Group=IDS_PROP_COLLISION_TOOLSTOCK;
//		h->ST_Value= NTool::GetToolStockCollision();
//		h->For_Enum.Add(IDS_TSC_CUT);
//		h->For_Enum.Add(IDS_TSC_ALL);
//		h->For_Enum.Add(IDS_FALSE);
//		h->Color = 1;
//		List->Add(h);
//		h = new PropSingle();
//		h->ID=10;
//		h->Name=IDS_TSC_UPSTOP;
//		h->Type=1;
//		h->Comment=IDS_COMM_TSC_UPSTOP;
//		h->Group=IDS_PROP_COLLISION_TOOLSTOCK;
//		h->ST_Value= (NTool::IgnoreUpToolStockColl ? IDS_TRUE : IDS_FALSE);
//		h->For_Enum.Add(IDS_FALSE);
//		h->For_Enum.Add(IDS_TRUE);
//		h->Color = 1;
//		List->Add(h);
//		// Offset tool XY
//		h = new PropSingle();
//		h->ID=8;
//		h->Name=IDS_TSC_XY;
//		h->Type=4;
//		h->Comment=IDS_COMM_TSC_XY;
//		h->Group=IDS_PROP_COLLISION_TOOLSTOCK;
//		h->V_Value=NTool::XYOffset;
//		h->Color = 1;
//		List->Add(h);
//		// Offset tool Z
//		h = new PropSingle();
//		h->ID=9;
//		h->Name=IDS_TSC_Z;
//		h->Type=4;
//		h->Comment=IDS_COMM_TSC_Z;
//		h->Group=IDS_PROP_COLLISION_TOOLSTOCK;
//		h->V_Value = NTool::ZOffset;
//		h->Color = 1;
//		List->Add(h);
//}
//

	return List;
}

PropList * NMapStringToNTool::Change(PropList * List,PropSingle * Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;

	switch(id)
	{
	case 1: // Edges
		NTool::ShapeVisibleTool = (Single->ST_Value == IDS_TRUE);
		break;
	case 2: // render mode tool
		NTool::RMTool = RenderMode(Single->ST_Value);
		break;
	case 3: // Edges
		NTool::ShapeVisibleHolder = (Single->ST_Value == IDS_TRUE);
		break;
	case 4: // render mode holder
		NTool::RMHolder = RenderMode(Single->ST_Value);
		break;
	case 5: // Transparency
		NTool::Transparency = (1. - Single->V_Value / 100.);
		break;
//	case 6: // Collisin ON/OFF
//		NTool::SetToolStockCollision(CollisWithStock(Single->ST_Value));
//		NCMProject::GetProject().GetGlobalState().SetMultiThreadDX(NTool::GetToolStockCollision() == CWS_OFF);
//		break;
////	case 7: // Stop 
////		NTool::BreakOnColl = (Single->ST_Value == IDS_TRUE);
////		break;
//	case 8: // Offset XY
//		NTool::XYOffset = Single->V_Value;
//		break;
//	case 9: // Offset Z
//		NTool::ZOffset = Single->V_Value;
//		break;
//	case 10: // Ignore Up 
//		NTool::IgnoreUpToolStockColl = (Single->ST_Value == IDS_TRUE);
//		break;
	}
	return 	Create(List);
}

bool NMapStringToNTool::MakeActual(void)
{
	// Make sure the map keys are correspondent to the tools parameters
	bool IsChanged = false;
	for each (const auto & el in *this)
	{
		NToolCombined* pTc = static_cast<NToolCombined*>(el.second);
		NToolID ToolID = el.first;
		NToolID ActToolID;
		ActToolID.Set(pTc->Position, pTc->Turret);
		if(ToolID != ActToolID)
		{
			IsChanged = true;
			RemoveKey(ToolID);
			(*this)[ActToolID] = pTc;
		}
	}
	return IsChanged;
}

void NMapStringToNTool::ClearUsed()
{
	for(int i = 0; i < GetSize(); ++i)
	{
		NToolCombined *pTool = NULL;
		CString Key;
		GetAt(i, Key, (CObject *&)pTool);
		if(pTool)
			pTool->SetIsUsed(false);
	}
}

void NMapStringToNTool::DeleteUnUsed()
{
	for(int i = 0; i < GetSize(); ++i)
	{
		NToolCombined *pTool = NULL;
		CString Key;
		GetAt(i, Key, (CObject *&)pTool);
		if(!pTool)
		{
			RemoveKey(Key);
			--i;
			continue;
		}
		if(!pTool->GetIsUsed())
		{
			delete pTool;
			RemoveKey(Key);
			--i;
			continue;
		}
	}
}

void NMapStringToNTool::FullCopy(const NMapStringToNTool & Source)
{
	FullRemoveAll();
	for(int i = 0; i < Source.GetSize(); ++i)
	{
		NToolCombined *pSourceTool = NULL;
		CString Key;
		Source.GetAt(i, Key, (CObject *&)pSourceTool);
		NToolCombined *pDestTool = pSourceTool->CreateCopyTool();
		SetAt(Key, pDestTool); 
	}
}

void NMapStringToNTool::GetFullNames(std::vector<CString> &Names)
{
	Names.clear();
	// Find max start length ( part before first blank)
	int StartLength = 0;
	for (int i = 0; i < GetSize(); ++i)
	{
		NToolCombined *pTc = NULL;
		NToolID ToolID;
		GetAt(i, ToolID, (CObject *&)pTc);
		CString FullName = GetFullName(ToolID, pTc);
		int L = FullName.Find(' ');
		StartLength = max(StartLength, L);
	}
	for (int i = 0; i < GetSize(); ++i)
	{
		NToolCombined *pTc = NULL;
		NToolID ToolID;
		GetAt(i, ToolID, (CObject *&)pTc);
		CString FullName = GetFullName(ToolID, pTc);
		int L = FullName.Find(' ');
		for (int k = 0; k < StartLength - L; ++k)
			FullName = ' ' + FullName;
		Names.push_back(FullName);
	}
	std::sort(Names.begin(), Names.end());
}

const CString NMapStringToNTool::GetFullName(NToolID ToolID, const NToolCombined *pTc)
{
	const CString PrefixFormat = "%d" + CString(NToolID::TOOL_PRINT_DELIM);
	CString Prefix;
	if (ToolID.GetTurretNum() != 0)
		Prefix.Format(PrefixFormat, ToolID.GetTurretNum());

	return Prefix + ToolID.GetToolStr() + " " + pTc->GetName();
}

BOOL NMapStringToNTool::Lookup(const CString& key, NToolCombined*& pObj) const
{
	const auto it = find(key);
	if (it == end())
		return FALSE;
	pObj = static_cast<NToolCombined*>(it->second);
	return TRUE;
}

