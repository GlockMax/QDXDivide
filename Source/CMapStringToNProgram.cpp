#include "stdafx.h"
#include "NCMProject.h"
#include "CMapStringToNProgram.h"
#include "NCUnit.h"
#include "PropList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CMapStringToNProgram::CMapStringToNProgram()
{
	Visible = true;
	PartialDraw = TrajPart::TP_WHOLE;
	ProgComp = TT_PROG;
	WormAfter = 1;
	WormBefore = 5;
	PCMode = PCM_DEFAULT;
	AllProgramsSize = 0;
	MainProgCount = 0;
	Active = true;
	DrawOnlyActiveToolPart = false;
	UseAxes = false;
	DrawNodes = false;
	UseToolColor4Prog = false;
}

CMapStringToNProgram::~CMapStringToNProgram()
{
}

BOOL CMapStringToNProgram::Lookup(const CString& key, NProgram*& pObj) const
{
	const auto it = find(key);
	if (it == end())
		return FALSE;
	pObj = static_cast<NProgram *>(it->second);
	return TRUE;
}

void CMapStringToNProgram::SetDrawOnlyActiveToolPart(bool Val)
{
	DrawOnlyActiveToolPart = Val; 
}

void CMapStringToNProgram::SetProgColorMode(PROG_COLOR_MODE Mode)
{
	PCMode = Mode;
	for each (const auto & el in *this)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);

		pProg->SetProgColorMode(PCMode);
		pProg->PrepRenders(NCM_PROJECT.GetDouble("TolLineApp", 0.01));
 	}
}

bool CMapStringToNProgram::HaveCompTraj(void) const
{
	return (ProgComp == TT_COMP) || (ProgComp == TT_BOTH);
}

bool CMapStringToNProgram::HaveProgTraj(void) const
{
	return (ProgComp == TT_PROG) || (ProgComp == TT_BOTH);
}
int CMapStringToNProgram::CalcAllProgramsSize(void)
{
	AllProgramsSize = 0;
	for each (const auto & el in *this)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		AllProgramsSize += pProg->GetSize();
 	}
	return AllProgramsSize;
}
int CMapStringToNProgram::GetAllProgramsSize(void) const
{
	return AllProgramsSize;
}
PropList * CMapStringToNProgram::Create(PropList * List)
{
	List->Clear();
	// Активность
	PropSingle * h = new PropSingle();
	h->ID=1;
	h->Name=IDS_ACTIVE;
	h->Type=1;
	h->Comment=IDS_COMM_PROG_ACTIVE;
	h->Group=IDS_PROP_PROG;
	h->ST_Value= (Active ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Visibility
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_PROG;
	h->Group=IDS_PROP_PROG;
	if(Visible && DrawOnlyActiveToolPart)
		h->ST_Value = IDS_COLTOOL;
	else if(!Visible)
		h->ST_Value = IDS_FALSE;
	else
		h->ST_Value = IDS_MIX;
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	h->For_Enum.Add(IDS_COLTOOL);
	List->Add(h);
	// ProgComp
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_PROG_COMP;
	h->Type=1;
	h->Comment=IDS_COMM_PROG_COMP;
	h->Group=IDS_PROP_PROG;
	UINT Value = IDS_TRAJ_PROG;
	switch(ProgComp)
	{
	case TT_PROG:
		Value = IDS_TRAJ_PROG;
		break;
	case TT_COMP:
		Value = IDS_TRAJ_COMP;
		break;
	case TT_BOTH:
		Value = IDS_TRAJ_BOTH;
		break;
	}
	h->ST_Value= Value;
	h->For_Enum.Add(IDS_TRAJ_PROG);
	h->For_Enum.Add(IDS_TRAJ_COMP);
	h->For_Enum.Add(IDS_TRAJ_BOTH);
	List->Add(h);
	// Show Tool Vector
	h = new PropSingle();
	h->ID = 10;
	h->Name = IDS_PROP_TOOLVECTOR;
	h->Type = 1;
	h->Comment = IDS_PROP_TOOLVECTOR_COMM;
	h->Group = IDS_PROP_PROG;
	h->ST_Value = (UseAxes ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	List->Add(h);
	// Show Points
	h = new PropSingle();
	h->ID = 11;
	h->Name = IDS_PROP_TRPOINTS;
	h->Type = 1;
	h->Comment = IDS_PROP_TRPOINTS_COMM;
	h->Group = IDS_PROP_PROG;
	h->ST_Value = (DrawNodes ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	h->For_Enum.Add(IDS_FALSE);
	List->Add(h);
	// PartialDraw
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_PROG_PARTDRAW;
	h->Type=1;
	h->Comment=IDS_COMM_PROG_PARTDRAW;
	h->Group=IDS_PROP_PROG;
	h->ST_Value = IDS_TP_WHOLE;
	switch(PartialDraw)
	{
	case TrajPart::TP_WHOLE:
		h->ST_Value = IDS_TP_WHOLE;
		break;
	case TrajPart::TP_PART:
		h->ST_Value = IDS_TP_PART;
		break;
	case TrajPart::TP_WORM:
		h->ST_Value = IDS_TP_WORM;
		break;
	}
	h->For_Enum.Add(IDS_TP_WHOLE);
	h->For_Enum.Add(IDS_TP_PART);
	h->For_Enum.Add(IDS_TP_WORM);
	List->Add(h);
	// Program color mode
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_PROG_COLORMODE;
	h->Type=1;
	h->Comment=IDS_COMM_PROG_COLORMODE;
	h->Group=IDS_PROP_PROG;
	h->ST_Value = IDS_PCM_DEFAULT;
	if(UseToolColor4Prog)
		h->ST_Value = IDS_PCM_TOOLS;
	else
	{
		switch (PCMode)
		{
		case PCM_DEFAULT:
			h->ST_Value = IDS_PCM_DEFAULT;
			break;
		case PCM_INTERP:
			h->ST_Value = IDS_PCM_INTERP;
			break;
		case PCM_DCOMP:
			h->ST_Value = IDS_PCM_DCOMP;
			break;
		case PCM_LCOMP:
			h->ST_Value = IDS_PCM_LCOMP;
			break;
		case PCM_CYCLE:
			h->ST_Value = IDS_PCM_CYCLE;
			break;
		case PCM_COOL:
			h->ST_Value = IDS_PCM_COOL;
			break;
		}
	}
	h->For_Enum.Add(IDS_PCM_DEFAULT);
	h->For_Enum.Add(IDS_PCM_TOOLS);
	h->For_Enum.Add(IDS_PCM_INTERP);
	h->For_Enum.Add(IDS_PCM_DCOMP);
	h->For_Enum.Add(IDS_PCM_LCOMP);
	h->For_Enum.Add(IDS_PCM_CYCLE);
	h->For_Enum.Add(IDS_PCM_COOL);
	List->Add(h);
	// Worm before
	h = new PropSingle();
	h->ID=6;
	h->Name=IDS_PROG_BEFORE;
	h->Type=5;
	h->Comment=IDS_COMM_PROG_BEFORE;
	h->Group=IDS_PROP_PROG;
	h->Min=0.;
	h->Max=1000000.;
	h->V_Value=WormBefore;
	List->Add(h);
	// Worm after
	h = new PropSingle();
	h->ID=7;
	h->Name=IDS_PROG_AFTER;
	h->Type=5;
	h->Comment=IDS_COMM_PROG_AFTER;
	h->Group=IDS_PROP_PROG;
	h->Min=0.;
	h->Max=1000000.;
	h->V_Value=WormAfter;
	List->Add(h);
	// Programs length
	double Length = 0.;
	for each (const auto & el in *this)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		Length += pProg->GetInfo().WorkLength + pProg->GetInfo().FastLength;
 	}

	h = new PropSingle();
	h->ID=8;
	h->Name=IDS_PROG_LENGTH;
	h->Type=4;
	h->Comment=IDS_COMM_PROG_ALLLENGTH;
	h->Group=IDS_PROP_PROG;
	h->V_Value=Length;
	h->Color = 0;
	List->Add(h);

	// Info
	int Size = 0;
	double Time = 0.;
	for each (const auto & el in *this)
	{
		NProgram* pProg = static_cast<NProgram*>(el.second);
		Size += pProg->GetInfo().LinesNumber;
		Time += pProg->GetInfo().Time;
 	}

	// Programs size
	h = new PropSingle();
	h->ID=9;
	h->Name=IDS_PROG_SIZE;
	h->Type=5;
	h->Comment=IDS_COMM_PROG_ALLSIZE;
	h->Group=IDS_PROP_PROG;
	h->V_Value=Size;
	h->Color = 0;
	List->Add(h);
	// Programs time
	h = new PropSingle();
	h->ID=10;
	h->Name=IDS_PROG_TIME;
	h->Type=6;
	h->Comment=IDS_COMM_PROG_ALLTIME;
	h->Group=IDS_PROP_PROG;
	h->Time=Time;
	h->Color = 0;
	List->Add(h);


	return List;
}

PropList * CMapStringToNProgram::Change(PropList * List,PropSingle * Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;
	switch(id)
	{
	case 1: // IDS_ACTIVE
		{
			bool NewActive = (Single->ST_Value == IDS_MIX);
			if(Active != NewActive)
			{
				Group = OG_PROGRAMS;
				Active = NewActive;
			}
		}
		break;
	case 2: // IDS_VISIBILITY
		switch (Single->ST_Value)
		{
		case IDS_FALSE:
			Visible = false;
			SetDrawOnlyActiveToolPart(false);
			break;
		case IDS_MIX:
			Visible = true;
			SetDrawOnlyActiveToolPart(false);
			break;
		case IDS_COLTOOL:
			SetDrawOnlyActiveToolPart(true);
			Visible = true;
			break;
		}
		break;
	case 3: // ProgComp
		Group = OG_OCTREE;
		ProgComp = TT_PROG;
		switch(Single->ST_Value)
		{
		case IDS_TRAJ_PROG:
			ProgComp = TT_PROG;
			break;
		case IDS_TRAJ_COMP:
			ProgComp = TT_COMP;
			break;
		case IDS_TRAJ_BOTH:
			ProgComp = TT_BOTH;
			break;
		}
		break;
	case 4: // PartialDraw
		PartialDraw = TrajPart::TP_WHOLE;
		switch(Single->ST_Value)
		{
		case IDS_TP_WHOLE:
			PartialDraw = TrajPart::TP_WHOLE;
			break;
		case IDS_TP_PART:
			PartialDraw = TrajPart::TP_PART;
			break;
		case IDS_TP_WORM:
			PartialDraw = TrajPart::TP_WORM;
			break;
		}
		break;
	case 5: // Edges thickness
		PCMode = PCM_DEFAULT;
		UseToolColor4Prog = false;
		switch(Single->ST_Value)
		{
		case IDS_PCM_DEFAULT:
			PCMode = PCM_DEFAULT;
			break;
		case IDS_PCM_TOOLS:
			PCMode = PCM_DEFAULT;
			UseToolColor4Prog = true;
			break;
		case IDS_PCM_INTERP:
			PCMode = PCM_INTERP;
			break;
		case IDS_PCM_DCOMP:
			PCMode = PCM_DCOMP;
			break;
		case IDS_PCM_LCOMP:
			PCMode = PCM_LCOMP;
			break;
		case IDS_PCM_CYCLE:
			PCMode = PCM_CYCLE;
			break;
		case IDS_PCM_COOL:
			PCMode = PCM_COOL;
			break;
		}
		SetProgColorMode(PCMode);
		break;
	case 6: // Worm after
		WormBefore = int(Single->V_Value);
		break;
	case 7: // Worm before
		WormAfter = int(Single->V_Value);
		break;
	case 10: // tool vector
		UseAxes = (Single->ST_Value == IDS_TRUE);
		break;
	case 11: // tool vector
		DrawNodes = (Single->ST_Value == IDS_TRUE);
		break;
	}
	return 	Create(List);
}

void CMapStringToNProgram::Serialize(CArchive& ar)
{
	NMapStringToOb::Serialize(ar);

	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		SerializeElements(ar, &Visible, 1);
		SerializeElements(ar, &PartialDraw, 1);
		SerializeElements(ar, &ProgComp, 1);
		SerializeElements(ar, &WormAfter, 1);
		SerializeElements(ar, &WormBefore, 1);
		SerializeElements(ar, &PCMode, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.91")))
	{
		SerializeElements(ar, &MainProgCount, 1);
	}
	else
	{
		MainProgCount = int(GetSize());
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.3")))
		SerializeElements(ar, &Active, 1);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.6")))
	{
		SerializeElements(ar, &DrawOnlyActiveToolPart, 1);
		SerializeElements(ar, &UseAxes, 1);
		SerializeElements(ar, &DrawNodes, 1);
		SerializeElements(ar, &UseToolColor4Prog, 1);
	}
}

NCadr *CMapStringToNProgram::GetCadr(cadrID ID) const
{
	if(ID.Prog < 0 )
		return NULL;
	CString Key;
	BProgram *pProg = NULL;
	GetAt(ID.Prog, Key, (CObject *&)pProg);
	if(!pProg)
		return NULL;
	if(int(ID.Cadr) >= pProg->GetSize())
		return NULL;
	return (*pProg)[ID.Cadr];
}

NCadrGeom* CMapStringToNProgram::GetCadrGeom(cadrID HistID, int StockPos)
{
	const auto pProg = GetProg(HistID);
	if (pProg == nullptr)
		return nullptr;
	const auto pGeomArray = &pProg->GetGeomArrayByStPos(StockPos);
	if (pGeomArray == nullptr)
		return nullptr;
	const auto AbsID = pProg->GetCadrByHist(HistID.Cadr);
	return pGeomArray->GetAt(AbsID.Cadr);
}

NCadrGeom* CMapStringToNProgram::GetCadrCompGeom(cadrID HistID, int StockPos)
{
	const auto pProg = GetProg(HistID);
	if (pProg == nullptr)
		return nullptr;
	const auto pGeomArray = &pProg->GetCompGeomArrayByStPos(StockPos);
	if (pGeomArray == nullptr)
		return nullptr;
	const auto AbsID = pProg->GetCadrByHist(HistID.Cadr);
	return pGeomArray->GetAt(AbsID.Cadr);
}

CString CMapStringToNProgram::GetProgName(cadrID ID) const
{
	CString Key(_T(""));
	BProgram *pProg;
	GetAt(ID.Prog, Key, (CObject *&)pProg);
	return Key;
}

BProgram *CMapStringToNProgram::GetProg(cadrID ID) const
{
	if(ID.Prog < 0 )
		return NULL;
	CString Key(_T(""));
	BProgram *pProg = NULL;
	GetAt(ID.Prog, Key, (CObject *&)pProg);
	return pProg;
}

BProgram *CMapStringToNProgram::GetProg(const CString &ProgName) const
{
	auto it = find(ProgName);
	if (it == end())
		return nullptr;
	return static_cast<BProgram*>(it->second);
}

void CMapStringToNProgram::AddMainProg(LPCTSTR key, CObject* newValue)
{
	auto it = find(key);
	if (it == end())
	{

		Order.insert(Order.begin() + MainProgCount, key);
		++MainProgCount;
	}
	insert(make_pair(key, newValue));
}

void CMapStringToNProgram::RemoveAt(INT_PTR nIndex)
{
	const CString &key = Order.at(nIndex);
	if(!key.IsEmpty())
	{
		erase(key);
		Order.erase(Order.begin() + nIndex);
	}
	if(nIndex < MainProgCount)
		--MainProgCount;
}
void CMapStringToNProgram::RemoveAll()
{
	NMapStringToOb::RemoveAll();
	MainProgCount = 0;
}

BOOL CMapStringToNProgram::RemoveKey(LPCTSTR key)
{
	int Index = GetIndex(key);
	if(Index < 0)
		return FALSE;
	if(Index < MainProgCount)
		--MainProgCount;
	return BMapStringToNProgram::RemoveKey(key); 
}

cadrID CMapStringToNProgram::GetNearest(cadrID SourceHID, cadrID DestID) const
{
	// Find first occurrence of DestID in the programs history, starting at SourceHID
	// cyclically
	// result is cadrID calculated according to rules for CadrIDEx (CurHCadr but not CurHCadrAbs)
	cadrID DestHID;
	DestHID.Prog = -1;
	DestHID.Cadr = 0;
	if(SourceHID.Prog < 0 || DestID.Prog < 0)
		return DestHID;
	auto i = SourceHID.Prog;
	auto j = SourceHID.Cadr;
	// Find from start position to the end
	for(; i < GetMainProgCount(); ++i, j = 0)
	{
		CString ProgName;
		BProgram *pProg = NULL;
		if(!GetAt(i, ProgName, (CObject *&)pProg))
			return DestHID;
		if(!pProg)
			continue;
		for(; j < unsigned int(pProg->GetHCadr2HistorySize()); ++j)
		{
			auto jHist = pProg->GetHistByHCadr(j);
			if(pProg->GetHistory()[jHist] == DestID)
			{
				DestHID.Prog = i;
				DestHID.Cadr = j;
				return DestHID;
			}
		}
	}
	// Find from the beginning to the start position
	for(i = 0; i <= SourceHID.Prog; ++i)
	{
		CString ProgName;
		BProgram *pProg = NULL;
		if(!GetAt(i, ProgName, (CObject *&)pProg))
			return DestHID;
		if(!pProg)
			continue;
		unsigned int MaxJ = int((i == SourceHID.Prog) ? SourceHID.Cadr : pProg->GetHCadr2HistorySize());
		for(unsigned int k = 0; k < MaxJ; ++k)
		{
			auto jHist = pProg->GetHistByHCadr(k);
			if(pProg->GetHistory()[jHist] == DestID)
			{
				DestHID.Prog = i;
				DestHID.Cadr = k;
				return DestHID;
			}
		}
	}

	return DestHID;
}

cadrID CMapStringToNProgram::GetNearestTCFor(cadrID SourceHID, bool *pProjEnd) const
{
	*pProjEnd = false;
	// Find first tool change in the programs history, starting at SourceHID
	// result is cadrID calculated according to rules for CadrIDEx (CurHCadr but not CurHCadrAbs)
	cadrID DestHID;
	DestHID.Prog = -1;
	DestHID.Cadr = 0;
	if(SourceHID.Prog < 0)
	{
		SourceHID.Prog = 0;
		SourceHID.Cadr = 0;
	}
	
	// Find from start position to the end
 	BProgram *pProg = NULL;
	for(int i = SourceHID.Prog, j = SourceHID.Cadr; i < GetMainProgCount(); ++i, j = 0)
	{
		CString ProgName;
		if(!GetAt(i, ProgName, (CObject *&)pProg))
			return DestHID;
		if(!pProg)
			continue;
		for(int k = 0; k < pProg->GetTCHistSize(); ++k)
		{
			int jTC;
			NToolID ToolID;
			pProg->GetTCHist(k, jTC, ToolID);
			if(jTC < j)
				continue;
			DestHID.Prog = i;
			DestHID.Cadr = jTC;
			return DestHID;
		}
	}
	if (DestHID.Prog < 0 && pProg != NULL)
	{
		DestHID = cadrID(GetMainProgCount() - 1, pProg->GetLastHCadr());
		*pProjEnd = true;
	}
	return DestHID;
}

cadrID CMapStringToNProgram::GetNearestTCBack(cadrID SourceHID) const
{
	// Find first tool change in the programs history, starting at SourceHID
	// result is cadrID calculated according to rules for CadrIDEx (CurHCadr but not CurHCadrAbs)
	cadrID DestHID;
	DestHID.Prog = 0;
	DestHID.Cadr = 0;
	if(SourceHID.Prog < 0)
		return DestHID;
	int i = SourceHID.Prog;
	int j = SourceHID.Cadr;
	// Find from start position to the beginning
	bool LastTCFound = false;
	for(; i > -1; --i)
	{
		CString ProgName;
		BProgram *pProg = NULL;
		if(!GetAt(i, ProgName, (CObject *&)pProg))
			return DestHID;
		if(!pProg)
			continue;
		for(int k = pProg->GetTCHistSize() - 1; k > -1; --k)
		{
			int jTC;
			NToolID ToolID;
			pProg->GetTCHist(k, jTC, ToolID);
			if (LastTCFound) // This may happen on the first loop iteration only
			{
				DestHID.Prog = i;
				DestHID.Cadr = jTC;
				return DestHID;
			}
			if(jTC >= j)
				continue;
			if (k > 0)
			{
				pProg->GetTCHist(k - 1, jTC, ToolID);
				DestHID.Prog = i;
				DestHID.Cadr = jTC;
				return DestHID;
			}
			LastTCFound = true;// k == 0 so needed TC is in the previous program
		}
		j = int(pProg->GetHistorySize() - 1);
	}
	return DestHID;
}

bool CMapStringToNProgram::IsCadrProtected(cadrID CadrID) const
{
	BProgram *pProg = GetProg(CadrID);
	if(pProg)
		return pProg->IsProtected();
	return false;
}
