#include "stdafx.h"
#include "NCMProject.h"
#include "NStockStl.h"
#include "NStockCont.h"
#include "CMapStringToNStock.h"
#include "PropList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CMapStringToNStock::CMapStringToNStock()
{
	Visible = true;
	RMode = RM_SHADE;
	EdgesAttrs.SetEdgesColor(NColor(0., 0., 0., 1.));
	EdgesAttrs.SetEdgesWidth(2.);
	EdgesAttrs.SetSmoothAngle(15.);
	Transparency = 0.5;
	DrawSharpEdgesS = true;
	DrawSmoothSurf = true;
	MooveWithStock = true;
}

CMapStringToNStock::~CMapStringToNStock()
{
}
void CMapStringToNStock::SetVisible(bool vis)
{
	Visible = vis;
}

bool CMapStringToNStock::IsVisible(void) const
{
	return Visible;
}

void CMapStringToNStock::SetEdgesColor(const NColor & Col)
{
	EdgesAttrs.SetEdgesColor(Col);
	SetEdgesAttrs(EdgesAttrs);
}

void CMapStringToNStock::SetEdgesWidth(double Width)
{
	EdgesAttrs.SetEdgesWidth(Width);
	SetEdgesAttrs(EdgesAttrs);
}

void CMapStringToNStock::SetSmoothAngle(double Angle)
{
	EdgesAttrs.SetSmoothAngle(Angle);
	SetEdgesAttrs(EdgesAttrs);
}

void CMapStringToNStock::SetTransparency(double Transp)
{
	Transparency = Transp;
}

void CMapStringToNStock::Draw(RenderMode GlobRMode, bool DrawSurf)
{
	if(!Visible)
		return;
	for each(const auto& el in *this)
	{
		NStock *pP = static_cast<NStock *>(el.second);
		CString key = el.first;

		if(!pP)
			continue;
		BOOL BufE = pP->GetEdgesS();
		if(!DrawSharpEdgesS)
			pP->SetEdgesS(false);

		bool BufS = pP->GetDrawSmoothSurf();
		if(!DrawSmoothSurf)
			pP->SetDrawSmoothSurf(false);

		if(GlobRMode == RM_TRANSLUCENT && RMode == RM_TRANSLUCENT)
			pP->SetTransparency(Transparency);

		int i = GetIndex(key);
		glPushName(GLuint(i));
		pP->DrawOriginal(GlobRMode, GetRMode(), DrawSurf);
		glPopName();

		if(IsSelectedKey(key))
		{
			if(GlobRMode != RM_TRANSLUCENT)
			{
				pP->DrawSelected();
			}
		}

		if(!DrawSharpEdgesS)
			pP->SetEdgesS(BufE);

		if(!DrawSmoothSurf)
			pP->SetDrawSmoothSurf(BufS);
	}
}

bool CMapStringToNStock::IsThereAnyVisObj(void) const
{
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if(pS->Visible)
			return true;
	}
	return false;
}

bool CMapStringToNStock::IsThere0StPos(void) const
{
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if (pS->GetStockGr().Get() == 0)
			return true;
	}
	return false;
}

BBox CMapStringToNStock::GetGabar(void) const
{
	BBox Ret;
	for each (const auto& el in *this)
	{
		NStock* pS = static_cast<NStock*>(el.second);
		if(pS)
			Ret.Expand(pS->GetGabar());
	}
	return Ret;
}

void CMapStringToNStock::SetEdgesAttrs(const BEdgesAttrs &Attr)
{
	EdgesAttrs = Attr;
	for each (const auto& el in *this)
	{
		NStock* pStock = static_cast<NStock*>(el.second);
		if (pStock != nullptr)
			pStock->SetEdgesAttrs(Attr);
	}
}

PropList * CMapStringToNStock::Create(PropList * List)
{
	List->Clear();
	// Позиционирование
	PropSingle * h = new PropSingle();
	h->ID=1;
	h->Name=IDS_MOVEWITHSTOCK;
	h->Type=1;
	h->Comment=IDS_COMM_MOVWITHSTOCK;
	h->Group=IDS_PROP_PART;
	h->ST_Value= (MooveWithStock ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	List->Add(h);
	// Visibility
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_MACH;
	h->Group=IDS_PROP_PART;
	h->ST_Value= (Visible ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Edges
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_EDGE;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_EDGE;
	h->Group=IDS_PROP_PART;
	h->ST_Value = (DrawSharpEdgesS ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Edges color
	h = new PropSingle();
	h->ID=4;
	h->Comment=IDS_COMM_MACH_COLOR;
	h->Name=IDS_EDGE_COLOR;
	h->Type=3;
	h->Group=IDS_PROP_PART;
	h->V_Value=GetEdgesColor().GetRGB();
	List->Add(h);
	// Edges thickness
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_EDGE_THIKNESS;
	h->Type=4;
	h->Comment=IDS_COMM_MACH_THICK;
	h->Group=IDS_PROP_PART;
	h->Min=1.;
	h->Max=10.;
	h->V_Value = GetEdgesWidth();
	List->Add(h);
	// Render mode
	h = new PropSingle();
	h->ID=6;
	h->Name=IDS_DISPLAY_MODE;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_RENDER;
	h->Group=IDS_PROP_PART;
	h->ST_Value = GetRMode();
	h->For_Enum.Add(IDS_RM_SHADE);
//	h->For_Enum.Add(IDS_RM_WFRAME);
	h->For_Enum.Add(IDS_RM_GLITTER);
	h->For_Enum.Add(IDS_RM_TRANSLUCENT);
	h->For_Enum.Add(IDS_RM_UNDEF);
	h->Color = 1;
	List->Add(h);
	// Transparency
	h = new PropSingle();
	h->ID=7;
	h->Name=IDS_TRANSPARENCY_DEGREE;
	h->Type=5;
	h->Group=IDS_PROP_PART;
	h->Max=100;
	h->Min=0;
	h->Comment=IDS_COMM_MACH_TRANSP;
	h->V_Value=int((1. - GetTransparency()) * 100. + 0.5);
	List->Add(h);
	// Smoothing
	h = new PropSingle();
	h->ID=8;
	h->Name=IDS_SMOOTHING;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_SMOOTH;
	h->Group=IDS_PROP_PART;
	h->ST_Value = (DrawSmoothSurf ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Smooth angle
	h = new PropSingle();
	h->ID=9;
	h->Name=IDS_SMOOTHING_ANGLE;
	h->Type=4;
	h->Comment=IDS_COMM_MACH_SMOOTHANGL;
	h->Group=IDS_PROP_PART;
	h->V_Value = GetSmoothAngle();
	h->Max=180;
	h->Min=0;
	List->Add(h);

return List;
}
PropList * CMapStringToNStock::Change(PropList * List,PropSingle * Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;

	switch(id)
	{
	case 1: // IDS_MOVEWITHSTOC
		MooveWithStock = (Single->ST_Value == IDS_TRUE);
		break;
	case 2: // IDS_VISIBILITY
		Visible = (Single->ST_Value == IDS_MIX);
		break;
	case 3: // Edges
		DrawSharpEdgesS = (Single->ST_Value == IDS_MIX);
		break;
	case 4: // Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetEdgesColor(Col);
			break;
		}
	case 5: // Edges thickness
		SetEdgesWidth(Single->V_Value);
		break;
	case 6: // Render mode
		SetRMode(RenderMode(Single->ST_Value));
		break;
	case 7: // Transparency
		SetTransparency(1. - Single->V_Value / 100.);
		break;
	case 8: // Smoothing
		DrawSmoothSurf = (Single->ST_Value == IDS_MIX);
		break;
	case 9: // Smooth angle
		SetSmoothAngle(Single->V_Value);
		break;
	}
	return 	Create(List);
}

void CMapStringToNStock::Serialize(CArchive &ar)
{
	NMapStringToOb::Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		SerializeElements(ar, &Visible, 1);
		SerializeElements(ar, &DrawSharpEdgesS, 1);

		//SerializeElements(ar, &EdgesColor, 1);
		int a=0;
		if (ar.IsLoading())
			ar>>a;
		else
			ar<<a;
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.7")))
		{
			EdgesAttrs.Serialize(ar);
			SerializeElements(ar, &RMode, 1);
			SerializeElements(ar, &Transparency, 1);
			SerializeElements(ar, &DrawSmoothSurf, 1);
		}
		else
		{// Read old format
			NColor Col;
			Col.Serialize(ar);
			EdgesAttrs.SetEdgesColor(Col);

			double b;
			SerializeElements(ar, &b, 1);
			EdgesAttrs.SetEdgesWidth(b);
			SerializeElements(ar, &RMode, 1);
			SerializeElements(ar, &Transparency, 1);
			SerializeElements(ar, &DrawSmoothSurf, 1);
			SerializeElements(ar, &b, 1);
			EdgesAttrs.SetSmoothAngle(b);
		}
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.3")))
		SerializeElements(ar, &MooveWithStock, 1);

	SetEdgesAttrs(EdgesAttrs);// to store EdgesAttrs to all stocks in the array
}

bool CMapStringToNStock::HasTranslucent()
{
	if(RMode == RM_TRANSLUCENT && IsThereAnyVisObj() && IsVisible())
		return true;

	if(RMode == RM_UNDEF)
	{
		for each (const auto& el in *this)
		{
			NStock* pS = static_cast<NStock*>(el.second);
			if(pS != nullptr)
				if(pS->GetRMode() == RM_TRANSLUCENT && pS->IsVisible())
					return true;
		}
		return false;
	}

	return false;
}

void CMapStringToNStock::MakeName(CStringA &Name) const
{
	while(find(Name) != end())
	{
		int iDot = Name.ReverseFind(':');
		if(iDot < 0)
			Name += ":1";
		else
		{
			int Num = atoi(Name.Right(Name.GetLength() - iDot - 1));
			Name.Delete(iDot + 1, Name.GetLength());
			Name += "%d";
			CString Buf;
			Buf.Format(Name, Num + 1);
			Name = Buf;
		}
	}
}

void CMapStringToNStock::AddObj(CString& TrunName, int& ind, CObject* pObj)
{
	MakeName(TrunName);
	if (ind == -1)
	{
		Order.push_back(TrunName);
		ind = int(Order.size()) - 1;
	}
	else
		Order.insert(Order.begin() + ind, TrunName);
	if (!TrunName.IsEmpty())
		std::map<CString, CObject*>::insert(std::make_pair(TrunName, pObj));// to prevent addition new element to Order
}

CString CMapStringToNStock::DeleteObjByInd(int ind)
{
	CString key;
	if (ind < GetSize() && ind >= 0)
	{
		CObject* stock = nullptr;
		GetAt(ind, key, stock);

		((NStock*)stock)->Deleted();
		RemoveKey(key);
	}
	return key;
}
