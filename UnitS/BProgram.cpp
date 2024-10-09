#include "BProgram.h"
// BProgram.cpp: implementation of the BProgram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <algorithm>
#include "NCMProject.h"
#include "ConstDef.h"
#include "NCUnitState.h"
#include "BCurve.h"

#include "BGeomArray.h"
#include "NCadrGeom.h"
#include "NCycle.h"
#include "NCadr.h"
#include "SMachState.h"
#include "math.h"
#include "BProgram.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(BProgram, CObject, 0)

const int HIST_EXT = 4094;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BProgram::BProgram()
{
	InitEmpty();
	SetAuxColors();
	ResetAllGeom();
	StPos2Ind.insert(std::pair(0, 0));
}

BProgram::~BProgram()
{
	for (int n = 0; n < CadrArray.GetSize(); n++)
	{
		delete CadrArray[n];
	}
	CadrArray.RemoveAll();
	DeleteAllGeom();
}

double BProgram::GetTrajLength(bool Comp/* = false*/) const
{
	const BGeomArray &GArray = Comp ? GetGeomArray() : GetCompGeomArray();
	double Length = 0;
	for(int i = 0; i < GArray.GetSize(); ++i)
		Length += GArray[i]->Length();
	return Length;
}

CString BProgram::BLoad(const CString& filename)
{
	Gabar.Init();
	CStdioFile f;
	if (!f.Open(filename,CFile::typeText | CFile::modeRead ))
		return _T("");

	Info.FileName = filename;

	FILETIME Creation, Access, Write;
	if(::GetFileTime(f, &Creation, &Access, &Write))
		FileTime = CTime(Write);
	char buf[256];
	while (f.ReadString(buf, 256))
	{
		for (int i = 0; buf[i] == '\x00'; ++i)
			buf[i] = ' ';
		CString str(buf);
		str.Remove('\n');
		str.Remove('\x0D');
		if (str.IsEmpty())
			continue;
		CadrArray.Add(new NCadr(str));
	}
	f.Close();
	for (auto i = BookMarkArray.GetSize() - 1; i >= 0; --i)
	{
		if (BookMarkArray[i] < 0 || BookMarkArray[i] >= CadrArray.GetSize())
			BookMarkArray.RemoveAt(i);
	}

	return filename;
}

void BProgram::Serialize(CArchive &ar)
{
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.2")))
	{ // Version > 4.1
		// New variant is noticeably faster for large (> 100000 lines) programs
		if (ar.IsStoring())
		{
			ar.WriteCount(CadrArray.GetSize());
			for (DWORD_PTR i = 0; i < (DWORD_PTR)CadrArray.GetSize(); i++)
				ar << CadrArray[i]->GetOrText();
		}
		else
		{
			DWORD_PTR nOldSize = ar.ReadCount();
			CadrArray.SetSize(nOldSize);
			for (DWORD_PTR i = 0; i < nOldSize; i++)
			{
				CString str;
				ar >> str;
				CadrArray.SetAt(i, new NCadr(str));
			}
		}
	}
	else
	{// Version <= 4.1
		CadrArray.Serialize(ar);
	}
	Colors[PC_MAIN].Serialize(ar);
	SerializeElements(ar,&Visible,1);
	if(ar.IsLoading())
	{
		History.SetSize(0,HIST_EXT);
		ResetAllGeom();
		TCHistory.SetSize(0);
		ErrorHistory.RemoveAll();
		TAGHistory.SetSize(0);
		ProbeHistory.SetSize(0);
		MSHistory.SetSize(0);
		HCadr2History.SetSize(0);
	}
	SerializeElements(ar,&Comment,1);
	BookMarkArray.Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.8")))
	{
		SerializeElements(ar, &Thickness, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.91")))
	{
		SerializeElements(ar, &Aux, 1);
		SerializeElements(ar, &Protected, 1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.3")))
		SerializeElements(ar, &Active, 1);
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.8")))
		SerializeElements(ar, &AuxFromUnit, 1);
}

void BProgram::SerializeLines(CArchive& ar)
{
	if (!ar.IsStoring())
		return;
	int Size = 0;
	BYTE* pBuf = nullptr;
	if (TrajRenders.size() > 0)
	{
		RLines& Lin = TrajRenders[0];
		Size = Lin.StoreSample(pBuf);
	}
	SerializeElements(ar, &Size, 1);
	if (Size > 0)
	{
		SerializeElements(ar, pBuf, Size);
		delete[] pBuf;
	}
}

int BProgram::GetSize() const
{
	return int(CadrArray.GetSize());
}

NCadr * BProgram::operator [](int i) const
{
	return CadrArray[i];
}

void BProgram::InitEmpty()
{
	Comment = "";
	Gabar.Init();
	ManChanged = FALSE;
	NColor *c;
	if(NCM_PROJECT.Colors.Lookup(_T("Программа"), c))
		Colors[PC_MAIN] = *c;
	else
		Colors[PC_MAIN].SetRGB(RGB(255,255,255));

	PCM = PCM_DEFAULT;

	NTiParams &Par = NCM_PROJECT.Defaults;
	Thickness = Par.GetDouble(_T("Defaults/Program/ToolPath@Thick"), 1.5);
	Visible = Par.GetBool(_T("Defaults/Program/ToolPath@Visible"), true);

	IsInit = FALSE;
	ClearEvaluated();// This function may be called after glGenLists only
	Aux = false;
	AuxFromUnit = false;
	Protected = false;
	Active = true;
	NeedToolAxisDir = true;
}


void BProgram::FillGabar(const BMatr *MatrArr)
{
	Gabar.Init();
	for (int l = 0; l < GetGeomArraysCount(); ++l)
	{
		for (int i = 0; i < GetGeomArraySize(); i++)
		{
			const NCadrGeom* pGeom = GeomArrays.at(l)->GetAt(i);
			if (pGeom->GetMatrNum() == 0)
				pGeom->ExpandBox(&Gabar);
			else
			{
				NCadrGeom BGeom = *pGeom;
				BGeom.Tr(MatrArr[GetGeomArray(l)[i]->GetMatrNum()]).ExpandBox(&Gabar);
			}
		}
		for (int i = 0; i < GetGeomArraySize(); i++)
		{
			const NCadrGeom* pGeom = CompGeomArrays.at(l)->GetAt(i);
			if (pGeom->GetMatrNum() == 0)
				pGeom->ExpandBox(&Gabar);
			else
			{
				NCadrGeom BGeom = *pGeom;
				BGeom.Tr(MatrArr[GetCompGeomArray(l)[i]->GetMatrNum()]).ExpandBox(&Gabar);
			}
		}
	}
}

void BProgram::AddCadr(NCadr *pCadr)
{
	CadrArray.Add(pCadr);
}

void BProgram::DrawInt(int StockPos, bool CompOn, bool ProgOn, bool Draw4Pick, const std::vector<BProgVisInterval> & VisInt
	, int BoldCadr /*= -1*/, double t, bool UseAxes /*= false*/, bool DrawNodes /*= false*/)
{
	if (TrajRenders.empty())
		return;
	if(VisInt.empty())
		return;
	glLineWidth(GLfloat(Thickness));
	const int Ti = GetIndexByStPos(StockPos);
	if (Ti < 0)
		return;
	if (UseAxes != TrajRenders[Ti].GetUseAxes())
	{
		TrajRenders[Ti].SetUseAxes(UseAxes);
		TrajCompRenders[Ti].SetUseAxes(UseAxes);

		if (UseAxes && GetGeomArraySize() > 0) // if old axes are still alive we use it without recomputing
			PrepRenders1(Ti, NCM_PROJECT.GetDouble("TolLineApp", 0.01));
	}

	TrajRenders[Ti].SetDrawNodes(DrawNodes);
	TrajCompRenders[Ti].SetDrawNodes(DrawNodes);
	if (DrawNodes)
	{
		double NodesSize = NCM_PROJECT.GetDouble("ProgNodesSize", 1.);
		glPointSize(GLfloat(NodesSize));
	}

	if (CompOn)
		TrajCompRenders[Ti].DrawInt(Draw4Pick, VisInt, BoldCadr, t);
	if (ProgOn)
		TrajRenders[Ti].DrawInt(Draw4Pick, VisInt, BoldCadr, t);
}

bool BProgram::IsCycle()
{
	return false;
}

void BProgram::SetEvaluated()
{
	AjustCadrTimes();
	Info.LinesNumber = int(CadrArray.GetSize());
	IsEvaluated = TRUE;
}
void BProgram::SetGeomReady()
{
	if(IsGeomReady)
		return;
	IsGeomReady = TRUE;
	for (int Gi = 0; Gi < GeomArrays.size(); ++Gi)
	{
		GeomArrays[Gi]->Close(false);
		//	CompGeomArray.MakeContinuous();// Нужно для обеспечения непрерывности скорректированной траектории при смене номера корректора без смены инструмента
		CompGeomArrays[Gi]->Close(true);
	}
	MakeHCadr2History();
}

void BProgram::AjustCadrTimes()
{
	if (GeomArrays.size() == 1)
		return;
	if (GeomArrays.size() != CompGeomArrays.size())
		return;
	auto GSize = GeomArrays[0]->GetSize();
	for (int i = 1; i < GeomArrays.size(); ++i)
	{
		if (GeomArrays[i]->GetSize() != GSize)
			return;
		if (CompGeomArrays[i]->GetSize() != GSize)
			return;
	}
	for (int k = 0; k < GSize; ++k)
	{
		double MaxTime = 0.;
		for (int i = 0; i < GeomArrays.size(); ++i)
		{
			double Time = GeomArrays[i]->GetAt(k)->GetWorkTimeS();
			MaxTime = fmax(MaxTime, Time);
			Time = CompGeomArrays[i]->GetAt(k)->GetWorkTimeS();
			MaxTime = fmax(MaxTime, Time);
		}
		for (int i = 0; i < GeomArrays.size(); ++i)
		{
			GeomArrays[i]->GetAt(k)->SetWorkTimeS(MaxTime);
			CompGeomArrays[i]->GetAt(k)->SetWorkTimeS(MaxTime);
		}
	}
}

void BProgram::ClearEvaluated(const std::set<int>& PositionsSet)
{
	ClearEvaluated();
	DeleteAllGeom();
	GeomArrays.resize(PositionsSet.size());
	CompGeomArrays.resize(PositionsSet.size());
	StPos2Ind.clear();
	int Ind = 0;
	for each (auto StPos in PositionsSet)
	{
		GeomArrays[Ind] = new BGeomArray();
		GeomArrays[Ind]->SetSize(0, HIST_EXT);
		CompGeomArrays[Ind] = new BGeomArray();
		CompGeomArrays[Ind]->SetSize(0, HIST_EXT);
		StPos2Ind.insert(std::pair(StPos, Ind++));
	}
}
void BProgram::ClearEvaluated()
{
	Info.Clear();
	IsEvaluated = FALSE;
	IsGeomReady = FALSE;
	SetHaveErrors(FALSE);
	HaveSub = FALSE;
	HaveSpl = FALSE;
	HaveACoord = FALSE;
	History.RemoveAll();
	TCHistory.RemoveAll();
	ErrorHistory.RemoveAll();
	TAGHistory.RemoveAll();
	MSHistory.RemoveAll();
	HCadr2History.RemoveAll();
	History.SetSize(0, HIST_EXT);
	ProgList.Clear();
	ProgList.isCreated = false;
	ProgListByLineNumber.Clear();
	GotoList.Clear();
	GotoList.isCreated = false;
	ToolAxisDir.clear();
	ProbeHistory.RemoveAll();
	for (int i = 0; i < GetSize(); ++i)
		(*this)[i]->SetErrCode(0);
}

void BProgram::ClearEvaluatedPart(int StockPos)
{
	Info.Clear();
	IsEvaluated = FALSE;
	IsGeomReady = FALSE;
	GetGeomArrayByStPos(StockPos).Open();
	GetCompGeomArrayByStPos(StockPos).Open();
}

void BProgram::DeleteAllGeom()
{
	for (int Gi = 0; Gi < GeomArrays.size(); ++Gi)
	{
		delete GeomArrays[Gi];
		delete CompGeomArrays[Gi];
	}
}

void BProgram::ResetAllGeom()
{
	DeleteAllGeom();
	GeomArrays.resize(1);
	GeomArrays[0] = new BGeomArray();
	GeomArrays[0]->SetSize(0, HIST_EXT);
	CompGeomArrays.resize(1);
	CompGeomArrays[0] = new BGeomArray();
	CompGeomArrays[0]->SetSize(0, HIST_EXT);
}

int BProgram::GetHistByHCadr(int HCadrNum) const
{
//// CurHCadr не учитывает наличие циклов (при этом AddCompGeom и AddState учитывается)
//// метод возвращает реальный индекс в History по известному CurHCadr,

	if(HCadrNum < 0)
		return 0;
	if(HCadrNum >= HCadr2History.GetSize())
		return GetGeomArraySize() - 1;
	return HCadr2History[HCadrNum];
}

cadrID BProgram::GetCadrByHist(int HistNum)  const
{
	const auto Size = History.GetSize();
	if(HistNum < 0 || Size <= 0)
	{
		cadrID ID;
		ID.Prog = -1;
		ID.Cadr = 0;
		return ID;
	}
	if(HistNum >= Size)
	{
		cadrID ID;
		ID = History.GetAt(Size - 1);
		++ID.Cadr;
		return ID;
	}
	return History.GetAt(HistNum);
}

BOOL BProgram::HaveSubprog() const
{
	return HaveSub;
}

BOOL BProgram::HaveSpline() const
{
	return HaveSpl;
}

BOOL BProgram::HaveAddCoord() const
{
	return HaveACoord;
}

void BProgram::SetHaveAddCoord()
{
	HaveACoord = TRUE;
}

void BProgram::SetHaveSub()
{
	HaveSub = TRUE;
}

void BProgram::SetHaveSpline()
{
	HaveSpl = TRUE;
}

BOOL BProgram::IsManChanged()
{
	return ManChanged;
}

const CString &BProgram::GetCadrText(INT_PTR i) const
{
	return CadrArray[i]->GetOrText();
}

const CString &BProgram::GetCadrTextSafe(INT_PTR i) const
{
	if(i < 0)
		i = 0;
	if(i > CadrArray.GetSize() - 1)
		i = CadrArray.GetSize() - 1;
	return CadrArray[i]->GetOrText();
}

const CString BProgram::GetCadrReTextSafe(INT_PTR i) const
{
	if (i < 0)
		i = 0;
	if (i > CadrArray.GetSize() - 1)
		i = CadrArray.GetSize() - 1;
	if (i < 0)
		return CString();
	return CadrArray[i]->GetReText();
}

BOOL BProgram::IsEval() const
{
	return IsEvaluated;
}

const BBox & BProgram::GetGabar() const
{
	return Gabar;
}

BOOL BProgram::IsVisible() const
{
	return Visible;
}

void BProgram::InvertVisible()
{
	Visible = !Visible;
}

void BProgram::SetVisible(bool Val)
{
	Visible = Val;
}

const NColor & BProgram::GetColor() const
{
	return Colors[PC_MAIN];
}

void BProgram::AddToolChangePos(int hpos, const CString &ntool, int nturret)
{
	TCHistory.Add(ToolChangePos(hpos, ntool, nturret));
}

void BProgram::AddErrorPos(int hpos, const NProgError& ProgError)
{
	ErrorHistory.SetAt(hpos, ProgError);
}

void BProgram::AddTAGPos(const NAutoToolGenParams& TAGPos)
{
	TAGHistory.Add(TAGPos);
}

void BProgram::AddProbePos(int hpos, bool Collision)
{
	ProbeHistory.Add(ProbePos(hpos, false, -1., Collision ? ProbePos::PR_Collision : ProbePos::PR_Release));
}

BProgram::ToolChangePos::ToolChangePos()
{
	hpos=-1;
	ntool = CString("");
}

BProgram::ToolChangePos::ToolChangePos(int pos, const CString &tool, int turret)
{
	hpos = pos;
	ntool.Set(atoi(tool), turret);
}

BProgram::ErrorPos::ErrorPos(int pos, int ErrCode)
{
	hpos = pos;
	Code = ErrCode;
}

BProgram::ErrorPos::ErrorPos()
{
}

BProgram::ProbePos::ProbePos()
{
	hpos = -1;
	Processed = false;
	ActPart = -1.;
	ProbeResult = ProbePos::PR_Empty;
}

BProgram::ProbePos::ProbePos(int pos, bool iProcessed, double iActPart, NProbeResult iProbeResult)
{
	hpos = pos;
	Processed = iProcessed;
	ActPart = iActPart;
	ProbeResult = iProbeResult;
}

int BProgram::GetTCHistSize() const
{
	return int(TCHistory.GetSize());
}
int BProgram::GetMSHistSize() const
{
	return int(MSHistory.GetSize());
}

BOOL BProgram::GetTCHist(int ind, int &pos, NToolID &tool) const
{
// Get tool change position
	if(ind < 0 || ind >= TCHistory.GetSize())
		return FALSE;
	ToolChangePos b = TCHistory.GetAt(ind);
	pos = b.hpos;
	tool = b.ntool;
	return TRUE;
}

void BProgram::SetComment(const CString &text)
{
	Comment = text;
}

const CString & BProgram::GetComment()
{
	return Comment;
}

void BProgram::FillInfo(NCUnitState *pState)
{
	CString ProgName;
	if(!pState)
		ProgName = Info.FileName;
	Info.Clear();
	Info.Comment = Comment;
	Info.LinesNumber = int(CadrArray.GetSize());
	Info.StepsNumber = int(GetGeomArraySize());
	for(int i = 0; i < Info.StepsNumber; ++i)
	{
		NCadrGeom *pGeom = (*CompGeomArrays[0])[i];
		double CadrTime = pGeom->GetWorkTimeS();
		double CadrLength = pGeom->Length();
		if(pGeom->HaveGeom() || pGeom->IsRotate())
		{
			if(pGeom->IsFast())
			{
				Info.FastLength += CadrLength;
				Info.FastTime += CadrTime;
			}
			else
			{
				Info.WorkLength += CadrLength;
				Info.WorkTime += CadrTime;
			}
		}
		Info.Time += CadrTime;
		
	}
	if(!pState)
		Info.FileName = ProgName;
	else
		Info.FileName = pState->GetCurProg();
}

InfoProgram & BProgram::GetInfo()
{
	return Info;
}

void BProgram::SetHaveErrors(BOOL val)
{
	HaveErrors = val;
}

BOOL BProgram::HaveErr()
{
	return HaveErrors;
}

void BProgram::Clear(void)
{
	for(int n = 0; n < CadrArray.GetSize(); ++n)
		delete CadrArray[n];
	CadrArray.RemoveAll();
}

const NToolID BProgram::GetToolByHist(int HistInd, int *pHistPos) const
{
	ASSERT(HistInd >= 0);

	auto CurInd = GetTCIndByHist(HistInd);
	auto NextInd = CurInd + 1;
	*pHistPos = (NextInd < GetTCHistSize()) ? TCHistory.GetAt(NextInd).hpos : GetGeomArraySize();
	return TCHistory.IsEmpty() ? NToolID() : TCHistory.GetAt(CurInd).ntool;
}
const int BProgram::GetTCIndByHist(int HistInd) const
{
	if(GetTCHistSize() < 1)
		return 0;
	for(int i = 1; i < GetTCHistSize(); ++i)
	{
		if(TCHistory.GetAt(i).hpos > HistInd)
			return i - 1;
	}
	return GetTCHistSize() - 1;
}

void BProgram::PrepRenders(double Eps)
{
	if(!IsGeomReady)
		return;
	TrajRenders.clear();
	TrajCompRenders.clear();
	TrajRenders.resize(GeomArrays.size());
	TrajCompRenders.resize(GeomArrays.size());
	for (int Ti = 0; Ti < GeomArrays.size(); ++Ti)
		PrepRenders1(Ti, Eps);
}

void BProgram::PrepRenders1(int Ti, double Eps)
{
	TrajRenders[Ti].SetAxisLength(NCM_PROJECT.GetDouble("ProgToolAxesLength", 20.));
	TrajCompRenders[Ti].SetAxisLength(NCM_PROJECT.GetDouble("ProgToolAxesLength", 20.));

	TrajRenders[Ti].SetAxisThickness(NCM_PROJECT.GetDouble("ProgToolAxesThickness", 1.));
	TrajCompRenders[Ti].SetAxisThickness(NCM_PROJECT.GetDouble("ProgToolAxesThickness", 1.));

	TrajRenders[Ti].SetAxisThickness(NCM_PROJECT.GetDouble("ProgToolAxesThickness", 1.));
	TrajCompRenders[Ti].SetAxisThickness(NCM_PROJECT.GetDouble("ProgToolAxesThickness", 1.));
	TrajRenders[Ti].SetTolerance(Eps);
	CreateRLines(*GeomArrays[Ti], TrajRenders[Ti], GetColor().Inv(), false);
	TrajCompRenders[Ti].SetTolerance(Eps);
	CreateRLines(*CompGeomArrays[Ti], TrajCompRenders[Ti], GetColor(), true);
}

bool BProgram::CreateRLines(const BGeomArray &GeomArray, RLines &LinesRender, const NColor &CBold, bool Compens)
{
	LinesRender.Create(true, GeomArray.GetSize(), CBold);
	if (ToolAxisDir.size() != GeomArray.GetSize())
		NeedToolAxisDir = false;
	NColor *pToolAxisColor;
	if (NeedToolAxisDir)
		NCM_PROJECT.Colors.Lookup("ВекторИнструмента", pToolAxisColor);
	BPoint Axis(0., 0., 1., 0.);
	for(int i = 0, k = 0; i < GeomArray.GetSize(); ++i)
	{
		NCadrGeom *pGeom = GeomArray.GetAt(i);
		if(pGeom->IsCycle())
		{// Process cycle
			int CycleStart = i;
			for(++i; i < GeomArray.GetSize(); ++i)
			{
				pGeom = GeomArray.GetAt(i);
				if(pGeom->IsCycle())
					continue;
				// First non cycle cadr found
				int CycleEnd = i;
				//Draw cycle approach
				if (NeedToolAxisDir)
					Axis = ToolAxisDir[i];
				LinesRender.AddElem(pGeom, Axis, i, 0, k, GetCadrColor(pGeom, Compens), *pToolAxisColor);
				//Draw cycle body
				for(int l = CycleStart; l < CycleEnd; ++l)
				{
					NCadrGeom *pCGeom = GeomArray.GetAt(l);
					if (NeedToolAxisDir)
						Axis = ToolAxisDir[l];
					LinesRender.AddElem(pCGeom, Axis, i, l, k, GetCadrColor(pCGeom, Compens), *pToolAxisColor);
				}
				++k;
				break;
			}
		}
		else
		{
			if (NeedToolAxisDir)
				Axis = ToolAxisDir[i];
			LinesRender.AddElem(pGeom, Axis, i, 0, k, GetCadrColor(pGeom, Compens), *pToolAxisColor);
			++k;
		}
	}
	
	return LinesRender.Close();
}


int BProgram::GetLastHCadr()
{
	int LastHCadr = GetGeomArraySize() - 1;
	for(int i = 0; i < GetGeomArraySize(); ++i)
		if((*GeomArrays[0])[i]->IsCycle())
			--LastHCadr;
	return LastHCadr;
}

void BProgram::AddMSChangePos(int StPos, const class SMachState & Coords, const class SMachState & Flags)
{
	int HistPosAbs = int(GetGeomArrayByStPos(StPos).GetSize()) - 1;
	for (int i = 0; i < Coords.GetSize() - 1; ++i)
	{
		auto Flag = Flags.GetCoord(i);
		if (Flag != 0.)
		{
			MSHistory.Add(BMSChangePos(Coords.GetCoord(i), i, HistPosAbs, Flag == 1.));// Flag == 1 - fixed value
		}
	}
}

//void BProgram::AddMSChangePos(int CoordInd, double CoordVal)
//{
//	int HistPosAbs = GetGeomArraySize() - 1;
//	MSHistory.Add(BMSChangePos(CoordVal, CoordInd, HistPosAbs));
//}

void BProgram::AddStartMSChangePos(const class SMachState & Curr)
{
	for(int i = 0; i < Curr.GetSize() - 1; ++i)
		MSHistory.Add(BMSChangePos(Curr.GetCoord(i), i, 0, false));
}

void BProgram::ChangeMSChangeStart(const SMachState& Curr)
{
	for (int i = 0; i < Curr.GetSize() - 1 && i < MSHistory.GetSize(); ++i)
		MSHistory.SetAt(i, BMSChangePos(Curr.GetCoord(i), i, 0, false));
}

void BProgram::SetAddCoord(int MSHistNum, const SMachState& AddCoord)
{
	int CoordsNum = AddCoord.GetSize() - 1;
	bool FixFlags[32];
	bool* Flags = FixFlags;
	if (CoordsNum > 32)
		Flags = new bool[CoordsNum];

	for (int i = 0; i < CoordsNum; ++i)
		Flags[i] = false;

	for (int k = MSHistNum, l = 0; k >= 0 && l < CoordsNum; --k)
	{
		int Ind = MSHistory[k].GetIndex();
		if (!Flags[Ind])
		{
			++l;
			const double NewValue = AddCoord.GetCoord(Ind);
			const double OldValue = MSHistory[k].GetValue();
			if (fabs(NewValue - OldValue) > MIND) // coordinate changes and it becomes fixed regardless of its previous state
				MSHistory[k].SetValue(NewValue, true);
			Flags[Ind] = true;
		}
	}
	if (CoordsNum > 32)
		delete[] Flags;
}

void BProgram::GetAddCoord(int MSHistNum, SMachState &AddCoord) const
{
	if (!IsEval())
		return;// internal error
	int CoordsNum = AddCoord.GetSize() - 1;
	bool FixFlags[32];
	bool *Flags = FixFlags;
	if(CoordsNum > 32)
		Flags = new bool[CoordsNum];

	for(int i = 0; i < CoordsNum; ++i)
		Flags[i] = false;

	for(int k = MSHistNum, l = 0; k >= 0 && l < CoordsNum; --k)
	{
		int Ind = MSHistory[k].GetIndex();
		if(!Flags[Ind])
		{
			++l;
			AddCoord.SetCoord(Ind, MSHistory[k].GetValue());
			Flags[Ind] = true;
		}
	}
	if(CoordsNum > 32)
		delete[] Flags;
}
void BProgram::GetAddCoordPrev(int MSHistNum, SMachState &AddCoord) const
{
	int MSHistNumPrev = MSHistNum - 1;
	for(; MSHistNumPrev >= 0 ; --MSHistNumPrev)
	{
		if(MSHistory[MSHistNumPrev].GetPos() != MSHistory[MSHistNum].GetPos())
			break;
	}
	GetAddCoord(MSHistNumPrev, AddCoord);
}

CString & BProgram::TranslitRus(CString & Str, bool Tr2Ru)
{
	const int ArrSize = 79;
	const CString ArrEn[ArrSize] = {"sh", "cs", "ya", "ja", "zh", "ch", "ye", "je", "yu", "je",
							"ju", "yo", "jo", "y", "j", "u", "k", "e", "n", "g",
							"z", "h", "`", "f", "v", "a", "p", "r", "o", "l",
							"d",  "q", "s", "m", "i", "t", "'", "b", "c", "SH",
							"SC", "Y", "YA", "JA", "ZH", "CH",  "YE", "JE", "YU", "JU",
							"YO", "JO", "JE", "Y", "J", "U", "K", "E", "N", "G",
							"Z", "H", "~", "F", "V", "A", "P", "R", "O", "L",
							"D", "Q", "S", "M", "I", "T", "\"", "B", "C"};

	const CString ArrRu[ArrSize] = {"ш", "щ", "я", "я", "ж", "ч", "е", "е", "ю", "э",
							"ю", "ё", "ё", "ы", "й", "у", "к", "е", "н", "г",
							"з", "х", "ъ", "ф", "в", "а", "п", "р", "о", "л",
							"д", "э", "с", "м", "и", "т", "ь", "б", "ц", "Ш",
							"Щ", "Ы", "Я", "Я", "Ж", "Ч", "Е", "Е", "Ю", "Ю",
							"Ё", "Ё", "Э", "Й", "Й", "У", "К", "Е", "Н", "Г",
							"З", "Х", "Ъ", "Ф", "В", "А", "П", "Р", "О", "Л",
							"Д", "Э", "С", "М", "И", "Т", "Ь", "Б", "Ц"};
	if(Tr2Ru)
	{
		for(int i = 0; i < ArrSize; ++i)
			Str.Replace(ArrEn[i], ArrRu[i]);
	}
	else
	{
		for(int i = 0; i < ArrSize; ++i)
			Str.Replace(ArrRu[i], ArrEn[i]);
	}
    return Str;
}
 

NColor BProgram::GetCadrColor(NCadrGeom *pGeom, bool Compens)
{
	const NColor *pRes = &Colors[PC_MAIN];
	switch(PCM)
	{
	case PCM_DEFAULT:
		switch(pGeom->GetAttr())
		{
		case 0:
			pRes = &Colors[PC_MAIN];
			break;
		case 1:// Cycle attrib
		case 6:// Cycle attrib
		case 7:// Cycle attrib
			pRes = &Colors[PC_CYCLE];
			break;
		case 2:// Spindle off
		case 5:
			pRes = &Colors[PC_FAST];
			break;
		case 3:
			pRes = &NColor(1., 1., 0., 1.);
			break;
		case 4:
			pRes = &NColor(0., 0., 1., 1.);
			break;
		default:
			pRes = &Colors[PC_MAIN];
			break;
		}

		if(pGeom->IsFast())
		{
			pRes = &Colors[PC_FAST];
		}
		break;
	case PCM_INTERP:
		if(pGeom->IsRotate())
			pRes = &Colors[PC_ROTATE];
		else if(pGeom->IsFast())
			pRes = &Colors[PC_FAST];
		else if(pGeom->IsLine())
			pRes = &Colors[PC_LINE];
		else if(pGeom->IsCCWArc())
			pRes = &Colors[PC_CCWARC];
		else if(pGeom->IsCWArc())
			pRes = &Colors[PC_CWARC];
		break;
	case PCM_DCOMP:
		if(pGeom->GetType().flags & CU_DCOM)
			pRes = &Colors[PC_DCOMPON];
		else
			pRes = &Colors[PC_OFF];
		break;
	case PCM_LCOMP:
		if(pGeom->GetType().flags & CU_LCOMP_BEG)
			pRes = &Colors[PC_LCOMPON];
		else
			pRes = &Colors[PC_OFF];
		break;
	case PCM_CYCLE:
		if(pGeom->GetType().flags & CU_CYCL || pGeom->IsCycle())
			pRes = &Colors[PC_CYCLE];
		else
			pRes = &Colors[PC_OFF];
		break;
	case PCM_COOL:
		if(pGeom->GetType().flags & CU_COOL)
			pRes = &Colors[PC_COOLON];
		else
			pRes = &Colors[PC_OFF];
		break;
	}
	if(Compens)
		return pRes->Inv();
	else
		return *pRes;
}
void BProgram::SetManChanged()
{
	ManChanged = TRUE;
	FileTime = CFileTime();
}
void BProgram::MakeHCadr2History(void)
{
	int k = 0;//current index in History and GeomArray
	int i = 0;//current number (вычисляемый по правилам CurHCadr) 
	for(; k < GetGeomArraySize(); ++k)
	{
		if((*GeomArrays[0])[k]->IsCycle())
			continue;
		++i;
	}
	HCadr2History.SetSize(i);
	k = 0;
	i = 0;
	for(; k < GetGeomArraySize(); ++k)
	{
		if((*GeomArrays[0])[k]->IsCycle())
			continue;
		HCadr2History[i++] = k;
	}
}
void BProgram::SetAuxColors(void)
{
	for(int i = 0; i < PROG_COLORS_NUM; ++i)
		Colors[i] = Colors[PC_MAIN];

	NColor *c;
	if(NCM_PROJECT.Colors.Lookup("ХолХод", c))
		Colors[PC_FAST] = *c;

	if(NCM_PROJECT.Colors.Lookup("Цикл", c))
		Colors[PC_CYCLE] = *c;

	if(NCM_PROJECT.Colors.Lookup("ДвижениеПоОтрезку", c))
		Colors[PC_LINE] = *c;

	if(NCM_PROJECT.Colors.Lookup("ДвижениеПЧС", c))
		Colors[PC_CWARC] = *c;

	if(NCM_PROJECT.Colors.Lookup("ДвижениеПрЧС", c))
		Colors[PC_CCWARC] = *c;

	if(NCM_PROJECT.Colors.Lookup("БезОхлаждения", c))
		Colors[PC_OFF] = *c;

	if(NCM_PROJECT.Colors.Lookup("ОхлаждениеЖидкостью", c))
		Colors[PC_COOLON] = *c;

	if(NCM_PROJECT.Colors.Lookup("КоррекцияВлево", c))
		Colors[PC_DCOMPON] = *c;

	if(NCM_PROJECT.Colors.Lookup("КоррекцияВверх", c))
		Colors[PC_LCOMPON] = *c;

	if(NCM_PROJECT.Colors.Lookup("Поворот", c))
		Colors[PC_ROTATE] = *c;
}

bool BProgram::CheckLabelAttainability(int Source, int Dest) const
{
	// Returns true if there is not subprogram start between Source and Dest
	return !ProgList.CheckRange(Source, Dest);
}

BGeomArray& BProgram::GetCompGeomArrayByStPos(int StPos)
{
	auto Ind = GetIndexByStPos(StPos);
	if (Ind < 0)
		Ind = 0;
	return *CompGeomArrays.at(Ind);
}

void BProgram::SyncTCandTAG()
{
	if (TCHistory.IsEmpty())
		return;
	for (int TCStart = -1, TAGIt = 0; TAGIt < TAGHistory.GetSize();)
	{
		if (TAGHistory[TAGIt].CrToolCode != 0)
		{
			++TAGIt;
			continue;
		}
		int TCend = TCStart + 1;
		// is TAGIt positioned properly?
		bool PropPos = true;
		if (TCStart >= 0)
			PropPos = PropPos && (TAGHistory[TAGIt].hpos >= TCHistory[TCStart].hpos);
		if (TCend < TCHistory.GetSize())
			PropPos = PropPos && (TAGHistory[TAGIt].hpos < TCHistory[TCend].hpos);
		if (!PropPos)
		{
			++TCStart;
			continue;
		}
		// TAGIt positioned properly
		// find nearest
		int StartDist = (TCStart < 0) ? INT_MAX : TAGHistory[TAGIt].hpos - TCHistory[TCStart].hpos;
		int EndDist = (TCend < TCHistory.GetSize()) ? TCHistory[TCend].hpos - TAGHistory[TAGIt].hpos : INT_MAX;
		int FoundInd = TCStart;
		if (StartDist > EndDist)
			FoundInd = TCend;
		TAGHistory[TAGIt].AToolID = TCHistory[FoundInd].ntool;
		++TAGIt;
	}
}

void BProgram::PropogateTimeInd()
{
	for (int i = 1; i < CompGeomArrays.size(); ++i)
	{
		for (int k = 0; k < CompGeomArrays[0]->GetSize(); ++k)
		{
			auto Ind = CompGeomArrays.at(0)->GetAt(k)->GetTimeInd();
			CompGeomArrays.at(i)->GetAt(k)->SetTimeInd(Ind);
		}
	}
}

bool BProgram::HasBookMark(int Ind) const
{
	for (int i = 0; i < BookMarkArray.GetSize(); ++i)
		if (Ind == BookMarkArray[i])
			return true;
	return false;
}

int BProgram::GetIndexByStPos(int StPos) const
{
	int Ind = -1;
	auto it = StPos2Ind.find(StPos);
	if (it != StPos2Ind.end())
		Ind = it->second;
	return Ind;
}

//bool BProgram::CalcSpline(void)
//{
//	if(!HaveSpline())
//		return false;
//	bool SplineActive = false;
//	std::vector<int> SplinesB;
//	std::vector<int> SplinesE;
//	for(int i = 0; i < GeomArray.GetSize(); ++i)
//	{
//		if(GeomArray[i]->IsSpline())
//		{
//			if(!SplineActive)
//			{
//				// Start new spline
//				SplineActive = true;
//				SplinesB.push_back(i);
//			}
//			// Add spline point
//		}
//		else
//		{
//			if(SplineActive)
//			{
//				SplineActive = false;
//				SplinesE.push_back(i);
//			}
//		}
//	}
//	if(SplinesE.size() < SplinesB.size())
//		SplinesE.push_back(GeomArray.GetSize());
//	for(size_t SplInd = 0; SplInd < SplinesB.size(); ++SplInd)
//	{// For each spline
//		int IndB = SplinesB[SplInd];
//		int IndE = SplinesE[SplInd];
//		std::vector<int> Indexes; // of spline cadrs in GeomArray
//		for(int i = IndB; i < IndE; ++i)
//		{
//			NCadrGeom *pGeom = GeomArray[i];
//			if(pGeom->HaveGeom())
//				Indexes.push_back(i);
//		}
//		// Make spline
//		MakeSplineA(Indexes, GeomArray);
//		MakeSplineA(Indexes, CompGeomArray);
//	}
//	return true;
//}

BGeomArray& BProgram::GetGeomArrayByStPos(int StPos)
{
	auto Ind = GetIndexByStPos(StPos);
	if (Ind < 0)
		Ind = 0;
	return *GeomArrays.at(Ind);
}

const BGeomArray& BProgram::GetCompGeomArrayByStPos(int StPos) const
{
	auto Ind = GetIndexByStPos(StPos);
	if (Ind < 0)
		Ind = 0;
	return *CompGeomArrays.at(Ind);
}

const BGeomArray& BProgram::GetGeomArrayByStPos(int StPos) const
{
	auto Ind = GetIndexByStPos(StPos);
	if (Ind < 0)
		Ind = 0;
	return *GeomArrays.at(Ind);
}

const BGeomArray& BProgram::GetCompGeomArray(int Ind /*= 0*/) const
{
	return *CompGeomArrays.at(Ind);
}

const BGeomArray& BProgram::GetGeomArray(int Ind /*= 0*/) const
{
	return *GeomArrays.at(Ind);
}

int BProgram::GetGeomArraySize() const
{
	if (GeomArrays.empty())
		return 0;

	return int(GeomArrays.at(0)->GetSize());
}

int BProgram::Get1GeomArraySize(int StPos) const
{
	const auto Ind = GetIndexByStPos(StPos);
	if (Ind < 0)
		return 0;

	return int(GeomArrays.at(Ind)->GetSize());
}

MainTime BProgram::SetTimesInt(int StartInd, int EndInd, MainTime StartTime)
{
	if (EndInd == -1)
		EndInd = int(CompGeomArrays[0]->GetSize() - 1);
	MainTime IntStartTime = 0;
	for each (auto pArr in CompGeomArrays)
	{
		IntStartTime = StartTime;
		for (int i = StartInd; i <= EndInd; ++i)
		{
			auto pCadr = pArr->GetAt(i);
			if (pCadr->IsCycle())
			{
				NCadrGeom* pApprCadr = nullptr;
				int j = i + 1;
				for (; j <= EndInd; ++j)
				{
					pApprCadr = pArr->GetAt(j);
					if (!pApprCadr->IsCycle())
						break;
				}
				if (pApprCadr != nullptr)
				{
					pApprCadr->ShiftStartTime2(IntStartTime);
					IntStartTime = pApprCadr->GetEndTime();
				}
				for (int k = i; k < j; ++k)
				{
					pArr->GetAt(k)->ShiftStartTime2(IntStartTime);
					IntStartTime = pArr->GetAt(k)->GetEndTime();
				}
				i = j;
			}
			else
			{
				pCadr->ShiftStartTime2(IntStartTime);
				IntStartTime = pCadr->GetEndTime();
			}
		}
	}
	return IntStartTime;
}

void BProgram::SetWorkT(int CadrInd, MainTime iTime)
{
	for each (auto pArr in CompGeomArrays)
	{
		auto pCadr = pArr->GetAt(CadrInd);
		pCadr->ShiftWorkTime2(iTime);
	}
}

void BProgram::SetEndT(int CadrInd, MainTime iTime)
{
	for each (auto pArr in CompGeomArrays)
	{
		auto pCadr = pArr->GetAt(CadrInd);
		pCadr->SetEndTime(iTime);
	}
}

INT_PTR BUIntArray::Find(UINT Val, INT_PTR StartInd = 0, INT_PTR EndInd = -1) const
{
	// Find in ordered array
	// Find minimum Ind where Array[Ind] >= CadrID
	auto End = min(EndInd, GetSize() - 1);
	auto Start = max(StartInd, 0);
	Start = min(Start, End);
	if (Val == GetAt(End))
		return End; // This case is frequent
	if (Val <= GetAt(Start)) // Next "while" is not correct without this "if"
		return Start;
	if (Val > GetAt(End))
		return -1;
	while (End - Start > 1)
	{
		auto Mid = (End + Start) / 2;
		if(GetAt(Mid) >= Val)
			End = Mid;
		else
			Start = Mid;
	}
	return End;
}
