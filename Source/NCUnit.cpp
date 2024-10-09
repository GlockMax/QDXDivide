// NCUnit.cpp: implementation of the NCUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlcoll.h>
#include <set>
#include <map>
#include <filesystem>
#include "NLine.h"
#include "NProgram.h"
#include "ConstDef.h"
#include "BSpacePos.h"
#include "BGeomArray.h"
#include "BFastMoveMan.h"
#include "NCMProject.h"
#include "NCMDoc.h"
#include "NCycle.h"
#include "NCadr.h"
#include "NCadrSpeedPar.h"
#include "NStock.h"
#include "curve.h"
#include "NTurnCycle.h"
#include "NHHApprCycle.h"
#include "NTurnThreadACycle.h"
#include "NTurnThreadCycle.h"
#include "NTurnCutCycle.h"
#include "NTurnGroovCycle.h"
#include "NTurnThreadCutCycle.h"
#include "NTurnCutRoughCycle.h"
#include "NMillRectCycle.h"
#include "NMillDrillCycle.h"
#include "NThreadingTapCycle.h"
#include "NMillRectInCycle.h"
#include "NMillRectOutCycle.h"
#include "NMillCircCycle.h"
#include "NMillCircInCycle.h"
#include "NMillContCycle.h"
#include "NMapStringToOb.h"
#include "NUnitNameDlg.h"
#include "NToolCombined.h"
#include "NToolCombinedTurn.h"
#include "NEvalExp.h"
#include "math.h"
#include "NControlPanel.h"
#include "AptCLOUT.h"
#include "NTextErrDlg.h"
#include "NMileStone.h"
#include "NToolCombinedWeld.h"
#include "SCoordsDef.h"
#include "SMachStateFix.h"
#include "NVarSet.h"
#include "NChannel.h"
#include "NEvalStopCrit.h"
#include "Decrypt.h"
#include "MTConfig.h"
#include "ncunit.h"

const int ModalSubr2Base = 500;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const int WHOLE_PROG = 10000000;
const int START_REG_SHIFT = 12000;

double Dist2(double x1,double y1,double z1,double x2,double y2,double z2)
{
	return (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1);
}

#define GCD GetCommonDataC()
#define GCDV GetCommonDataV()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Initialize the static members


bool NCUnit::CheckNext = false;

CNCMDoc *NCUnitBase::pDoc;

bool NCUnit::CyclicA = false;
bool NCUnit::CyclicB = false;
bool NCUnit::CyclicC = false;
bool NCUnit::FileProtection = false;
bool NCUnit::AddZDispToComp = false;
bool NCUnit::SaveCoord2Reg = true;
bool NCUnit::PartialCompCancel = true;
bool NCUnit::ConsecutiveCalc = false;
bool NCUnit::UseIntegerArithm = false;

CString NCUnitBase::OpCharSet;
CString NCUnitBase::ExprCharSet;
CString NCUnitBase::LogCharSet;
CString NCUnitBase::Sign;
CString NCUnitBase::CharSet;
regex NCUnit::VarTestExp;
regex NCUnit::CharTestExp;

NCUnitBase::NCUnitBase()
{
}

NCUnitBase::~NCUnitBase()
{
}

void NCUnit::Init()
{
	Error = 0;
	ErrorMinor = 0;
	pAptOut = NULL; // Apt-CL export object
	Reset(false);
}

NCUnit::~NCUnit()
{
}

NCUnit::NCUnit(CNCMDoc *pD, int InChannelInd) : FixedDef(0), AddCoordsDef(0), AddCoordsScale(&AddCoordsDef), AddCoordsFeed(&AddCoordsDef)
{
	pDoc = pD;
	FixedDef.Define(_T("X\nY\nZ\nA\nB\nC"));
	pCommonData = make_shared<NCUnitCommonData>();
	pCommonData->CheckProgramText = false;// Special check for Heidenhine only
	pCommonData->ChannelInd = InChannelInd;
	pCommonData->LinearDecimalZero = -1;
	pCommonData->LinearScale = 1.;
	pCommonData->RotaryScale = 1.;
	pCommonData->XScale = 1.;
	pCommonData->YScale = 1.;
	pCommonData->ZScale = 1.;
	pCommonData->IScale = 1.;
	pCommonData->JScale = 1.;
	pCommonData->KScale = 1.;
	pCommonData->SpiralG2G3 = true;
	pCommonData->RegisterSymbol = '#';
	pCommonData->VariableSymbol = '\x0';
	pCommonData->VariableSymbolLoc = '\x0';
	pCommonData->ObserveGroove = false;
	pCommonData->AutoFixRotAxes = false;

	pCommonData->MaxWorkFeed = 100000.;
	pCommonData->DecimalConversion = 1.;
	pCommonData->CircleCenterMode = CCM_Incremental;
	pCommonData->NumAxes = 3;
	pCommonData->StockPos4Turn = 0;
	pCommonData->MyStockPos.insert(0);
	pCommonData->UseStdFuncNames = false;

	pCommonData->LType = LT_EMPTY;
	pCommonData->BaltSysStyle = false;
	pCommonData->SinumericStyle = false;
	pCommonData->AllowToolString = false;
	pCommonData->PolarCoordRecalc = true;
	pCommonData->MinArcDist = DMUO;
	pCommonData->ReplaceSet;
	Init();
	ASSERT_VALID(pD);
}

NCUnit::NCUnit(CNCMDoc* pD, const NCUnit& In) : FixedDef(In.FixedDef), AddCoordsDef(In.AddCoordsDef), AddCoordsScale(&AddCoordsDef), AddCoordsFeed(&AddCoordsDef)
{
	pDoc = pD;
	pCommonData = In.pCommonData;
	AddCoordsScale = In.AddCoordsScale;
	AddCoordsFeed = In.AddCoordsFeed;
	StartState.ForceMakeEqualTo(In.StartState);
	State.AddCoords = AddCoordsScale;
	AddState.AddCoords = AddCoordsScale;
	St.AddCoords = AddCoordsScale;
	StartState.AddCoordsFlags = AddCoordsScale;
	State.AddCoordsFlags = AddCoordsScale;
	AddState.AddCoordsFlags = AddCoordsScale;
	ProgGroupList = In.ProgGroupList;
	Init();
}

int NCUnit::VerifyProgramText(BProgram *pProg, NProgError& LocError)
{
	bool ErrFound = false;
	int ErrLabel = 0;
	CMap<int, int &, int, int&> Labels;
	for(int i = 0; i < pProg->GetSize(); ++i)
	{
		CString text = pProg->GetCadrText(i);


		if(text.GetLength() < 1)
			continue;
		CString buf = text.SpanIncluding("0123456789 \t");
		text.Delete(0, buf.GetLength());
		if(text.Left(3) != "LBL")
			continue;
		text.Delete(0, 3);
		int num = atoi(text);
		if(num == 0)
			continue;
		int Val;
		if(Labels.Lookup(num, Val))
		{
			if(!ErrFound)
			{
				ErrLabel = num;
				Error = 1116;// level ???
				cadrID Cadr;
				Cadr.Prog = GetChannel().NGetProgramListCn()->GetIndex(pProg);
				Cadr.Cadr = i;
				LocError.Set(Cadr, State.GetCurProg(), Error);
			}
			ErrFound = true;
			if(ErrLabel != num)
				continue;
			++(Labels[num]);
			if (Val + 1 < EV_MAX_NUM)
				LocError.Values[Val + 1].Format(_T("%d"), i + 1);
		}
		else
			Labels[num] = 0;
	}
	if(ErrFound)
	{
		LocError.Values[0].Format(_T("%d"), Labels[ErrLabel]);
		return -1;
	}
	else
		return 0;
}

void NCUnit::CheckProgramText(BProgram *pProg)
{
	NProgError LocError;
	if(VerifyProgramText(pProg, LocError) != 0)
	{
		pDoc->MarkErrCadr(&GetChannel(), LocError.GetCadr());
		BProgram *pProgText = GetChannel().NGetProgramListCn()->GetProg(LocError.GetCadr());
		if(pProgText)
		{
			if(!pProgText->HaveErr())
			{
				pProgText->SetHaveErrors();
				NTextErrDlg ErrDlg;
				ErrDlg.SetMes(pProgText->GetCadrText(LocError.GetCadr().Cadr),
							LocError.GetErrorString());
							
				if(IDYES == ErrDlg.DoModal())
					pDoc->GotoCadr(&GetChannel(), LocError.GetCadr());
			}
		}
	}
}

int NCUnit::RunProgram(BProgram *pProg, BProgram *pParentProg4Cycles/* = nullptr*/, int WholeProgSize/* = -1*/, bool ResetMatrs/* = false*/)
{
	CWaitCursor w;
	constexpr int InitProgNum = -1;
	//static CString TTool;// The last tool used by the program (refactored 3.10.18)
	//static int TTurret = -1;// The last turret used by the program
	int prog_size = WholeProgSize <= 0 ? pProg->GetSize() : WholeProgSize;
	if(prog_size <= 0)
		return 0;
	State.SetCurCadr(cadrID(pProg->IsInitProg() ? InitProgNum : State.GetCurCadrID().Prog, 0));
	if(!pProg->IsCycle())
	{
		State.ClearCurHCadr();
		State.ClearCurHCadrAbs();
	}

	State.ProgramStarted = false;

	State.EndProgram = false;
	BProgram *pActualProg = pParentProg4Cycles == nullptr ? pProg : pParentProg4Cycles;

	pProg->AddStartMSChangePos(State.AddCoords);
	
	NCM_PROJECT.GetProgressPercent().StartPart();
	NCUnitStatePrev::EvaluateCadrResult ret = NCUnitStatePrev::ECR_OK;
	bool DoBreak = false;
	bool Inactive = (!pProg->IsActive() || !GetChannel().NGetProgramListCn()->IsActive());
	for (int n = 0; !State.EndProgram; n++)
	{
		NCM_PROJECT.GetProgressPercent().NextStep(double(n) / prog_size);

		cadrID CadrID = State.GetCurCadrID();
		NCadr *pCurCadr = nullptr;
		if (pProg->IsCycle())
		{
			if (int(CadrID.Cadr) < pProg->GetSize())
				pCurCadr = (*pProg)[CadrID.Cadr];
		}
		else if (pProg->IsInitProg())
		{
			if (CadrID.Prog == InitProgNum && int(CadrID.Cadr) < pProg->GetSize())
				pCurCadr = (*pProg)[CadrID.Cadr];
			else
				pCurCadr = GetChannel().NGetProgramListCn()->GetCadr(CadrID);
		}
		else
		{
			if (ResetMatrs)
				GetStates().SendState(State, AddState, St.Geom.HaveGeom() ? St : LastGeomState);
			pCurCadr = GetChannel().NGetProgramListCn()->GetCadr(CadrID);
		}
		if (pCurCadr == nullptr)
			break;

		ret = RunProgramStep(pProg, pActualProg, pCurCadr, Inactive, ResetMatrs);

		if (ret == NCUnitStatePrev::ECR_PROG_END)
			break;

		DoBreak = (pDoc->ProgErrHandler.GetErrorReaction(ret, Error) == BProgErrHandler::ER_BREAK);
		if (DoBreak)
			break;

		if(n % 500000 == 0 && n > 0)
		{
			if (AfxMessageBox(IDS_MES_LOOP, MB_YESNO) != IDYES)
			{
				if (StoreError(1102))
				{
					ret = NCUnitStatePrev::ECR_ERROR;
					break;
				}
			}
		}
	}
	if (!ProcRetErrorCode(ret))
	{
		if (State.Spline.IsCancelling() || State.Spline.IsActive())
		{
			if (StoreError(1171))
			{
				ProcRetErrorCode(NCUnitStatePrev::ECR_ERROR);
				DoBreak = true;
			}
		}
	}
	if ((!(pProg->IsCycle() || pProg->IsInitProg())) && pProg->GetNeedToolAxisDir() && ResetMatrs)
	{
		FillToolAxes(pProg);
	}

	pProg->SetGeomReady();
	pProg->FillGabar(BSpacePos::GetRotMatrs());
	pProg->FillInfo(GetState());
	pProg->SetEvaluated();
	return DoBreak ? -1 : 1;
}

bool NCUnit::Check4InvalidChar(const CString& Text, char* pInvalidChar) const
{
	CString LocText(Text);
	RemoveComment(LocText);
	std::string str(LocText);
	std::match_results<std::string::const_iterator> mr2;
	if (regex_search(str, mr2, CharTestExp))
	{
		*pInvalidChar = mr2.str().at(0);
		return true;
	}
	return false;
}

void NCUnit::ProcErrors(NCadr* pCurCadr, BProgram* pActualProg)
{
	if (pCurCadr->GetErrCode() != 0 /* for level 1 cadrs only */ || ErrorMinor)
	{
		cadrID Cadr = pActualProg->GetCadrByHist(State.GetCurHCadrAbs());
		const CString& Text = pActualProg->GetCadrText(Cadr.Cadr);
		int ErrorCode = pCurCadr->GetErrCode() != 0 ? pCurCadr->GetErrCode() : ErrorMinor;
		NProgError LocErr;
		auto& CurError = TempError.IsPresent() ? TempError : LocErr;
		CurError.Set(Cadr, State.GetCurProg(), ErrorCode);
		char InvalidChar = ' ';
		if (Check4InvalidChar(Text, &InvalidChar))
		{
			CurError.ClearValues();
			CurError.Values[0] = InvalidChar;
			CurError.SetErrNum(1212);// invalid character
		}
		pActualProg->AddErrorPos(State.GetCurHCadrAbs(), CurError);
		CurError.Reset();
		pCurCadr->SetErrCode(0);
	}
}

NCUnitStatePrev::EvaluateCadrResult NCUnit::RunProgramStep(BProgram *pProg, BProgram *pActualProg, NCadr *pCurCadr, bool Inactive, bool ResetMatrs)
{
	// 
	// This fragment ensures redrawing of the interface while long process is running
	//		MSG msg;
	//		while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE | PM_QS_PAINT))
	//			AfxGetThread()->PumpMessage();
	// END:This fragment ...
	// Store current state
	BGeomArray *pCompGeomArray = &pActualProg->GetCompGeomArrayByStPos(GetCurStockPosition());
	BGeomArray *pGeomArray = &pActualProg->GetGeomArrayByStPos(GetCurStockPosition());
	BHistArray *pHistory = &pActualProg->GetHistory();
	BProgram::MSChangeHist *pMSHistory = &pActualProg->GetMSHistory();


	auto HistSize = pHistory->GetSize();
	cadrID CadrID = State.GetCurCadrID();
	const NCadr *pNextCadr = nullptr;
	if (CheckNext)
	{
		cadrID NextCadrID = CadrID;
		NextCadrID.Cadr++;
		pNextCadr = GetChannel().NGetProgramListCn()->GetCadr(NextCadrID);
	}
	NCUnitStatePrev::EvaluateCadrResult ret = EvaluateCadr(pCurCadr, pNextCadr, ResetMatrs);
	if (Inactive)
		State.EndProgram = true;


	if (pCurCadr->pCycle != NULL)// Last processed cadr is cycle call
		for (; HistSize < pHistory->GetSize(); ++HistSize)// Replace cycle cadr number by calling cadr number
			pHistory->SetAt(HistSize, CadrID);
	if (pProg->IsCycle())
		State.IncrCurHCadr(-1);// Decrement CurHCadr to cancel
						   // its incrementing in EvaluteCadr
	if (ret == NCUnitStatePrev::ECR_ERROR || ret == NCUnitStatePrev::ECR_PROG_END)
		return ret;

	// APT-CL export block
	if (pAptOut) // AptCL export is active
		if (State.GetCurProg() == pAptOut->GetProgName())
			if (!pProg->IsCycle())
				pAptOut->PostState(GetState(), State.Geom);
	// END:APT-CL export block

	if (!pProg->IsEval())
	{
		if (pProg->IsCycle())
		{
			State.Geom.SetAttr(1);
//			State.Geom.SetTime(/*0.5 * */State.Geom.GetTime());
			State.CompGeom.SetAttr(1);
//			State.CompGeom.SetTime(/*0.5 * */State.CompGeom.GetTime());
			State.AddCompGeom.SetAttr(1);
//			State.AddCompGeom.SetTime(/*0.5 * */State.AddCompGeom.GetTime());
			if (State.ThreadCadr)
			{
				State.Geom.SetAttr(7);// Thread attr
				State.CompGeom.SetAttr(7);
			}
		}
		else
		{
			if (State.ThreadCadr)
			{
				State.Geom.SetAttr(7);// Thread attr
				State.CompGeom.SetAttr(7);
			}
			if (State.Spindle[SPINDLE_TURN].SpindleDir == R_UNDEF && State.Spindle[SPINDLE_MILL].SpindleDir == R_UNDEF) // Spindle off
			{
				State.Geom.SetAttr(2);
				State.CompGeom.SetAttr(2);
				State.AddCompGeom.SetAttr(2);
			}
			else// Spindle on
			{
				if (State.Spindle[SPINDLE_MILL].SpindleDir == R_CCW)
				{
					switch (State.TechTable - (State.TechTable / 1000L) * 1000L)
					{
					case 0:
						break;
					case 30L:// Разметка
						State.Geom.SetAttr(3);
						State.CompGeom.SetAttr(3);
						State.AddCompGeom.SetAttr(3);
						break;
					case 440L:// Кернение
						State.Geom.SetAttr(4);
						State.CompGeom.SetAttr(4);
						State.AddCompGeom.SetAttr(4);
						break;
					default:
						break;
					}
				}
			}
			if (State.Feed > GCD.MaxWorkFeed) // Large feed
			{
				State.Geom.SetAttr(5);
				State.CompGeom.SetAttr(5);
				State.AddCompGeom.SetAttr(5);
			}
		}
	}
	if(!pActualProg->IsEval())
	{
		ProcLengthComp4Cycle();
		auto OldGeomArraySize = pGeomArray->GetSize();
		// Учет смещений
		BMatr ShiftMatr;
		BPoint V = State.CurToolDisplacement;
		ShiftMatr.Trans(V.GetX(), V.GetY(), V.GetZ());
		if (!CycleGeom.IsEmpty())
		{
			int MatrNum = State.Geom.GetMatrNum();
			if (State.Geom.IsRotate() && BSpacePos::LastMatrUsed == false && BSpacePos::pLastMT)
			{
				BMatr Q = BSpacePos::pLastMT->GetRespMatr(BSpacePos::LastCSMatr, BSpacePos::LastABC, BSpacePos::LastTurret, 0, GetCurStockPosition(), &GetFixedDef());// Ins STInd
				BSpacePos::AddRotMatr(Q, BSpacePos::LastABC);
				MatrNum = BSpacePos::GetLastMatrInd();
			}
			for (int k = 0; k < CycleGeom.GetSize(); ++k)
			{
				CycleGeom[k]->SetMatrNum(MatrNum);
				CycleGeom[k]->SetStockPos(CurStockPosition);
				CycleGeom[k]->SetGrooveFlag(State.Geom.IsGroove());
				if (CycleGeom.GetSize() == CycleGeomComp.GetSize())
				{
					CycleGeomComp[k]->SetMatrNum(MatrNum);
					CycleGeomComp[k]->SetStockPos(CurStockPosition);
					CycleGeomComp[k]->SetGrooveFlag(State.Geom.IsGroove());
				}
				if (!CycleGeom[k]->IsTimeDefined())
					State.IncrementTimer(CycleGeom[k]);
				pGeomArray->AddElement(*CycleGeom[k]);
				if (CycleGeom.GetSize() == CycleGeomComp.GetSize())
					pCompGeomArray->AddElement(CycleGeomComp[k]->Tr(ShiftMatr));
				else
					pCompGeomArray->AddElement(CycleGeom[k]->Tr(ShiftMatr));
				State.SetCurHCadrAbs(pHistory->Add(CadrID));
				pCompGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
				pGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
			}
			CycleGeom.RemoveAll();
		}
		if (State.AddCompGeom.HaveGeom())
		{
			pCompGeomArray->AddElement(State.AddCompGeom);
			NCadrGeom dummy;
			dummy.SetMatrNum(State.AddCompGeom.GetMatrNum());
			dummy.SetStockPos(State.AddCompGeom.GetStockPos());
			pGeomArray->AddElement(dummy);
			State.SetCurHCadrAbs(pHistory->Add(CadrID));
			pCompGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
			pGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
		}
		pCompGeomArray->AddElement(State.CompGeom);
		pGeomArray->AddElement(State.Geom);
		State.SetCurHCadrAbs(pHistory->Add(CadrID));
		pCompGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
		pGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
		if (AddState.Geom.HaveGeom())
		{
			if (AddState.AddCompGeom.HaveGeom())
			{
				pCompGeomArray->AddElement(AddState.AddCompGeom);
				NCadrGeom dummy;
				dummy.SetMatrNum(AddState.AddCompGeom.GetMatrNum());
				dummy.SetStockPos(AddState.AddCompGeom.GetStockPos());
				pGeomArray->AddElement(dummy);
				State.SetCurHCadrAbs(pHistory->Add(CadrID));
				pCompGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
				pGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
			}
			pCompGeomArray->AddElement(AddState.CompGeom);
			pGeomArray->AddElement(AddState.Geom);
			State.SetCurHCadrAbs(pHistory->Add(CadrID));
			pCompGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
			pGeomArray->GetLast()->SetHistID(cadrID(State.GetCurCadrIDEx().Prog, State.GetCurHCadrAbs()));
		}
		if (State.Spline.IsCancelling() && (!State.Spline.NeedMove() || (State.Spline.NeedMove() && State.CompGeom.HaveGeom())))
		{
			State.Spline.SetCancelling(false);
			// Find spline cadrs indexes
			std::vector<int> Indexes; // of spline cadrs in GeomArray
			Indexes.push_back(int(pCompGeomArray->GetSize() - 1));// End direction if needed or some extra cadr otherwise
			for (int i = int(pCompGeomArray->GetSize() - 2); i >= 0; --i)
			{
				NCadrGeom *pGeom = pCompGeomArray->GetAt(i);
				if (pGeom->HaveGeom())
				{
					if (pGeom->IsSpline())
						Indexes.push_back(i);
					else
						break;
				}
			}
			for (size_t i = 0; i < Indexes.size() / 2; ++i)
			{// Reverse Indexes
				int B = Indexes[i];
				Indexes[i] = Indexes[Indexes.size() - 1 - i];
				Indexes[Indexes.size() - 1 - i] = B;
			}
			// Make spline
			int SplRet = 0;
			if ((SplRet = MakeSplineA(Indexes, *pGeomArray)) < 0)
			{
				switch (SplRet)
				{
				case -1:
					Error = 1170;// level 1
					break;
				}
				ret = NCUnitStatePrev::ECR_ERROR;
			}
			//if (MakeSplineA(Indexes, *pCompGeomArray))
			//{
			//	switch (SplRet)
			//	{
			//	case -1:
			//		Error = 1170;
			//		break;
			//	}
			//	ret = NCUnitStatePrev::ECR_ERROR;
			//}
		}
		ProcErrors(pCurCadr, pActualProg);
		if (State.CurToolN != St.CurToolN || State.CurTurret != St.CurTurret)
		{// The active tool is changed in this cadr
			pActualProg->AddToolChangePos(State.GetCurHCadr() - 1, State.CurToolN, State.CurTurret);
		}
		if (AutoToolGenParams.Cool)
		{// 
			AutoToolGenParams.Cool = false;
			AutoToolGenParams.hpos = State.GetCurHCadr() - 1;
			pActualProg->AddTAGPos(AutoToolGenParams);
		}
		if (AutoToolGenParams.IsCool())
		{
			AutoToolGenParams.Cool = false;
			AutoToolGenParams.hpos = State.GetCurHCadr() - 1;
			AutoToolGenParams.AToolID.Set(AutoToolGenParams.CrToolCode, AutoToolGenParams.CrTurretNum);
			pActualProg->AddTAGPos(AutoToolGenParams);
		}
		if (State.ProbeCadr)
		{
			GetChannel().SetProbePresent();
			pActualProg->AddProbePos(State.GetCurHCadrAbs(), State.ProbeCollision);
		}
		if (State.AddCoordsChanged)
		{
			pActualProg->SetHaveAddCoord();
			pActualProg->AddMSChangePos(GetCurStockPosition(), State.AddCoords, State.AddCoordsFlags);
		}
		if (!CheckPoints.empty())
		{
			AddCheckPointPos(pActualProg, CheckPoints);

		}
		auto MSHistoryInd = pMSHistory->GetSize() - 1;
		for (auto i = OldGeomArraySize; i < pGeomArray->GetSize(); ++i)
		{
			pGeomArray->GetAt(i)->SetMSHistNum(MSHistoryInd);
			pCompGeomArray->GetAt(i)->SetMSHistNum(MSHistoryInd);
		}
		if (State.OrderChanged)
			pProg->SetHaveSub();
	}
	if (State.Spline.IsActive())
		pProg->SetHaveSpline();
	return ret;
}

int NCUnit::RunProgramProbePart(BProgram * pProg, BProgram * pParentProg4Cycles, int WholeProgSize/* = -1*/)
{// Regenerate program from current state to the first unexecuted probe
	//returns 0 - OK or 1 - error

	CWaitCursor w;
	int prog_size = WholeProgSize <= 0 ? pProg->GetSize() : WholeProgSize;
	if (prog_size <= 0)
		return 0;

	pProg->ClearEvaluatedPart(GetCurStockPosition());
	// check start position
	BProgram *pActualProg = pParentProg4Cycles == nullptr ? pProg : pParentProg4Cycles;

	if (!State.ProgramStarted)
	{
		pProg->AddStartMSChangePos(State.AddCoords);
		pProg->AddToolChangePos(0, State.CurToolN, State.CurTurret);
	}

	NCM_PROJECT.GetProgressPercent().StartPart();
	bool ResetMatrs = true;
	NCUnitStatePrev::EvaluateCadrResult ret = NCUnitStatePrev::ECR_OK;
	bool DoBreak = false;
	bool Inactive = (!pProg->IsActive() || !GetChannel().NGetProgramListCn()->IsActive());
	for (int n = 0; !State.EndProgram; ++n)
	{
		NCM_PROJECT.GetProgressPercent().NextStep(double(n) / prog_size);

		cadrID CadrID = State.GetCurCadrID();
		NCadr *pCurCadr = nullptr;
		if (pProg->IsCycle())
		{
			if (int(CadrID.Cadr) < pProg->GetSize())
				pCurCadr = (*pProg)[CadrID.Cadr];
		}
		else if (pProg->IsInitProg())
		{
			if (CadrID.Prog == 0 && int(CadrID.Cadr) < pProg->GetSize())
				pCurCadr = (*pProg)[CadrID.Cadr];
			else
				pCurCadr = GetChannel().NGetProgramListCn()->GetCadr(CadrID);
		}
		else
		{
			if (ResetMatrs)
				GetStates().SendState(State, AddState, St.Geom.HaveGeom() ? St : LastGeomState);
			pCurCadr = GetChannel().NGetProgramListCn()->GetCadr(CadrID);
		}
		if (pCurCadr == nullptr)
			break;

		ret = RunProgramStep(pProg, pActualProg, pCurCadr, Inactive, ResetMatrs);

		if (ret == NCUnitStatePrev::ECR_PROG_END)
			break;

		DoBreak = (pDoc->ProgErrHandler.GetErrorReaction(ret, Error) == BProgErrHandler::ER_BREAK);
		if (DoBreak)
			break;

		if (n % 500000 == 0 && n > 0)
		{
			if (AfxMessageBox(IDS_MES_LOOP, MB_YESNO) != IDYES)
			{
				if (StoreError(1102))
				{
					ret = NCUnitStatePrev::ECR_ERROR;
					break;
				}
			}
		}
	}
	if (!ProcRetErrorCode(ret))
	{
		if (State.Spline.IsCancelling() || State.Spline.IsActive())
		{
			if (StoreError(1171))
			{
				ProcRetErrorCode(NCUnitStatePrev::ECR_ERROR);
				DoBreak = true;
			}
		}
	}
	if ((!(pProg->IsCycle() || pProg->IsInitProg())) && pProg->GetNeedToolAxisDir() && ResetMatrs)
	{
		FillToolAxes(pProg);
	}

	pProg->FillGabar(BSpacePos::GetRotMatrs());
	pProg->FillInfo(GetState());
	pProg->SetGeomReady();
	pProg->SetEvaluated();
	return DoBreak ? 1 : 0;
}

bool NCUnit::ProcRetErrorCode(NCUnitStatePrev::EvaluateCadrResult retCode)
{
	// returns true if program run should be broken
	if (retCode == NCUnitStatePrev::ECR_ERROR)
	{
		NProgError LocError;
		LocError.Set(GetState()->GetCurCadrID(), GetState()->GetCurProg(), Error);
		pDoc->MarkErrCadr(&GetChannel(), GetState()->GetCurCadrID());
		BProgram* pProgText = GetChannel().NGetProgramListCn()->GetProg(LocError.GetCadr());
		if (pProgText)
		{
			if (!pProgText->HaveErr())
			{
				pProgText->SetHaveErrors();
				NTextErrDlg ErrDlg;
				ErrDlg.SetMes(pProgText->GetCadrTextSafe(GetState()->GetCurCadrID().Cadr),
					LocError.GetErrorString());

				if (IDYES == ErrDlg.DoModal())
					pDoc->GotoCadr(&GetChannel(), GetState()->GetCurCadrID());
			}
		}
		State.EndProgram = true;
		return true;
	}
	return false;
}

void NCUnit::FillToolAxes(BProgram * pProg) const
{
	pProg->ResizeToolAxisDir();
	int TCPosBeg;
	NToolID ToolID;
	pProg->GetTCHist(0, TCPosBeg, ToolID);
	for (int TCIndex = 1; TCIndex <= pProg->GetTCHistSize(); ++TCIndex)
	{
		NToolCombined *pTool = nullptr;
		auto ToolFound = pDoc->NGetToolList()->Lookup(ToolID, pTool);
		int TCPosEnd;
		if (!pProg->GetTCHist(TCIndex, TCPosEnd, ToolID))
			TCPosEnd = pProg->GetGeomArraySize();
		if (ToolFound)
		{
			int MatrNum = -1;
			BPoint ToolAxis(0., 0., 1., 0.);
			for (int i = 0; i < TCPosBeg; ++i)// tool is not defined
				pProg->SetToolAxisDir(i, ToolAxis);

			for (int HistInd = TCPosBeg; HistInd < TCPosEnd; ++HistInd)
			{
				const NCadrGeom &Geom = *pProg->GetGeomArray()[HistInd];
				if (!Geom.IsUndef())
				{
					if (Geom.GetCurve() != nullptr)
					{
						ToolAxis = Geom.GetCurve()->GetToolAxisVec();
						MatrNum = -1;
					}
					else if (Geom.GetMatrNum() != MatrNum)
					{
						MatrNum = Geom.GetMatrNum();
						SMachState MachState(BPoint(0., 0., 0., 1.), BSpacePos::GetABC(MatrNum), &GetFixedDef());
						BMatr Matr = pDoc->GetMachTool().GetStock2ToolMatrWRec(MachState
							, Geom.GetStockPos(), pTool->Turret, pTool->Attach);
						ToolAxis = BPoint(0., 0., 1., 0.) * Matr;
					}
				}
				pProg->SetToolAxisDir(HistInd, ToolAxis);
			}
		}
		TCPosBeg = TCPosEnd;
	}
}


int NCUnit::MakeSplineA(const std::vector<int> &Indexes, BGeomArray &GeomArray)
{
	// Make Akima spline on the points referred in Indexes
	// Store spline segments in GeomArray
	if(Indexes.size() < 4)
		return -1;
	std::vector<BPoint> Pts;
	Pts.resize(Indexes.size() + 3);
	Pts[1] = GeomArray[Indexes[0]]->GetB(); 
	for(size_t i = 0; i < Indexes.size(); ++i)
		Pts[i + 2] = GeomArray[Indexes[i]]->GetE();
	// Calc Mi
	std::vector<BPoint> Mi;
	Mi.resize(Indexes.size() + 3);
	for(size_t i = 1; i < Indexes.size() + 1; ++i)
		Mi[i] = Pts[i + 1] - Pts[i];
	Mi[0] = Mi[2] * 3 - Mi[3] * 2;
	Mi[1] = Mi[2] * 2 - Mi[3];
	Mi[Indexes.size()] = Mi[Indexes.size()] * 2 - Mi[Indexes.size() - 1];
	Mi[Indexes.size() + 1] = Mi[Indexes.size() - 1] * 3 - Mi[Indexes.size() - 2] * 2;
	// Calc derivatives
	std::vector<BPoint> Der;
	Der.resize(Indexes.size() - 1);
	for(size_t i = 3; i < Pts.size() - 1; ++i)
	{
		auto iM = i - 1;
		auto iD = i - 3;
		BPoint P0 = (Mi[iM - 1] - Mi[iM - 2]).CAbs();
		BPoint P1 = (Mi[iM] - Mi[iM + 1]).CAbs();
		BPoint Pz = P0 + P1;
		bool ZeroX = (Pz.GetX() < MIND);
		bool ZeroY = (Pz.GetY() < MIND);
		bool ZeroZ = (Pz.GetZ() < MIND);
		if(ZeroX) {P0.SetX(1.); P1.SetX(1.);}
		if(ZeroY) {P0.SetY(1.); P1.SetY(1.);}
		if(ZeroZ) {P0.SetZ(1.); P1.SetZ(1.);}
		Pz = P0 + P1;
		Pz.SetH(1.);
		Der[iD] = (P1.CMul(Mi[iM - 1]) + P0.CMul(Mi[iM])).CDiv(Pz) * 0.5;
	}
	// Boundary conditions
	if(State.Spline.GetStartCond() == BSplineParams::Tang)
	{
		Der[0] = (Pts[2] - Pts[1]) * 0.5;
	}

	if(State.Spline.GetEndCond() == BSplineParams::Tang)
	{
		Der[Indexes.size() - 2] = (Pts[Indexes.size() + 1] - Pts[Indexes.size()]) * 0.5;
	}
	// Make segments
	for(size_t i = 1; i < Indexes.size() - 1; ++i)
	{
		BCurve *pCurve = BCurve::NewBCurve(1);
		GeomArray[Indexes[i]]->SetCurve(pCurve);
		pCurve->SetPoint(0, Pts[i + 1]);
		pCurve->SetPoint(1, Pts[i + 1] + Der[i - 1]);
		pCurve->SetPoint(2, Pts[i + 2] - Der[i]);
		pCurve->SetPoint(3, Pts[i + 2]);
	}
	// Boundary conditions
	if(State.Spline.GetStartCond() == BSplineParams::ZeroCurv)
	{
		GeomArray[Indexes[1]]->GetCurve()->ZeroCurv(true);
	}

	if(State.Spline.GetEndCond() == BSplineParams::ZeroCurv)
	{
		GeomArray[Indexes[ Indexes.size() - 2]]->GetCurve()->ZeroCurv(false);
	}
	return true;
}

void NCUnit::Reset(bool NeedInitProg /* = true*/)
{
	LastGeomState.Reset();
	State.ResetRegisters();
	StartState.GetVarSet().Clear();
	if (NeedInitProg)
	{
		CString Str;
		GCD.SOthers.Lookup("MainInitStr1", Str);
		if (RunInitProg(Str) < 0)
			ConfFileErrMess();
	}
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;

	if(pDoc->GetMachTool().GetCurConfig().IsDefined())
		pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.CallStack.Clear();
	State.CycleStack.Clear();
	State.WhileStack.Clear();
	HiddenCycle = CYCLE_OFF;
	WordDef.Clear();
	StoredPlane = P_UNDEF;
	CurStockPosition = 0;
	AutoToolGenParams.Cool = false;
}

NCUnitStatePrev::EvaluateCadrResult NCUnit::EvaluateCurrent( bool Silent)
{
//Current cadr of current program
// returns -1 when program is over or terminated	
//			<-1 when error was founded
//			1 when breakpoint
//			0 when OK
	NProgram *pProg;
	CMapStringToNProgram *plist = GetChannel().NGetProgramListCn();
	if(GetState()->GetCurProg().IsEmpty()) // Get first program
	{
		if(plist->GetMainProgCount() <= 0)
		{
			if(StoreError(1086))
				return NCUnitStatePrev::ECR_ERROR; // Program list is empty
		}
		CString str;
		plist->GetAt(0, str, (CObject *&)pProg);
		SetCurProg(str);
	}
	else 
	{
		if(!plist->Lookup(GetState()->GetCurProg(), pProg))
			return NCUnitStatePrev::ECR_ERROR; //Error in CurProg
		if(plist->GetIndex(pProg) > plist->GetMainProgCount())
			return NCUnitStatePrev::ECR_ERROR; //Error in CurProg
	}
	if(State.EndProgram)
		return NCUnitStatePrev::ECR_PROG_END; //Program is over
	//if (State.GetCurHCadrAbsEx().Cadr >= pProg->GetHistorySize())
	//	return NCUnitStatePrev::ECR_PROG_END; //Program is over
	int HistInd = pProg->GetHistByHCadr(State.GetCurHCadr());
	cadrID CadrID = State.GetCurCadrID();
	const NCadr *pNextCadr = NULL;
	if(CheckNext)
	{
		cadrID NextCadrID = CadrID;
		NextCadrID.Cadr++;
		pNextCadr = GetChannel().NGetProgramListCn()->GetCadr(NextCadrID);
	}
	NCadr *pCurCadr = NULL;
	if(pProg->IsCycle())
	{
		if(int(CadrID.Cadr) < pProg->GetSize())
			pCurCadr = (*pProg)[CadrID.Cadr];
	}
	else
	{
		pCurCadr = GetChannel().NGetProgramListCn()->GetCadr(CadrID);
	}
	NCUnitStatePrev::EvaluateCadrResult ret = NCUnitStatePrev::ECR_OK;
	if (pProg->HaveErr() && State.GetCurHCadr() >= pProg->GetHCadr2HistorySize())
		ret = NCUnitStatePrev::ECR_ERROR;
	else
		ret = EvaluateCadr(pCurCadr, pNextCadr);

	if (pProg->HaveErr() && State.GetCurHCadr() > pProg->GetHCadr2HistorySize())
		ret = NCUnitStatePrev::ECR_ERROR;
	else
	{
		//Get time and length from history
		State.CadrTime = 0.; 
		int i = HistInd;
		for (; i < pProg->Get1GeomArraySize(GetCurStockPosition()); ++i)
		{
			if (pProg->GetCadrByHist(i) != CadrID)
				break;

			NCadrGeom* pGeom = pProg->GetCompGeomArrayByStPos(GetCurStockPosition())[i];
			if (pGeom->IsCycle())
				break;

			if (pGeom->IsTimeDefined())
				State.CadrTime += pGeom->GetWorkTimeS();
			State.CalcLength(pGeom);
		}
		State.SetCurHCadrAbs(i - 1);
		i = HistInd - 1;
		for (; i >= 0; --i)
		{
			if (pProg->GetCadrByHist(i) != CadrID)
				break;

			NCadrGeom* pGeom = pProg->GetCompGeomArrayByStPos(GetCurStockPosition())[i];
			if (!pGeom->IsCycle())
				break;

			if (pGeom->IsTimeDefined())
				State.CadrTime += pGeom->GetWorkTimeS();
			State.CalcLength(pGeom);
		}
		State.Time += State.CadrTime;
		State.ProcessLifeTime();
	}
	//END:Get time from history
	if (ret == NCUnitStatePrev::ECR_PROG_END || ret == NCUnitStatePrev::ECR_BREAK)
	{
		return ret;
	}
	if (ret == NCUnitStatePrev::ECR_OK)
	{
		if (State.ProbeCadr)
		{
			ret = State.ProcessProbe(HistInd, pProg);
			if (ret == NCUnitStatePrev::ECR_ERROR)
			{
				Error = (State.ProbeExpTouch ? 1168 : 1169);// level 0
			}
			BProgram::ProbePos::NRegChanges RegChanges;
			FillRegChanges(RegChanges);
			// Apply reg changes
			for (int k = 0; k < RegChanges.size(); ++k)
			{
				auto& Change = RegChanges[k];
				if (Change.second == 0.)
					State.GetRegisters().RemoveKey(Change.first);
				else
					State.GetRegisters()[Change.first] = Change.second;
			}
			pDoc->GetMachTool().ProcCadrStep(*this, State);
		}
		if (ret == NCUnitStatePrev::ECR_OK && pDoc->MileStones.IsMileStone(this, &St))// Check mile stone
		{
			GetState()->MileStoneFound = true;
			ret = NCUnitStatePrev::ECR_BREAK;// Must be before next cadr determination due to bookmarks work properly
		}
	}
	if (ret == NCUnitStatePrev::ECR_ERROR)
	{
		if (!Silent && !NTextErrDlg::Busy)
		{
			ProcRetErrorCode(ret);
		}
	}
	return ret;
}

CString NCUnit::GetField(CString &str) const
{
	CString	key = "";
	int i = 0;
	for( ; i<str.GetLength(); ++i)
	{
		if(str[i] == ' ' || str[i] == '\t')
			break;

		if(str[i] == '\"')
		{
			if(i == str.GetLength()-1)
				break;
			if(	str[i+1] == ' '  || 
				str[i+1] == '\t')
				break;
		}
		key = key + str[i];
	}
// key string is filled
// need to erase all delimeters
	if(i == str.GetLength())
	{
		str = "";
		return key;
	}
	str.Delete(0,i);
	for(i = 0; i<str.GetLength(); ++i)
	{
		if(str[i] == ' ' || str[i] == '\t')
			continue;

		if(str[i] == '\"')
		{
			if(i == 0)
				continue;
			++i;
			break;
		}
		break;
	}
	if(i == str.GetLength())
	{
		str = "";
		return key;
	}
	str.Delete(0,i);
	return key;
}

void NCUnit::ConfFileErrMess()
{
	CString Mess;
	Mess.Format(IDS_MES_ERR_FILE1, GCD.ConfFileName);
	AfxMessageBox(Mess);
}

void NCUnit::Serialize(CArchive &ar)
{
	if (ar.IsStoring() && IsTempName(GCD.ConfFileName))
	{
		CString Name2Store;
		MakeTempName(GCD.ConfFileName, Name2Store, true);
		SerializeElements(ar, &Name2Store, 1);
	}
	else
		SerializeElements(ar, &GCDV.ConfFileName, 1);
	if(ar.IsLoading())
		if(SetDef(GCDV.ConfFileName) < 0)
		{
			GCDV.ConfFileName = "";
		}

	StartState.Serialize(ar);
	if(StartState.MetricCoord != (MM_INCH == M_MM))
	{
		if(ar.m_strFileName.Right(6) == "NCMPrt")
		{
			StartState.MetricCoord = (MM_INCH == M_MM);
			StartState.FeedM = StartState.MetricCoord ? MMpMIN : INCHpMIN;
			StartState.MashZ = (MM_INCH == M_MM) ? 50. : 2.;
			StartState.MashcZ = (MM_INCH == M_MM) ? 50. : 2.;
		}
		else
			AfxMessageBox(IDS_UNIT_PROJECT);
	}
	if (ar.IsLoading())
		Init();
}

NCUnitStatePrev::EvaluateCadrResult NCUnit::EvaluateCadrBase(NCadr *pCadr, const NCadr *pNextCadr, bool ResetMatrs)
{
	// Returns	0 - if OK
	//			<-1 - if error
	//			1 - if breakpoint

	if(!pCadr)
		return NCUnitStatePrev::ECR_PROG_END;
	// Processing may not be stoped while cycle is active
	if(State.AllowBreak)
	{
		if((1<<(sizeof(SHORT) * 8 - 1)) & GetAsyncKeyState(VK_F10))
		{// Stop if F10 is currently pressed (most significant bit of GetAsyncState)
			if(AfxMessageBox(IDS_MES_BREAK, MB_YESNO )
				!= IDYES)
			{
				if (StoreError(1102))
					return NCUnitStatePrev::ECR_ERROR;
			}
		}
	}
	if(!GCD.IsDefined)
	{
		Error = 1000;// level 0
		return NCUnitStatePrev::ECR_ERROR;
	}
	// save previos state
	if(State.NonExecCount == 0)
	{
		if (!State.Geom.HaveGeom() && St.Geom.HaveGeom())
			LastGeomState = St;

		if (State.MachineCSActive)
		{
			State.CurBase = St.CurBase;
			State.MachineCSActive = false;
		}
		if(State.FeedNonModal)
		{
			State.Feed = St.Feed;
			State.FeedNonModal = false;
		}
		ApplyRCP(true);
		St.MakeEqualTo(State);//st = State;
	}
	if (State.IsChamfCoordStored())
	{
		AddState.ChamfCoord2Coord();
		State.SwapChamfCoord();
	}
	if (AddState.Geom.HaveGeom())
		St.MakeEqualTo(AddState);
	St.MashcX -= State.CurToolDisplacement.GetX();
	St.MashcY -= State.CurToolDisplacement.GetY();
	St.MashcZ -= State.CurToolDisplacement.GetZ();
// preprocessing
	// Get text from cadr
	CString text = pCadr->GetReText();
//	DoReplace(text);
	State.GetVarSet().Apply(text);
	text.Remove(' ');

	ErrorMinor = 0;
	CString SkipSym, ComSym;
	GCD.SWordDef.Lookup("BlockSkip", ComSym);
	GCD.SWordDef.Lookup("BlockDebug", SkipSym);
	CString FirstSym = text.Left(1);
	if((FirstSym == ComSym) || (FirstSym == SkipSym && (State.GetRegister(8999) == 0.)) || text.GetLength() == 0 || pCadr->GetErrCode() != 0)
	{
		int b;
		b = State.AddCompGeom.GetMatrNum();
		State.AddCompGeom.Clear();
		State.AddCompGeom.SetMatrNum(b);
		b = State.CompGeom.GetMatrNum();
		State.CompGeom.Clear();
		State.CompGeom.SetMatrNum(b);
		b = State.Geom.GetMatrNum();
		State.Geom.Clear();
		State.Geom.SetMatrNum(b);
		b = AddState.Geom.GetMatrNum();
		AddState.Geom.Clear();
		AddState.Geom.SetMatrNum(b);
		State.OrderChanged = false;
		State.CadrSkiped = true;
		State.CadrTime = 0.;
		State.AddCoordsChanged = false;
		State.AddCoordsFlags.ClearValues();
	}
	else
	{// ordinary processing

		// Execute Reports
		if(pDoc->pRepArray)
			pDoc->pRepArray->Evaluate();

		// Initialize
		InitNonExecCadr(St);
		if(State.NonExecCount == 0)
			InitExecCadr(St);
		else
			--State.NonExecCount;

		RemoveComment(text); // Added 1.10.07
		if(CheckNext && pNextCadr)
		{
			CString NextText = pNextCadr->GetReText();
//			DoReplace(NextText);
			RemoveComment(NextText);
			if(NChamfer::HavePattern(NextText))
				++State.NonExecCount;
		}

		for(int ElNum = 0; text.GetLength() > 0 ; ++ElNum)
		{
		// Process modal call
			if(State.ModalSubr2)
				if(!State.ModalSubrID.IsEmpty() && State.ModalSubrIDRet.IsEmpty())// Modal call is defined but is not called now
				{
					if(!ProcessRegSubst(text, ModalSubr2Base))
						return NCUnitStatePrev::ECR_ERROR;
					if(text.IsEmpty())
						break;
				}
			if (State.VariablesDecl)
			{
				if (!ProcessVariablesDecl(text, false))
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			switch (text[0])
			{
			case ' ':
				text.Delete(0);
				continue;
			case 'I': // To speed up only
				// process IF statement and IN(if not) statement
				bool If = text.Left(2) == "IF";
				bool In = text.Left(2) == "IN";
				if (If || In)
				{
					NRe2Set Tmp;
					Tmp.AddReplacePair(0, "(IF\\W|IN\\W)", "\\1");
					if (!Tmp.Apply(text))
					{
						Error = 1025;// level 1 // may be changed
						return NCUnitStatePrev::ECR_ERROR; 
					}
					if (!ProcessIF(text, In))
						return NCUnitStatePrev::ECR_ERROR; // Error defined in ProcessIF
					continue;
				}
				break;
			}
			if(GCD.LType == LT_SINUMERIK)
			{
				if(text.GetLength() >= 3)
				{
					if(isletter(text[0]) && isletter(text[1]))
					{// Label
						if(text.Find(':') > 0)
						{
							// label present
							const CString Label = text.SpanExcluding(_T(":"));
							const bool EndLabel = (Label == _T("ENDLABEL"));
							int Val = 0;
							if (EndLabel)
							{
								text.Delete(0, Label.GetLength() + 1);
							}
							else
							{
								if (!GetLValue(text, &Val))
								{
									Error = 1149;// level 1
									return NCUnitStatePrev::ECR_ERROR;
								}
							}
							if (State.ActiveCopyEnd == -1)
								continue;
							// Program copy is active
							if(Val == State.ActiveCopyEnd || EndLabel)
							{// Program copy is active and current cadr is the last cadr of copy
								if (!ProgCopyLC())
								{
									Error = 1001;// level 0
									return NCUnitStatePrev::ECR_ERROR;
								}
							}
							continue;
						}
						else
						{// May be sinumerik subroutine call
							if(GCD.SinumericStyle)
							{
								RemoveComment(text);
								const int BlankInd = text.Find("@@");
								CString NewText;
								if(BlankInd < 0)
								{
									NewText = text;
									text.Empty();
								}
								else
								{
									NewText = text.Left(BlankInd);
									text.Delete(0, BlankInd + 2);
								}
								text = _T("G1065L\"") + NewText + _T("\"") + text;
							}
						}
					}
				}
			}
			// recognize word
			CString CurWord = GetWord(text);
			if(CurWord == "ModalCommentOff")
				State.CommentActive = FALSE;
			if(State.CommentActive)
				break;
			if(CurWord == "ModalCommentOn")
			{
				State.CommentActive = TRUE;
				continue;
			}
			if(CurWord == "BlockSkip")
			{
				break;
			}
			if(CurWord == "BlockDebug")
			{
				if(State.GetRegister(8999) == 0.)
					break;
			}
			if(CurWord == "ProgramBeginEnd")
			{
				State.ProgramStarted = !State.ProgramStarted;
				if(State.ProgramStarted)
				{
					const int cadr = FindStartCadr();
					if(cadr != 0)
					{
						State.OrderChanged = true;
						State.NextCadr.Cadr = cadr;
					}
				}
				break;
			}
			if(!State.ProgramStarted)
				if(GetStartProgram().Find("Ignore") < 0)
					break;
				else // Ignore specified
				{
					State.ProgramStarted = TRUE;
					const int cadr = FindStartCadr();
					if(cadr != 0)
					{
						State.OrderChanged = true;
						State.NextCadr.Cadr = cadr;
					}
				}

			// Line number (line number may contain "line number" symbol or stay as a first number in the cadr)
			if(ElNum == 0 // Line number must be the first elem in the cadr
				&& (CurWord == "LineNumber" ||
				(CurWord.GetLength() == 0 && text.GetLength() > 0 && isdigit(UCHAR(text.GetAt(0))))))
			{
				if(!GetIValue(text,&State.CurCadrNum))
				{
					if (StoreError(1026))
						return NCUnitStatePrev::ECR_ERROR;//error
					else
						State.CurCadrNum = 1;
				}
				if(State.CurCadrNum == State.ActiveCopyEnd)
				{// Program copy is active and current cadr is the last cadr of copy
					if (!ProgCopyLC())
					{
						Error = 1001;// level 0
						return NCUnitStatePrev::ECR_ERROR;
					}
				}
				continue;
			}

			if(CurWord.GetLength() == 0)
			{
				CString str("No");
				GCD.SOthers.Lookup("IgnoreUnknownCh", str);
				if(str != "Yes")
				{
					if (StoreError(1028))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				text.Delete(0);
				double val;
				GetDValue(text, &val);
				continue;
			}
			if(State.CurCycle == CYCLE_OFF || State.IsNotDrillAxis(CurWord)
				|| State.CurCycle == TURNCUT || State.CurCycle == TURNFACECUT
				|| State.CurCycle == TURNCUTROUGH || State.CurCycle == TURNFACECUTROUGH
				|| State.CurCycle == TURNCHAMFER || State.CurCycle == TURNROUND
				|| State.CurCycle == TURNCLEAR
				)
			{
			// The corresponding ELSE is the last block in this cycle
				if( CurWord == "IKGoTo")
				{
					if(State.CurCurveType == cwarc || State.CurCurveType == ccwarc)
					{
						if(!State.ChangedCoord.i)
							CurWord = "CircleHorzCenter";
						else
							CurWord = "CircleDepthCenter";
					}
					else
					{// GoTo
						CString title;
						GCD.SWordDef.Lookup("ProgramNumber", title);
						text = title + text;
						DoGoTo(text, SD_FORBACK);
					}
				}
				if(	CurWord == "HorizontalAxis" ||// X-coord
					CurWord == "VerticalAxis" ||// Y-coord
					CurWord == "SpindleAxis" ||// Z-coord
					CurWord == "CircleHorzCenter" ||// I-coord
					CurWord == "CircleVerticalCenter" ||// J-coord
					CurWord == "CircleDepthCenter" ||// K-coord
					CurWord == "PolarDist" ||
					CurWord == "PolarAngle" ||
					CurWord == "PolarAngleInc" ||
					CurWord == "CircleRadius" )// R
				{
					if(!ProcessCoord(text,CurWord))
						return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessCoord

					continue;
				}
				if(	CurWord == "HorizontalAxisInc" ||// X-coord
					CurWord == "VerticalAxisInc" ||// Y-coord
					CurWord == "SpindleAxisInc" )// Z-coord
				{
					CurWord.Delete(CurWord.GetLength() - 3, 3);
					const BOOL BufAC = State.AbsoluteCoord;
					State.AbsoluteCoord = FALSE;
					if(!ProcessCoord(text,CurWord))
						return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessCoord
					State.AbsoluteCoord = BufAC;
					continue;
				}
				if(	CurWord == "HorizontalAxisAbs" ||// X-coord
					CurWord == "VerticalAxisAbs" ||// Y-coord
					CurWord == "SpindleAxisAbs" )// Z-coord
				{
					CurWord.Delete(CurWord.GetLength() - 3, 3);
					const BOOL BufAC = State.AbsoluteCoord;
					State.AbsoluteCoord = TRUE;
					if(!ProcessCoord(text,CurWord))
						return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessCoord
					State.AbsoluteCoord = BufAC;
					continue;
				}
				if(	CurWord == "CircleHorzCenterAbs" ||// I-coord
					CurWord == "CircleVerticalCenterAbs" ||// J-coord
					CurWord == "CircleDepthCenterAbs" )// K-coord
				{
					CurWord.Delete(CurWord.GetLength() - 3, 3);
					if(!ProcessCoord(text, CurWord, 1))
						return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessCoord
					continue;
				}
			}
			if(	CurWord == "CircleHorzCenterInc" ||// I-coord
				CurWord == "CircleVerticalCenterInc" ||// J-coord
				CurWord == "CircleDepthCenterInc" )// K-coord
			{
				CurWord.Delete(CurWord.GetLength() - 3, 3);
				if(!ProcessCoord(text, CurWord, -1))
					return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessCoord
				continue;
			}
			if(	CurWord == "HorizontalRotationAxis" || 
				CurWord == "VerticalRotationAxis" ||
				CurWord == "SpindleRotationAxis")
			{
				if(!ProcessCoordRot(text, CurWord))
					return NCUnitStatePrev::ECR_ERROR; // Error defined in ProcessCoordRot
				continue;
			}
			if(	CurWord == "HorizontalRotationAxisInc" || 
				CurWord == "VerticalRotationAxisInc" ||
				CurWord == "SpindleRotationAxisInc")
			{
				CurWord.Delete(CurWord.GetLength() - 3, 3);
				const BOOL BufAC = State.AbsoluteCoord;
				State.AbsoluteCoord = FALSE;
				if(!ProcessCoordRot(text, CurWord))
					return NCUnitStatePrev::ECR_ERROR; // Error defined in ProcessCoordRot
				State.AbsoluteCoord = BufAC;
				continue;
			}
			if(	CurWord == "HorizontalRotationAxisAbs" || 
				CurWord == "VerticalRotationAxisAbs" ||
				CurWord == "SpindleRotationAxisAbs")
			{
				CurWord.Delete(CurWord.GetLength() - 3, 3);
				const BOOL BufAC = State.AbsoluteCoord;
				State.AbsoluteCoord = TRUE;
				if(!ProcessCoordRot(text, CurWord))
					return NCUnitStatePrev::ECR_ERROR; // Error defined in ProcessCoordRot
				State.AbsoluteCoord = BufAC;
				continue;
			}
			if(IsAddCoordWord(CurWord))
			{
				// IsAddCoordWord method cuts "AddCoord" out from CurWord
				if(!ProcessAddCoord(text, CurWord))
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
 			if(CurWord == "DrillRepeatCycles")
			{
				cadrID p;
				int count;
				if(State.CycleStack.Pop(p, count)>=0)
				{
					State.CycleStack.Push(p, count);
					if(	State.CadrID == p)
					{// Ignore Repeat number
						int buf;
						if(!GetIValue(text, &buf))
						{
							if (StoreError(1031))
								return NCUnitStatePrev::ECR_ERROR;
						}
						if(State.CycleStack.Pop(p,count)<0)
						{
							continue;
						}
						count--;
						if(count > 0)// Cycle is not over
						{
							State.OrderChanged = true;
							State.NextCadr = p;
							State.CycleStack.Push(p, count);
						}
						continue;
					}
				}
				if(!GetIValue(text, &count))
				{
					Error = 1032;// level 1
					return NCUnitStatePrev::ECR_ERROR;//error
				}
				p = State.GetCurCadrID();
				if(State.CycleStack.Push(p, count) != 0)
				{
					Error = 1033;// level 1
					return NCUnitStatePrev::ECR_ERROR;//error
				}
				if(State.CycleStack.Pop(p, count)<0)
				{
					continue;
				}
				count--;
				if(count > 0)// Cycle is not over
				{
					State.OrderChanged = true;
					State.NextCadr = p;
					State.CycleStack.Push(p, count);
				}
				continue;
			}
			if(CurWord == "RepeatNumber")
			{
				double dbuf;
				if(!GetDIValue(text, &dbuf))
				{
					if (StoreError(1034))
						return NCUnitStatePrev::ECR_ERROR;
					else
						continue;
				}
				const int Number = int(dbuf + 0.5);
				if(State.ActiveCopyStarting)
				{
					cadrID p = State.CadrID;
					int buf;
					if(State.CallStack.Pop(p, buf) != 0)
					{
						Error = 1022;// level 1
						return NCUnitStatePrev::ECR_ERROR;
					}
					if(State.CallStack.Push(p, Number) != 0)
					{
						Error = 1022;// level 1
						return NCUnitStatePrev::ECR_ERROR;
					}
				}
				else
				{
					State.AutoCycleCount = max(1, Number);
					cadrID p;
					int count;
					if(State.CycleStack.Pop(p,count)>=0)
					{
						State.CycleStack.Push(p, count);
						if(	State.CadrID  == p)
						{// Ignore Repeat number
							continue;
						}
					}
					count = State.AutoCycleCount;
		//????			count++;// Ones more than H param
					p = State.GetCurCadrID();
					if(State.CycleStack.Push(p,count) != 0)
					{
						if (StoreError(1036))
							return NCUnitStatePrev::ECR_ERROR;
					}
				}
				continue;
			}
			if(CurWord == "FunctionCode")// G-code
			{
				if(!ProcessFunction(text))
					return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessFunction
				continue;
			}
			if(CurWord == "MiscFunctionCode")// M-code
			{
				if(!ProcessMiscFunction(text))
					return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessMiscFunction
				if (State.MachLink.IsIndW())
				{
					if (!ProcessConfChange(text, ResetMatrs))
						return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessConfChange
				}
				continue;
			}
			if(CurWord == "Register")// Register
			{
				if(!ProcessRegister(text))
					return NCUnitStatePrev::ECR_ERROR;////Error defined in ProcessRegister
				continue;
			}
			if(CurWord == "ProgramStartEnd")
			{
				CString val;
				bool SkipSubroutine = false;
				if(GetOtherValue("SkipSubroutine", val))
					if(val == "Yes")
						SkipSubroutine = true;
				if((!St.OrderChanged || State.CallStack.IsEmpty()) && SkipSubroutine)
				{
					if(!GoToSubEndNext())
						return NCUnitStatePrev::ECR_ERROR;
					text = "";
					continue;
				}
				State.ProgramName = text;
				text = "";
				continue;
			}
			if(CurWord == "ProgramNumber")
			{
				if(!GetTValue(text, &State.ProgramNumber))
				{
					if (StoreError(1037))
						return NCUnitStatePrev::ECR_ERROR;
				}
				if (State.ProgramNumber.IsEmpty())
				{
					Error = 1068;// level 1
					return NCUnitStatePrev::ECR_ERROR;
				}
				continue;
			}
			if(CurWord == "ProgramNumberStore")
			{
				if(!GetTValue(text, &State.ProgramNumberStore))
				{
					if (StoreError(1037))
						return NCUnitStatePrev::ECR_ERROR;
				}
				if (State.ProgramNumberStore.IsEmpty())
				{
					Error = 1068;// level 1
					return NCUnitStatePrev::ECR_ERROR;
				}
				continue;
			}
			if (CurWord == "GlobalDwell")
			{
				if (!GetDIValue(text, &State.GlobalDwell))
				{
					if (StoreError(1038))
						return NCUnitStatePrev::ECR_ERROR;
				}
				continue;
			}
			if (CurWord == "GlobalDwellSec")
			{
				if (!GetDIValue(text, &State.GlobalDwell))
				{
					if (StoreError(1038))
						return NCUnitStatePrev::ECR_ERROR;
				}
				double step = DWELL_STEP;
				CString Sstep;
				if (GCD.SOthers.Lookup("DwellTimeDiscr", Sstep))
					step = atoi(Sstep) * 0.001;
				State.GlobalDwell /= step;
				continue;
			}
			if(CurWord == "DefineCycle")
			{
				if(!GetIValue(text, &State.DefinedCycleNum))
				{
					Error = 1111;// level ???
					return NCUnitStatePrev::ECR_ERROR;//error
				}
				continue;
			}
			if(CurWord == "NoMove")
			{
				++State.NonExecCount;
				continue;
			}
			if(CurWord == "FeedCode" || CurWord == "FeedCodeNM")
			{
				double Val;
				if (!GetDValue(text, &Val, _T("FeedDotDecimalConversion")))
				{
					if (StoreError(1039))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					State.Feed = Val;
					CString str;
					BOOL CodedFeed = FALSE;
					if (GCD.SOthers.Lookup("CodedFeed", str))
						CodedFeed = (str == "Yes");
					if (CodedFeed)
						State.Feed = DecodeFeed(State.Feed);
					State.Feed *= GetFeedScale();
					if (State.NonExecCount != 0)
					{
						CString Fl;
						if (GCD.SOthers.Lookup("FeedInLine", Fl))
							if (Fl == "Yes")
								State.NonExecCount++;
					}
					ProcContextFeed();
					if (CurWord == "FeedCodeNM")
						State.FeedNonModal = true;
				}
				continue;
			}
			if(CurWord == "SpeedCodeTurn")
			{
				if(SetSpindleSpeed(text, SPINDLE_TURN) < 0)
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			if(CurWord == "SpeedCodeMill")
			{
				if(SetSpindleSpeed(text, SPINDLE_MILL) < 0)
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			if(CurWord == "SpeedCode")
			{
				CString Btext = text;
				int res = SetSpindleSpeed(text, SPINDLE_MILL);
				if(res < 0)
					return NCUnitStatePrev::ECR_ERROR;
				res = SetSpindleSpeed(Btext, SPINDLE_TURN);
				if(res < 0)
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			if(CurWord == "PunchStep")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1041))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				else
				{
					State.CurPunchStep = Val;
					State.CurPunchStep *= GCD.LinearScale;
				}
				continue;
			}
			if(CurWord == "ToolCode")
			{
				if (!ProcessToolCode(text))
					return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessToolCode
				CString NeedM06 = "No";
				CString PreSelect = "No";
				if (!GCD.SOthers.Lookup("ToolChangeWithM06", NeedM06))
					NeedM06 = "No";
				
				if (!GCD.SOthers.Lookup("ToolPreSelect", PreSelect))
					PreSelect = "No";
				if(NeedM06 != "Yes")
				{
					if(PreSelect == "Yes")
					{
						if(State.NextToolNum >= 0)
							State.CurToolN.Set(State.NextToolNum, State.CurTurret);
						State.NextToolNum = State.ThisCadrToolN;
					}
					else // Don't PreSelect
					{
						State.CurToolN.Set(State.ThisCadrToolN, State.CurTurret);
					}
				}

				continue;
			}
			if(CurWord == "TurretNum")
			{
				if(!GetIValue(text,&State.CurTurret))
				{
					if (StoreError(1003))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				continue;
			}
			if(CurWord == "LineNumber")// May be program copy
			{
				if(!GetIValue(text,&State.CurCadrNum))
				{
					if (StoreError(1026))
						return NCUnitStatePrev::ECR_ERROR;//error
					else
						State.CurCadrNum = 1;
				}
				continue;
			}
			if(CurWord == "ProgramCopyChar")
			{
				int StartNum = State.CurCadrNum;
				if(StartNum < 0)
				{
					if (StoreError(1187))
						return NCUnitStatePrev::ECR_ERROR;//error
					StartNum = 0;
				}
				int EndNum = 0;
				if(!GetIValue(text, &EndNum))
				{
					if (StoreError(1187))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				cadrID p;
				const int count = 1;
				p = State.CadrID;
				++p.Cadr;
				if(State.CallStack.Push(p,count) != 0)
				{
					Error = 1022;// level 1
					return NCUnitStatePrev::ECR_ERROR;//error
				}
				cadrID tmp;
				if(!FindCopy(StartNum,&tmp))
					return NCUnitStatePrev::ECR_ERROR;// Error defined in FindCopy
				State.ActiveCopyEnd = EndNum;
				State.OrderChanged = true;
				State.NextCadr = tmp;
				// Process repeat number
				CString RepNum;
				int nbuf = 1;
				double dbuf = 1.;
				if(GCD.SWordDef.Lookup("RepeatNumber",RepNum))
					if(text.Left(1) == RepNum)
					{
						text.Delete(0,1);
						if(!GetDIValue(text,&dbuf))
						{
							Error = 1048;// level 1
							return NCUnitStatePrev::ECR_ERROR;//error
						}
					}
				nbuf = int(dbuf + 0.5);
				if(State.CallStack.Push(tmp,nbuf) !=0)
					return NCUnitStatePrev::ECR_ERROR;

				continue;
			}
			if(CurWord == "LengthCompOffset")
			{
				double dn;
				int n = 0;
				if(!GetDIValue(text,&dn))
				{
					if (StoreError(1049))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				else
				{
					n = (int)(dn + 0.5);
					ProcLengthCompOffset(n);
				}
				continue;
			}
			if(CurWord == "TurnRoughDepthD")
			{
				if(State.CurCycle == TURNROUGH)
					if( ProcRoughDParam(text) < 0)
						return NCUnitStatePrev::ECR_ERROR;
				if(State.CurCycle == TURNLOOP)
					if( ProcLoopDParam(text) < 0)
						return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			if(CurWord == "TurnConeX")
			{
				if( ProcThreadRParam(text) < 0)
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			if(CurWord == "TurnConeZ")
			{
				if( ProcThreadRParam(text) < 0)
					return NCUnitStatePrev::ECR_ERROR;
				continue;
			}
			if(CurWord == "TurnThreadQ")
			{
				if(State.CurX != 0 || State.CurZ != 0)
				{// Second cadr
					double val;
					GetDValue(text, &val);
					State.SetRegister(9900, val * GCD.LinearScale);
				}
				else
				{//First cadr
					double val;
					GetDValue(text, &val);
					State.SetRegister(5140, val * GCD.LinearScale);
				}
				continue;
			}
			if(CurWord == "TurnThreadR")
			{
				if(State.CurX != 0 || State.CurZ != 0)
				{// Second cadr
					if( ProcThreadRParam(text) < 0)
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{//First cadr
					double val;
					GetDValue(text, &val);
					State.SetRegister(5141, val * GCD.LinearScale);
				}
				continue;
			}
			if(CurWord == "TurnThreadP")
			{
				if(State.CurX != 0 || State.CurZ != 0)
				{// Second cadr
					double val;
					GetDValue(text, &val);
					State.SetRegister(9901, val * GCD.LinearScale);
				}
				else
				{//First cadr
					int val;
					GetIValue(text, &val);
					State.SetRegister(5142, int(val / 10000)); //First 2 digits from 6
				}
				continue;
			}
			if(CurWord == "CycleDwellTime")
			{
				double dbuf;
				if(!GetDIValue(text, &dbuf))
				{
					if (StoreError(1072))
						return NCUnitStatePrev::ECR_ERROR;//error
					else
						dbuf = 0.;
				}
				State.CycleDwellTime = int(dbuf + 0.5);
				continue;
			}
			if(CurWord == "DiaCompOffset")
			{
				if(State.CurCycle == TURNROUGH)
				{
					if( ProcRoughDParam(text) < 0)
						return NCUnitStatePrev::ECR_ERROR;
					continue;
				}
				if(State.CurCycle == TURNLOOP)
				{
					if( ProcLoopDParam(text) < 0)
						return NCUnitStatePrev::ECR_ERROR;
					continue;
				}
				double dn = 0.;
				if(!GetDIValue(text, &dn))
				{
					if (StoreError(1050))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				else
				{
					
					int n = (int)(dn + 0.5);
					ProcDiaCompOffset(n);
				}
				continue;
			}
			if(CurWord == "DiaCompOffsetVal")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1051))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					State.CurDiamComp = val * GCD.LinearScale;
				}
				continue;
			}
			if(CurWord == "DiaCompCodeVal")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1051))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					State.CurCodeComp = val;
				}
				continue;
			}
			if(CurWord == "CoordComp")
			{
				if(!GetIValue(text,&State.CoordCompCode))
				{
					if (StoreError(1054))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				continue;
			}
			if(CurWord == "DiaCompHorizVec")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1052))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					State.CurDiaCompHorVec = val * GCD.LinearScale;
				}
				continue;
			}
			if(CurWord == "DiaCompVerticalVec")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					State.CurDiaCompVerVec = val * GCD.LinearScale;
				}
				continue;
			}
			if(CurWord == "ThreadParm")
			{
				if(!GetIValue(text, &State.CycleThreadParm))
				{
					if (StoreError(1054))
						return NCUnitStatePrev::ECR_ERROR;//error
				}
				continue;
			}
			if(CurWord == "CommentStart")
			{
				CString b;
				GCD.SWordDef.Lookup("CommentEnd", b);
				text.Delete(0, text.SpanExcluding(b).GetLength());
				if(text.GetLength() > 0)
					text.Delete(0);
				continue;
			}
			if(CurWord == "ProgramBeginEnd")
			{
				text = "";
				continue;
			}
			if(CurWord == "TurnRoughDepthR")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.SetRegister(9001, val * GCD.LinearScale);
				continue;
			}
			if(CurWord == "TurnRoughDepthR2")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.SetRegister(4322, val * GCD.LinearScale);
				continue;
			}
			if(CurWord == "TurnGroovDepthP")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.SetRegister(9000, val * GCD.LinearScale);
				continue;
			}
			if(CurWord == "TurnGroovDepthQ")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.SetRegister(4321, val * GCD.LinearScale);
				continue;
			}
			if(CurWord == "TurnRoughDepthW1")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.SetRegister(5136, val * GCD.LinearScale);
				continue;
			}
			if(CurWord == "TurnRoughDepthU1")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1053))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.SetRegister(5135, val * GCD.LinearScale);
				continue;
			}
			if(CurWord == "AdditionalTextStart")
			{
				State.EndProgram = true;
				continue;
			}
			if(CurWord == "ChamferAngle")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1100))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					State.CurAngle = Val;
					State.CurAngleDefined = true;
				}
				continue;
			}

			if(	CurWord == "PeckIncrementDepth")
			{
				if(!SetVal2Reg(text, 222))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(	CurWord == "DrillRef")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1014))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					val *= GCD.LinearScale;
					State.SetRegister(201, val);
				}
				continue;
			}

			if(CurWord == "DrillRefRad")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1014))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					val *= GCD.LinearScale / (GCD.XScale * State.CurXScale);
					if (State.AbsoluteCoord)
						val += State.CurBase.z0;
					State.SetRegister(201, val);
				}
				continue;
			}
			if(CurWord == "DrillRelR")
			{
				if(!SetVal2Reg(text, 201))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "DrillRelZ")
			{
				if(!SetVal2Reg(text, 203))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "DrillRelW")
			{
				if(!SetVal2Reg(text, 204))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "DrillRelX")
			{
				if(!SetVal2Reg(text, 205))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillCircU")
			{
				if(!SetVal2Reg(text, 223))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillRectU")
			{
				if(!SetVal2Reg(text, 218))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillRectV")
			{
				if(!SetVal2Reg(text, 219))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillR")
			{
				if(!SetVal2Reg(text, 203))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillZ")
			{
				if(!SetVal2Reg(text, 201))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MccZ")
			{
				if(!SetVal2Reg(text, 1))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MccJ")
			{
				if(!SetVal2Reg(text, 3))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MccR")
			{
				if(!SetVal2Reg(text, 5))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MccV")
			{
				if(!SetVal2Reg(text, 7))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MccK")
			{
				if(!SetVal2Reg(text, 10))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MccQ")
			{
				if(!SetVal2Reg(text, 15))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillQ")
			{
				if(!SetVal2Reg(text, 9200))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillK")
			{
				if(!SetVal2Reg(text, 202))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillP")
			{
				if(!SetVal2Reg(text, 370))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillI")
			{
				if(!SetVal2Reg(text, 368))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillJ")
			{
				if(!SetVal2Reg(text, 369))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillJ1")
			{
				if(!SetVal2Reg(text, 342))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillW")
			{
				if(!SetVal2Reg(text, 204))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillCR")
			{
				if(!SetVal2Reg(text, 220))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillO")
			{
				if(!SetVal2Reg(text, 224))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillL")
			{
				if(!SetVal2Reg(text, 366))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillE")
			{
				if(!SetVal2Reg(text, 9202))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "MillCD")
			{
				if(!SetVal2Reg(text, 9203))
					return NCUnitStatePrev::ECR_ERROR;// error
				continue;
			}
			if(CurWord == "HhLen")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1103))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.HhLen = Val;
				continue;
			}
			if(CurWord == "HhCca")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1104))
						return NCUnitStatePrev::ECR_ERROR;
				}
				continue;
			}
			if(CurWord == "HhR")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1105))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.HhR = Val;
				continue;
			}
			if(CurWord == "ChamferSize")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1113))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					if (State.Chamfer.GetType() != CHAMFER)
					{
						if (StoreError(1113))
							return NCUnitStatePrev::ECR_ERROR;
					}
					else
						State.Chamfer.SetSize(val);
				}
				continue;
			}
			if(CurWord == "RoundSize")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1113))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					if (State.Chamfer.GetType() != ROUND)
					{
						if (StoreError(1113))
							return NCUnitStatePrev::ECR_ERROR;
					}
					else
						State.Chamfer.SetSize(val);
				}
				continue;
			}
			if(CurWord == "DraftAngle")
			{
				double val;
				if (!GetDValue(text, &val))
				{
					if (StoreError(1133))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
				{
					if (St.DraftCode.IsAngleDefined() && !St.DraftCode.IsAngleActive())
						continue;// Angle was processed in previous cadr
					State.DraftCode.SetAngle(val);
				}
				continue;
			}
			//	Travel Limits processing
			if(CurWord == "HorizontalAxisMin")
			{
				if(!SetLimit(0, TL_NEGATIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "VerticalAxisMin")
			{
				if(!SetLimit(1, TL_NEGATIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "SpindleAxisMin")
			{
				if(!SetLimit(2, TL_NEGATIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "HorizontalRotationAxisMin")
			{
				if(!SetLimit(3, TL_NEGATIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "VerticalRotationAxisMin")
			{
				if(!SetLimit(4, TL_NEGATIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "SpindleRotationAxisMin")
			{
				if(!SetLimit(5, TL_NEGATIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "HorizontalAxisMax")
			{
				if(!SetLimit(0, TL_POSITIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "VerticalAxisMax")
			{
				if(!SetLimit(1, TL_POSITIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "SpindleAxisMax")
			{
				if(!SetLimit(2, TL_POSITIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "HorizontalRotationAxisMax")
			{
				if(!SetLimit(3, TL_POSITIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "VerticalRotationAxisMax")
			{
				if(!SetLimit(4, TL_POSITIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "SpindleRotationAxisMax")
			{
				if(!SetLimit(5, TL_POSITIVE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "HorizontalAxisAct")
			{
				if(!SetLimit(0, TL_ACTIVATE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "VerticalAxisAct")
			{
				if(!SetLimit(1, TL_ACTIVATE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "SpindleAxisAct")
			{
				if(!SetLimit(2, TL_ACTIVATE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "HorizontalRotationAxisAct")
			{
				if(!SetLimit(3, TL_ACTIVATE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "VerticalRotationAxisAct")
			{
				if(!SetLimit(4, TL_ACTIVATE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "SpindleRotationAxisAct")
			{
				if(!SetLimit(5, TL_ACTIVATE, text))
					return NCUnitStatePrev::ECR_ERROR;//error code defined in SetLimit
				continue;
			}
			if(CurWord == "CParPIT")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.PIT = Val;
				continue;
			}
			if(CurWord == "CParMPIT")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.MPIT = Val;
				continue;
			}
			if(CurWord == "CParSPL")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.SPL = Val;
				continue;
			}
			if(CurWord == "CParFPL")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.FPL = Val;
				continue;
			}
			if(CurWord == "CParDM1")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.DM1 = Val;
				continue;
			}
			if(CurWord == "CParDM2")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.DM2 = Val;
				continue;
			}
			if(CurWord == "CParAPP")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.APP = Val;
				continue;
			}
			if(CurWord == "CParROP")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.ROP = Val;
				continue;
			}
			if(CurWord == "CParTDEP")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.TDEP = Val;
				continue;
			}
			if(CurWord == "CParFAL")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.FAL = Val;
				continue;
			}
			if(CurWord == "CParIANG")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.IANG = Val;
				continue;
			}
			if(CurWord == "CParNSP")
			{
				double Val;
				if (!GetDValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.NSP = Val;
				continue;
			}
			if(CurWord == "CParNRC")
			{
				int Val;
				if (!GetIValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.NRC = Val;
				continue;
			}
			if(CurWord == "CParNID")
			{
				int Val;
				if (!GetIValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.NID = Val;
				continue;
			}
			if(CurWord == "CParVARI")
			{
				int Val;
				if (!GetIValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.VARI = Val;
				continue;
			}
			if(CurWord == "CParNUMT")
			{
				int Val;
				if (!GetIValue(text, &Val))
				{
					if (StoreError(1142))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CPar.NUMT = Val;
				continue;
			}
			if(CurWord == "DrillMPI" || CurWord == "DrillMPJ" || CurWord == "DrillMPK" || CurWord == "DrillMPK1" )
			{
				double buf;
				if (!GetDValue(text, &buf))
				{
					if (StoreError(1014))
						return NCUnitStatePrev::ECR_ERROR;
				}
				continue;
			}
			if(CurWord == "CircleFullTurns")
			{
				double dn;
				if (!GetDIValue(text, &dn))
				{
					if (StoreError(1150))
						return NCUnitStatePrev::ECR_ERROR;
				}
				else
					State.CircleFullTurns = (int)(dn + 0.5);
				continue;
			}

			if(CurWord == "PlaceToolAxes")
			{
				int Val = 2;
				if (!GetIValue(text, &Val))
				{
					if (StoreError(1029))
						return NCUnitStatePrev::ECR_ERROR;
				}
				if (Val < 1 || Val > 3)
				{
					if (StoreError(1147))
						return NCUnitStatePrev::ECR_ERROR;
				}
				switch(Val)
				{// 1 - AB, 2 - BC, 3 - AC
				case 1:
					State.PlaceToolAxes = RCP_AB;
					break;
				case 2:
					State.PlaceToolAxes = RCP_BC;
					break;
				case 3:
					State.PlaceToolAxes = RCP_AC;
					break;
				}
				continue;
			}

			if(CurWord == _T("LabelLast"))
			{
				CString CLast;
				GCD.SWordDef.Lookup(_T("LabelLast"), CLast);
				CString CFirst;
				GCD.SWordDef.Lookup(_T("LabelFirst"), CFirst);
				if(CFirst == CLast)
					CurWord = _T("LabelFirst");
				// Continue not needed
			}
			if(CurWord == _T("LabelFirst"))
			{
				int Val;
				if(!GetLValue(text, &Val))
				{
					Error = 1149;// level 1
					return NCUnitStatePrev::ECR_ERROR;
				}
				if(Val == State.ActiveCopyEnd)
				{// Program copy is active and current cadr is the last cadr of copy
					if (!ProgCopyLC())
					{
						Error = 1001;// level 0
						return NCUnitStatePrev::ECR_ERROR;
					}
				}
				continue;
			}
			if (CurWord == _T("VariablePrefix"))
			{
				ProcessVariable(text, false);
				continue;
			}
			if (CurWord == _T("VariablePrefixLoc"))
			{
				ProcessVariable(text, true);
				continue;
			}

			/// This part mast be the last part! ///////////
			// The corresponding if(State.CurCycle == CYCLE_OFF) is placed upper
			if(State.CurCycle != CYCLE_OFF) // Cycle is active
			{
				
				if(!ProcessCycleParam(text,CurWord))
					return NCUnitStatePrev::ECR_ERROR;//Error defined in ProcessCycleParam

				continue;
			}
			ProcessText(text);
		}// main cycle
		if (State.ChangedCoord.byte != 0)
		{
			if (State.TouchCadr)
			{
				if (StoreError(1069))
					return NCUnitStatePrev::ECR_ERROR;
			}
			if (State.PolarInterp.IsDeactivating())
			{
				State.PolarInterp.Deactivated();
			}
		}
		if (State.IsChamfCoordStored())
		{
			if (State.ChangedCoord.byte != 0)
				State.ChamfCoord2Prev(St);
			else
				State.SwapChamfCoord(); // We should do swap to compensate swap in the beginnig if cadr has no movement
		}
		// Process MirrorRotate
		if(State.MirrorRotate.IsDefined())
		{
			CString MRmode;
			if(!GCD.SOthers.Lookup("MirrorRotateType", MRmode))
				MRmode = "Incremental";
			if(MRmode == "Absolute")
			{
				double x = State.MashX;
				double y = State.MashY;
				State.MashX = State.MirrorRotate.GetX(x,y);
				State.MashY = State.MirrorRotate.GetY(x,y);
				x = State.CurI;
				y = State.CurJ;
				State.CurI = State.MirrorRotate.GetX(x,y);
				State.CurJ = State.MirrorRotate.GetY(x,y);
				if(State.MirrorRotate.IsMirror())
				{
					if(State.CurCurveType == cwarc)
						State.CurCurveType = ccwarc;
					else if(State.CurCurveType == ccwarc)
						State.CurCurveType = cwarc;
				}
			}
			else // MRmode == Incremental
			{
				State.MashX -= State.CurX;
				State.MashY -= State.CurY;
				double x = State.CurX;
				double y = State.CurY;
				State.CurX = State.MirrorRotate.GetX(x,y);
				State.CurY = State.MirrorRotate.GetY(x,y);
				State.MashX += State.CurX;
				State.MashY += State.CurY;
				x = State.CurI;
				y = State.CurJ;
				State.CurI = State.MirrorRotate.GetX(x,y);
				State.CurJ = State.MirrorRotate.GetY(x,y);
				if(State.MirrorRotate.IsMirror())
				{
					if(State.CurCurveType == cwarc)
						State.CurCurveType = ccwarc;
					else if(State.CurCurveType == ccwarc)
						State.CurCurveType = cwarc;
				}
			}
		}// End MirrorRotate

		if(State.PlaceTool)
		{
			if(!ProcessPlaceTool())
				return NCUnitStatePrev::ECR_ERROR;
		}
		if (State.MachineCSActive)
		{
			State.RecalcFromMachCS();
		}
		// Calculate Cadr geom
		if(State.NonExecCount == 0)
		{
			// Process polar coord
			if(State.PolarCoordActive)
			{
				if(!State.AbsoluteCoord)
				{
					const double bPD = State.PolarDist;
					const double bPA = State.PolarAngle;
					State.SetPolarCenter(State.CurBase.GetBPoint());
					State.CalcPolarCoord(State.MashX, State.MashY, State.MashZ);
					State.PolarAngle += bPA;
					if(bPD != 0.)
					{
						State.PolarDist = bPD;
						State.SetPolarCenter(BPoint(State.MashX, State.MashY, State.MashZ, 1.));
					}

				}
				else
					State.SetPolarCenter(State.CurBase.GetBPoint());
				State.ApplyPolarCoord();
				State.CurX = State.MashX - St.MashX;
				State.CurY = State.MashY - St.MashY;
				State.CurZ = State.MashZ - St.MashZ;
			}

			// Process draft angle
			if (!(State.CurCurveType == line || State.CurCurveType == fast) && State.DraftCode.IsAngleDefined())
			{
				if (StoreError(1132))
					return NCUnitStatePrev::ECR_ERROR;
				else
					State.DraftCode.ClearAngle();
			}
			if(State.DraftCode.IsAngleDefined())
			{
				if(!St.DraftCode.IsAngleActive())
				{// This EvaluateCadr is not called from GetNextState
					switch(State.ChangedCoord.byte & 0x07)
					{
					case 0x00:// Nothing changed
						{
							State.DraftCode.SetAngleActive();
							NCUnitState NextState;
							if(!GetNextState(NextState))
							{
								Error = 1137;// level ???
								return NCUnitStatePrev::ECR_ERROR;
							}
							State.DraftCode.ClearAngleActive();
							if(!NextState.DraftCode.IsAngleDefined())
							{
								Error = 1137;// level ???
								return NCUnitStatePrev::ECR_ERROR;
							}

							const double Eps = GetScale("DraftAngleTol");
							const double Angle = State.DraftCode.GetAngle();
							const double NextAngle = NextState.DraftCode.GetAngle();
							if(fabs(NextAngle - Angle) < Eps || fabs(fabs(NextAngle - Angle) - 180.) < Eps)
							{
								Error = 1131;// level ???
								return NCUnitStatePrev::ECR_ERROR;
							}
							double dX1 = NextState.MashX - State.MashX;
							double dZ1 = NextState.MashZ - State.MashZ;
							switch(State.CurPlane)
							{
							case XY:
								if((NextState.ChangedCoord.byte & 0x07) != 0x03)
								{
									Error = 1137;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								dX1 = NextState.MashY - State.MashY;
								dZ1 = NextState.MashX - State.MashX;
								break;
							case XZ:
								if((NextState.ChangedCoord.byte & 0x07) != 0x05)
								{
									Error = 1137;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								dX1 = NextState.MashX - State.MashX;
								dZ1 = NextState.MashZ - State.MashZ;
								break;
							case YZ:
								if((NextState.ChangedCoord.byte & 0x07) != 0x06)
								{
									Error = 1137;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								dX1 = NextState.MashY - State.MashY;
								dZ1 = NextState.MashZ - State.MashZ;
								break;
							}
							const double dZ = (dX1 - dZ1 * tan(NextAngle * PI / 180.)) / (tan(Angle * PI / 180.) - tan(NextAngle * PI / 180.));
							const double dX = dZ * tan(Angle * PI / 180.);
							switch(State.CurPlane)
							{
							case XY:
								State.CurX = dZ;
								State.CurY = dX;
								State.MashX = St.MashX + State.CurX;
								State.MashY = St.MashY + State.CurY;
								break;
								break;
							case XZ:
								State.CurX = dX;
								State.CurZ = dZ;
								State.MashX = St.MashX + State.CurX;
								State.MashZ = St.MashZ + State.CurZ;
								break;
							case YZ:
								State.CurY = dX;
								State.CurZ = dZ;
								State.MashY = St.MashY + State.CurY;
								State.MashZ = St.MashZ + State.CurZ;
								break;
							}
						}
						break;
					case 0x01:// X changed
						switch(State.CurPlane)
						{
						case XY:
							{
								const double Angle = State.DraftCode.GetAngle();
								const double Eps = GetScale("DraftAngleTol");
								if(fabs(90. - fabs(Angle)) < Eps || fabs(270. - fabs(Angle)) < Eps)
								{
									Error = 1134;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								State.CurY = State.CurX * tan(Angle * PI / 180.);
								State.MashY = St.MashY + State.CurY;
							}
							break;
						case XZ:
							{
								const double Angle = State.DraftCode.GetAngle();
								const double Eps = GetScale("DraftAngleTol");
								if(fabs(Angle) < Eps || fabs(180. - fabs(Angle)) < Eps)
								{
									Error = 1135;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								State.CurZ = State.CurX / tan(Angle * PI / 180.);
								State.MashZ = St.MashZ + State.CurZ;
							}
							break;
						case YZ:
							Error = 1129;// level ???
							return NCUnitStatePrev::ECR_ERROR;
						}
						State.DraftCode.ClearAngle();
						break;
					case 0x02:// Y changed
						switch(State.CurPlane)
						{
						case XY:
							{
								const double Angle = State.DraftCode.GetAngle();
								const double Eps = GetScale("DraftAngleTol");
								if(fabs(Angle) < Eps || fabs(180. - fabs(Angle)) < Eps)
								{
									Error = 1134;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								State.CurX = State.CurY / tan(Angle * PI / 180.);
								State.MashX = St.MashX + State.CurX;
							}
							break;
						case XZ:
							Error = 1129;// level ???
							return NCUnitStatePrev::ECR_ERROR;
						case YZ:
							{
								const double Angle = State.DraftCode.GetAngle();
								const double Eps = GetScale("DraftAngleTol");
								if(fabs(Angle) < Eps || fabs(180. - fabs(Angle)) < Eps)
								{
									Error = 1136;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								State.CurZ = State.CurY / tan(Angle * PI / 180.);
								State.MashZ = St.MashZ + State.CurZ;
							}
							break;
						}
						State.DraftCode.ClearAngle();
						break;
					case 0x03:// X,Y changed
						Error = 1130;// level ???
						return NCUnitStatePrev::ECR_ERROR;
						break;
					case 0x04:// Z changed
						switch(State.CurPlane)
						{
						case XY:
							Error = 1129;// level ???
							return NCUnitStatePrev::ECR_ERROR;
						case XZ:
							{
								const double Angle = State.DraftCode.GetAngle();
								const double Eps = GetScale("DraftAngleTol");
								if(fabs(90. - fabs(Angle)) < Eps || fabs(270. - fabs(Angle)) < Eps)
								{
									Error = 1135;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								State.CurX = State.CurZ * tan(Angle * PI / 180.);
								State.MashX = St.MashX + State.CurX;
							}
							break;
						case YZ:
							{
								const double Angle = State.DraftCode.GetAngle();
								const double Eps = GetScale("DraftAngleTol");
								if(fabs(90. - fabs(Angle)) < Eps || fabs(270. - fabs(Angle)) < Eps)
								{
									Error = 1136;// level ???
									return NCUnitStatePrev::ECR_ERROR;
								}
								State.CurY = State.CurZ * tan(Angle * PI / 180.);
								State.MashY = St.MashY + State.CurY;
							}
							break;
						}
						State.DraftCode.ClearAngle();
						break;
					case 0x05:// X, Z changed
						Error = 1130;// level ???
						return NCUnitStatePrev::ECR_ERROR;
						break;
					case 0x06:// Y, Z changed
						Error = 1130;// level ???
						return NCUnitStatePrev::ECR_ERROR;
						break;
					case 0x07:// X, Y, Z changed
						Error = 1130;// level ???
						return NCUnitStatePrev::ECR_ERROR;
						break;
					}
				}
			}
			else
			{
				if(St.DraftCode.IsAngleActive())
				{// This EvaluateCadr is called from GetNextState
					return NCUnitStatePrev::ECR_ERROR;
				}

			}
			//END: Process draft angle
			if(State.DefinePolarCenter)
			{
				State.SetPolarCenter(BPoint(State.MashX, State.MashY, State.MashZ, 1.));
				State.MashX = St.MashX;
				State.MashY = St.MashY;
				State.MashZ = St.MashZ;
				State.CurX = 0.;
				State.CurY = 0.;
				State.CurZ = 0.;
			}
			else if(State.CurCurveType == line || State.CurCurveType == fast)
			{
				//Fill cadr geom 
				if(State.CurAngleDefined)
				{
					if(!ProcessCurAngle())
					{
						return NCUnitStatePrev::ECR_ERROR;
					}
				}
				if(State.Chamfer.GetType() != OFF)
				{
					struct FDef *pBuf = nullptr;
					const bool ChamferEnabled =
						CheckNext || GCD.SFuncDef.Lookup("DraftRound", pBuf) || GCD.SFuncDef.Lookup("DraftChamfer", pBuf);

					if (ChamferEnabled && !St.DraftCode.IsAngleActive())
					{
						State.Geom.Set(State.CurCurveType,
							State.MashX - State.CurX,
							State.MashY - State.CurY,
							State.MashZ - State.CurZ,
							State.MashX, State.MashY, State.MashZ,
							0., 0., 0., State.CurPlane);
						if (MakeChamfer() != 0)
							return NCUnitStatePrev::ECR_ERROR;
					}
				}
				else
				{
					if(!(State.CurX == 0. && State.CurY == 0. && State.CurZ == 0.))
					{
						if (State.PolarInterp.IsActivating())
						{
							if (!State.ChangedCoord.y)
							{
								State.CurY = -State.MashY;
								State.MashY = State.MashcY = 0.;
							}
							State.PolarInterp.Activated();
						}
						bool OrtChamfer = false;
						if(State.CurR != 0 || State.CurI != 0 || State.CurJ != 0 || State.CurK != 0 )
						{
							CString val;
							if(GetOtherValue("EnableOrtChamfer", val))
							{
								if(val == "Yes")
								{
									if(MakeOrtChamfer() != NCUnitStatePrev::ECR_OK)
										return NCUnitStatePrev::ECR_ERROR;

									OrtChamfer = true;
								}
							}
						}
						if(!OrtChamfer)
						{// Main case
							State.Geom.Set(State.CurCurveType,
										State.MashX - State.CurX,
										State.MashY - State.CurY,
										State.MashZ - State.CurZ,
										State.MashX, State.MashY, State.MashZ,
										0., 0., 0., State.CurPlane);
							if(State.Geom.IsFast() && BFastMoveMan::PartInterp())
							{
								AddState = State;
								BFastMoveMan::MakeCadrs(State.Geom, AddState.Geom);
								if(AddState.Geom.HaveGeom())
								{
									State.Geom.GetE(&State.MashX, &State.MashY, &State.MashZ);
									AddState.Geom.GetE(&AddState.MashX, &AddState.MashY, &AddState.MashZ);
									State.CurX = State.MashX - St.MashX;
									State.CurY = State.MashY - St.MashY;
									State.CurZ = State.MashZ - St.MashZ;
									AddState.CurX = AddState.MashX - State.MashX;
									AddState.CurY = AddState.MashY - State.MashY;
									AddState.CurZ = AddState.MashZ - State.MashZ;
								}
							}
						}
					}
					else
					{
						State.Geom.Set(undef,
							State.MashX - State.CurX,
							State.MashY - State.CurY,
							State.MashZ - State.CurZ,
							State.MashX, State.MashY, State.MashZ,
							0., 0., 0., State.CurPlane);
						if (State.PolarInterp.IsActive() && State.ChangedCoord.byte != 0 && !State.ZeroPreset)
							State.Geom.SetType(State.CurCurveType);// Start and end points 
							//may be transformed with different matrices when PolarInterp is turning on
					}

				}
			}
			else if(State.CurCurveType == cwarc || State.CurCurveType == ccwarc)
			{
				if(State.PlaceTool != PT_EMPTY)
				{
					if (StoreError(1148))
						return NCUnitStatePrev::ECR_ERROR;
				}
				if(State.CurR != 0.)// Radius is defined in this cadr
				{
					if(CreateRArc(St, &State.Geom) != 0)
					{
						State.Geom.Set(line, St.MashX, St.MashY, St.MashZ, State.MashX, State.MashY, State.MashZ);
						if (StoreError(1055))
							return NCUnitStatePrev::ECR_ERROR;
					}
				}
				else//Radius is NOT defined in this cadr
				{
					if(IsArcCenterAbs())
					{
						if(State.ArcCenterFromPolarCenter)
						{
							BPoint C = State.GetPolarCenter();
							State.CurI = C.GetX();
							State.CurJ = C.GetY();
							State.CurK = C.GetZ();
						}
						switch(State.CurPlane)
						{
						case XY:
							State.Geom.Set(State.CurCurveType,
											St.MashX,St.MashY,St.MashZ,
											State.MashX,State.MashY,State.MashZ,
											State.CurI-St.MashX,
											State.CurJ-St.MashY,
											0.,
											State.CurPlane);
							break;
						case XZ:
							State.Geom.Set(State.CurCurveType,
											St.MashX,St.MashY,St.MashZ,
											State.MashX,State.MashY,State.MashZ,
											State.CurI-St.MashX,
											0.,
											State.CurK-St.MashZ,
											State.CurPlane);
							break;
						case YZ:
							State.Geom.Set(State.CurCurveType,
											St.MashX,St.MashY,St.MashZ,
											State.MashX,State.MashY,State.MashZ,
											0.,
											State.CurJ-St.MashY,
											State.CurK-St.MashZ,
											State.CurPlane);
							break;
						}
					}
					else
					{// Incremental mode
						if(State.ArcCenterFromPolarCenter)
						{
							const BPoint C = State.GetPolarCenter();
							State.CurI = C.GetX() - St.MashX;
							State.CurJ = C.GetY() - St.MashY;
							State.CurK = C.GetZ() - St.MashZ;
						}
						if(!(State.CurI == 0. && State.CurJ == 0. && State.CurK == 0.))
						{

							State.Geom.Set(State.CurCurveType,
											St.MashX,St.MashY,St.MashZ,
											State.MashX,State.MashY,State.MashZ,
											State.CurI,State.CurJ,State.CurK,
											State.CurPlane);
							CString type;
							if(GCD.SOthers.Lookup("CircleCenterType",type))
								if(type == "ABSIJ")
									if(!State.Geom.SetABSIJ(State.CurPlane))
									{
										State.Geom.Set(line, St.MashX, St.MashY, St.MashZ, State.MashX, State.MashY, State.MashZ);
										if (StoreError(1056))
											return NCUnitStatePrev::ECR_ERROR;
									}
						}
						else
						{
							if(!(fabs(State.CurX) < MIND && fabs(State.CurY) < MIND && fabs(State.CurZ) < MIND))
							{
								if (StoreError(1089))
									return NCUnitStatePrev::ECR_ERROR;
							}
						}
					}
				}
				State.Geom.SetInterpNum(State.CircleFullTurns); // InterpNum here is a number of the full circles
				if (State.Chamfer.GetType() != OFF)
				{
					struct FDef *pBuf = nullptr;
					const bool ChamferEnabled =
						CheckNext || GCD.SFuncDef.Lookup("DraftRound", pBuf) || GCD.SFuncDef.Lookup("DraftChamfer", pBuf);

					if (ChamferEnabled && !St.DraftCode.IsAngleActive())
						if (MakeChamfer() != 0)
							return NCUnitStatePrev::ECR_ERROR;
				}
			}
			if(State.CurCurveMod != NotDefCM)
			{
				if(ProcessCurCurvMod() < 0)
					return NCUnitStatePrev::ECR_ERROR;
			}
		// End: Calculate Cadr geom

			// CheckGeom
			const double eps = GCD.MinArcDist;
			if(State.Geom.IsArc())
			{
				NCadrGeom GeomForCheck = State.Geom;
				BPoint Pe(GeomForCheck.GetE());
				BPoint Pb(GeomForCheck.GetB());
				BPoint N(GeomForCheck.GetN());
				double d = ((Pe - Pb) * N);
				if(State.AllowSpiral)
				{// project end point to start point plane
					Pe = Pe - N * d;
					GeomForCheck.SetE(Pe);
				}
				else
				{
					if(fabs(d) > MIND)
					{
						CString val;
						if(!GCD.SOthers.Lookup("CheckGeom",val) || val != "No")
						{
							if (StoreError(1176))
								return NCUnitStatePrev::ECR_ERROR;
						}
					}
				}
				const bool ValidGeom = GeomForCheck.IsGeomValid(eps);
				if(!ValidGeom)
				{
					CString Format(_T("%10.4lf"));
					//Format.LoadString(ID...);
					CString val;
					if(!GCD.SOthers.Lookup("CheckGeom",val) || val != "No")
					{
						double R1 = sqrt((GeomForCheck.GetB() - GeomForCheck.GetC()).D2());
						double R2 = sqrt((GeomForCheck.GetE() - GeomForCheck.GetC()).D2());
						TempError.Values[0].Format(Format, R1);
						TempError.Values[1].Format(Format, R2);
						TempError.Values[2].Format(Format, R1 - R2);
						TempError.Values[3].Format(Format, GCD.MinArcDist);
						TempError.SetErrNum(1057);
						if(StoreError(1057))
							return NCUnitStatePrev::ECR_ERROR;
					}
				}
				CString val;
				if (GCD.SOthers.Lookup("CircleInQuadrants", val))
					if (val == "Yes")
						if (State.Geom.CalcQuadsNum() > 1)
						{
							if (StoreError(1159))
								return NCUnitStatePrev::ECR_ERROR;
						}
			}
			if(State.Geom.IsFullArc(eps))
			{
				if(!State.Geom.Is3DArc())
				{
					State.Geom.SetE(State.Geom.GetB());
					State.Geom.GetE(&State.MashX, &State.MashY, &State.MashZ);
					State.CurX = 0.;
					State.CurY = 0.;
					State.CurZ = 0.;
				}
			}
			//Check travel limits
			if(!CheckTravelLimits())
				return NCUnitStatePrev::ECR_ERROR;// Error code is defined in CheckTravelLimits
			// Calculate compensation
			const enum LR tmpComp = State.CurDirComp;
			if(State.CurCycle == TURNCUT || State.CurCycle == TURNFACECUT)
				State.CurDirComp = LR_OFF;// The compensation was processed in cycle definition 
			//First movement
			bool NextGeomFlag = true;
			if(ApprCycleActive())
				NextGeomFlag = false;
			NCadrGeom NextGeom;
			if(State.CurDirComp != LR_OFF && NextGeomFlag) // Включена контурная коррекция
			{
				if(AddState.Geom.HaveGeom())
					NextGeom = AddState.Geom;
				else
					NextGeomFlag = GetNextGeom(NextGeom);// This call must be done before the ParToNext check
			}
			
			if(NextGeomFlag)
				if (ProcessDiamComp(NextGeom) == NCUnitStatePrev::ECR_ERROR)
					return NCUnitStatePrev::ECR_ERROR;
			if (ProcessLengthComp() == NCUnitStatePrev::ECR_ERROR)
				return NCUnitStatePrev::ECR_ERROR;
			if (ProcessDisplComp() == NCUnitStatePrev::ECR_ERROR)
				return NCUnitStatePrev::ECR_ERROR;
			//Second movement
			if(AddState.Geom.HaveGeom())
			{
				auto StB = St;
				State.MashcX -= State.CurToolDisplacement.GetX();
				State.MashcY -= State.CurToolDisplacement.GetY();
				State.MashcZ -= State.CurToolDisplacement.GetZ();
				St = State;
				State = AddState;
				State.CurCurveType = St.CurCurveType;// AddState may be an arc but such an arc should not be modal
				NextGeomFlag = true;
				if(State.CurDirComp != LR_OFF) // Включена контурная коррекция
				{
					NextGeomFlag = GetNextGeom(NextGeom);// This call must be done before the ParToNext check
				}
				if (NextGeomFlag)
					if (ProcessDiamComp(NextGeom) == NCUnitStatePrev::ECR_ERROR)
						return NCUnitStatePrev::ECR_ERROR;
				if (ProcessLengthComp() == NCUnitStatePrev::ECR_ERROR)
					return NCUnitStatePrev::ECR_ERROR;
				if (ProcessDisplComp() == NCUnitStatePrev::ECR_ERROR)
					return NCUnitStatePrev::ECR_ERROR;
				AddState = State;
				State.ReplaceByPrev(St);
				State.CurX += AddState.CurX;
				State.CurY += AddState.CurY;
				State.CurZ += AddState.CurZ;
				State.MashX = AddState.MashX;
				State.MashY = AddState.MashY;
				State.MashZ = AddState.MashZ;
				State.MashcX = AddState.MashcX;
				State.MashcY = AddState.MashcY;
				State.MashcZ = AddState.MashcZ;
				St = StB;
			}
			if(State.CurCycle == TURNCUT || State.CurCycle == TURNFACECUT)
				State.CurDirComp = tmpComp;

		// END: Calculate compensation
			

			if (State.MoveRotateCSCancel)
			{
				State.CurI = St.CurI;
				State.CurJ = St.CurJ;
				State.CurK = St.CurK;
				State.CurX = St.CurX;
				State.CurY = St.CurY;
				State.CurZ = St.CurZ;
				RemoveCSMatr();
				State.Geom.SetType(undef);
				State.CompGeom.SetType(undef);
			}
			if(State.MoveRotateCS || State.MoveRotateCSRel)
			{
				if (State.MoveRotateCS && State.MoveRotateCSRel)
				{
					Error = 1166;// level 1
					return NCUnitStatePrev::ECR_ERROR;
				}
				if (State.ChangedCoord.x == FALSE && State.ChangedCoord.y == FALSE && State.ChangedCoord.z == FALSE)
				{// Rotation center is the base point of the active UCS
					NCBase ActiveBase = State.GetActiveUCS();
					State.MashX = ActiveBase.x0;
					State.MashY = ActiveBase.y0;
					State.MashZ = ActiveBase.z0;
				}
				State.CSMatr = State.MoveRotateCSRel ? State.CSMatr * CalcMRCS() : CalcMRCS();
				State.CurI = St.CurI;
				State.CurJ = St.CurJ;
				State.CurK = St.CurK;
				State.CurX = St.CurX;
				State.CurY = St.CurY;
				State.CurZ = St.CurZ;
				EnableCSMatr();
				State.Geom.SetType(undef);
				State.CompGeom.SetType(undef);
			}
			if(TiltPlane.IsChanging())
			{
				State.CSMatr = TiltPlane.GetCSMatr();
				State.CurI = St.CurI;
				State.CurJ = St.CurJ;
				State.CurK = St.CurK;
				State.CurX = St.CurX;
				State.CurY = St.CurY;
				State.CurZ = St.CurZ;
				EnableCSMatr();
				State.Geom.SetType(undef);
				State.CompGeom.SetType(undef);
				State.Geom.SetType(undef);
				State.CompGeom.SetType(undef);

				BMatr invM1 = State.TiltNewSys.invr();
				BPoint MashNS = BPoint(St.MashX, St.MashY, St.MashZ, 1.) * St.CSMatr * invM1;

				double ResA = 0., ResB = 0., ResC = 0.;
				TiltPlane.GetResAngles(GetFixedDef(), &ResA, &ResB, &ResC);
				int Reg = -1;
				if ((Reg = TiltPlane.GetStartRegNum()) >= 0)
				{
					State.SetRegister(Reg, MashNS.GetX());
					State.SetRegister(Reg + 1, MashNS.GetY());
					State.SetRegister(Reg + 2, MashNS.GetZ());
					State.SetRegister(Reg + 3, ResA);
					State.SetRegister(Reg + 4, ResB);
					State.SetRegister(Reg + 5, ResC);
				}
				State.CurBase.Clear();
				State.CurBase.Visible= true;
			}
			if(State.RotateCSABC)
			{
				BPoint XYZ, ABC;
				pDoc->GetMachTool().GetState(XYZ, ABC, GetFixedDef());
				pDoc->GetMachTool().SetState(State.MachLink, SMachState(BPoint(0., 0., 0., 1.), BPoint(State.MashA, State.MashB, State.MashC, 1.), &GetFixedDef()));
				State.UCSMatr = pDoc->GetMachTool().GetStockMatr(CurStockPosition, true);
				pDoc->GetMachTool().SetState(State.MachLink, SMachState(XYZ, ABC, &GetFixedDef()));
				//State.CurBase.MoveBPoint(State.UCSMatr);
				State.CurBase = State.GetActiveUCS();
				State.MashA = St.MashA;
				State.MashB = St.MashB;
				State.MashC = St.MashC;
				State.Geom.SetType(undef);
				State.CompGeom.SetType(undef);
			}
			// Process FixCutSpeed
			if(State.Spindle[SPINDLE_TURN].FixCutSpeed && State.Spindle[SPINDLE_TURN].SpindleDir != R_UNDEF)
			{
				if(State.Spindle[SPINDLE_TURN].FixedCutSpeed <= 0.)
				{
					Error = 1098;// level ???
					return NCUnitStatePrev::ECR_ERROR;
				}
				const double R = fabs(State.MashcX);
				if( R < State.Spindle[SPINDLE_TURN].FixedCutSpeed / (2. * PI * State.Spindle[SPINDLE_TURN].MaxSpeed))
					State.Spindle[SPINDLE_TURN].Speed = State.Spindle[SPINDLE_TURN].MaxSpeed;
				else
					State.Spindle[SPINDLE_TURN].Speed = State.Spindle[SPINDLE_TURN].FixedCutSpeed / (2. * PI * R);
			}
			if(St.CylInterp.IsActive() && !State.CylInterp.IsActive())
			{// Cyl interpolation is turning off in this cadr
				pDoc->GetMachTool().ProcCadrStep(*this, St);
				BPoint XYZ, ABC;
				pDoc->GetMachTool().GetState(XYZ, ABC, GetFixedDef());
				State.MashX = XYZ.GetX();
				State.MashY = XYZ.GetY();
				State.MashcX = State.MashX;
				State.MashcY = State.MashY;
				State.MashC = ABC.GetZ();
			}
			ApplyRCP(false);
		// TEMPORARY 4X MILLING
			if (State.GrooveON && State.Spindle[SPINDLE_TURN].SpindleDir != R_UNDEF && State.CompGeom.IsLine())
			{// Calculate C according to spindle rotation speed
				if (!(State.CompGeom.IsLine() || State.CompGeom.IsUndef()))
				{
					if (StoreError(1157))
						return NCUnitStatePrev::ECR_ERROR;
				}
				if (State.CompGeom.IsLine())
				{
					State.SetCadrTime(&State.CompGeom);
					if (State.CompGeom.IsTimeDefined())
					{
						double CadrAngle = State.CompGeom.GetWorkTimeS() * State.Spindle[SPINDLE_TURN].CalcSpeed();
						if (CadrAngle > 1000.)
						{
							if (StoreError(1158))
								return NCUnitStatePrev::ECR_ERROR;
						}
						if (State.Spindle[SPINDLE_TURN].SpindleDir == R_CW)
							CadrAngle *= -1;
						State.MashC += CadrAngle * 360.;
					}
				}
			}
			bool Prohibit5xCheck = false;
			if (St.PolarInterp.IsActive() && !State.PolarInterp.IsActive())
			{// Polar interpolation is turning off in this cadr
				Prohibit5xCheck = true;
				BPoint P = BPoint(State.MashX, State.MashY, State.MashZ, 1.);
				BPoint XYZ(0., 0., 0., 1.), ABC(0., 0., 0., 1.);
				State.PolarInterp.MapP2PA(P, XYZ, ABC);
				State.MashX = XYZ.GetX();
				State.MashY = XYZ.GetY();
				State.MashZ = XYZ.GetZ();
				State.MashC = ABC.GetZ() + State.CurBase.c0;
				if (State.PolarInterp.GetFanucStyle())
					State.CurBase = State.PolarInterp.GetStoredBase();
				State.MashcX = State.MashX;
				State.MashcY = State.MashY;
				State.Geom.SetType(undef);
				State.CompGeom = State.Geom;
			}
			else if (State.GetInterp().IsActive())
			{
				if (State.PolarInterp.IsActive())
				{
					if (!St.PolarInterp.IsActive())
					{// Polar interpolation is turning on in this cadr
						if (fabs(State.MashY) > MIND)
						{
							Error = 1167;// level 1
							return NCUnitStatePrev::ECR_ERROR;
						}
						Prohibit5xCheck = true;
						BPoint XYZ(State.MashX, State.MashY, State.MashZ, 1.);
						BPoint ABC(State.MashA - State.CurBase.a0, State.MashB - State.CurBase.b0, State.MashC - State.CurBase.c0, 1.);
						BPoint P;
						State.PolarInterp.MapPA2P(XYZ, ABC, P);
						State.MashX = P.GetX();
						State.MashY = P.GetY();
						State.MashZ = P.GetZ();
						State.MashC = State.CurBase.c0;
						State.MashcX = State.MashX;
						State.MashcY = State.MashY;
					}
					State.SetPolarGeom();
					AddState.SetPolarGeom();
				}
			}
				
			BPoint ABCE(State.MashA, State.MashB, State.MashC, 1.);

			if(St.MashA != State.MashA || St.MashB != State.MashB || St.MashC != State.MashC)
			{
				if(RecalcRotCoords(St, State, ABCE) != 0)// Can change State
					return NCUnitStatePrev::ECR_ERROR;// error set in RecalcRotCoords
			}
			ProcessRCP();

			if (ResetMatrs)
			{
				int MatrNum = 0;
				if (!Prohibit5xCheck && (St.MashA != State.MashA || St.MashB != State.MashB || St.MashC != State.MashC || State.IsRCPActive()))
				{
					//5x

					BCurve* pCurve = nullptr;
					BCurve* pCurveComp = nullptr;
					if (pDoc->GetMachTool().GetCurConfig().IsDefined())
					{
						BPoint ToolBaseComp(0., 0., 0., 1.);
						BPoint ToolBase(0., 0., -GetLengthComp(), 1.);
						const BStockToolInds STInds = GetCurSTInds();
						pCurveComp = pDoc->GetMachTool().Make5xLine(State.RCPMask, *this, St, State, ToolBaseComp, STInds);
						pCurve = pDoc->GetMachTool().Make5xLine(State.RCPMask, *this, St, State, ToolBase, STInds);
					}
					State.Geom.SetCurve(pCurve);
					State.CompGeom.SetCurve(pCurveComp);

					State.Geom.SetType(::rotate);
					State.CompGeom.SetType(::rotate);

					State.SetInterpNum2Geom(State.IsRCPActive() ? 0 : int(BSpacePos::GetMatrsSize() - 1));

					State.Geom.SetN(ABCE);
					State.CompGeom.SetN(ABCE);
					State.Geom.SetI(St.MashA, St.MashB, St.MashC);
					State.CompGeom.SetI(St.MashA, St.MashB, St.MashC);
					State.Geom.SetB(St.MashX, St.MashY, St.MashZ);
					State.CompGeom.SetB(St.MashcX, St.MashcY, St.MashcZ);
					State.Geom.SetE(State.MashX, State.MashY, State.MashZ);
					State.CompGeom.SetE(State.MashcX, State.MashcY, State.MashcZ);

					PrepNewRotMatr(ABCE);
				}
				else if (!Prohibit5xCheck && TiltPlane.IsChanging())
				{
					// TiltPlane

					BCurve* pCurve = nullptr;
					BCurve* pCurveComp = nullptr;
					if (pDoc->GetMachTool().GetCurConfig().IsDefined())
					{
						BPoint ToolBaseComp(0., 0., 0., 1.);
						BPoint ToolBase(0., 0., -GetLengthComp(), 1.);
						const BStockToolInds STInds = GetCurSTInds();
						pCurveComp = pDoc->GetMachTool().Make5xLine(State.RCPMask, *this, St, State, ToolBaseComp, STInds);
						pCurve = pDoc->GetMachTool().Make5xLine(State.RCPMask, *this, St, State, ToolBase, STInds);
					}
					State.Geom.SetCurve(pCurve);
					State.CompGeom.SetCurve(pCurve);

					State.Geom.SetType(::rotate);
					State.CompGeom.SetType(::rotate);

					State.SetInterpNum2Geom(State.IsRCPActive() ? 0 : int(BSpacePos::GetMatrsSize() - 1));

					State.Geom.SetN(ABCE);
					State.CompGeom.SetN(ABCE);
					State.Geom.SetI(St.MashA, St.MashB, St.MashC);
					State.CompGeom.SetI(St.MashA, St.MashB, St.MashC);
					State.Geom.SetB(St.MashX, St.MashY, St.MashZ);
					State.CompGeom.SetB(St.MashcX, St.MashcY, St.MashcZ);
					State.Geom.SetE(State.MashX, State.MashY, State.MashZ);
					State.CompGeom.SetE(State.MashcX, State.MashcY, State.MashcZ);

					PrepNewRotMatr(ABCE);
					if (TiltPlane.Cancelling)
					{// TiltPlane is turning off in this cadr
						State.TiltNewSys.SetE();
						TiltPlane.Cancelling = false;
					}
					BMatr Q = BSpacePos::pLastMT->GetRespMatr(BSpacePos::LastCSMatr, BSpacePos::LastABC, BSpacePos::LastTurret, 0, GetCurStockPosition(), &GetFixedDef());// Ins STInd
					BSpacePos::AddRotMatr(Q, BSpacePos::LastABC);
					MatrNum = BSpacePos::GetLastMatrInd();
				}
				else if (State.MoveRotateCS || State.MoveRotateCSRel || State.MoveRotateCSCancel)
				{
					PrepNewRotMatr(ABCE);
				}
				else if (St.PolarInterp.IsActive() && !State.PolarInterp.IsActive())
				{// Polar interpolation is turning off in this cadr
					PrepNewRotMatr(ABCE);
				}
				else if (TiltPlane.Cancelling)
				{// tilt plane is turning off in this cadr
					PrepNewRotMatr(ABCE);
					State.TiltNewSys.SetE();
					TiltPlane.Cancelling = false;
				}
				else if (State.GetInterp().IsActive())
				{
					int InterpNum = 0;
					if (State.PolarInterp.IsActive())
					{
						if (!St.PolarInterp.IsActive())
						{// Polar interpolation is turning on in this cadr
							PrepNewRotMatr(ABCE);
						}
						InterpNum = BSpacePos::GetLastPolarInd();
						if (BSpacePos::LastMatrUsed == false && BSpacePos::pLastMT)
						{
							BMatr Q = BSpacePos::pLastMT->GetRespMatr(BSpacePos::LastCSMatr, BSpacePos::LastABC, BSpacePos::LastTurret, 0, GetCurStockPosition(), &GetFixedDef());// Ins STInd
							BSpacePos::AddRotMatr(Q, BSpacePos::LastABC);
						}
						MatrNum = BSpacePos::GetLastMatrInd();// Need to be changed for non C-axis polar interp
					}
					else
					{// Cyclindric interp is active
						State.SetCylindricGeom();
						AddState.SetCylindricGeom();
						InterpNum = BSpacePos::GetLastCylInd();
						MatrNum = 0;
					}
					State.SetInterpNum2Geom(InterpNum);
					AddState.SetInterpNum2Geom(InterpNum);
					if (State.CylInterp.IsActive() && pDoc->GetMachTool().GetCurConfig().IsDefined() && State.Geom.HaveGeom())
					{
						MakeCurve4CylInt(State.Geom);
						MakeCurve4CylInt(State.CompGeom);
						MakeCurve4CylInt(State.AddCompGeom);
					}
				}
				else if (State.RCPCanceling)
				{
					PrepNewRotMatr(ABCE);
					BMatr Q = BSpacePos::pLastMT->GetRespMatr(BSpacePos::LastCSMatr, BSpacePos::LastABC, BSpacePos::LastTurret, 0, GetCurStockPosition(), &GetFixedDef());// Ins STInd
						BSpacePos::AddRotMatr(Q, BSpacePos::LastABC);
				}
				else
				{
					if (BSpacePos::LastMatrUsed == false && BSpacePos::pLastMT)
					{
						BMatr Q;
						if (State.IsRCPActive())
							Q.SetE();
						else
							Q = BSpacePos::pLastMT->GetRespMatr(BSpacePos::LastCSMatr, BSpacePos::LastABC, BSpacePos::LastTurret, 0, GetCurStockPosition(), &GetFixedDef());// Ins STInd
						BSpacePos::AddRotMatr(Q, BSpacePos::LastABC);
					}
					MatrNum = BSpacePos::GetLastMatrInd();
				}
				State.SetMatrNum2Geom(MatrNum);
				AddState.SetMatrNum2Geom(MatrNum);
			}// if(ResetMatrs) 

			ProcessRCPCancel();
			if (State.MachLink != St.MachLink)
			{
				BMTLinkageDiff Diff = State.MachLink - St.MachLink;
				State.SetMTConfNum2Geom(Diff);
				AddState.SetMTConfNum2Geom(Diff);
			}


			if(State.CylInterp.IsActive() && !St.CylInterp.IsActive())
			{// Cyl interpolation is turning on in this cadr
				State.MashX = St.MashX;
				State.MashY = 0;
				State.MashcX = State.MashX;
				State.MashcY = State.MashY;
				State.MashC = 0.;
			}
			if(St.CylInterp.IsActive() && !State.CylInterp.IsActive())
			{// Cyl interpolation is turning off in this cadr
				State.Geom.SetType(undef);
				State.CompGeom = State.Geom;
			}
			if(TiltPlane.IsChanging())
			{
				State.Geom.SetType(undef);
				State.CompGeom.SetType(undef);
			}
			State.CurA = State.MashA - St.MashA;
			State.CurB = State.MashB - St.MashB;
			State.CurC = State.MashC - St.MashC;
			State.CalcUCSCoords();
			State.MashX2 = State.MashX * 2.;
			State.CurI2 = State.CurI * 2.;
			if (State.AddCoordsChanged && State.TouchCadr)
			{
				GetChannel().SetTouchPresent();
				ProcessTouch(ResetMatrs);
			}
			if (/*NumAxes > 3 || */ !ResetMatrs)// May be NumAxes check is unnecessary
				pDoc->GetMachTool().ProcCadrStep(*this, State);
		// Process modal call
			if (!State.ModalSubrID.IsEmpty() && State.ModalSubrIDRet.IsEmpty())// Modal call is defined but is not called now
			{
				if (!ProcessModalCall(ResetMatrs))
					return NCUnitStatePrev::ECR_ERROR;
			}
		// Process cycle

			// Cycle init
			if(pCadr->pCycle)
				delete pCadr->pCycle;

			pCadr->pCycle = NULL;
			if(State.CurCycle != CYCLE_OFF)
			{
				auto ErrorMinorBuf = ErrorMinor;
				if (!ProcessCycle(pCadr, ResetMatrs))
					return NCUnitStatePrev::ECR_ERROR;
				if (ErrorMinorBuf != 0)
					ErrorMinor = ErrorMinorBuf;
			}// if ! CYCLE_OFF
			// Calculate time
			if(pDoc->CalcTime)
			{
				if (St.CurToolN != State.CurToolN || St.CurTurret != State.CurTurret)
				{// the tool changed in this cadr
					const NToolCombined *pTool = GetActiveTool();
					double ToolLifeTime = 0.;
					if (pTool)
						ToolLifeTime = pTool->GetLifeTime();
					State.ProcLifeTime4ToolChange(IntToolID(St.CurToolN.GetToolNum(), St.CurTurret), ToolLifeTime);
				}

				if (ResetMatrs)
				{
					SetFlags();
					// calculate by comp geom
					State.IncrementTimer(&State.CompGeom);
					ProcAutoFix(&State.CompGeom);
					if (State.AddCompGeom.HaveGeom())
					{
						const auto b = State.CadrTime;
						State.IncrementTimer(&State.AddCompGeom);
						State.CadrTime += b;
					}
					if(AddState.CompGeom.HaveGeom())
						AddState.IncrementTimer(&AddState.CompGeom);
					if(AddState.AddCompGeom.HaveGeom())
					{
						const auto b = AddState.CadrTime;
						AddState.IncrementTimer(&AddState.AddCompGeom);
						AddState.CadrTime += b;
					}
					State.Time += AddState.CadrTime;
					State.CadrTime += AddState.CadrTime;

					for(int kc = 0; kc < CycleGeom.GetSize(); ++kc)
					{
						if(!CycleGeom[kc]->IsTimeDefined())
							State.IncrementTimer(CycleGeom[kc]);
						else
							State.Time += CycleGeom[kc]->GetWorkTimeS();
						ProcAutoFix(CycleGeom[kc]);
					}
					// Tool change time
					if((St.CurToolN != State.CurToolN)
						|| (St.CurTurret != State.CurTurret))
					{
						CString Val;
						if(GetOtherValue("ToolChangeTime", Val))
						{
							State.CadrTime += atof(Val);
							State.Time += atof(Val);
							State.Geom.SetWorkTimeS(State.CadrTime);
							State.CompGeom.SetWorkTimeS(State.CadrTime);
						}
					}
					//Calculate additional coordinates time
					if(State.AddCoords.GetSize() > 1)
					{
						SMachState Dif = St.AddCoords - State.AddCoords;
						if(!Dif.IsZero())
						{
							double MaxTime = 0.;
							for(int i = 0; i < Dif.GetSize() - 1; ++i)
							{
								MaxTime = max(fabs(Dif.GetCoord(i)) * 60. / AddCoordsFeed.GetCoord(i), MaxTime);
							}
							State.CadrTime = MaxTime;
							State.Time += State.CadrTime;
							State.Geom.SetWorkTimeS(State.CadrTime);
							State.CompGeom.SetWorkTimeS(State.CadrTime);
						}
					}
					State.ProcessLifeTime();
				}
			}
			if (State.CircleFullTurns != 0 && ResetMatrs)
			{
				Make3DArcCycle();
			}
			// Calculate geom params
			if(pDoc->CalcGeom)
			{
				// Calculate angle between two cadrs
				State.CalcAngle(St);
				// Calculate angle between current cadr and coordinate axis
				if(GetChannel().NGetProgramListCn()->HaveCompTraj())
				{// calculate by comp geom
					State.CalcAxisAngles(&State.CompGeom);
				}
				else
				{// calculate by programmed geom
					State.CalcAxisAngles(&State.Geom);
				}
				// Calculate Arc parameters
				if(GetChannel().NGetProgramListCn()->HaveCompTraj())
				{// calculate by comp geom
					State.CalcArcParam(&State.CompGeom);
				}
				else
				{// calculate by programmed geom
					State.CalcArcParam(&State.Geom);
				}
			}

			if(State.AddCompGeom.HaveGeom())
				State.IncrCurHCadr();
			if(AddState.Geom.HaveGeom())
			{
				State.IncrCurHCadr();
				if(AddState.AddCompGeom.HaveGeom())
					State.IncrCurHCadr();
			}
		}
		if(State.AddCoordsChanged)
		{
			if(State.Geom.IsUndef())
			{
				State.Geom.SetType(addcoord); // SetType must be before SetB
				State.CompGeom.SetType(addcoord); // SetType must be before SetB

				State.Geom.SetB(State.MashX, State.MashY, State.MashZ); 
				State.Geom.SetE(State.Geom.GetB()); 
				 
				State.CompGeom.SetB(State.MashcX, State.MashcY, State.MashcZ); 
				State.CompGeom.SetE(State.CompGeom.GetB()); 
			}
		}
	}
	State.SetStockPos2Geom(CurStockPosition);
	AddState.SetStockPos2Geom(CurStockPosition);
	SetFlags();
	if (State.Geom.HaveGeom5())
		State.CurZCompChanging = false;

	NCUnitStatePrev::EvaluateCadrResult ret = NCUnitStatePrev::ECR_OK;
	if (ResetMatrs)
	{
		// Check mile stone
		if (pDoc->MileStones.IsMileStone(this, &St))
		{
			ret = NCUnitStatePrev::ECR_BREAK;// Must be before next cadr determination due to bookmarks work properly
		}
	}
	const NMileStone * pStone = pDoc->CheckStones.IsMileStone(this, &St);
	if(pStone)
	{
		if (StoreError(pStone->GetCode()))
			return NCUnitStatePrev::ECR_ERROR;
	}
	// Next cadr
	if(State.OrderChanged)
	{
		SetCurCadr(State.NextCadr);
		if(!State.ModalSubrIDRet.IsEmpty())
		{
			if(State.NextCadr == State.ModalSubrIDRet)
			{
				State.ModalSubrIDRet.Prog = -1;
				State.ModalSubrIDRet.Cadr = 0;
			}
		}
	}
	else
		State.SetCurCadr(State.GetCurCadr() + 1);
	State.IncrCurHCadr();

	if(SaveCoord2Reg)
		Coord2Regs();

	return ret;	
}

NCUnitState BState;
NCUnitState BAddState;
NCUnitStatePrevGeom BGeomSt;
NCUnitStatePrev::EvaluateCadrResult NCUnit::EvaluateCadr(NCadr* pCadr, const NCadr* pNextCadr, bool ResetMatrs)
{
	// GetCurHCadrAbsEx is an index in CompGeomArray
	BState = State;
	BAddState = AddState;
	BGeomSt = St.Geom.HaveGeom() ? St : LastGeomState;

	NCUnitStatePrev::EvaluateCadrResult ret = EvaluateCadrBase(pCadr, pNextCadr, ResetMatrs);

	if (pDoc->ProgErrHandler.GetErrorReaction(ret, Error) == BProgErrHandler::ER_SKIP_LINE)
	{
		Jump2State(BState, BAddState, BGeomSt);
		pCadr->SetErrCode(Error);
		ret = EvaluateCadrBase(pCadr, pNextCadr, ResetMatrs);
		if (!ResetMatrs)
			pCadr->SetErrCode(0);
	}

	return ret;
}

void NCUnit::ProcessRCP(void)
{
	if(!State.IsRCPActive())
		return;
	const auto STinds = GetCurSTInds();
	MTMachineTool::RCPData RCP;
	RCP.Set(State.RCPStartABC);
	RCP.SetRCPMask(State.RCPMask);
	pDoc->GetMachTool().CalcRCPDisp(SMachState(St.GetMachStateFixComp(), &GetFixedDef()), SMachState(State.GetMachStateFixComp(), &GetFixedDef()),  STinds.stPos_, STinds.turret_, STinds.attach_, RCP, GetLengthComp());
	const BPoint& XYZ = RCP.GetCoordD();
	State.MashX += XYZ.GetX();
	State.MashY += XYZ.GetY();
	State.MashZ += XYZ.GetZ();
	State.CurX += XYZ.GetX();
	State.CurY += XYZ.GetY();
	State.CurZ += XYZ.GetZ();
	State.RCPcX = -XYZ.GetX();
	State.RCPcY = -XYZ.GetY();
	State.RCPcZ = -XYZ.GetZ();
	State.MashcX += XYZ.GetX();
	State.MashcY += XYZ.GetY();
	State.MashcZ += XYZ.GetZ();
}

void NCUnit::ProcessRCPCancel(void) noexcept
{
	if (!State.RCPCanceling)
		return;
	State.RCPCanceling = false;
	State.MashX -= State.RCPcX;
	State.MashY -= State.RCPcY;
	State.MashZ -= State.RCPcZ;
	State.MashcX -= State.RCPcX;
	State.MashcY -= State.RCPcY;
	State.MashcZ -= State.RCPcZ;
	State.CurX -= State.RCPcX;
	State.CurY -= State.RCPcY;
	State.CurZ -= State.RCPcZ;
	State.RCPcX = 0.;
	State.RCPcY = 0.;
	State.RCPcZ = 0.;
}

void NCUnit::Coord2Regs(void)
{
	State.SetRegister(8200, State.MashX);
	State.SetRegister(8201, State.MashY);
	State.SetRegister(8202, State.MashZ);
	State.SetRegister(8203, State.MashA);
	State.SetRegister(8204, State.MashB);
	State.SetRegister(8205, State.MashC);
	State.SetRegister(8206, State.MashcX);
	State.SetRegister(8207, State.MashcY);
	State.SetRegister(8208, State.MashcZ);
	//State.SetRegister(8215, State.MashXUCS);
	//State.SetRegister(8216, State.MashYUCS);
	//State.SetRegister(8217, State.MashZUCS);
	SetRegister(State, _T("MTXUCS"), State.MashXUCS, 8215);
	SetRegister(State, _T("MTYUCS"), State.MashYUCS, 8216);
	SetRegister(State, _T("MTZUCS"), State.MashZUCS, 8217);
	int CurveType = 0;
	switch(State.CurCurveType)
	{
	case fast:
	default:
		CurveType = 0;
		break;
	case line:
		CurveType = 1;
		break;
	case cwarc:
		CurveType = 2;
		break;
	case ccwarc:
		CurveType = 3;
		break;
	}
	State.SetRegister(8210, CurveType);

	int AbsCoord = 91;
	if(State.AbsoluteCoord)
		AbsCoord = 90;
	State.SetRegister(8211, AbsCoord);

	int Tmp = 99;
	if(State.CurCycleRet == TOSTART) 
		Tmp = 98;
	State.SetRegister(8218, Tmp);

	State.SetRegister(8212, State.ActiveUCS);
	SetRegister(State, _T("TCur"), State.CurToolN.GetToolNum(), 8213);
	SetRegister(State, _T("TTurret"), State.CurTurret, 8214);
	SetRegister(State, _T("TCode"), State.ThisCadrToolN, 8223);
	SetRegister(State, _T("TNext"), State.NextToolNum, 8233);
	int FeedMCode = 0;
	switch(State.FeedM)
		{
		case MMpMIN:
			FeedMCode = 0;
			break;
		case MMpREV:
			FeedMCode = 1;
			break;
		case MMpTOOTH:
			FeedMCode = 2;
			break;
		case INCHpMIN:
			FeedMCode = 3;
			break;
		case INCHpREV:
			FeedMCode = 4;
			break;
		case INCHpTOOTH:
			FeedMCode = 5;
			break;
		case REVApMIN:
			FeedMCode = 6;
			break;
		case REVBpMIN:
			FeedMCode = 7;
			break;
		case REVCpMIN:
			FeedMCode = 8;
			break;
	}
	State.SetRegister(8219, FeedMCode);
	State.SetRegister(8220, State.Feed);
	State.SetRegister(8221, State.Spindle[SPINDLE_MILL].Speed);
	State.SetRegister(8222, State.Spindle[SPINDLE_TURN].Speed);
}
void NCUnit::ProcContextFeed(void)
{
	BOOL ContextFeed = FALSE;
	CString str;
	if(GCD.SOthers.Lookup("ContextFeed", str))
		ContextFeed = (str == "Yes"); 
	if(ContextFeed)
	{
		switch(State.LeadingAxis)
		{
		case 0:
		case 1:
		case 2:
			State.FeedM = State.MetricCoord ? MMpMIN : INCHpMIN;
			break;
		case 3:
			State.FeedM = REVApMIN;
			break;
		case 4:
			State.FeedM = REVBpMIN;
			break;
		case 5:
			State.FeedM = REVCpMIN;
			break;
		}
	}
}

NCUnitState * NCUnit::GetState()
{
	return &State;
}

const NCUnitState * NCUnit::GetState() const
{
	return &State;
}

const NCUnitStatePrev * NCUnit::GetSt() const
{
	return &St;
}

NCUnitStatePrev * NCUnit::GetSt()
{
	return &St;
}

NCUnitStatePrevGeom * NCUnit::GetLastGeomState()
{
	return &LastGeomState;
}

BOOL NCUnit::ProcessFunction(CString &text)
{
// find function definition
	CString code;
	code = text.SpanIncluding("0123456789.");
	// remove code from text
	text.Delete(0, code.GetLength());
	if(code.GetLength() == 0)
	{
		CString val;
		if(GetOtherValue("ExprGCode", val))
			if(val == "Yes")
			{
				double dcode;
				if(!GetDIValue(text, &dcode))
				{
					if (StoreError(1059))
						return FALSE;
					dcode = 0;
				}
				code.Format("%d", int(dcode + 0.5));
			}
	}
	if(code.GetLength() == 0)
	{
		CString str;
		if (!GCD.SOthers.Lookup("EmptyGZero", str))
		{
			if (StoreError(1059))
				return FALSE;
		}
		if(str != "Yes")
		{
			if (StoreError(1059))
				return FALSE;
		}
		code = "0";
	}
	// delete leading zero
	if(code[0] == '0' && code.GetLength() > 1)
		code.Delete(0);
	struct FDef* pFDef;
	if(!GCD.SFuncDefInv.Lookup(code, pFDef))
	{
		CString str("No");
		GCD.SOthers.Lookup("IgnoreUnknownG", str);
		if(str != "Yes")
		{
			if (StoreError(1060))
				return FALSE;
			else
				return TRUE;
		}
		else
			return TRUE;
	}
	const CString &def = pFDef->Name;

	if (!pFDef->FConcepts.IsEmpty())
	{
		State.FavorConcepts.AddConcepts(pFDef->FConcepts, GCD.SWordDef, pFDef->ModalConcepts);
	}

// check if this func have been processed in this cadr
// execute function
	//Coord type
	if(def == "PolarInterp")
	{
		return SetPolarInterp(true);
	}
	if(def == "PolarInterpCancel")
	{
		return SetPolarInterp(false);
	}
	if(def == "CylindricInterp")
	{
		return SetCylInterp(text);
	}
	if(def == "CylindricInterpCancel")
	{
		State.CylInterp.DeActivate();
		State.CurPlane = State.CylInterp.GetPreviousPlane();
		return TRUE;
	}
	//Curve type
	if(def == "Rapid")
	{
		CString Val;
		GCD.SOthers.Lookup("FastMovPolarInt", Val);
		if(State.PolarInterp.IsActive() && Val != "Yes")
		{
			if (StoreError(1152))
				return FALSE;
		}
		State.CurCurveType = fast;
		SetInterpChanged();
		State.AllowSpiral = GCD.SpiralG2G3;
		return TRUE;
	}
	if(def == "Linear")
	{
		State.CurCurveType = line;
		SetInterpChanged();
		State.AllowSpiral = GCD.SpiralG2G3;
		return TRUE;
	}
	if(def == "CircularCW")
	{
		State.CurCurveType = cwarc;
		SetInterpChanged();
		State.AllowSpiral = GCD.SpiralG2G3;
		return TRUE;
	}
	if(def == "CircularCCW")
	{
		State.CurCurveType = ccwarc;
		SetInterpChanged();
		State.AllowSpiral = GCD.SpiralG2G3;
		return TRUE;
	}
	if(def == "CircularCWM")
	{
		State.CurCurveType = cwarc;
		SetInterpChanged();
		State.AllowSpiral = GCD.SpiralG2G3;
		State.NonExecCount = 4;
		return TRUE;
	}
	if(def == "CircularCCWM")
	{
		State.CurCurveType = ccwarc;
		SetInterpChanged();
		State.AllowSpiral = GCD.SpiralG2G3;
		State.NonExecCount = 4;
		return TRUE;
	}
	if(def == "SpiralCW")
	{
		State.CurCurveType = cwarc;
		SetInterpChanged();
		State.AllowSpiral = TRUE;
		return TRUE;
	}
	if(def == "SpiralCCW")
	{
		State.CurCurveType = ccwarc;
		SetInterpChanged();
		State.AllowSpiral = TRUE;
		return TRUE;
	}
	if(def == "PolarCW")
	{
		State.CurCurveType = cwarc;
		SetInterpChanged();
		State.ArcCenterFromPolarCenter = TRUE;
		return TRUE;
	}
	if(def == "PolarCCW")
	{
		State.CurCurveType = ccwarc;
		SetInterpChanged();
		State.ArcCenterFromPolarCenter = TRUE;
		return TRUE;
	}
	if(def == "PolarCenter")
	{
		State.DefinePolarCenter = TRUE;
		return TRUE;
	}
	if(def == "ArcTang")
	{
		State.CurCurveType = undef;
		State.CurCurveMod = HhArcTang;
		State.AllowSpiral = false;
		return TRUE;
	}
	//Abs - incr
	if(def == "AbsoluteUnit")
	{
		State.AbsoluteCoord = TRUE;
		return TRUE;
	}
	if(def == "IncrementalUnit")
	{
		State.AbsoluteCoord = FALSE;
		State.PolarAngle = 0.;
		State.PolarDist = 0.;
		return TRUE;
	}
	//Inch - metric
	if(def == "InchUnit")
	{
		State.MetricCoord = FALSE;
		return TRUE;
	}
	if(def == "MetricUnit")
	{
		State.MetricCoord = TRUE;
		return TRUE;
	}
	//Plane
	if(def == "XYPlane")
	{
		State.CurPlane = XY;
		return TRUE;
	}
	if(def == "ZXPlane")
	{
		State.CurPlane = XZ;
		return TRUE;
	}
	if(def == "YZPlane")
	{
		State.CurPlane = YZ;
		return TRUE;
	}
	// LengthComp
	if(def == "LengthComp")
	{
		if (State.CurZComp != UP)
		{
			State.CurZCompChanging = true;
			State.CurZComp = UP;
		}
		return TRUE;
	}
	if(def == "LengthCompOn")
	{
		if (State.CurZComp != DOWN)
		{
			State.CurZCompChanging = true;
			State.CurZComp = DOWN;
		}
		return TRUE;
	}
	if(def == "LengthOffsetCancel")
	{
		if (State.CurZComp != UD_OFF)
		{
			State.CurZCompChanging = true;
			State.CurZComp = UD_OFF;
		}
		return TRUE;
	}
	// DiamComp
	if(def == "ToolCompCancel")
	{
		if( !State.SetCurDirComp(LR_OFF) )
		{
			if (StoreError(1076))
				return FALSE;
		}
		State.CompCancel = TRUE;
		return TRUE;
	}
	if(def == "CompCancelNext")
	{
		State.RetractNext = TRUE;
		State.RetractComp = TRUE;
		return TRUE;
	}
	if(def == "ToolCompLeft")
	{
		if( !State.SetCurDirComp(LEFT) )
		{
			if (StoreError(1077))
				return FALSE;
		}
		return TRUE;
	}
	if(def == "ToolCompLeftInToolNumber")
	{
		if( !State.SetCurDirComp(LEFT) )
		{
			if (StoreError(1077))
				return FALSE;
		}
		ProcDiaCompOffset(State.CurToolN.GetToolNum());
		return TRUE;
	}
	if(def == "ToolCompRight")
	{
		if( !State.SetCurDirComp(RIGHT) )
		{
			if (StoreError(1077))
				return FALSE;
		}
		return TRUE;
	}
	if(def == "ToolCompRightInToolNumber")
	{
		if( !State.SetCurDirComp(RIGHT) )
		{
			if (StoreError(1077))
				return FALSE;
		}
		ProcDiaCompOffset(State.CurToolN.GetToolNum());
		return TRUE;
	}
	if(def == "EnableExtraArcModal")
	{
		State.EnableExtraArc = TRUE;
		State.EnableExtraArcModal = TRUE;
		return TRUE;
	}
	if(def == "DisableExtraArcModal")
	{
		State.EnableExtraArc = FALSE;
		State.EnableExtraArcModal = FALSE;
		return TRUE;
	}
	if(def == "EnableExtraArc")
	{
		State.EnableExtraArc = TRUE;
		return TRUE;
	}
	if(def == "ParToNext")
	{
		State.ParToNext = TRUE;
		return TRUE;
	}
	if(def == "RetractComp")
	{
		State.RetractComp = TRUE;
		return TRUE;
	}

	//OffsetComp
	if(def == "LengthOffsetIncr")
	{
		State.Offset = INCR;
		return TRUE;
	}
	if(def == "LengthOffsetIncr2X")
	{
		State.Offset = INCR2X;
		return TRUE;
	}
	if(def == "LengthOffsetIncr2XITN")
	{
		State.Offset = INCR2X;
		ProcDiaCompOffset(State.CurToolN.GetToolNum());
		return TRUE;
	}
	if(def == "LengthOffsetDecr")
	{
		State.Offset = DECR;
		return TRUE;
	}
	if(def == "LengthOffsetDecr2X")
	{
		State.Offset = DECR2X;
		return TRUE;
	}
	if(def == "LengthOffsetDecr2XITN")
	{
		State.Offset = DECR2X;
		ProcDiaCompOffset(State.CurToolN.GetToolNum());
		return TRUE;
	}
	//Scaling
	if(def == "ScalingCancel")
	{
		State.CurXScale = State.CurYScale = State.CurZScale = 1.;
		State.CurIScale = State.CurJScale = State.CurKScale = 1.;
		return TRUE;
	}
	if(def == "ScalingOn")
	{
		return ProcessScalingOn(text, 1.);
	}
	if(def == "ScalingOn1")
	{
		return ProcessScalingOn(text, 0.01);
	}
	// Coord compensation
	if(def == "CoordCompSignumPlus")
	{
		State.CoordCompSignum = 1;
		return TRUE;
	}
	if(def == "CoordCompSignumMinus")
	{
		State.CoordCompSignum = -1;
		return TRUE;
	}
	if(def == "CoordCompSignumOff")
	{
		State.CoordCompSignum = 0;
		return TRUE;
	}
	// Speed
	if(def == "SetMaxSpeed")
	{
		State.Spindle[SPINDLE_TURN].SetMaxSpeed = TRUE;
		return TRUE;
	}
	if(def == "FixCutSpeed")
	{
		if(State.Spindle[SPINDLE_TURN].MaxSpeed <= 0.)
		{
			if (StoreError(1097))
				return FALSE;
		}
		State.Spindle[SPINDLE_TURN].FixCutSpeed = TRUE;
		return TRUE;
	}
	if(def == "UnFixCutSpeed")
	{
		State.Spindle[SPINDLE_TURN].FixCutSpeed = FALSE;
		return TRUE;
	}
	// Polar coord
	if(def == "PolarCoordModeOn")
	{
		State.PolarCoordActive = true;
		return TRUE;
	}
	if(def == "PolarCoordModeOff")
	{
		State.PolarCoordActive = false;
		return TRUE;
	}

	//Cycles
	if(def == "PredefinedCycle")
	{
		CString g;
		if(State.DefinedCycleNum < 0 || !GCD.SWordDef.Lookup("FunctionCode",g))
		{
			Error = 1110;// level 1
			return FALSE;
		}
		CString buf;
		buf.Format("%d", State.DefinedCycleNum);
		text = g + buf + text;
		return TRUE;
	}
	if(def == "CancelCycle")
	{
		return ProcCancelCycle();
	}
	if(def == "DrillCyclePlane")
	{
		if( !SetCurCycle(def, DRILL, text) )
			return FALSE;
		switch(State.CurPlane)
		{
		case XY:
			State.CycleStartZ = State.MashZUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
			break;
		case XZ:
			State.CycleStartZ = State.MashYUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.YScale * State.CurYScale));
			break;
		case YZ:
			State.CycleStartZ = State.MashXUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.XScale * State.CurXScale));
			break;
		}
		State.FavorConcepts.AddConcepts(
			CString("RapidClearanceLevel1\nRapidClearanceLevel2\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "DrillCycle")
	{
		if (State.CurCycle != DRILL_OLD)
			StoredPlane = State.CurPlane;
		State.CurPlane = XY;
		if( !SetCurCycle(def, DRILL_OLD, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		State.FavorConcepts.AddConcepts(
			CString("RapidClearanceLevel1\nRapidClearanceLevel2\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "DrillCycleX")
	{
		if (State.CurCycle != DRILL_X)
			StoredPlane = State.CurPlane;
		State.CurPlane = YZ;
		if( !SetCurCycle(def, DRILL_X, text) )
			return FALSE;
		State.CycleStartZ = State.MashXUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.XScale * State.CurXScale));
		return TRUE;
	}
	if(def == "DrillCycleY")
	{
		if (State.CurCycle != DRILL_Y)
			StoredPlane = State.CurPlane;
		State.CurPlane = XZ;
		if( !SetCurCycle(def, DRILL_Y, text) )
			return FALSE;
		State.CycleStartZ = State.MashYUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.YScale * State.CurYScale));
		return TRUE;
	}
	if(def == "DefMPParams")
	{
		State.FavorConcepts.AddConcepts(
			CString("DrillRelR\nDrillRelZ\nDrillRelW\nDrillRelX\nDrillMPI\nDrillMPJ\nDrillMPK\nDrillMPK1\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "DrillCycle_MP81")
	{
		if( !SetCurCycle(def, DRILL_MP81, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "DrillCycleRel")
	{
		if( !SetCurCycle(def, DRILL_HM, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		State.FavorConcepts.AddConcepts(
			CString("DrillRelR\nDrillRelZ\nDrillRelW\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "DrillCycle_HM")
	{
		if( !SetCurCycle(def, DRILL_HM, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "DrillCycle_HMC")
	{
		if( !SetCurCycle(def, DRILL_HMC, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "DrillCycle_HN")
	{
		if( !SetCurCycle(def, DRILL_HN, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "DrillDwellCycle")
	{
		if( !SetCurCycle(def, DRILLDWELL, text) )
			return FALSE;
		switch(State.CurPlane)
		{
		case XY:
			State.CycleStartZ = State.MashZUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
			break;
		case XZ:
			State.CycleStartZ = State.MashYUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.YScale * State.CurYScale));
			break;
		case YZ:
			State.CycleStartZ = State.MashXUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.XScale * State.CurXScale));
			break;
		}
		State.FavorConcepts.AddConcepts(
			CString("CycleDwellTime\nRapidClearanceLevel1\nRapidClearanceLevel2\n"), GCD.SWordDef, true);
		return TRUE;
	}
	if(def == "RHTappingCyclePlane")
	{
		if (State.CurCycle != RHTAPPINGPLANE)
			StoredPlane = State.CurPlane;
		if( !SetCurCycle(def, RHTAPPINGPLANE, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("DrillRef\nCycleDwellTime\n"), GCD.SWordDef, true);
		switch(State.CurPlane)
		{
		case XY:
			State.CycleStartZ = State.MashZUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
			break;
		case XZ:
			State.CycleStartZ = State.MashYUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.YScale * State.CurYScale));
			break;
		case YZ:
			State.CycleStartZ = State.MashXUCS;
			if(State.CurCycleRet == TOSTART)
				State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.XScale * State.CurXScale));
			break;
		}
		return TRUE;
	}
	if(def == "RHTappingCycleZ")
	{
		if (State.CurCycle != RHTAPPINGZ)
			StoredPlane = State.CurPlane;
		State.CurPlane = XY;
		if( !SetCurCycle(def, RHTAPPINGZ, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("DrillRef\nCycleDwellTime\n"), GCD.SWordDef, true);
		return TRUE;
	}
	if(def == "RHTappingCycleX")
	{
		if (State.CurCycle != RHTAPPINGX)
			StoredPlane = State.CurPlane;
		State.CurPlane = YZ;
		if( !SetCurCycle(def, RHTAPPINGX, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("DrillRefRad\nCycleDwellTime\n"), GCD.SWordDef, true);
		return TRUE;
	}
	if(def == "DrillPeckCycle")
	{
		if( !SetCurCycle(def, DRILLPECK, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		State.FavorConcepts.AddConcepts(
			CString("DrillRef\nPeckIncrementDepth\n"), GCD.SWordDef, true);
		return TRUE;
	}
	if(def == "DrillPeckCycleZ")
	{
		if(State.CurCycle != DRILLPECKZ)
			StoredPlane = State.CurPlane;
		State.CurPlane = XY;
		if( !SetCurCycle(def, DRILLPECKZ, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		State.FavorConcepts.AddConcepts(
			CString("DrillRef\nPeckIncrementDepth\n"), GCD.SWordDef, true);
		return TRUE;
	}
	if(def == "DrillPeckCycleX")
	{
		if (State.CurCycle != DRILLPECKX)
			StoredPlane = State.CurPlane;
		State.CurPlane = YZ;
		if( !SetCurCycle(def, DRILLPECKX, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		State.FavorConcepts.AddConcepts(
			CString("DrillRef\nPeckIncrementDepth\n"), GCD.SWordDef, true);
		return TRUE;
	}
	if(def == "RightRHTappingCycle")
	{
		if( !SetCurCycle(def, RIGHTRHTAPPING, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if (def == "CuttingCycle")
	{
		if (!SetCurCycle(def, CUTTING, text))
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if (State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf", State.CycleStartZ / (GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if (def == "GrooveCycle")
	{
		if (!SetCurCycle(def, GROOVE, text))
			return FALSE;
		State.CycleZ = text;
		text.Empty();
		return TRUE;
	}
	if (def == "LeftHandCutting")
	{
		if( !SetCurCycle(def, LEFTCUTTING, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "LeftHandTapping")
	{
		if( !SetCurCycle(def, LEFTTAPPING, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "DrillCycleNUMER")
	{
		if( !SetCurCycle(def, NUMERICON, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "TappingCycle")
	{
		if( !SetCurCycle(def, TAPPING, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "RectPoketCycle")
	{
		if(!DefineRect(text))
			return FALSE;
		if( !SetCurCycle(def, RECTPOKET, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "RectPoketCycleQ")
	{
		if(!DefineRectQ(text))
			return FALSE;
		if( !SetCurCycle(def, RECTPOKET, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "CircularPoketCycle")
	{
		if( !SetCurCycle(def, CIRCULARPOKET, text) )
			return FALSE;
		State.CycleI = CYCLE_UNDEF;
		State.CycleJ = CYCLE_UNDEF;
		State.CycleTappingZ = CYCLE_UNDEF;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "ReamCycle")
	{
		if( !SetCurCycle(def, REAM, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "BoreCycle")
	{
		if( !SetCurCycle(def, BORE, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "BackBoreCycle")
	{
		if( !SetCurCycle(def, BACKBORE, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "BoreDwellCycle")
	{
		if( !SetCurCycle(def, BOREDWELL, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "ReamDwellCycle")
	{
		if( !SetCurCycle(def, REAMDWELL, text) )
			return FALSE;
		State.CycleStartZ = State.MashZUCS;
		if(State.CurCycleRet == TOSTART)
			State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	// Added 26.10.04
		
	if(def == "RectCycle")
	{
		if(!DefineRect(text))
			return FALSE;
		if( !SetCurCycle(def, RECTANGLE, text) )
			return FALSE;
		return TRUE;
	}




	if(def == "TurnRoundCycle")
	{
		if(State.CurCurveType == line) 
		{
			if( !SetCurCycle(def, TURNROUND, text) )
				return FALSE;
		}
		return TRUE;
	}
	if(def == "TurnChamferCycle")
	{
		if(State.CurCurveType == line && State.CurCycle == CYCLE_OFF) 
		{
			if( !SetCurCycle(def, TURNCHAMFER, text) )
				return FALSE;
		}
		return TRUE;
	}
	if(def == "TurnGroovXCycle")
	{
		CString title;
		if(!GCD.SWordDef.Lookup("TurnRoughDepthR",title))
		{
			Error = 1093;// level 1
			return FALSE;
		}
		State.FavorConcepts.AddConcepts(
			CString("TurnRoughDepthR"), GCD.SWordDef, false);
		if(title == text.Left(1))
		{// First cadr of the cycle
			return TRUE;// Do nothing
		}
		State.FavorConcepts.AddConcepts(
			CString("TurnGroovDepthP\nTurnGroovDepthQ\nTurnRoughDepthR2\n"), GCD.SWordDef, false);
		// Second string of the cycle
		if( !SetCurCycle(def, TURNGROOVX, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "TurnGroovZCycle")
	{
		CString title;
		if(!GCD.SWordDef.Lookup("TurnRoughDepthR",title))
		{
			Error = 1093;// level 1
			return FALSE;
		}
		State.FavorConcepts.AddConcepts(
			CString("TurnRoughDepthR"), GCD.SWordDef, false);
		if(title == text.Left(1))
		{// First cadr of the cycle
			return TRUE;// Do nothing
		}
		State.FavorConcepts.AddConcepts(
			CString("TurnGroovDepthP\nTurnGroovDepthQ\nTurnRoughDepthR2\n"), GCD.SWordDef, false);
		// Second string of the cycle
		if( !SetCurCycle(def, TURNGROOVZ, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "TurnThreadCutCycle")
	{
		if(State.CurPlane != XZ)
		{
			Error = 1140;// level ???
			return FALSE;
		}
		if(State.FeedM != MMpREV && State.FeedM != INCHpREV)
		{
			Error = 1141;// level ???
			return FALSE;
		}
		if( !SetCurCycle(def, TURNTHREADCUT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("CParPIT\nCParMPIT\nCParSPL\nCParFPL\nCParDM1\nCParDM2\nCParAPP\nCParROP\nCParTDEP\nCParFAL\nCParIANG\nCParNSP\nCParNRC\nCParNID\nCParVARI\nCParNUMT\n")
			, GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "TurnThreadACycle")
	{
		if( !SetCurCycle(def, TURNTHREADAUTO, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("TurnThreadP\nTurnThreadR\nTurnThreadQ\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "TurnThreadCCycle")
	{
		if( !SetCurCycle(def, TURNTHREADC, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "TurnLoopCycle")
	{
		CString title;
		if(!GCD.SWordDef.Lookup("ProgramCopyStart",title))
		{
			Error = 1023;// level 1
			return FALSE;
		}
		if(title != text.Left(1))
		{// First cadr of the cycle
			if(GCD.SWordDef.Lookup("TurnRoughDepthU1",title))
			{
				if(	title == text.Left(1))
				{
					text.Delete(0);
					double U1;
					if(!GetDValue(text,&U1))
					{
						if (StoreError(1015))
							return -6;
						else
							U1 = 0.;
					}
					State.SetRegister(5135, U1);
					if(GCD.SWordDef.Lookup("TurnRoughDepthW1",title))
					{
						if(	title == text.Left(1))
						{
							text.Delete(0);
							double W1;
							if(!GetDValue(text,&W1))
							{
								if (StoreError(1015))
									return -6;
								else
									W1 = 0.;
							}
							State.SetRegister(5136, W1);
							if(GCD.SWordDef.Lookup("TurnRoughDepthR",title))
							{
								if(	title == text.Left(1))
								{
									text.Delete(0);
									double R;
									if(!GetDValue(text,&R))
									{
										if (StoreError(1015))
											return -6;
										else
											R = 0.;
									}
									State.SetRegister(5137, R);
								}
							}
						}
					}
				}
			}
			return TRUE;
		}
		if( !SetCurCycle(def, TURNLOOP, text) )
			return FALSE;
		BOOL res = ProcessLoopCycle(text);
		State.FavorConcepts.AddConcepts(
			CString("HorizontalAxisInc\nSpindleAxisInc\n"), GCD.SWordDef, false);
		return res;
	}


	if(def == "MillContCycle")
	{
		if( !SetCurCycle(def, MILLCONT, text) )
			return FALSE;
		// Z J R K Q V - modal
		State.FavorConcepts.AddConcepts(
			CString("MccZ\nMccJ\nMccR\nMccK\nMccQ\nMccV\n"), GCD.SWordDef, false);
		return TRUE;
	}

	if(def == "TurnRoughCycle" || def == "TurnFacingCycle")
	{
		CString title;
		if(!GCD.SWordDef.Lookup("ProgramCopyStart",title))
		{
			Error = 1023;// level 1
			return FALSE;
		}
		if(title != text.Left(1))
		{// First cadr of the cycle
			if(GCD.SWordDef.Lookup("TurnRoughDepthU1",title))
			{
				if(	title == text.Left(1))
				{
					text.Delete(0);
					double U1;
					if(!GetDValue(text,&U1))
					{
						if (StoreError(1015))
							return -6;
						else
							U1 = 0.;
					}
					State.SetRegister(5132, U1);
					if(GCD.SWordDef.Lookup("TurnRoughDepthR",title))
					{
						if(	title == text.Left(1))
						{
							text.Delete(0);
							double R;
							if(!GetDValue(text,&R))
							{
								if (StoreError(1015))
									return -6;
								else
									R = 0.;
							}
							State.SetRegister(5133, R);
						}
					}
				}
			}

			return TRUE;
		}
		if( !SetCurCycle(def, TURNROUGH, text) )
			return FALSE;
		BOOL res = MakeCycleGeom(text, CString(_T("RoughCycleSearch")));
		if(!res)
		{
			Error = 1087;// level 1
			return FALSE;
		}
		return TRUE;
	}
	if(def == "SimThreadCycleT")
	{
		if( !SetCurCycle(def, TURNTHREAD, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "HHApprLT")
	{
		if( !SetCurCycle(def, APPRLT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhLen\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHApprLN")
	{
		if( !SetCurCycle(def, APPRLN, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhLen\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHApprCT")
	{
		if( !SetCurCycle(def, APPRCT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhR\nHhCca\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHApprLCT")
	{
		if( !SetCurCycle(def, APPRLCT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhR\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHDepLT")
	{
		if( !SetCurCycle(def, DEPLT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhLen\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHDepLN")
	{
		if( !SetCurCycle(def, DEPLN, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhLen\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHDepCT")
	{
		if( !SetCurCycle(def, DEPCT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhR\nHhCca\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "HHDepLCT")
	{
		if( !SetCurCycle(def, DEPLCT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("HhR\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "TurnCutCycle")
	{
		if( !SetCurCycle(def, TURNCUT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("TurnConeX\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "TurnCutRoughCycle")
	{
		if( !SetCurCycle(def, TURNCUTROUGH, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("TurnRoughDepthR\nTurnGroovDepthP\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "TurnFaceCycle")
	{
		if( !SetCurCycle(def, TURNFACECUT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("TurnConeZ\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "TurnFaceRoughCycle")
	{
		if( !SetCurCycle(def, TURNFACECUTROUGH, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("TurnRoughDepthR\nTurnGroovDepthP\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCenterCycle")
	{
		if( !SetCurCycle(def, MILLRECTCENTER, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillR\nMillZ\nMillQ\nMillK\nMillP\nMillJ\nMillW\nMillO"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCornerCycle")
	{
		if( !SetCurCycle(def, MILLRECTCORNER, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillR\nMillZ\nMillQ\nMillK\nMillP\nMillJ\nMillW\nMillO"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCenterInCycle")
	{
		if( !SetCurCycle(def, MILLRECTCENTERIN, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillO\nMillR\nMillZ\nMillCR\nMillQ\nMillJ\nMillK\nMillP\nMillI\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCenterInClearCycle")
	{
		if( !SetCurCycle(def, MILLRECTCENTERINCLEAR, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillO\nMillR\nMillZ\nMillCR\nMillQ\nMillJ1\nMillK\nMillP\nMillI\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCornerInCycle")
	{
		if( !SetCurCycle(def, MILLRECTCORNERIN, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillO\nMillR\nMillZ\nMillCR\nMillQ\nMillJ\nMillK\nMillP\nMillI\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCornerInClearCycle")
	{
		if( !SetCurCycle(def, MILLRECTCORNERINCLEAR, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillO\nMillR\nMillZ\nMillCR\nMillQ\nMillJ1\nMillK\nMillP\nMillI\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCornerOutCycle")
	{
		if( !SetCurCycle(def, MILLRECTCORNEROUT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillO\nMillR\nMillZ\nMillCR\nMillQ\nMillJ\nMillK\nMillP\nMillI\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillRectCenterOutCycle")
	{
		if( !SetCurCycle(def, MILLRECTCENTEROUT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillRectU\nMillRectV\nMillO\nMillR\nMillZ\nMillCR\nMillQ\nMillJ\nMillK\nMillP\nMillI\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillSlotInCycle")
	{
		if( !SetCurCycle(def, MILLSLOTIN, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "MillCircCycle")
	{
		if( !SetCurCycle(def, MILLCIRC, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillCircU\nMillR\nMillZ\nMillQ\nMillK\nMillP\nMillJ\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillCircInCycle")
	{
		if( !SetCurCycle(def, MILLCIRCIN, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillCircU\nMillR\nMillZ\nMillQ\nMillL\nMillK\nMillP\nMillI\nMillJ\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillCircInClearCycle")
	{
		if( !SetCurCycle(def, MILLCIRCINCLEAR, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillCircU\nMillR\nMillZ\nMillQ\nMillL\nMillK\nMillP\nMillI\nMillJ1\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "MillHoleClearCycle")
	{
		if( !SetCurCycle(def, MILLHOLECLEAR, text) )
			return FALSE;
		return TRUE;
	}
	if(def == "MillCircOutCycle")
	{
		if( !SetCurCycle(def, MILLCIRCOUT, text) )
			return FALSE;
		State.FavorConcepts.AddConcepts(
			CString("MillCircU\nMillR\nMillZ\nMillQ\nMillL\nMillK\nMillP\nMillI\nMillJ1\nMillW"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "ReturnToStartPlane")
	{
		State.CurCycleRet = TOSTART;
		State.R2.Format("%.6lf",State.CycleStartZ/(GCD.LinearScale*GCD.ZScale * State.CurZScale));
		return TRUE;
	}
	if(def == "ReturnToRefPlane")
	{
		State.CurCycleRet = TOREF;
		State.R2 = State.R1;
		return TRUE;
	}
	//Feed mesure
	if(def == "FeedPerMin")
	{
		State.FeedM = State.MetricCoord ? MMpMIN : INCHpMIN;
		return TRUE;
	}
	if(def == "FeedPerRev")
	{
		State.FeedM = State.MetricCoord ? MMpREV : INCHpREV;
		return TRUE;
	}
	//Punch
	if(def == "PunchStart")
	{
		State.Punch = PUNCH_ON;
		return TRUE;
	}
	if(def == "PunchCancel")
	{
		State.Punch = PUNCH_OFF;
		return TRUE;
	}
	//MirrorRotate
	if(def == "MirrorRotateOff")
	{
		State.MirrorRotate.Reset();
		return CheckPolar();
	}
	if(def == "MirrorRotate1")
	{
		State.MirrorRotate.SetXY(1.,0.,0.,-1.);
		return CheckPolar();
	}
	if(def == "MirrorRotate2")
	{
		State.MirrorRotate.SetXY(-1.,0.,0.,-1.);
		return CheckPolar();
	}
	if(def == "MirrorRotate3")
	{
		State.MirrorRotate.SetXY(-1.,0.,0.,1.);
		return CheckPolar();
	}
	if(def == "MirrorRotate4")
	{
		State.MirrorRotate.SetXY(0.,-1.,-1.,0.);
		return CheckPolar();
	}
	if(def == "MirrorRotate5")
	{
		State.MirrorRotate.SetXY(0.,-1.,1.,0.);
		return CheckPolar();
	}
	if(def == "MirrorRotate6")
	{
		State.MirrorRotate.SetXY(0.,1.,1.,0.);
		return CheckPolar();
	}
	if(def == "MirrorRotate7")
	{
		State.MirrorRotate.SetXY(0.,1.,-1.,0.);
		return CheckPolar();
	}
	// Coordinate systems
	if(def == "RotateCSABC")
	{
		State.RotateCSABC = TRUE;
		return CheckPolar();
	}
	if(def == "MoveRotateCS")
	{
		State.MoveRotateCSAngle = 0.;
		if (!GetDParamVal(text, _T("MoveRotateCSAngle"), &State.MoveRotateCSAngle))
		{
			if (Error != 0)
				return FALSE;
		}
		State.MoveRotateCS = TRUE;
		return CheckPolar();
	}
	if(def == "MoveRotateCSRel")
	{
		State.MoveRotateCSAngle = 0.;
		if (!GetDParamVal(text, _T("MoveRotateCSAngle"), &State.MoveRotateCSAngle))
		{
			if (Error != 0)
				return FALSE;
		}
		State.MoveRotateCSRel = TRUE;
		return CheckPolar();
	}
	if(def == "MoveRotateCSCancel")
	{
		State.MoveRotateCSCancel = TRUE;
		return CheckPolar();
	}
	if(def == "ZeroPreset")
	{
		State.ZeroPreset = TRUE;
		State.ZeroPresetType = ZPT_STD;
		return CheckPolar();
	}
	if (def == "ZeroPresetRel")
	{
		State.ZeroPreset = TRUE;
		State.ZeroPresetType = ZPT_REL;
		return CheckPolar();
	}
	if (def == "ZeroPresetSht")
	{
		State.ZeroPreset = TRUE;
		State.ZeroPresetType = ZPT_SHT;
		return CheckPolar();
	}
	if(def == "MachineCoordSys")
	{
		CString MCSString;
		GetOtherValue("MachineCS", MCSString);
		double Coords[6] = { 0., 0., 0., 0., 0., 0.};
		for(int i = 0; i < 6 && !MCSString.IsEmpty(); ++i)
		{
			CString Num = MCSString.SpanExcluding(",");
			MCSString.Delete(0, Num.GetLength() + 1);
			Coords[i] = atof(Num);
		}

		State.RecalcToMachCS();
		BPoint XYZ(Coords[0], Coords[1], Coords[2], 1.);
		State.CurBase.x0 = XYZ.GetX();
		State.CurBase.y0 = XYZ.GetY();
		State.CurBase.z0 = XYZ.GetZ();
		State.CurBase.a0 = Coords[3];
		State.CurBase.b0 = Coords[4];
		State.CurBase.c0 = Coords[5];

		State.MachineCSActive = true;

		return CheckPolar();
	}
	// Work coord systems
	const auto BaseLength = CString(_T("WorkCoordSys")).GetLength();
	if (def.GetLength() > BaseLength)
		if (def.Left(BaseLength) == _T("WorkCoordSys"))
		{
			CString bdef = def;
			bdef.Delete(0, BaseLength);
			int UcsInd = atoi(bdef);
			if (UcsInd < 1)
			{
				if (bdef == _T("Par"))
				{
					State.FavorConcepts.AddConcepts(
						CString(_T("UCSNum\n")), GCD.SWordDef, false);
					double UcsIndD = 0.;
					if (!GetDIParamVal(text, _T("UCSNum"), &UcsIndD))
					{
						if(Error != 0)
							return FALSE;
					}
					UcsInd = int(UcsIndD + 0.5);
					CString SInd;
					if (GetOtherValue("WCSParStartInd", SInd))
					{
						int PInd = atoi(SInd);
						if (GetOtherValue("WCSParMaxNum", SInd))
						{
							if (UcsInd < 1 || UcsInd > atoi(SInd))
							{
								Error = 1189;// level 1
								return FALSE;
							}
						}
						UcsInd += PInd - 1;
					}
				}
			}
			if(UcsInd < 1 || UcsInd > MAX_UCS || UcsInd - 1 > NUCSs::MaxUcsInd)
			{
				Error = 1190;// level 1
				return FALSE;
			}
			State.ActiveUCS = UcsInd - 1;
			State.UCSs.SetUsed(State.ActiveUCS);
			CString val;
			GetOtherValue("KeepUCSDisp", val);
			if (val == "Yes")
				State.CurBase = State.UCSs[State.ActiveUCS] - State.GetActiveUCS() + State.CurBase;
			else
				State.CurBase = State.UCSs[State.ActiveUCS];
			State.CurBase.MoveBPoint(State.UCSMatr);
			return CheckPolar();
		}
	//CopyCall
	if(def == "TurnClearCycle")
	{
		CString title;
		if(!GCD.SWordDef.Lookup("ProgramCopyStart",title))
		{
			Error = 1023;// level 1
			return FALSE;
		}
		if( !SetCurCycle(def, TURNCLEAR, text) )
			return FALSE;
		State.SetRegister(9002, State.MashX);
		State.SetRegister(9003, State.MashZ);
		State.SetRegister(9004, State.MashcX);
		State.SetRegister(9005, State.MashcZ);
		// There is no return statement i.e. we need next if ("ProgramCopy") to run
	}
	if (def == "ProgramCopy1L")
	{
		int count = 1;
		cadrID p = State.CadrID;
		++p.Cadr;
		if (State.ActiveCopyEnd == -2)
		{
			// "ProgramCopy1L" is active
			// check if it is this program copy or another
			cadrID Cadr;
			int buf;
			if (State.CallStack.Pop(Cadr, buf) != 0)
				return FALSE;
			cadrID Cadr1;
			int buf1;
			if (State.CallStack.Pop(Cadr1, buf1) != 0)
				return FALSE;
			if (State.CallStack.Push(Cadr1, buf1) != 0)
				return FALSE;
			if (State.CallStack.Push(Cadr, buf) != 0)
				return FALSE;
			if (Cadr1 == p)
			{
				//this program copy is active
				if(!ProgCopyLC())
				{
					Error = 1001;// level 0
					return FALSE;
				}
				text.Empty();
				return TRUE;
			}
		}
		if (State.ActiveCopyEnding)
			p = State.NextCadr;
		if (State.CallStack.Push(p, State.ActiveCopyEnd) != 0)
			return FALSE;

		SearchDir SDir = GetSDir(_T("ClearCycleSearch"), false);
		cadrID tmp;
		if (!FindCopy1L(text, &tmp, SDir))
			return FALSE;// Error defined in FindCopy1L
		State.ActiveCopyEnd = -2; // for 1 Label ProgramCopy. Must be not equal to -1 and not equal to any cadr num or label
		if (State.CallStack.Push(tmp, count) != 0)
			return FALSE;
		State.OrderChanged = true;
		State.NextCadr = tmp;
		State.ActiveCopyStarting = true;
		State.FavorConcepts.AddConcepts(
			CString("RepeatNumber\n"), GCD.SWordDef, false);

		return TRUE;
	}
	if (def == "ProgramCopy" || def == "TurnClearCycle" || def == "ProgramCopyB")
	{
		int count = 1;
		cadrID p = State.CadrID;
		++p.Cadr;
		if(State.ActiveCopyEnding)
			p = State.NextCadr;
		if(State.CallStack.Push(p, State.ActiveCopyEnd) != 0)
			return FALSE;

		SearchDir SDir = GetSDir(_T("ClearCycleSearch"), def == "ProgramCopyB");
		cadrID tmp;
		if(!FindCopy(text, &tmp, &State.ActiveCopyEnd, SDir))
			return FALSE;// Error defined in FindCopy
		if(State.CallStack.Push(tmp, count) != 0)
			return FALSE;
		State.OrderChanged = true;
		State.NextCadr = tmp;
		State.ActiveCopyStarting = true;
		State.FavorConcepts.AddConcepts(
			CString("RepeatNumber\n"), GCD.SWordDef, false);

		return TRUE;
	}
	if(def == "ProgramCopyMP")
	{
		int count = 1;
		cadrID p = State.CadrID;
		++p.Cadr;
		if(State.ActiveCopyEnding)
			p = State.NextCadr;
		if(State.CallStack.Push(p, State.ActiveCopyEnd) != 0)
			return FALSE;

		SearchDir SDir = GetSDir(_T("ClearCycleSearch"), false);
		cadrID tmp;
		CString Buftext = text;
		if(!FindCopy(text, &tmp, &State.ActiveCopyEnd, SDir))
		{
			if(Error != 0)// level ???
				return FALSE;// Error defined in FindCopy
			int num;
			if(!GetIParamVal(Buftext, "ProgramCopyStart", &num))
			{
				return FALSE;//error	
			}
			State.ActiveCopyEnd = num;
		}
		if(State.CallStack.Push(tmp, count) != 0)
			return FALSE;
		State.OrderChanged = true;
		State.NextCadr = tmp;
		State.ActiveCopyStarting = true;
		State.FavorConcepts.AddConcepts(
			CString("RepeatNumber\n"), GCD.SWordDef, false);

		return TRUE;
	}
	// Added 16.03.04
	if(def == "CallMacro")
	{
		CString YesNo;
		GetOtherValue(_T("UseRegStack"), YesNo);
		if(YesNo == _T("Yes"))
		{
			State.Push133Registers();
			if(!ProcessRegSubst(text, REG_COPY_BASE))
				return FALSE;// Error defined in ProcessRegSubst
			State.Restore133Registers();
		}
		else
		{
			if(!ProcessRegSubst(text, 0))
				return FALSE;// Error defined in ProcessRegSubst
		}

		return ProcessSubrCall(text, false);
	}
	if(def == "ModalCallMacro")
	{
		State.ModalSubr2 = false;
		if(!ProcessRegSubst(text, 0))
			return FALSE;// Error defined in ProcessRegSubst
		CString BufFormat;
		GCD.SOthers.Lookup(_T("SubrCallFormat"), BufFormat);
		GCDV.SOthers[_T("SubrCallFormat")] = "N"; // Needed to prevent deleting L from "text"

		if(!FindNextCadr(text, &State.ModalSubrID, false))
			return FALSE; // Error defined in FindNextCadr
		GCDV.SOthers[_T("SubrCallFormat")] = BufFormat;
		return TRUE;
	}
	if(def == "ModalCallMacro2")
	{
		State.ModalSubr2 = true;
		if(!ProcessRegSubst(text, ModalSubr2Base))
			return FALSE;// Error defined in ProcessRegSubst
		CString BufFormat;
		GCD.SOthers.Lookup(_T("SubrCallFormat"), BufFormat);
		GCDV.SOthers[_T("SubrCallFormat")] = "N"; // Needed to prevent deleting L from "text"

		if(!FindNextCadr(text, &State.ModalSubrID, false))
			return FALSE; // Error defined in FindNextCadr
		GCDV.SOthers[_T("SubrCallFormat")] = BufFormat;
		return TRUE;
	}
	if(def == "ModalCallCancel")
	{
		State.ModalSubrIDRet.Prog = -1;
		State.ModalSubrIDRet.Cadr = 0;
		State.ModalSubrID.Prog = -1;
		State.ModalSubrID.Cadr = 0;
		State.ModalAutoCycleCount = -1;
		return TRUE;
	}
	// Added 27.10.04
	if(def == "ForceCycle")
	{
		CString FCycle;
		if(GCD.SOthers.Lookup("ForcedCycles",FCycle))
			if(FCycle == "Yes")
				State.SetCurCycle(HiddenCycle);
	}
	// Chamfer
	if(def == "Round" || def == "DraftRound")
	{
		State.Chamfer.SetType(ROUND);
		State.FavorConcepts.AddConcepts(
			CString("RoundSize\n"), GCD.SWordDef, false);
		return TRUE;
	}
	if(def == "Chamfer" || def == "DraftChamfer")
	{
		State.Chamfer.SetType(CHAMFER);
		State.FavorConcepts.AddConcepts(
			CString("ChamferSize\n"), GCD.SWordDef, false);
		return TRUE;
	}
	// MTComp
	if(def == "StockCompOn")
	{
		State.MTComp.CompStock = true;
		pDoc->GetMachTool().SetCompensations(State.MTComp);
		return TRUE;
	}
	if(def == "StockCompOff")
	{
		State.MTComp.CompStock = false;
		pDoc->GetMachTool().SetCompensations(State.MTComp);
		return TRUE;
	}
	if(def == "ToolCompOn")
	{
		State.MTComp.CompTool = true;
		State.MTComp.CompLocal = true;
		pDoc->GetMachTool().SetCompensations(State.MTComp);
		return TRUE;
	}
	if(def == "ToolCompOff")
	{
		State.MTComp.CompTool = false;
		State.MTComp.CompLocal = false;
		pDoc->GetMachTool().SetCompensations(State.MTComp);
		return TRUE;
	}

	//
	// 	Machine travel limits
	if(def == "DefLimitsPos")
	{
//		State.CoordLimits.SetMax(StartState.CoordLimits);
		State.TLFlag = TL_POSITIVE;
		State.FavorConcepts.AddConcepts(
			CString("HorizontalAxisMax\nVerticalAxisMax\nSpindleAxisMax\nHorizontalRotationAxisMax\nVerticalRotationAxisMax\nSpindleRotationAxisMax\n")
			, GCD.SWordDef, false);
		return TRUE;
	}

	if(def == "DefLimitsNeg")
	{
//		State.CoordLimits.SetMin(StartState.CoordLimits);
		State.TLFlag = TL_NEGATIVE;
		State.FavorConcepts.AddConcepts(
			CString("HorizontalAxisMin\nVerticalAxisMin\nSpindleAxisMin\nHorizontalRotationAxisMin\nVerticalRotationAxisMin\nSpindleRotationAxisMin\n")
			, GCD.SWordDef, false);
		return TRUE;
	}

	if(def == "ActivateLimits")
	{
		State.TLFlag = TL_ACTIVATE;
		State.FavorConcepts.AddConcepts(
			CString("HorizontalAxisAct\nVerticalAxisAct\nSpindleAxisAct\nHorizontalRotationAxisAct\nVerticalRotationAxisAct\nSpindleRotationAxisAct\n")
			, GCD.SWordDef, false);
	}
	//Splines
	if(def == "SplineA")
	{
		State.Spline.Activate();
		State.Spline.SetInterpPlane(State.CurPlane);
		double CompVal = ((State.CurDirComp == LR_OFF) ? 0. : ((State.CurDirComp == RIGHT) ? -1. : 1.) * State.CurDiamComp);
		State.Spline.SetComp(CompVal);
		return TRUE;
	}
	if(def == "EAuto")
	{
		State.Spline.SetEndCond(BSplineParams::Free);
		return TRUE;
	}
	if(def == "BAuto")
	{
		State.Spline.SetStartCond(BSplineParams::Free);
		return TRUE;
	}
	if(def == "ENut")
	{
		State.Spline.SetEndCond(BSplineParams::ZeroCurv);
		return TRUE;
	}
	if(def == "BNut")
	{
		State.Spline.SetStartCond(BSplineParams::ZeroCurv);
		return TRUE;
	}
	if(def == "ETan")
	{
		State.Spline.SetEndCond(BSplineParams::Tang);
		return TRUE;
	}
	if(def == "BTan")
	{
		State.Spline.SetStartCond(BSplineParams::Tang);
		return TRUE;
	}
	//Other
	if(def == "Dwell")
	{
		return ProcessDwell(text);
	}
	if(def == "TechTable")
	{
		return ProcessTechTable(text);
	}
	if (def == "TiltPlane")
	{
		if (!CheckPolar())
			return FALSE;
		return ProcessTiltPlane(text, true);
	}
	if (def == "TiltPlaneV")
	{
		if (!CheckPolar())
			return FALSE;
		return ProcessTiltPlane(text, false);
	}
	if(def == "AbsolutePosition")
	{
		return TRUE;
	}
	if (def == "PlaceTool")
	{
		State.PlaceTool = PT_IJK;
		State.FavorConcepts.AddConcepts(
			CString("PlaceToolAxes\n"), GCD.SWordDef, false);
		return CheckPolar();
	}
	if (def == "ProbeFor")
	{
		State.ProbeCadr = true;
		State.ProbeCollision = true;
		State.EndProgram = true;
		return TRUE;
	}
	if (def == "ProbeBack")
	{
		State.ProbeCadr = true;
		State.ProbeCollision = false;
		State.EndProgram = true;
		return TRUE;
	}

	if (def == "CreateMillingTool")
	{
		return ProcessCreateMTool(text);
	}
// get specific parameters (xyz for g0g1;xyzijkr for g2g3)

	return TRUE;
}

void NCUnit::ProcessTouch(bool ResetMatrs)
{
	if (!ResetMatrs && !State.GetNextGeomActive)
	{
		NProgram* pProg = GetCurRunProg();
		if (pProg == nullptr)
			return; // internal error
		int HCadr = GetState()->GetCurHCadrAbs() + 1;// +
		// find current cadr original geom
		NCadrGeom* pCompGeom = pProg->GetCompGeomArray(0).GetAt(HCadr);
		if (!(pCompGeom->GetType().flags & CU_TOUC))// Flag was removed in DoTouching
		{
			pProg->GetAddCoord(pCompGeom->GetMSHistNum(), State.AddCoords);
			State.TouchCadr = false; // Touch cadr is right already. MSHistory has been changed. We don't need to recalculate it. Used in Unit::RunSteps
		}
	}
	//MTMachineTool& Mach = GetMachTool();
	//Mach.ProcCadrStep(St, GetChannel().NGetProgramListCn()->HaveCompTraj());
}

BOOL NCUnit::CheckPolar()
{
	if (!State.PolarInterp.IsActive())
		return TRUE;
	if (!State.PolarInterp.GetFanucStyle())
		return TRUE;
	Error = 1177;// level 1
	return FALSE;
}

BOOL NCUnit::GetMiscKey(CString &text, CString &key)
{
	key.Empty();
	CString code;
	code = text.SpanIncluding("0123456789.");
	// remove code from text
	text.Delete(0, code.GetLength());
	if(code.GetLength() == 0)
	{
		CString val;
		if (GetOtherValue("ExprMCode", val))
		{
			if (val == "Yes")
			{
				double dcode;
				if (!GetDIValue(text, &dcode))
				{
					if (StoreError(1061))
						return FALSE;
					else
						return TRUE;
				}
				code.Format("%d", int(dcode + 0.5));
			}
		}
		else
		{
			if (StoreError(1061))
				return FALSE;
			else
				return TRUE;
		}
	}
	// delete leading zero
	if(code[0] == '0' && code.GetLength()>1)
		code.Delete(0);
	POSITION pos;
	CString val;
	BOOL Found = FALSE;
	for( pos = GCD.SMiscFuncDef.GetStartPosition(); pos != NULL; )
	{
		GCD.SMiscFuncDef.GetNextAssoc( pos, key, val);
		if(code == val)
		{
			Found = TRUE;
			break;
		}
	}
	if(!Found)
	{
		key.Empty();
		CString str("No");
		GCD.SOthers.Lookup("IgnoreUnknownM", str);
		if(str != "Yes")
		{
			if (StoreError(1062))
				return FALSE;
			else
				return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	return TRUE;
}
BOOL NCUnit::ProcessMiscFunction(CString &text)
{
	CString key;
	
	if(!GetMiscKey(text, key))
		return FALSE;// Errod code defined in GetMiscKey

	if(key.IsEmpty())
		return TRUE;
	
	if(key == "ToolChange")
	{
		CString PreSelect = { "No" };
		GCD.SOthers.Lookup("ToolPreSelect", PreSelect);
		int Tpos;
		CString T;
		if(!GCD.SWordDef.Lookup("ToolCode",T))
		{  
			Error = 1063;// level 1
			return FALSE;
		}

		if((Tpos = text.Find(T[0])) >= 0)
		{// Tool code after M6
			CString tmp = text.Right(text.GetLength()-Tpos-1);
			ProcessToolCode(tmp);
		}
		if(PreSelect == "Yes")
		{
			if(State.NextToolNum >= 0)
				State.CurToolN.Set(State.NextToolNum, State.CurTurret);
			if(State.ThisCadrToolN >= 0)
				State.NextToolNum = State.ThisCadrToolN;
		}
		else // Don't pre select
		{
			if(State.ThisCadrToolN < 0)
			{
				if (StoreError(1064))
					return FALSE;
			}
			else
				State.CurToolN.Set(State.ThisCadrToolN, State.CurTurret);
		}
		State.ThisCadrToolN = -1;//Need new value
// Set D and H comp if needed
		CString toitn;
		if(GCD.SOthers.Lookup("ToolOffsetInToolNumber",toitn))
			if(toitn == "Yes")
			{
				int n = State.CurToolN.GetToolNum();
				double val;
				if(!State.DComp.Lookup(n,val))
				{
					StartState.DComp[n] = 0.;
					val = 0.;
				}
				State.CurDiamComp = val;
				ProcLengthCompOffset(n);
			}
		return TRUE;
	}
	if(key == "HhToolCallDef")
	{
// HEIDENHAIN special
//		CString NCType;
//		if(GCD.SGeneral.Lookup("Type", NCType))
//			if(NCType.MakeUpper() == "HEIDENHAIN")
			{
				CString DCOV(GCD.RegisterSymbol);
				if(DCOV[0] != '0')
				{
					text.Replace("G02", DCOV + "9201=-");
					text.Replace("G03", DCOV + "9201=");
					if(text.Replace("R", DCOV + "108=") <= 0)
					{
						text += "M8100" + DCOV + "108=0.5*" + DCOV + "8101";
					}
				}
				text.Delete(0);
				if(GCD.SWordDef.Lookup("ToolCode", DCOV))
				{
					if(isdigit(text[0]) || text[0] == '"')
						text = DCOV + text;
				}
				if(GCD.SWordDef.Lookup("HorizontalAxis", DCOV))
					text.Replace(DCOV, "G19");
				if(GCD.SWordDef.Lookup("VerticalAxis", DCOV))
					text.Replace(DCOV, "G18");
				if(GCD.SWordDef.Lookup("SpindleAxis", DCOV))
					text.Replace(DCOV, "G17");
			}
// End HEIDENHAIN special
		return TRUE;
	}
	if(key == "NonModalCycleCall")
	{
		State.NonModalCycleCall = true;
	}

	if(key == "SetNextTool")
	{
		int tn = State.CurToolN.GetToolNum();
		State.CurToolN.Set(tn + 1, State.CurTurret);
		return TRUE;
	}
	if(key == "SpindleStop")
	{
		State.Spindle[SPINDLE_MILL].SpindleDir = R_UNDEF;
		State.Spindle[SPINDLE_MILL].Speed = 0.;
		State.Spindle[SPINDLE_TURN].SpindleDir = R_UNDEF;
		State.Spindle[SPINDLE_TURN].Speed = 0.;
		return TRUE;
	}
	if(key == "SpindleStopMill")
	{
		State.Spindle[SPINDLE_MILL].SpindleDir = R_UNDEF;
		State.Spindle[SPINDLE_MILL].Speed = 0.;
		return TRUE;
	}
	if(key == "SpindleStopTurn")
	{
		State.Spindle[SPINDLE_TURN].SpindleDir = R_UNDEF;
		State.Spindle[SPINDLE_TURN].Speed = 0.;
		return TRUE;
	}
	if(key == "SpindleCW")
	{
		State.Spindle[SPINDLE_MILL].SpindleDir = R_CW;
		return TRUE;
	}
	if(key == "SpindleCCW")
	{
		State.Spindle[SPINDLE_MILL].SpindleDir = R_CCW;
		return TRUE;
	}
	if(key == "SpindleCWTurn")
	{
		State.Spindle[SPINDLE_TURN].SpindleDir = R_CW;
		return TRUE;
	}
	if(key == "SpindleCCWTurn")
	{
		State.Spindle[SPINDLE_TURN].SpindleDir = R_CCW;
		return TRUE;
	}
	if(key == "CoolantOff")
	{
		State.Coolant = COOLOFF;
		return TRUE;
	}
	if(key == "CoolantMist")
	{
		State.Coolant = MIST;
		return TRUE;
	}
	if(key == "CoolantFlood")
	{
		State.Coolant = FLOOD;
		return TRUE;
	}
	if(key == "Stop")
	{
		State.ProgStop = true;
		return TRUE;
	}
	if(key == "OptionalStop")
	{
		State.OptionalStop = true;
		return TRUE;
	}
	if(key == "FixX")
	{
		State.FixX = TRUE;
		return TRUE;
	}
	if(key == "UnFixX")
	{
		State.FixX = FALSE;
		return TRUE;
	}
	if(key == "FixY")
	{
		State.FixY = TRUE;
		return TRUE;
	}
	if(key == "UnFixY")
	{
		State.FixY = FALSE;
		return TRUE;
	}
	if(key == "FixZ")
	{
		State.FixZ = TRUE;
		return TRUE;
	}
	if(key == "UnFixZ")
	{
		State.FixZ = FALSE;
		return TRUE;
	}
	if(key == "FixA")
	{
		State.FixA = TRUE;
		return TRUE;
	}
	if(key == "UnFixA")
	{
		State.FixA = FALSE;
		return TRUE;
	}
	if(key == "FixB")
	{
		State.FixB = TRUE;
		return TRUE;
	}
	if(key == "UnFixB")
	{
		State.FixB = FALSE;
		return TRUE;
	}
	if(key == "FixC")
	{
		State.FixC = TRUE;
		return TRUE;
	}
	if(key == "UnFixC")
	{
		State.FixC = FALSE;
		return TRUE;
	}
	if(key == "EndProgram")
	{
		State.EndProgram = true;
		State.Spindle[SPINDLE_MILL].SpindleDir = R_UNDEF;
		State.Spindle[SPINDLE_MILL].Speed = 0.;
		State.Spindle[SPINDLE_TURN].SpindleDir = R_UNDEF;
		State.Spindle[SPINDLE_TURN].Speed = 0.;
		return TRUE;
	}
	if(key == "SubroutineCallEnd")
	{
		return ProcessSubrCallEnd(text);
	}
	if(key == "SubroutineCall")
	{
		return ProcessSubrCall(text, false);
	}
	if(key == "FileCall")
	{
		return ProcessFileCall(text);
	}
	if(key == "SubroutineCallN")
	{
		return ProcessSubrCall(text, true);
	}
	if(key == "WhileStart")
	{
		int count = 0;
		cadrID p = State.CadrID;
		State.WhileStack.Push(p, count);
		text = "IN" + text;
		ProcessIF(text, TRUE);
		if(text.GetLength() > 0)// While cycle is over
			State.WhileStack.Pop(p, count);
		return TRUE;
	}
	if (key == "ForStartSinum")
	{
		return ProcessForStartSinum(text);// Error defined in ProcessForStartSinum
	}
	if (key == "LoopStartSinum")
	{
		return ProcessLoopStartSinum(text);// Error defined in ProcessLoopStartSinum
	}
	if (key == "IfSinum")
	{
		return ProcessIfSinum(text);// Error defined in ProcessIfSinum
	}
	if (key == "ElseSinum")
	{
		return ProcessElseSinum(text);// Error defined in ProcessElseSinum
	}
	if (key == "ElseIfSinum")
	{
		return ProcessElseIfSinum(text);// Error defined in ProcessElseIfSinum
	}
	if (key == "WhileStartSinum")
	{
		return ProcessWhileStartSinum(text);// Error defined in ProcessWhileStartSinum
	}
	if (key == "RepeatSinum")
	{
		return ProcessRepeatSinum();// Error defined in ProcessRepeatSinum
	}
	if (key == "UntilSinum")
	{
		return ProcessUntilSinum(text);// Error defined in ProcessUntilSinum
	}
	if (key == "ForStart")
	{//  ForRegister ForStartValue ForEndValue
		int ForRegister = 0;
		if(!GetIParamVal(text, _T("ForRegister"), &ForRegister))
		{
			Error = 1191;// level 1
			return FALSE;
		}
		double ForEndValue = 0;
		if(!GetDParamVal(text, _T("ForEndValue"), &ForEndValue))
		{
			Error = 1191;// level 1
			return FALSE;
		}
		int count = 0;
		cadrID p;
		bool First = (State.WhileStack.Pop(p, count) < 0);
		if(!First)
		{
			if(p != State.CadrID)
			{
				First = true;
				State.WhileStack.Push(p, count);
			}
		}
		if(First)
		{
			double ForStartValue = 0;
			if(!GetDParamVal(text, _T("ForStartValue"), &ForStartValue))
			{
				Error = 1191;// level 1
				return FALSE;
			}
			if(ForEndValue <= ForStartValue)
			{
				Error = 1191;// level 1
				return FALSE;
			}
			State.SetRegister(ForRegister, ForStartValue);
			p = State.CadrID;
			count = ForRegister;
		}
		text.Empty();
		if(State.GetRegister(ForRegister) + 1 > ForEndValue - 1.e-8)
			count *= -1; // Last cycle body

		State.WhileStack.Push(p, count);
		return TRUE;
	}
	if(key == "SubroutineEnd")
	{
		return ReturnFromSub(FALSE);
	}
	if(key == "SubroutineEndCont")
	{
		return ReturnFromSub(TRUE);
	}
	if(key == "AutoCycleEnd")
	{
		int count;
		cadrID p;
		if(State.CycleStack.Pop(p,count)<0)
		{
			return TRUE;
		}
		count--;
		if(count > 0)// Cycle is not over
		{
			State.OrderChanged = true;
			State.NextCadr = p;
			State.CycleStack.Push(p,count);
		}
		return TRUE;
	}
	if(key == "WhileEnd")
	{
		int count;
		cadrID p;
		if(State.WhileStack.Pop(p,count)<0)
		{
			return TRUE;
		}
		State.OrderChanged = true;
		State.NextCadr = p;
		return TRUE;
	}
	if (key == "ForEnd")
	{
		int count;
		cadrID p;
		if (State.WhileStack.Pop(p, count) < 0)
		{
			return TRUE;
		}
		bool Last = (count < 0);
		count = abs(count);
		State.SetRegister(count, State.GetRegister(count) + 1.);
		if (Last)
			return TRUE;
		State.WhileStack.Push(p, count);
		State.OrderChanged = true;
		State.NextCadr = p;
		return TRUE;
	}
	if (key == "CycleEndSinum")
	{
		return ProcessCycleEndSinum();
	}
	if(key == "GoTo")
	{
		return DoGoTo(text, SD_FORBACK);
	}
	if(key == "GoToForward")
	{
		return DoGoTo(text, SD_FOR);
	}
	if(key == "GoToBackward")
	{
		return DoGoTo(text, SD_BACK);
	}
	if(key == "GoToProg")
	{
		State.OrderChanged = true;
		cadrID tmp;
		if(!FindNextCadr(text, &tmp, false))
			return FALSE; // Error defined in FindNextCadr
		State.NextCadr = tmp;
		return TRUE;
	}
	if(key == "GoToNext")
	{
		BOOL b = DoGoTo(text, SD_FORBACK);
		if(b)
			State.NextCadr.Cadr++;
		return b;
	}
	if(key == "GoToNextForward")
	{
		BOOL b = DoGoTo(text, SD_FOR);
		if(b)
			State.NextCadr.Cadr++;
		return b;
	}
	if (key == "GoToNextBackward")
	{
		BOOL b = DoGoTo(text, SD_BACK);
		if (b)
			State.NextCadr.Cadr++;
		return b;
	}
	if (key == "GoToNextGroupEnd")
	{
		BOOL b = DoGoToGroupEnd(State.CadrID);
		if (b)
			State.NextCadr.Cadr++;
		return b;
	}
	if(key == "ThreadCut")
	{
		State.ThreadCadr = true;
		return TRUE;
	}
	if(key == "StoreToolParams")
	{
		StoreActiveToolStdParams();
		return TRUE;
	}
	if(key == "DebugOn")
	{
		State.SetRegister(8999, 1.);
		return TRUE;
	}
	if(key == "DebugOff")
	{
		State.SetRegister(8999, 0.);
		return TRUE;
	}
	if(key == "GrooveOn")
	{
		State.GrooveON = true;
		return TRUE;
	}
	if(key == "GrooveOff")
	{
		State.GrooveON = false;
		return TRUE;
	}
	// Polar helix
	if(key == "PolarHelix")
	{
		State.PolarHelix = true;
		return TRUE;
	}
	if(key == "ClearAllRegisters")
	{
		State.ResetRegisters();
		return TRUE;
	}
	// RCP
	if(key == "RCPOn")
	{
		return ProcessRCPOn(text) ? TRUE : FALSE;
	}
	if(key == "RCPOff")
	{
		State.RCPCanceling = true;
		State.RCPActive = false;
		State.RCPStartABC.Set(0., 0., 0., 1.);
		State.RCPMask = 0;
		return TRUE;
	}
	if (key == "ErrorMessage")
	{
		TempError.SetErrText(text);
		TempError.SetErrNum(1155);
		if (StoreError(1155))
			return FALSE;
		else
			return TRUE;
	}
	// Rotation type

	//Linear,
	//	ShortestDistance,
	//	AlwaysCW,
	//	AlwaysCCW,
	//	PositiveCW,
	//	PositiveCCW,
	//	DoNotChange

	if (key == "TRTLinear")
	{
		State.RotType = Linear;
		return TRUE;
	}
	if (key == "TRTShortestDistance")
	{
		State.RotType = ShortestDistance;
		return TRUE;
	}
	if (key == "TRTAlwaysCW")
	{
		State.RotType = AlwaysCW;
		return TRUE;
	}
	if (key == "TRTAlwaysCCW")
	{
		State.RotType = AlwaysCCW;
		return TRUE;
	}
	if (key == "TRTPositiveCW")
	{
		State.RotType = PositiveCW;
		return TRUE;
	}
	if (key == "TRTPositiveCCW")
	{
		State.RotType = PositiveCCW;
		return TRUE;
	}
	if (key == "TRTDoNotChange")
	{
		State.RotType = DoNotChange;
		return TRUE;
	}
	if (key == "RestartLifeTime")
	{
		NToolCombined *pTool = GetActiveTool();
		double ToolLifeTime = 0.;
		if (pTool)
			ToolLifeTime = pTool->GetLifeTime();
		State.RestartLifeTime(ToolLifeTime);
	}
	if (key == "RestartRotAngles")
	{
		St.MashA = Fit360(St.MashA);
		State.MashA = Fit360(State.MashA);
		St.MashB = Fit360(St.MashB);
		State.MashB = Fit360(State.MashB);
		St.MashC = Fit360(St.MashC);
		State.MashC = Fit360(State.MashC);
		return TRUE;
	}
	if (key == "VariablesDecl")
	{
		if (!ProcessVariablesDecl(text, false))
			return FALSE;
		return TRUE;
	}
	if (key == "VariablesLocDecl")
	{
		if (!ProcessVariablesDecl(text, true))
			return FALSE;
		return TRUE;
	}
	if (key == "DefinesDecl")
	{
		if (!ProcessDefinesDecl(text))
			return FALSE;
		return TRUE;
	}
	if (key == _T("ProbeExpTouch"))
	{
		State.ProbeExpTouch = true;
		return TRUE;
	}
	if (key == _T("ProbeExpNoTouch"))
	{
		State.ProbeExpNoTouch = true;
		return TRUE;
	}
	if (key == _T("ReadMTSpecPos"))
	{
		return ProcessReadMTSpecPos(text);
	}
	if (key == _T("ReadMTSpecPos4Tool"))
	{
		return ProcessReadMTSpecPos4Tool(text);
	}
	if (key == _T("StoreStockParams"))
	{
		return	StoreStockStdParams(text);
	}
	if (key == _T("StoreAddCoord"))
	{
		return	StoreAddCoord(text);
	}
	if (key == "TouchFor")
	{
		State.TouchCadr = true;
		State.ProbeCollision = true;
		return TRUE;
	}
	if (key == "TouchBack")
	{
		State.TouchCadr = true;
		State.ProbeCollision = false;
		return TRUE;
	}
	if (key == "MTReposBranch")
	{
		if (State.MachLink.IsIndW())
		{
			Error = 1182;// level 1
			return FALSE;
		}
		State.MachLink.SetIndW1();// Need to be set
		return TRUE;
	}
	if (key == "MTSwapAxesNames")
	{
		if (State.MachLink.IsIndW())
		{
			Error = 1182;// level 1
			return FALSE;
		}
		State.MachLink.SetIndW2();// Need to be set
		return TRUE;
	}
	if (key == "MTReposStock")
	{
		return ProcessReposStock(text);
	}
	if (key == "MTReposStockByGr")
	{
		return ProcessReposStockByGr(text);
	}
	if (key == _T("SyncEvent") || key == _T("SyncEventRem") || key == _T("SyncWait"))
	{
		return ProcSyncCheckPoint(key, text);
	}

	return TRUE;
}

BOOL NCUnit::ProcSyncCheckPoint(const CString& key, CString& text)
{
	NCheckPoint CheckPoint;
	if (key == _T("SyncEvent"))
		CheckPoint.SetEvent();
	else if (key == _T("SyncEventRem"))
		CheckPoint.SetEventRem();
	else if (key == _T("SyncWait"))
		CheckPoint.SetWait();
	else
		return FALSE; // internal error
	
	char First = '{';
	char Last = '}';
	if (text.GetLength() < 3)
	{
		Error = 1196;// level 1
		return FALSE;
	}
	if (text.GetAt(0) != First)
	{
		Error = 1196;// level 1
		return FALSE;
	}
	CString NewText;
	int ChanInd = 0;
	for (int ParInd = 0; ParInd < 4; ++ParInd)
	{
		text.Delete(0);
		if (text.IsEmpty())
		{
			Error = 1192;// level 1
			return FALSE;
		}
		CString ExStr(CString(_T(",")) + CString(Last));
		CString ParValue = text.SpanExcluding(ExStr);
		text.Delete(0, ParValue.GetLength());
		switch (ParInd)
		{
		case 0://a channel index for SyncID for Wait
			ChanInd = atoi(ParValue);
			break;
		case 1://Priority
			CheckPoint.SetPriority(atoi(ParValue));
			break;
		case 2://IsBefore
			if (atoi(ParValue) == 0)
				CheckPoint.SetBefore();
			else
				CheckPoint.SetAfter();
			break;
		case 3://SyncID.Value
			if (!CheckPoint.IsWait())
				ChanInd = GetChannelInd();
			if (ParValue.IsEmpty())
			{
				Error = 1193;// level 1
				return FALSE;
			}
			if (ParValue.GetAt(0) == '"')
				CheckPoint.SetID(NCheckPoint::NSyncID(ChanInd, ParValue.Mid(1, ParValue.GetLength() - 2)));
			else
				CheckPoint.SetID(NCheckPoint::NSyncID(ChanInd, atoi(ParValue)));
			break;
		default:// internal error
			return FALSE;
		}
	}
	if (text.GetAt(0) != Last)
	{
		Error = 1194;// level 1
		return false;
	}
	text.Delete(0);
	CheckPoint.SetChanInd(GetChannelInd());
	CheckPoints.push_back(CheckPoint);
	return TRUE;
}

BOOL NCUnit::ProcessText(CString &text)
{
// remove all leading digits from text
	int l = text.SpanIncluding("-0123456789.").GetLength();
	text.Delete(0,l);
	return TRUE;
}

BOOL NCUnit::ReplaceVariables(CString &text)
{
	if (GCD.VariableSymbol != '\0')
	{
		for (int pos = text.Find(GCD.VariableSymbol); pos >= 0; pos = text.Find(GCD.VariableSymbol))
		{
			CString buf = text;
			buf.Delete(0, pos + 1);
			if (!ProcessVariable(buf, false))
				return FALSE;
			text = text.Left(pos) + buf;
		}
	}
	if (GCD.VariableSymbolLoc != '\0' && GCD.VariableSymbolLoc != GCD.VariableSymbol)
	{
		for (int pos = text.Find(GCD.VariableSymbolLoc); pos >= 0; pos = text.Find(GCD.VariableSymbolLoc))
		{
			CString buf = text;
			buf.Delete(0, pos + 1);
			if (!ProcessVariable(buf, true))
				return FALSE;
			text = text.Left(pos) + buf;
		}
	}
	return TRUE;
}

BOOL NCUnit::ProcessVariable(CString &text, bool LocalVar)
{
	// Find end
	CString End;
	if (!GCD.SWordDef.Lookup(_T("VariableEnd"), End))
		return FALSE;
	CString Start;
	if (!GCD.SWordDef.Lookup(LocalVar ? _T("VariablePrefixLoc") : _T("VariablePrefix"), Start))
		return FALSE;
	int count = 1;
	int i = 0;
	for (; i < text.GetLength() && count != 0; ++i)
	{
		if (text[i] == End)
			--count;
		else if (text[i] == Start)
			++count;
	}
	if (count != 0)
		return FALSE;
	CString Rest = text.Right(text.GetLength() - 1 - i);
	text = text.Left(i + 1);
	NVarSet& VarSet = GetState()->GetVarSet();
	for (int spos = -1; ; )
	{
		int epos = text.Find(End, spos + 1);
		if (epos < 0)
			return false;
		int bpos = text.Find(_T("{"), spos + 1);
		if (bpos < 0)
		{// simple variable
			CString part = text.Mid(spos + 1, epos - spos - 1);
			text.Delete(spos + 1, epos - spos);
			int Index = VarSet.StoreName(part.GetBuffer(), LocalVar);
			CString RegCode;
			RegCode.Format("%d", Index);
			text.Insert(spos + 1, GCD.RegisterSymbol + RegCode);
		}
		else
		{// array element
			CString part = text.Mid(spos + 1, bpos - spos - 1);
			text.Delete(spos + 1, bpos - spos);
			// name{ -> #[ind+[
			int Index = VarSet.StoreArrName(part.GetBuffer(), LocalVar);
			CString RegCode;
			RegCode.Format("%d", Index);
			CString Ins(GCD.RegisterSymbol);
			Ins += CString(NEvalExp::OpenBC()) + RegCode + _T("+") + CString(NEvalExp::OpenBC());
			text.Insert(spos + 1, Ins);
		}
		spos = text.Find(Start);
		if (spos < 0)
			break;
		else
		{
			text.Delete(spos, 1);
			--spos;
		}
	}
	text.Replace(_T("}") + End, CString(NEvalExp::CloseBC()) + CString(NEvalExp::CloseBC()));
	text += Rest;
	return TRUE;
}

BOOL NCUnit::ProcessCoord(CString &text, const CString &CurWord, int ChangedMode/* = 0*/)
{
	// ChangedMode used for circle center coords only
	// if ChangedMode = 0 ordinary processing
	// if ChangedMode = -1 process as Incremental
	// if ChangedMode = 1 process as Absolute
	double val=0;
	if(!GetDValue(text,&val))
	{
		if (StoreError(1029))
			return FALSE;
		else
			return TRUE;
	}
	if(State.ZeroPreset && CurWord == "CircleRadius")
	{
		State.MirrorRotate.SetAngle(val);
		return TRUE;
	}
	val *= GCD.LinearScale;

	if(GCD.LinearDecimalZero > 0.)
		if(fabs(val) <= GCD.LinearDecimalZero)
			val = 0.;

	switch(CurWord[0])
	{
	case 'H':
//	if(	CurWord == "HorizontalAxis" || CurWord == "HorizontalAxisInc")// X-coord
	{
		if(State.LeadingAxis < 0)
			State.LeadingAxis = 0;
		State.ChangedCoord.x = TRUE;
		val *= GCD.XScale * State.CurXScale;
		if(State.ZeroPreset) // G92 is active
		{
			if(State.ZeroPresetType == ZPT_REL)
				State.CurBase.x0 = State.MashX - val;
			else if(State.ZeroPresetType == ZPT_SHT)
				State.CurBase.x0 += val;
			else
			{
				if(State.AbsoluteCoord)
					State.CurBase.x0 = State.GetActiveUCS().x0 + val;
				else
					State.CurBase.x0 += val;
			}
		}
		else
		{
			if(State.FixX && !State.MoveRotateCS && !State.MoveRotateCSRel)
			{
				if (StoreError(1011))
					return FALSE;
			}
 			bool Used = false;
			if(State.PolarCoordActive)
			{
				switch(State.CurPlane)
				{
				case XY:
					Used = true;
					State.PolarDist = val;
					break;
				case XZ:
					Used = true;
					State.PolarAngle = val;
					break;
				}
			}
			if(!Used)
			{
				if(State.AbsoluteCoord)
					State.CurX = val + State.CurBase.x0 - State.MashX;
				else
					State.CurX = val;
				State.MashX += State.CurX;
			}
		}
		return TRUE;
	}
	break;
	case 'V':
//	if(	CurWord == "VerticalAxis" || CurWord == "VerticalAxisInc")// Y-coord
	{
		if(State.LeadingAxis < 0)
			State.LeadingAxis = 1;
		State.ChangedCoord.y = TRUE;
		val *= GCD.YScale * State.CurYScale;
		if(State.ZeroPreset) // G92 is active
		{
			if(State.ZeroPresetType == ZPT_REL)
				State.CurBase.y0 = State.MashY - val;
			else if (State.ZeroPresetType == ZPT_SHT)
				State.CurBase.y0 += val;
			else
			{
				if(State.AbsoluteCoord)
					State.CurBase.y0 = State.GetActiveUCS().y0 + val;
				else
					State.CurBase.y0 += val;
			}
		}
		else
		{
			if (State.FixY && !State.MoveRotateCS && !State.MoveRotateCSRel)
			{
				if (StoreError(1012))
					return FALSE;
			}
			bool Used = false;
  			if(State.PolarCoordActive)
			{
				switch(State.CurPlane)
				{
				case XY:
					Used = true;
					State.PolarAngle = val;
					break;
				case YZ:
					Used = true;
					State.PolarDist = val;
					break;
				}
			}
			if(!Used)
			{
				if(State.AbsoluteCoord)
					State.CurY = val + State.CurBase.y0 - State.MashY;
				else
					State.CurY = val;
				State.MashY += State.CurY;
			}
		}
		return TRUE;
	}
	break;
	case 'S':
//	if(	CurWord == "SpindleAxis" ||	CurWord == "SpindleAxisInc")// Z-coord
	{
		if(State.LeadingAxis < 0)
			State.LeadingAxis = 2;
		State.ChangedCoord.z = TRUE;
		val *= GCD.ZScale * State.CurZScale;
		if(State.ZeroPreset) // G92 is active
		{
			if(State.ZeroPresetType == ZPT_REL)
				State.CurBase.z0 = State.MashZ - val;
			else if (State.ZeroPresetType == ZPT_SHT)
				State.CurBase.z0 += val;
			else
			{
				if(State.AbsoluteCoord)
					State.CurBase.z0 = State.GetActiveUCS().z0 + val;
				else
					State.CurBase.z0 += val;
			}
		}
		else
		{
			if (State.FixZ && !State.MoveRotateCS && !State.MoveRotateCSRel)
			{
				if (StoreError(1013))
					return FALSE;
			}
			bool Used = false;
  			if(State.PolarCoordActive)
			{
				switch(State.CurPlane)
				{
				case XZ:
					Used = true;
					State.PolarDist = val;
					break;
				case YZ:
					Used = true;
					State.PolarAngle = val;
					break;
				}
			}
			if(!Used)
			{
				if(State.AbsoluteCoord)
					State.CurZ = val + State.CurBase.z0 - State.MashZ;
				else
					State.CurZ = val;
				State.MashZ += State.CurZ;
			}
		}
		return TRUE;
	}
	break;
	}
	if(	CurWord == "CircleRadius")// R
	{
		State.CurR = val;
		State.ChangedCoord.r = TRUE;
		return TRUE;
	}

	BOOL mabs = IsArcCenterAbs();

	if(	CurWord == "CircleHorzCenter")// I-coord
	{
		val *= GCD.IScale * State.CurXScale * State.CurIScale;
		if(mabs && (ChangedMode == -1))
		{
			val += St.MashX;
		}
		else if(!mabs && (ChangedMode == 1))
		{
			val += State.CurBase.x0 - St.MashX;
		}
		else 
		{ // Ordinary processing
			if(mabs)
				val += State.CurBase.x0;
		}
		State.CurI = val;

		State.ChangedCoord.i = TRUE;
		return TRUE;
	}
	if(	CurWord == "CircleVerticalCenter")// J-coord
	{
		val *= GCD.JScale * GCD.YScale * State.CurYScale * State.CurJScale;
		if(mabs && (ChangedMode == -1))
		{
			val += St.MashY;
		}
		else if(!mabs && (ChangedMode == 1))
		{
			val += State.CurBase.y0 - St.MashY;
		}
		else 
		{ // Ordinary processing
			if(mabs)
				val += State.CurBase.y0;
		}
		State.CurJ = val;
		State.ChangedCoord.j = TRUE;
		return TRUE;
	}
	if(	CurWord == "CircleDepthCenter")// K-coord
	{
		val *= GCD.KScale * GCD.ZScale * State.CurZScale * State.CurKScale;
		if(mabs && (ChangedMode == -1))
		{
			val += St.MashZ;
		}
		else if(!mabs && (ChangedMode == 1))
		{
			val += State.CurBase.z0 - St.MashZ;
		}
		else 
		{ // Ordinary processing
			if(mabs)
				val += State.CurBase.z0;
		}
		State.CurK = val;
		State.ChangedCoord.k = TRUE;
		return TRUE;
	}
	// Polar coord
	if(	CurWord == "PolarDist")
	{
		State.PolarDist = val;
		State.ApplyPolarCoord();
		State.CurX = State.MashX - St.MashX;
		State.CurY = State.MashY - St.MashY;
		State.CurZ = State.MashZ - St.MashZ;
		return TRUE;
	}
	if(	CurWord == "PolarAngle")
	{
		State.PolarAngle = val;
		State.ApplyPolarCoord();
		State.CurX = State.MashX - St.MashX;
		State.CurY = State.MashY - St.MashY;
		State.CurZ = State.MashZ - St.MashZ;
		return TRUE;
	}
	if(	CurWord == "PolarAngleInc")
	{
		if(State.PolarHelix)
		{
			int A = int(fabs(val));
			State.CircleFullTurns = A / 360;
			if(A % 360 == 0 && State.CircleFullTurns > 0)
				--State.CircleFullTurns;
		}
		State.PolarAngle += val;
		State.ApplyPolarCoord();
		State.CurX = State.MashX - St.MashX;
		State.CurY = State.MashY - St.MashY;
		State.CurZ = State.MashZ - St.MashZ;
		return TRUE;
	}
	return TRUE;
}

void NCUnit::ClearConf()
{
	GCDV.IsDefined = false;
	GCDV.SCycleDef.RemoveAll();
	GCDV.SGeneral.RemoveAll();
	GCDV.SMiscFuncDef.RemoveAll();
	GCDV.SOthers.RemoveAll();
	GCDV.SWordDef.RemoveAll();
	GCDV.SWordDefInv.RemoveAll();
	GCDV.SWordRep.RemoveAll();
	GCDV.SWordOrder.RemoveAll();
	GCDV.SRegIndexes.RemoveAll();
	GCDV.ReplaceSet.Clear();
	
	struct FDef *pFDef;
	CString key;
	for(POSITION pos = GCD.SFuncDef.GetStartPosition();pos != NULL;)
	{
		GCD.SFuncDef.GetNextAssoc(pos,key,pFDef);
		delete pFDef;
	}
	GCDV.SFuncDef.RemoveAll();
	GCDV.SFuncDefInv.RemoveAll();
}

bool NCUnit::PrepUnitSerialiseAll()
{
	bool SerializeAll = false;
	if (NCM_PROJECT.UnitConfs.GetSize() > GetChannelInd())
	{
		if (!NCM_PROJECT.UnitConfs[GetChannelInd()].IsEmpty())
		{
			SerializeAll = true;
			SetTempUnit();
		}
	}
	return SerializeAll;
}

int NCUnit::LoadConfFile(std::vector<CString>& ErrParams, bool LoadFromSerializeAll)
{// 	static_cast<CMainFrame *>(AfxGetMainWnd())->SetNCUnitName(GCD.ConfFileName);
// after call LoadConfFile
	ErrParams.clear();
	GetUsedUCSs().clear();
	StartState.UCSs.ClearUsed();
	StartState.UCSs.ClearNames();
	NUCSs::MaxUcsInd = 0;
	CArchive* pA;
	int Ret = ReadSGeneral(pA, ErrParams);
	if (Ret < 0)
		return Ret;
	CArchive& f = *pA;
	bool CheckSyntax = false;
	CString YN;
	if (GCDV.SGeneral.Lookup(_T("CheckSyntax"), YN))
		if(YN == _T("Yes"))
			CheckSyntax = true;
	// Load SWordRep
	CString Start;
	Start.LoadString(IDS_SECT_WORDREP);
	CString End;
	End.LoadString(IDS_SECT_END);
	CString str;
	if (str != Start)
	{ // Previous section may be absent
		f.ReadString(str);// empty string
		f.ReadString(str);
		if (str != Start)
		{
			ErrParams.push_back(str);
			ErrParams.push_back(Start);
			return -4;
		}
	}
	CString key;
	CString val;
	while(1)
	{
		if(!f.ReadString(str) && CheckSyntax)
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if(str == End)
			break;
		if (str.Left(2) == _T("//")) // comment string
			continue;
		CString Type = GetField(str);
		int Priority = 0;
		if(!Type.IsEmpty())
			if (Type[0] == '#')
			{
				Type.Delete(0, 1);
				Priority = atoi(Type);
				Type = GetField(str);
			}
		if (Type == _T("RegExp"))
		{
			key = GetField(str);
			if (key.GetLength() <= 0 && CheckSyntax)
			{
				ErrParams.push_back(Start);
				return -8;
			}
			val = GetField(str);
			GCDV.ReplaceSet.AddReplacePair(Priority, key, val);
		}
		else if (Type == _T("Replace"))
		{
			key = GetField(str);
			if (key.GetLength() <= 0 && CheckSyntax)
			{
				ErrParams.push_back(Start);
				return -8;
			}
			val = GetField(str);
			GCDV.SWordOrder.Add(key);
			GCDV.SWordRep.Add(val);
		}
		else if (Type == _T("RegExp1"))
		{
			CString First;
			if (str.GetLength() < 1 && CheckSyntax)
				return -16;
			if (str[str.GetLength() - 1] != '\"' && CheckSyntax)
				return -16;
			if (str.GetLength() > 0)
				First = str.Left(str.GetLength() - 1);
			if (!f.ReadString(str) && CheckSyntax)
			{
				ErrParams.push_back(Start);
				return -2;
			}
			Type = GetField(str);
			if (Type != _T("RegExp2") && CheckSyntax)
				return -10;
			if (str.GetLength() < 1 && CheckSyntax)
				return -16;
			if (str[str.GetLength() - 1] != '\"' && CheckSyntax)
				return -16;
			if (str.GetLength() > 0)
				GCDV.ReplaceSet.AddReplacePair(Priority, First, str.Left(str.GetLength() - 1));
		}
	}
	struct AxisFeedScale
	{
		CString AxisName;
		double Feed;
		double Scale;
	};
	std::map<CString, AxisFeedScale> MapAFC;
	int Format = 1;
	if (LookupSGeneral(_T("Format"), val))
		Format = atoi(val);
	CMapStringToString TempWordDef;
	if(Format >= 2)
	{
		// Load Coord Def
		Start.LoadString(IDS_SECT_COORDDEF);
		f.ReadString(str);// empty string
		f.ReadString(str);
		if (str != Start && CheckSyntax)
		{
			ErrParams.push_back(str);
			ErrParams.push_back(Start);
			return -4;
		}
		while (1)
		{
			if (!f.ReadString(str) && CheckSyntax)
			{
				ErrParams.push_back(Start);
				return -2;
			}
			if (str == End)
				break;
			if (str.Left(2) == _T("//")) // comment string
				continue;
			key = GetField(str);
			if (key.GetLength() <= 0 && CheckSyntax)
			{
				ErrParams.push_back(Start);
				return -8;
			}
			val = GetField(str);// main
			if (val.IsEmpty() && CheckSyntax)
			{
				ErrParams.push_back(Start);
				ErrParams.push_back(key);
				return -9;
			}
			if (val != _T("_"))
			{
				if (!val.IsEmpty() && FUNC_NAMES.Find(val) >= 0 && CheckSyntax) // Reserved character used
				{
					ErrParams.push_back(Start);
					ErrParams.push_back(val);
					return -15;
				}
				CString b;
				if (TempWordDef.Lookup(key, b) && CheckSyntax)
				{
					ErrParams.push_back(Start);
					ErrParams.push_back(key);
					return -1;
				}
				TempWordDef[key] = val;
			}
			val = GetField(str);// Inc
			if (val.IsEmpty() && CheckSyntax)
			{
				ErrParams.push_back(Start);
				ErrParams.push_back(key);
				return -9;
			}
			if (val != _T("_"))
			{
				if (!val.IsEmpty() && FUNC_NAMES.Find(val) >= 0 && CheckSyntax) // Reserved character used
				{
					ErrParams.push_back(Start);
					ErrParams.push_back(val);
					return -15;
				}
				CString keyInc = key + _T("Inc");
				CString b;
				if (TempWordDef.Lookup(keyInc, b) && CheckSyntax)
				{
					ErrParams.push_back(Start);
					ErrParams.push_back(keyInc);
					return -1;
				}
				TempWordDef[keyInc] = val;
			}
			val = GetField(str);// Abs
			if (val.IsEmpty() && CheckSyntax)
			{
				ErrParams.push_back(Start);
				ErrParams.push_back(key);
				return -9;
			}
			if (val != _T("_"))
			{
				if (!val.IsEmpty() && FUNC_NAMES.Find(val) >= 0 && CheckSyntax) // Reserved character used
				{
					ErrParams.push_back(Start);
					ErrParams.push_back(val);
					return -15;
				}
				CString keyAbs = key + _T("Abs");
				CString b;
				if (TempWordDef.Lookup(keyAbs, b) && CheckSyntax)
				{
					ErrParams.push_back(Start);
					ErrParams.push_back(keyAbs);
					return -1;
				}
				TempWordDef[keyAbs] = val;
			}
			val = GetField(str); // machine tool axis
			if (val.IsEmpty() && CheckSyntax)
			{
				ErrParams.push_back(Start);
				ErrParams.push_back(key);
				return -9;
			}
			AxisFeedScale AFC;
			AFC.AxisName = val;
			AFC.Scale = 1.;
			AFC.Feed = 5000.;
			val = GetField(str); // feed
			if (val.GetLength() > 0)
				AFC.Feed = atof(val);
			val = GetField(str); // scale
			if (val.GetLength() > 0)
				AFC.Scale = atof(val);
			MapAFC[key] = AFC;
		}
	}
	// Load GCD.SWordDef
	Start.LoadString(IDS_SECT_WORDDEF);
	f.ReadString(str);// empty string
	f.ReadString(str);
	if(str != Start)
	{
		ErrParams.push_back(str);
		ErrParams.push_back(Start);
		return -4;
	}
	for (auto i = TempWordDef.GetStartPosition(); i != NULL; )// preliminary load coordinates to ensure CheckSyntax
	{
		TempWordDef.GetNextAssoc(i, key, val);
		GCDV.SWordDef[key] = val;
		GCDV.SWordDefInv[val] = key;
	}
	while(1)
	{
		if(!f.ReadString(str))
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if(str == End)
			break;
		if (str.Left(2) == _T("//")) // comment string
			continue;
		key = GetField(str);
		if(key.GetLength() <= 0 && CheckSyntax)
		{
			ErrParams.push_back(Start);
			return -8;
		}
		val = GetField(str);
		if (val.IsEmpty() && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -9;
		}
		if(!val.IsEmpty() && FUNC_NAMES.Find(val) >= 0 && CheckSyntax) // Reserved character used
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(val);
			return -15;
		}
		CString b;
		if (GCD.SWordDef.Lookup(key, b) && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -1;
		}
		GCDV.SWordDef[key] = val;
		GCDV.SWordDefInv[val]=key;
	}
	for (auto i = TempWordDef.GetStartPosition(); i != NULL; )// reload coordinates to ensure its priority
	{
		TempWordDef.GetNextAssoc(i, key, val);
		GCDV.SWordDef[key] = val;
		GCDV.SWordDefInv[val] = key;
	}
// Load GCD.SFuncDef
	Start.LoadString(IDS_SECT_FUNCDEF);
	f.ReadString(str);// empty string
	f.ReadString(str);
	if(str != Start)
	{
		ErrParams.push_back(str);
		ErrParams.push_back(Start);
		return -4;
	}
	const auto BaseLength = CString(_T("WorkCoordSys")).GetLength();
	struct FDef *pFDef;
	while (1)
	{
		if (!f.ReadString(str))
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if (str == End)
			break;
		if (str.Left(2) == _T("//")) // comment string
			continue;
		pFDef = new struct FDef;
		pFDef->Name = GetField(str);
		if (pFDef->Name.GetLength() <= 0 && CheckSyntax)
		{
			ErrParams.push_back(Start);
			return -8;
		}
		val = GetField(str);
		if (val.IsEmpty() && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -9;
		}
		pFDef->Num = val;
		val = GetField(str);
		if (val.IsEmpty() && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -9;
		}
		pFDef->Group = atoi(val);
		val = GetField(str);
		if (val.IsEmpty() && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -9;
		}
		if (val == "Y")
			pFDef->Flag = true;
		else if (val == "N")
			pFDef->Flag = false;
		else if(CheckSyntax)
			return -6;
		val = GetField(str);
		if (val == "Y")
			pFDef->ModalConcepts = true;
		else
			pFDef->ModalConcepts = false;
		while(!(val = GetField(str)).IsEmpty())
			pFDef->FConcepts += val + _T("\n");

		struct FDef *pBuf = nullptr;
		if (GCD.SFuncDef.Lookup(pFDef->Name, pBuf) && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(pFDef->Name);
			return -17;
		}
		GCDV.SFuncDef[pFDef->Name] = pFDef;
		if (GCD.SFuncDefInv.Lookup(pFDef->Num, pBuf) && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(pFDef->Num);
			return -18;
		}
		GCDV.SFuncDefInv[pFDef->Num] = pFDef;
		if (pFDef->Name.GetLength() > BaseLength)
			if (pFDef->Name.Left(BaseLength) == _T("WorkCoordSys"))
			{
				auto key = pFDef->Name;
				key.Delete(0, BaseLength);
				int UcsInd = atoi(key) - 1;
				if (UcsInd >= 0) // may be < 0 if WorkCoordSysPar for example
				{
					NUCSs::MaxUcsInd = max(NUCSs::MaxUcsInd, UcsInd);
					StartState.UCSs.SetUsed(UcsInd);
				}
			}
	}
// Load GCD.SMiscFuncDef
	Start.LoadString(IDS_SECT_MFUNCDEF);
	f.ReadString(str);// empty string
	f.ReadString(str);
	if(str != Start)
	{
		ErrParams.push_back(str);
		ErrParams.push_back(Start);
		return -4;
	}
	CMapStringToString SMiscFuncDefInv;// for sintax check purposes only
	while(1)
	{
		if(!f.ReadString(str))
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if(str == End)
			break;
		if (str.Left(2) == _T("//")) // comment string
			continue;
		key = GetField(str);
		if(key.GetLength() <= 0 && CheckSyntax)
		{
			ErrParams.push_back(Start);
			return -8;
		}
		CString buf;
		if (GCD.SMiscFuncDef.Lookup(key, buf) && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -17;
		}
		val = GetField(str);
		if (val.IsEmpty() && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -9;
		}
		GCDV.SMiscFuncDef[key] = val;
		if (SMiscFuncDefInv.Lookup(val, buf) && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(val);
			return -18;
		}
		SMiscFuncDefInv[val] = key;
	}
// Load SOthers
	Start.LoadString(IDS_SECT_OTHERS);
	f.ReadString(str);// empty string
	f.ReadString(str);
	if(str != Start)
	{
		ErrParams.push_back(str);
		ErrParams.push_back(Start);
		return -4;
	}
	while(1)
	{
		if(!f.ReadString(str))
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if(str == End)
			break;
		if (str.Left(2) == _T("//")) // comment string
			continue;
		key = GetField(str);
		if(key.GetLength() <= 0 && CheckSyntax)
		{
			ErrParams.push_back(Start);
			return -3;
		}
		val = GetField(str);
		if (val.IsEmpty() && CheckSyntax)
		{
			ErrParams.push_back(Start);
			ErrParams.push_back(key);
			return -9;
		}
		GCDV.SOthers[key] = val;
	}
// Load SCycleDef
	Start.LoadString(IDS_SECT_CYCLEDEF);
	f.ReadString(str);// empty string
	f.ReadString(str);
	if(str != Start)
	{
		ErrParams.push_back(str);
		ErrParams.push_back(Start);
		return -4;
	}
	while(1)
	{
		if(!f.ReadString(str))
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if(str == End)
			break;
		GCDV.SCycleDef.Add(str);
	}
// Load registers settings
	Start.LoadString(IDS_SECT_REGS_MAP);
	bool MapFilled = false;
	f.ReadString(str);// empty string
	f.ReadString(str);
	NCUnitState::VisibleRegisters.clear();
	if (str == Start)
	{
		while (1)
		{
			if (!f.ReadString(str))
			{
				ErrParams.push_back(Start);
				return -2;
			}
			if (str == End)
				break;
			if (str.Left(2) == _T("//")) // comment string
				continue;
			key = GetField(str);
			if (key.GetLength() <= 0 && CheckSyntax)
			{
				ErrParams.push_back(Start);
				return -8;
			}
			if (key == _T("UCSRegisters"))
			{
				MapFilled = true;
				int StartUCS = atoi(GetField(str)) - 1;
				int NumUCS = atoi(GetField(str));
				int StartReg = atoi(GetField(str));
				int StepReg = atoi(GetField(str));
				for (int i = 0; i < NumUCS; ++i)
				{
					NCUnitState::UCSRegsSubst[StartUCS + i] = StartReg + StepReg * i;
				}
			}
			else if (key == _T("VisibleRegisters"))
			{
				int Start = atoi(GetField(str));
				int End = atoi(GetField(str));
				NCUnitState::VisibleRegisters.push_back(pair<int, int>(Start, End));
			}
			else
			{
				val = GetField(str);
				GCDV.SRegIndexes[key] = atoi(val);
			}
		}
	}
	CloseConfFile(pA);
	if (NCUnitState::VisibleRegisters.empty())
	{
		NCUnitState::VisibleRegisters.push_back(pair<int, int>(0, 8000));
	}
	if (!MapFilled)
	{
		for (int i = 0; i < MAX_UCS; ++i)
			NCUnitState::UCSRegsSubst[i] = 5221 + 20 * i;
	}
	StartState.ResetWOComp();
// Calculate MaxUcsInd
	CString bufw;
	GetOtherValue(_T("WCSParStartInd"), bufw);
	int StartInd = atoi(bufw);
	GetOtherValue(_T("WCSParMaxNum"), bufw);
	int MaxNum = atoi(bufw);
	NUCSs::MaxUcsInd = max(NUCSs::MaxUcsInd, (StartInd + MaxNum - 1));

	CString InitStrAdd;
	CString InitStrMain;
	CString AddCoord(_T("AddCoord"));
	// Set additional coordinates names
	for (int i = 0; i < 100; ++i)
	{
		CString Num;
		Num.Format("%02u", i);
		CString key(AddCoord + Num);
		CString Buf;
		if (GetWordValue(key, Buf) || GetWordValue(key + _T("Inc"), Buf) || GetWordValue(key + _T("Abs"), Buf))
		{
			CString Name;
			if (Format <= 1)
			{
				if (GetOtherValue(key + _T("Axis"), Buf))
					Name = Buf;
				else
					Name = key;
			}
			else// Format >= 2
			{
				auto it = MapAFC.find(key);
				if (it != MapAFC.end())
					Name = it->second.AxisName;
				else
					Name = key;
			}
			InitStrAdd = InitStrAdd + Name + "\n";
		}
	}
	AddCoordsDef.Define(InitStrAdd);
	SMachState BufMachNew(&AddCoordsDef);
	StartState.AddCoords = BufMachNew;
	State.AddCoords = BufMachNew;
	AddState.AddCoords = BufMachNew;
	St.AddCoords = BufMachNew;
	StartState.AddCoordsFlags = BufMachNew;
	State.AddCoordsFlags = BufMachNew;
	AddState.AddCoordsFlags = BufMachNew;
	AddCoordsScale = BufMachNew;
	AddCoordsFeed = BufMachNew;
	GetStates().SetAddCoords(BufMachNew);
	// Set main coordinates names
	std::vector<CString> Names = { _T("HorizontalAxis"), _T("VerticalAxis"), _T("SpindleAxis")
		, _T("HorizontalRotationAxis"), _T("VerticalRotationAxis"), _T("SpindleRotationAxis") };
	std::vector<CString> NamesDefault = { _T("X"), _T("Y"), _T("Z")
		, _T("A"), _T("B"), _T("C") };
	for (int i = 0; i < 6; ++i)
	{
		auto it = MapAFC.find(Names.at(i));
		if (it == MapAFC.end())
			InitStrMain = InitStrMain + NamesDefault.at(i) + "\n";
		else
			InitStrMain = InitStrMain + it->second.AxisName + "\n";
	}
	FixedDef.Define(InitStrMain);
	// Set additional coordinates scales and feeds
	if(Format <= 1)
	{
		for (int i = 0, k = 0; i < 100; ++i)
		{
			CString Num;
			Num.Format("%02u", i);
			CString key(AddCoord + Num);
			CString Buf;
			if (GetWordValue(key, Buf) || GetWordValue(key + _T("Inc"), Buf))
			{
				AddCoordsScale.SetCoord(k, GetScale(key + _T("Scale")));
				if (GetOtherValue(key + _T("Feed"), Buf))
					AddCoordsFeed.SetCoord(k, atof(Buf));
				else
					AddCoordsFeed.SetCoord(k, 5000.);
				++k;
			}
		}
	//END: Set additional coordinates names, scales and feeds
	}
	else // Format >= 2
	{
		for each (auto El in MapAFC)
		{
			if (El.first.Left(AddCoord.GetLength()) == AddCoord)
			{
				CString Num = El.first.Mid(AddCoord.GetLength(), 2);
				int Ind = atoi(Num);
				AddCoordsFeed.SetCoord(Ind, El.second.Feed);
				AddCoordsScale.SetCoord(Ind, El.second.Scale);
			}
		}
	}
	// Set 
	CString buf(_T("0"));
	GCD.SWordDef.Lookup("Register", buf);
	GCDV.RegisterSymbol = buf[0];
	CString vrb;
	if(GCD.SWordDef.Lookup("VariablePrefix", vrb))
		GCDV.VariableSymbol = vrb[0];
	if(GCD.SWordDef.Lookup("VariablePrefixLoc", vrb))
		GCDV.VariableSymbolLoc = vrb[0];
	CString YesNo;
	if(GCD.SOthers.Lookup("AlwaysIncremental",YesNo))
		if(YesNo == "Yes")
			StartState.AbsoluteCoord = FALSE;
	if(GCD.SOthers.Lookup("CycleZAbs",YesNo))
		if(YesNo == "Yes")
			StartState.AbsoluteCoord = TRUE;
	if(GCD.SOthers.Lookup("AutoFixRotAxes",YesNo))
		if(YesNo == "Yes")
			GCDV.AutoFixRotAxes = true;

	CString Type;
	StartState.RotType = Linear;
	if(GCD.SOthers.Lookup("TableRotationType", Type))
	{
		if(Type == "Linear")
			StartState.RotType = Linear;
		else if(Type == "ShortestDistance")
			StartState.RotType = ShortestDistance;
		else if(Type == "AlwaysCW")
			StartState.RotType = AlwaysCW;
		else if(Type == "AlwaysCCW")
			StartState.RotType = AlwaysCCW;
		else if(Type == "PositiveCW")
			StartState.RotType = PositiveCW;
		else if (Type == "PositiveCCW")
			StartState.RotType = PositiveCCW;
		else if (Type == "DoNotChange")
			StartState.RotType = DoNotChange;
	}
//
// Set scale values
	GCDV.LinearDecimalZero = GetLinearDecimalZero();
	GCDV.LinearScale = GetScale("LinearDecimalConversion");
	GCDV.RotaryScale = GetScale("RotaryDecimalConversion");
	GCDV.XScale = GetScale("LinearXConversion");
	GCDV.YScale = GetScale("LinearYConversion");
	GCDV.ZScale = GetScale("LinearZConversion");
	CString b;
	if(GCD.SOthers.Lookup("LinearIConversion", b))
		GCDV.IScale = GetScale("LinearIConversion");
	else
		GCDV.IScale = GCD.XScale;
	if(GCD.SOthers.Lookup("LinearJConversion", b))
		GCDV.JScale = GetScale("LinearJConversion");
	else
		GCDV.JScale = GCD.YScale;
	if(GCD.SOthers.Lookup("LinearKConversion", b))
		GCDV.KScale = GetScale("LinearKConversion");
	else
		GCDV.KScale = GCD.ZScale;

	CString Val;
	GetOtherValue("PartFastInterp",Val);
	BFastMoveMan::Fill(Val);
// Set spiral settings
	GCDV.SpiralG2G3 = true;
	CString Spiral;
	if(GCD.SOthers.Lookup("SpiralG2G3",Spiral))
		if(Spiral == "No")
			GCDV.SpiralG2G3 = false;
	
// Set Fast 	
	NCUnitState::FastFeedX = 12000.;
	if (GCD.SOthers.Lookup("FastFeedRateX", Val))
		NCUnitState::FastFeedX = atof(Val);
	NCUnitState::FastFeedY = 12000.;
	if (GCD.SOthers.Lookup("FastFeedRateY", Val))
		NCUnitState::FastFeedY = atof(Val);
	NCUnitState::FastFeedZ = 12000.;
	if (GCD.SOthers.Lookup("FastFeedRateZ", Val))
		NCUnitState::FastFeedZ = atof(Val);
	NCUnitState::FastFeedA = 2000.;
	if (GCD.SOthers.Lookup("FastFeedRateA", Val))
		NCUnitState::FastFeedA = atof(Val);
	NCUnitState::FastFeedB = 2000.;
	if (GCD.SOthers.Lookup("FastFeedRateB", Val))
		NCUnitState::FastFeedB = atof(Val);
	NCUnitState::FastFeedC = 2000.;
	if (GCD.SOthers.Lookup("FastFeedRateC", Val))
		NCUnitState::FastFeedC = atof(Val);
	// Set rotational coords types
	CyclicA = false;
	if(GCD.SOthers.Lookup("CyclicA", YesNo))
		CyclicA = (YesNo == "Yes");

	CyclicB = false;
	if(GCD.SOthers.Lookup("CyclicB", YesNo))
		CyclicB = (YesNo == "Yes");

	CyclicC = false;
	if(GCD.SOthers.Lookup("CyclicC", YesNo))
		CyclicC = (YesNo == "Yes");
// Set label type
	GCDV.LType = LT_EMPTY;
	if(GCD.SOthers.Lookup(_T("LabelType"), Val))
		if(Val == _T("BaltSys"))
			GCDV.LType = LT_BALTSYS;
		else if(Val == _T("Sinumerik"))
			GCDV.LType = LT_SINUMERIK;
		else if(Val == _T("Expression"))
			GCDV.LType = LT_EXPRESSION;

	SaveCoord2Reg = true;
	if(GCD.SOthers.Lookup("SaveCoord2Reg", YesNo))
		SaveCoord2Reg = (YesNo != "No");

	YesNo = _T("Yes");
	if (GCD.SOthers.Lookup("PolarInterpFanucStyle", YesNo))
		StartState.PolarInterp.SetFanucStyle(YesNo != "No");
	else
		StartState.PolarInterp.SetFanucStyle(true);

	GCDV.MaxWorkFeed = GetMaxWorkFeed();

	GCDV.DecimalConversion = GetScale(_T("DecimalConversion"));

	GCDV.MinArcDist = CalcMinArcDist();

	GCDV.CircleCenterMode = CCM_Incremental;
	if(GCD.SOthers.Lookup(_T("CircleCenterMode"), Val))
	{
		if(Val == _T("AsCoord"))
			GCDV.CircleCenterMode = CCM_AsCoord;
		else if(Val == _T("Absolute"))
			GCDV.CircleCenterMode = CCM_Absolute;
	}

	GCDV.UseStdFuncNames = false;
	if(GetOtherValue("UseStdFuncNames", YesNo))
		if(YesNo == "Yes")
			GCDV.UseStdFuncNames = true;



	GCDV.BaltSysStyle = false;
	if(GCD.SOthers.Lookup(_T("BaltSysStyle"), YesNo))
		GCDV.BaltSysStyle = (YesNo == "Yes");
	GCDV.SinumericStyle = false;
	if(GCD.SOthers.Lookup(_T("SinumerikStyle"), YesNo))
		GCDV.SinumericStyle = (YesNo == "Yes");
	GCDV.AllowToolString = false;
	if(GCD.SOthers.Lookup(_T("ToolIndex"), YesNo))
		GCDV.AllowToolString = (YesNo == "String");
	GCDV.PolarCoordRecalc = false;
	if(GCD.SOthers.Lookup(_T("PolarCoordRecalc"), YesNo))
		GCDV.PolarCoordRecalc = (YesNo == "Yes");
// Init CheckStones
	pDoc->CheckStones.Clear();
	if(GCD.SOthers.Lookup("MiCheckXYChamf",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckXYChamf, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckIJKRDef",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckIJKRDef, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckLengthComp",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckLengthComp, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckLengthOffsetNum",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckLengthOffsetNum, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckDiamOffsetNum",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckDiamOffsetNum, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckDiamCompOff",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckDiamCompOff, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckFeedTurnSpindleTurnOff",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckFeedTurnSpindleTurnOff, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckFeedTurnSpindleMillOn",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckFeedTurnSpindleMillOn, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckFeedMillSpindleMillOff",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckFeedMillSpindleMillOff, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("MiCheckFeedMillSpindleTurnOn",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckFeedMillSpindleTurnOn, AddMi);
			pDoc->CheckStones.Activate();
		}
		
		if(GCD.SOthers.Lookup("MiCheckZeroFeed",YesNo))
		if(YesNo == "Yes")
		{
			pDoc->CheckStones.Serve(MiCheckZeroFeed, AddMi);
			pDoc->CheckStones.Activate();
		}
	if(GCD.SOthers.Lookup("CheckLBLRepeat",YesNo))
		if(YesNo == "Yes")
		{
			GCDV.CheckProgramText = true;
		}
 // Define CheckNext and NChamfer
	struct FDef *pBuf;
	CheckNext = GCD.SFuncDef.Lookup("Round", pBuf) || GCD.SFuncDef.Lookup("Chamfer", pBuf);
	bool ChamferEnabled = CheckNext || GCD.SFuncDef.Lookup("DraftRound", pBuf) || GCD.SFuncDef.Lookup("DraftChamfer", pBuf);
	if(ChamferEnabled)
	{
		NChamfer::Patterns[0] = GetGCodeText(_T("Round"));
		NChamfer::Patterns[1] = GetGCodeText(_T("Chamfer"));
	}
// Set default travel limits
	if(GetOtherValue("HorizontalAxisMinDef", val))
		StartState.GetCoordLimits().SetMin(0, atof(val));
	if(GetOtherValue("VerticalAxisMinDef", val))
		StartState.GetCoordLimits().SetMin(1, atof(val));
	if(GetOtherValue("SpindleAxisMinDef", val))
		StartState.GetCoordLimits().SetMin(2, atof(val));
	if(GetOtherValue("HorizontalRotationAxisMinDef", val))
		StartState.GetCoordLimits().SetMin(3, atof(val));
	if(GetOtherValue("VerticalRotationAxisMinDef", val))
		StartState.GetCoordLimits().SetMin(4, atof(val));
	if(GetOtherValue("SpindleRotationAxisMinDef", val))
		StartState.GetCoordLimits().SetMin(5, atof(val));
	if(GetOtherValue("HorizontalAxisMaxDef", val))
		StartState.GetCoordLimits().SetMax(0, atof(val));
	if(GetOtherValue("VerticalAxisMaxDef", val))
		StartState.GetCoordLimits().SetMax(1, atof(val));
	if(GetOtherValue("SpindleAxisMaxDef", val))
		StartState.GetCoordLimits().SetMax(2, atof(val));
	if(GetOtherValue("HorizontalRotationAxisMaxDef", val))
		StartState.GetCoordLimits().SetMax(3, atof(val));
	if(GetOtherValue("VerticalRotationAxisMaxDef", val))
		StartState.GetCoordLimits().SetMax(4, atof(val));
	if(GetOtherValue("SpindleRotationAxisMaxDef", val))
		StartState.GetCoordLimits().SetMax(5, atof(val));

	if(LookupSGeneral(_T("NumAxes"), val))
		GCDV.NumAxes = atoi(val);

	GCDV.StockPos4Turn = 0;
	if (GetOtherValue(_T("StockPos4Turn"), val))
		GCDV.StockPos4Turn = atoi(val);
	GCDV.MyStockPos.clear();
	if (GetOtherValue(_T("MyStockPos"), val))
	{
		while (!val.IsEmpty())
		{
			CString num = val.SpanExcluding(",");
			GCDV.MyStockPos.insert(atoi(num));
			val.Delete(0, num.GetLength() + 1);
		}
	}
	if(GCDV.MyStockPos.empty())
		GCDV.MyStockPos.insert(0);

// Define DComp2RegStart and RegDCompEnd
	if (GetOtherValue("RegDCompStart", val))
	{
		NCUnitStatePrev::DComp2RegStart = atoi(val);
		NCUnitStatePrev::DComp2RegEnd = NCUnitStatePrev::DComp2RegStart - 1;
		if (GetOtherValue("RegDCompEnd", val))
			NCUnitStatePrev::DComp2RegEnd = atoi(val);
	}
	
	if (!LoadFromSerializeAll)
		LoadSubprograms();

	// File protection
	FileProtection = false;
	if(GCD.SOthers.Lookup(_T("FileProtection"), YesNo))
		if(YesNo == _T("Yes"))
			FileProtection = true;

	AddZDispToComp = false;
	if(GCD.SOthers.Lookup(_T("AddZDispToComp"), YesNo))
		if(YesNo == _T("Yes"))
			AddZDispToComp = true;

	PartialCompCancel = true;
	if(GCD.SOthers.Lookup("PartialCompCancel", str))
		if(str == "No")
			PartialCompCancel = false;
	
	ConsecutiveCalc = false;
	if (GCD.SOthers.Lookup("ConsecutiveCalc", YesNo))
		if (YesNo == _T("Yes"))
			ConsecutiveCalc = true;

	UseIntegerArithm = false;
	if (GCD.SOthers.Lookup("UseIntegerArithm", YesNo))
		if (YesNo == _T("Yes"))
			UseIntegerArithm = true;
	// 
	CString RegSym(_T("#"));
	GCD.SWordDef.Lookup("Register", RegSym);
	StartState.GetVarSet().SetDefaultName(RegSym.GetBuffer());

	CString Char;
	if (GCD.SOthers.Lookup("OpenBracketChar", Char))
		NEvalExp::SetOpenBC(Char[0]);
	else
		NEvalExp::SetOpenBC('[');

	if (GCD.SOthers.Lookup("CloseBracketChar", Char))
		NEvalExp::SetCloseBC(Char[0]);
	else
		NEvalExp::SetCloseBC(']');

	ExprCharSet = _T("0123456789.-+*/^$'") + CString(NEvalExp::OpenBC()) + CString(NEvalExp::CloseBC()); // Character set for the expression without logical operations
	LogCharSet = _T("<>=!?&|:"); // Character set with comparison and logical operations
	OpCharSet = _T("-+*/^$<>=!?&|:'") + CString(NEvalExp::OpenBC()) + CString(NEvalExp::CloseBC()); // Character set with all operations
	Sign = _T("-+*/^$'") + CString(NEvalExp::OpenBC());// Arithmetics operations
	CharSet = ExprCharSet + FUNC_NAMES;

	CString VTE;
	GCD.SOthers.Lookup("VarTestExp", VTE);
	VarTestExp.assign(std::string(VTE));
	if(GCD.SOthers.Lookup("CharTestExp", VTE)) // for testing for invalid character
		CharTestExp.assign(std::string(VTE));
	else
		CharTestExp.assign(std::string("[а-яА-Я]"));

	GCDV.IsDefined = true;
	return 0;
}

int NCUnit::ApplyMainInirStr()
{
	CString Str;
	GCD.SOthers.Lookup("MainInitStr", Str);
	auto res = RunInitProg(Str);
	StartState.ForceMakeEqualTo(State);
	if (res < 0)
	{
		std::vector<CString> ErrParams;
		const auto ErrNum = -res;
		CString ErrMessage;
		GetConfErrMessage(ErrNum, ErrParams, ErrMessage);
		AfxMessageBox(ErrMessage);
	}
	return res;
}

int NCUnit::LoadConfInh(std::vector<CString>& ErrParams, bool LoadFromSerializeAll)
{// load with inheritance
	// Make parents list
	CString UnitName = GCD.ConfFileName;
	GCDV.ConfParNames.push_back(UnitName);
	while (true)
	{
		GCDV.ConfFileName = UnitName;
		CArchive* pA = nullptr;
		int Ret = ReadSGeneral(pA, ErrParams);
		if (Ret < 0 || pA == nullptr)
			return Ret;
		CloseConfFile(pA);
		// Check format
		CString Val;
		int Format = 1;
		if (LookupSGeneral(_T("Format"), Val))
			Format = atoi(Val);
		if (Format < 2)
			return -11;
		// Check for parent
		if (!GCD.SGeneral.Lookup(_T("Parent"), UnitName))
			break;
		if (UnitName.IsEmpty())
			break;
		if (IsTempName(GCD.ConfFileName))
			MakeTempName(UnitName, UnitName, false);
		GCDV.ConfParNames.push_back(UnitName);
	}
	GCDV.ConfFileName = GCDV.ConfParNames.at(0);
	// load configs
	const CString CombinedFName(_T("Combined.temp.sm3"));
	int Ret = CombineUnitFiles(CombinedFName, GCDV.ConfParNames, ErrParams);
	if (Ret < 0)
		return Ret;
	GCDV.ConfFileName = CombinedFName;
	Ret = LoadConfFile(ErrParams, LoadFromSerializeAll);
	if (Ret < 0)
		return Ret;
	GCDV.ConfFileName = GCDV.ConfParNames.at(0);

	return 0;
}

int NCUnit::CombineUnitFiles(const CString& OutName, const std::vector<CString>& Names, std::vector<CString>& ErrParams) const
{
	std::vector<CStdioFile> Files(Names.size());// Files in inverted order
	for (int i = 0; i < Names.size(); ++i)
	{
		if(!Files.at(i).Open(GetConfPath() + Names.at(Names.size() - 1 - i), CFile::typeText | CFile::modeRead))
		{
			ErrParams.push_back(Names.at(i));
			return -7;
		}
	}
	CStdioFile OutFile;
	if (!OutFile.Open(GetConfPath() + OutName, CFile::typeText | CFile::modeWrite | CFile::modeCreate))
	{
		ErrParams.push_back(OutName);
		return -7;
	}
	WriteStartStrings(OutFile);
	CString Start;
	CString End;
	End.LoadString(IDS_SECT_END);
	Start.LoadString(IDS_SECT_GEN);
	for (int i = 0; i < Files.size(); ++i)
	{
		CString str;
		while (Files.at(i).ReadString(str))
			if (str == Start)
				break;
	}
	std::vector<CString> Result;
	// Read and combine one standard section
	int Ret = ReadStandardSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_WORDREP);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine replace section
	Ret = ReadReplaceSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_COORDDEF);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one standard section
	Ret = ReadStandardSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_WORDDEF);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one standard section
	Ret = ReadStandardSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_FUNCDEF);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one standard section
	Ret = ReadStandardSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_MFUNCDEF);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one standard section
	Ret = ReadStandardSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_OTHERS);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one standard section
	Ret = ReadStandardSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_CYCLEDEF);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one simple section
	Ret = ReadSimpleSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	Start.LoadString(IDS_SECT_REGS_MAP);
	Ret = ReadSectionTitle(Start, Files, ErrParams);
	if (Ret < 0)
		return Ret;
	// Read and combine one simple section
	Ret = ReadSimpleSect(Start, End, Files, ErrParams, Result);
	if (Ret < 0)
		return Ret;
	WriteSect(OutFile, Start, End, Result);
	Result.clear();

	return 0;
}

int NCUnit::ReadSectionTitle(const CString& Start, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams) const
{
	for (int i = 0; i < Files.size(); ++i)
	{
		CString str;
		Files.at(i).ReadString(str);// empty string
		Files.at(i).ReadString(str);
		if (str != Start)
		{
			ErrParams.push_back(str);
			ErrParams.push_back(Start);
			return -4;
		}
	}
	return 0;
}

int NCUnit::ReadStandardSect(const CString& Start, const CString& End, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams, std::vector<CString>& Result) const
{
	const size_t StartInd = Result.size();
	std::map<CString, size_t> CommonKeys;
	const bool CheckSyntax = true;
	for (int i = 0; i < Files.size(); ++i)
	{
		while (1)
		{
			std::set<CString> keys;
			CString str;
			if (!Files.at(i).ReadString(str))
			{
				ErrParams.push_back(Start);
				return -2;
			}
			if (str == End)
				break;
			if (str.Left(2) == _T("//")) // comment string
			{
				Result.push_back(str);
				continue;
			}
			CString strB = str;
			const CString key = GetField(str);
			if (key.GetLength() <= 0)
			{
				ErrParams.push_back(Start);
				return -3;
			}
			if(keys.find(key) != keys.end())
			{
				ErrParams.push_back(Start);
				ErrParams.push_back(key);
				return -1;
			}
			auto It = CommonKeys.find(key);
			if (It != CommonKeys.end())
				Result.at(It->second).Empty();// mark for delete
			Result.push_back(strB);
			keys.insert(key);
			CommonKeys[key] = Result.size() - 1;
		}
		// remove marked
		size_t Dest = StartInd;
		for (size_t Src = StartInd; Src < Result.size(); ++Src)
		{
			if (!Result.at(Src).IsEmpty())
			{
				if (Dest != Src)
					Result.at(Dest) = Result.at(Src);
				++Dest;
			}
		}
		Result.resize(Dest);
	}
	return 0;
}

int NCUnit::ReadSimpleSect(const CString& Start, const CString& End, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams, std::vector<CString>& Result) const
{
	const bool CheckSyntax = true;
	for (int i = 0; i < Files.size(); ++i)
	{
		while (1)
		{
			CString str;
			if (!Files.at(i).ReadString(str))
			{
				ErrParams.push_back(Start);
				return -2;
			}
			if (str == End)
				break;
			Result.push_back(str);
		}
	}
	return 0;
}

int NCUnit::ReadReplaceSect(const CString& Start, const CString& End, std::vector<CStdioFile>& Files, std::vector<CString>& ErrParams, std::vector<CString>& Result) const
{
	const size_t StartInd = Result.size();
	std::map<CString, size_t> CommonKeys;
	const bool CheckSyntax = true;
	for (int i = 0; i < Files.size(); ++i)
	{
		while (1)
		{
			std::set<CString> keys;
			CString str;
			if (!Files.at(i).ReadString(str))
			{
				ErrParams.push_back(Start);
				return -2;
			}
			if (str == End)
				break;
			Result.push_back(str);
			if (str.Left(2) == _T("//")) // comment string
				continue;
			CString Type = GetField(str);
			int Priority = 0;
			if (!Type.IsEmpty())
				if (Type[0] == '#')
				{
					Type.Delete(0, 1);
					Priority = atoi(Type);
					Type = GetField(str);
				}
			if (Type.GetLength() <= 0)
			{
				ErrParams.push_back(Start);
				return -3;
			}
			const CString key = GetField(str);
			if (keys.find(key) != keys.end())
			{
				ErrParams.push_back(Start);
				ErrParams.push_back(key);
				return -1;
			}
			if (Type == _T("RegExp1"))
			{
				if (!Files.at(i).ReadString(str))// read RegExp2 string
				{
					ErrParams.push_back(Start);
						return -2;
				}
				Result.back() += _T("\n") + str;
			}
			auto It = CommonKeys.find(key);
			if (It != CommonKeys.end())
				Result.at(It->second).Empty();// mark for delete
			keys.insert(key);
			CommonKeys[key] = Result.size() - 1;
		}
		// remove marked
		size_t Dest = StartInd;
		for (size_t Src = StartInd; Src < Result.size(); ++Src)
		{
			if (!Result.at(Src).IsEmpty())
			{
				if (Dest != Src)
					Result.at(Dest) = Result.at(Src);
				++Dest;
			}
		}
		Result.resize(Dest);
	}
	return 0;
}

int NCUnit::WriteSect(CStdioFile& OutFile, const CString& Start, const CString& End, const std::vector<CString>& Result)
{
	OutFile.WriteString(Start + "\n");
	for each (auto str in Result)
		OutFile.WriteString(str + "\n");
	OutFile.WriteString(End + "\n");
	OutFile.WriteString(_T("\n"));
	return 0;
}

int NCUnit::WriteStartStrings(CStdioFile& OutFile)
{
	OutFile.WriteString(_T("// Generated Unit file\n"));
	OutFile.WriteString(_T("//\n"));
	OutFile.WriteString(_T("\n"));
	return 0;
}

int NCUnit::ReadSGeneral(CArchive*& pA, std::vector<CString>& ErrParams)
{
	ClearConf();
	pA = nullptr;
	if (OpenConfFile(pA, ErrParams) != 0)
		return -7;
	CArchive& f = *pA;
	CString key, val;
	CString Start;
	CString End;
	End.LoadString(IDS_SECT_END);
	// Load GCD.SGeneral
	Start.LoadString(IDS_SECT_GEN);
	CString str;
	while (f.ReadString(str))
		if (str == Start)
			break;
	while (1)
	{
		if (!f.ReadString(str))
		{
			ErrParams.push_back(Start);
			return -2;
		}
		if (str == End)
			break;
		if (str.Left(2) == _T("//")) // comment string
			continue;
		key = GetField(str);
		if (key.GetLength() <= 0)
		{
			ErrParams.push_back(Start);
			return -3;
		}
		val = GetField(str);
		GCDV.SGeneral[key] = val;
	}
	return 0;
}

void NCUnit::LoadSubprograms()
{
	CMapStringToNProgram* pPrL = GetChannel().NGetProgramListCn();
	// Remove subprograms loaded from unit
	for (int i = pPrL->GetMainProgCount(); i < pPrL->GetSize(); ++i)
	{
		NProgram* pProg;
		CString key;
		pPrL->GetAt(i, key, (CObject*&)pProg);
		if (pProg == nullptr)
			continue;
		if (pProg->IsAuxFromUnit())
		{
			pDoc->DeleteProgObj(pProg);
			pPrL->RemoveKey(key);
			--i;
		}
	}
	CString str;
	GCD.SOthers.Lookup("LoadSubPrograms", str);
	// Load subprograms
	for (CString filename = str.SpanExcluding(_T(",")); !filename.IsEmpty(); filename = str.SpanExcluding(_T(",")))
	{
		str.Delete(0, filename.GetLength() + 1);
		filename = NCM_PROJECT.GetUnitsPath() + filename;
		NProgram* pP = pDoc->LoadNewProgram(filename);
		if (pP == nullptr)
		{
			CString s;
			s.Format(IDS_MES_READ, filename);

			if (AfxMessageBox(s, MB_ICONQUESTION | MB_YESNO) == IDYES)
				continue;
			else
				return;
		}
		pP->SetProtected(true);
		pP->SetAuxFromUnit();
		pDoc->NAddAuxProgram(filename, pP);
	}
}

int NCUnit::RunInitProg(const CString &Str)
{
	// Init Unit Start State by user defined string
	State.ForceMakeEqualTo(StartState);
	NProgram *pProg = MakeMainInitProg(Str);// Create global init prog
	pDoc->ImplRepl(pProg);
	if (pProg)
	{
		auto OldReactType = pDoc->ProgErrHandler.SetReactType(0);// all errors lead to break
		State.SetCurCadrEx(cadrID(-1, 0));
		int res = RunProgram(pProg, nullptr, -1, true);// Run initialisation program
		pDoc->DeleteProgObj(pProg);
		pDoc->ProgErrHandler.SetReactType(OldReactType);// restore error reaction
		if (res < 0)
			return -5;
		State.SetCurCadr(0);
		State.ClearCurHCadr();
		State.ClearCurHCadrAbs();
		State.SetCurProgEmpty();
	}
	return 0;
}

int NCUnit::SetDef(const CString &name)
{
	int res = SetDefWOInitStr(name);
	if (res < 0)
		return res;
	res = ApplyMainInirStr();
	return res;
}

int NCUnit::SetDefWOInitStr(const CString& name)
{
	GCDV.ConfParNames.clear();
	std::vector<CString> ErrParams;
	bool LoadFromSerializeAll = PrepUnitSerialiseAll();
	if (LoadFromSerializeAll)
		MakeTempName(name, GCDV.ConfFileName, false);
	else
		GCDV.ConfFileName = name;

	CArchive* pA = nullptr;
	int Ret = ReadSGeneral(pA, ErrParams);
	if (Ret < 0 || pA == nullptr)
		return Ret;
	CloseConfFile(pA);
	if (Ret >= 0)
	{
		// Check for parent
		bool HaveParent = false;
		CString ParentName;
		if (GCD.SGeneral.Lookup(_T("Parent"), ParentName))
			if (!ParentName.IsEmpty())
				HaveParent = true;
		//END: Check for parent
		if (HaveParent)
			Ret = LoadConfInh(ErrParams, LoadFromSerializeAll);
		else
			Ret = LoadConfFile(ErrParams, LoadFromSerializeAll);
	}
	if (Ret < 0)
	{
		const auto ErrNum = -Ret;
		CString ErrMessage;
		GetConfErrMessage(ErrNum, ErrParams, ErrMessage);
		AfxMessageBox(ErrMessage);
	}
	return Ret;
}

void NCUnit::JumpToProjStart()
{
//	CWaitCursor w;
	Reset();
	CMapStringToNProgram *plist = GetChannel().NGetProgramListCn();
	if(plist->GetMainProgCount() <= 0)
		return;
	CString key;
	NProgram *pProg;
	plist->GetAt( 0, key, (CObject *&)pProg );
	SetCurProg(key);
}

void NCUnit::JumpToProjEnd()
{
	// Jump to the last program of the project end
	int Ind = GetStates().FindMaxLowerInd(cadrID(-1));
	const NCUnitState *St, *AddSt;
	const NCUnitStatePrevGeom *GeomSt;
	GetStates().GetStatePair(Ind, St, AddSt, GeomSt);
	Jump2State(*St, *AddSt, *GeomSt);
}

bool NCUnit::IsCurCadrProtected() const
{
	if (IsFileProtectionEnabled())
		return GetChannel().NGetProgramListCn()->IsCadrProtected(GetState()->GetCurCadrID());
	else
		return false;
}

RunStepsStopReason NCUnit::RunSteps(NEvalStopCrit& SC)
{// Returns last Evaluate cadr result or SR_PROJ_END, if project ended
	if(!pDoc->CheckProgExist())
		return SR_ERROR;
//Run n steps from current position
	int ProcessedSteps = 0;
	int n = WHOLE_PROG;
	if (SC.IsCount())
		n = SC.GetCount();
	if (n <= 0)
		return SR_OK;
	//if(n > 1)
	//	CWaitCursor w;
	GetState()->MileStoneFound = false;
	RunStepsStopReason StopReason = SR_OK;
	NCUnitStatePrev::EvaluateCadrResult ret = NCUnitStatePrev::ECR_OK;
	if(NCM_PROJECT.GetGlobalState().GetCuttingMode())
	{
		if (!pDoc->IsCollWOStock())
		{
			if (!pDoc->IsThereAnyVisStock())
			{
				AfxMessageBox(IDS_COMP_NOSTOCK);
				return SR_ERROR;
			}
			if (!pDoc->NGetStockList()->IsThere0StPos())
			{
				AfxMessageBox(IDS_STOCK_NOTZERO);
				return SR_ERROR;
			}
		}

		const NSpeedPar* pSpeedPar = &NSpeedPar::Extern;
		//if (NCM_PROJECT.GetGlobalState().GetAnimMode() == NGlobalState::Result && pDoc->GetCollManager().IsForeType())
		//{
		//	pSpeedPar = &NSpeedPar::Intern;
		//}

		CMapStringToNProgram *plist = GetChannel().NGetProgramListCn();
		cadrID StartHCadr = GetState()->GetCurCadrIDEx();
		NProgram *pProg = GetCurRunProg();
		if(!pProg)
		{
			JumpToProjStart();
			pProg = GetCurRunProg();
			StartHCadr = GetState()->GetCurCadrIDEx();
		}
		if(pProg == nullptr)
			return SR_ERROR;

		if(pProg->GetProbeHistory().GetSize() > 0)
			if (State.GetCurHCadrAbsEx().Cadr >= pProg->GetHistorySize())
				return SR_PROG_END; // to prevent execution of the last cadr when probing is active
		if((!pDoc->MileStones.IsActive() || pDoc->MileStones.GetCount() < 1) && SC.IsInfinite() && !GetChannel().IsProbePresent() && !GetChannel().IsTouchPresent())
		{ // Full execution
			StopReason = SR_PROJ_END;
			//Store all fragments
			GetChannel().GetCutting().StoreProgFragment(pSpeedPar, StartHCadr.Prog, pProg, StartHCadr.Cadr, pProg->GetLastHCadr());
			for( int i = StartHCadr.Prog + 1; i < plist->GetMainProgCount(); ++i)
			{
				CString key;
				plist->GetAt(i, key, (CObject *&)pProg );
				GetChannel().GetCutting().StoreProgFragment(pSpeedPar, i, pProg, 0, pProg->GetLastHCadr());
			}
			if(0 > GetChannel().CutProgFragmentsCn(StopReason))// Nothing to cut
			{
				JumpToProjEnd();
				pDoc->SetCurProgrTDT();
				pDoc->UpdateAllViews(NULL, N_OGL | N_STATE | N_CADRCHANGED);
				GetChannel().SetCuttingResult(CUT_PROJ_END);
				AfxGetMainWnd()->PostMessage(WM_APP_CUTTING_STOPPED, GetChannelInd());
			}
			else
				pDoc->UpdateAllViews(NULL, N_STATE);
		}
		else if(pDoc->MileStones.HaveToolChangeOnly() && !GetChannel().IsProbePresent() && !GetChannel().IsTouchPresent())
		{
			bool ProjEnd = false;
			cadrID EndHCadr = pDoc->FindNextToolHID(&ProjEnd);
			if (ProjEnd)
				StopReason = SR_PROJ_END;
			for( int i = StartHCadr.Prog; i <= EndHCadr.Prog; ++i)
			{
				CString key;
				plist->GetAt(i, key, (CObject *&)pProg );
				const int StartInd = (i == StartHCadr.Prog) ? StartHCadr.Cadr : 0;
				const int EndInd = (i == EndHCadr.Prog) ? EndHCadr.Cadr : pProg->GetLastHCadr();
				GetChannel().GetCutting().StoreProgFragment(pSpeedPar, i, pProg, StartInd, EndInd);
			}
			GetChannel().CutProgFragmentsCn(StopReason);
			pDoc->JumpToNextTool();
		}
		else
		{//Mile Stones Present or n < WHOLE_PROG or Probe present or add coord present
			SCoordsDef CDef(0);
			SMachState MState(NULL);
			pDoc->GetMachTool().GetState(MState, CDef);
			const BMTLinkage ConfNum = pDoc->GetMachTool().GetMTLink();
			bool Continue = true;
			for (int i = 0; Continue && i < n; )
			{
				if(!SC.IsCount())
					if (State.GetCurCadrIDEx() == SC.GetStopCadr())
						break;
				ret = EvaluateCurrent(false);
				if (State.TouchCadr)
				{
					StopReason = SR_NEED_TOUCH;
					ProcessedSteps = i;
					break;
				}
				if (ret == NCUnitStatePrev::ECR_PROG_END)// Program ended
				{
					if(State.ProbeCadr)
					{
						StopReason = SR_NEED_PROBE;
						ProcessedSteps = i;
						break;
					}
					else
					{
						NProgram *pProg = GetCurRunProg();
						int EndHCadr = GetState()->GetCurHCadr();
						GetChannel().GetCutting().StoreProgFragment(pSpeedPar, plist->GetIndex(pProg), pProg, StartHCadr.Cadr, EndHCadr);

						Continue = JumpToNextProg();
						StartHCadr = GetState()->GetCurHCadr();
					}
				}
				else if (ret == NCUnitStatePrev::ECR_ERROR)// Error
				{
					StopReason = SR_ERROR;
					break;
				}
				else if (!IsCurCadrProtected())
				{
					if (SC.IsInfinite() && GetState()->MileStoneFound)// Breakpoint
					{
						GetState()->MileStoneFound = false;
						StopReason = SR_BREAK;
						break;
					}
					++i;
				}
			}
			if(Continue)
			{
				NProgram *pProg = GetCurRunProg();
				int EndHCadr = GetState()->GetCurHCadr();
				if (StopReason == SR_NEED_PROBE || StopReason == SR_NEED_TOUCH)
					--EndHCadr;
				GetChannel().GetCutting().StoreProgFragment(pSpeedPar, plist->GetIndex(pProg), pProg, StartHCadr.Cadr, EndHCadr);
			}
			else
			{
				StopReason = SR_PROJ_END;
			}
			pDoc->GetMachTool().SetStateWORecalc(ConfNum, MState);
			if (0 > GetChannel().CutProgFragmentsCn(StopReason))// Nothing to cut
			{
				pDoc->SetCurProgrTDT();
				pDoc->UpdateAllViews(NULL, N_CADRCHANGED | N_OGL | (GetChannel().IsProbePresent() ? 0 : N_STATE));
			}
			else
				if(!GetChannel().IsProbePresent())
					pDoc->UpdateAllViews(NULL, N_STATE);
		}//END Mile Stones Present
	}// if(NCM_PROJECT.GetGlobalState().GetCuttingMode())
	else
	{
		bool Continue = true;
		for (int i = 0; Continue && i < n;)
		{
			if (!SC.IsCount())
				if (State.GetCurCadrIDEx() == SC.GetStopCadr() && SC.GetStopCadr().Prog != -1)
					break;
			ret = EvaluateCurrent(false);

			if (ret == NCUnitStatePrev::ECR_PROG_END)// Program ended
				Continue = JumpToNextProg();
			else if (ret == NCUnitStatePrev::ECR_ERROR)// Error
			{
				StopReason = SR_ERROR;
				break;
			}
			if (!IsCurCadrProtected())
			{
				if (GetState()->MileStoneFound)// Breakpoint
				{
					GetState()->MileStoneFound = false;
					StopReason = SR_BREAK;
					break;
				}
				++i;
			}
		}
		if (!Continue)
			StopReason = SR_PROJ_END;
	}
	if (SC.IsCount())
		SC = NEvalStopCrit(n - ProcessedSteps);
	return StopReason;
}

void NCUnit::JumpSteps(int n)
{// this jump ignores breakpoints
	CWaitCursor w;
	//Jump n steps forward from current position
	if (n == -1)
		n = WHOLE_PROG;// Do until program end
	bool Continue = true;
	for (int i = 0; Continue && i < n;)
	{
		NCUnitStatePrev::EvaluateCadrResult ret = EvaluateCurrent(false);

		if (ret < NCUnitStatePrev::ECR_ERROR)// Error
			break;
		if (ret == NCUnitStatePrev::ECR_PROG_END)// Program ended
			Continue = JumpToNextProg();
		if (!IsCurCadrProtected())
			++i;
	}
}

void NCUnit::JumpForToBreakpoint()
{
	CWaitCursor w;
	//Jump until project ends or breakpoint occurs
	GetState()->MileStoneFound = false;
	bool Continue = true;
	while (Continue)
	{
		int ret = EvaluateCurrent(false);

		if (ret < -1)// Error
			break;
		if (ret == -1)// Program ended
			Continue = JumpToNextProg();
		if (!IsCurCadrProtected())
		{
			if (GetState()->MileStoneFound)// Breakpoint
			{
				GetState()->MileStoneFound = false;
				break;
			}
		}
	}
}

bool NCUnit::GetDValue(CString &text, double *val, const CString &ScaleName)
{
	return GetDValue(text, val, GetScale(ScaleName));
}

bool NCUnit::GetDValue4Wide(CString &text, double *val)
{
	auto b0 = UseIntegerArithm;
	UseIntegerArithm = false;
	auto b1 = ConsecutiveCalc;
	ConsecutiveCalc = false;
	text.Replace('(', '[');
	text.Replace(')', ']');
	text.MakeUpper();
	text.Remove(' ');
	NEvalExp expression((NCUnit *)this, UseIntegerArithm);
	auto Res = expression.Evaluate(text, val);
	UseIntegerArithm = b0;
	ConsecutiveCalc = b1;
	return Res;
}

bool NCUnit::GetDValue4Dial(CString &text, double *val)
{
	auto b0 = UseIntegerArithm;
	UseIntegerArithm = false;
	auto b1 = ConsecutiveCalc;
	ConsecutiveCalc = false;
	text.Replace('(', '[');
	text.Replace(')', ']');
	text.MakeUpper();
	for (int ind = 0; ind < FUNC_NUMBER; ind++)
	{
		CString key = FUNC_TITLES[ind];
		text.Replace(key, CString(FUNC_NAMES[ind]));
	}
	auto Res = GetDValue(text, val, 1.);
	UseIntegerArithm = b0;
	ConsecutiveCalc = b1;
	return Res;
}

bool NCUnit::GetDValue(CString &text, double *val, double DotScale)
{
// Get double value from text


	int n = text.GetLength();

	double sgn = 1.;
	int i = 0;
	char* p = text.GetBuffer();
	if(n > 0)
	{
		if(p[0] == '=')
		{
			text.Delete(0);
			p = text.GetBuffer();
			--n;		
		}
	}
	if(n > 0)
	{
		if(p[0] == '+' || p[0] == '-')
		{
			if(p[0] == '-') sgn = -1.;
			++i;
		}
	}
	bool isConst = false;
	int pointPos = -1;
	char s = '\0';
	double cons = 0.;
	int Istart = i;
	for(; i < n; ++i)
	{
		s = p[i];
		if(s >= 48 && s <= 57)
		{
			isConst = true;
			if(pointPos >= 0)
				cons = cons + (s - 48) * pow(10.0, pointPos - i);
			else
				cons = cons * 10 + (s - 48);
		}
		else if(s == '.')
		{
			if(pointPos >= 0)
				return false;
			pointPos = i;
		}
		else
			break;
	}
	if((!(i == Istart && (s == GCD.RegisterSymbol || s == GCD.VariableSymbol || s == GCD.VariableSymbolLoc)) && CharSet.Find(s) < 0) || i == n)
	{
		if(isConst == true)
		{
			*val = cons * sgn;
			text.Delete(0, i);
			if(pointPos < 0)
				*val *= DotScale;
			return true;
		}
		else
		{
			if(i != 0)
				return false;
			//Emty
			CString str;
			if(GCD.SOthers.Lookup(_T("EmptyZero"),str))
				if(str == _T("Yes"))
				{
					*val = 0.;
					return true;
				}
			return false;
		}
	}
	// Expression detected
	ReplaceVariables(text);
	CString exp(_T(""));
	for(int i = 0; i < n; ++i)
	{
		CString tmp = text.SpanIncluding(CharSet);
		text.Delete(0, tmp.GetLength());

		exp += tmp;
		if(text[0] == GCD.RegisterSymbol)
		{
			if(Sign.Find(exp.Right(1)) >= 0 || FUNC_NAMES.Find(exp.Right(1)) >= 0)// Sign before register
			{
				text.Delete(0);
				exp += GCD.RegisterSymbol;
			}
			else
				break;
		}
		else
			break;
	}

	NEvalExp expression( this, UseIntegerArithm, ConsecutiveCalc);
	return expression.Evaluate( exp, val );	
}

cadrID NCUnit::PrepRunToCurCadr()
{
	auto CHCadr = State.GetCurCadrIDEx();

	JumpToProjStart();

	return CHCadr;
}

double NCUnit::CalcDist2(double x0, double y0, double z0, double x1, double y1, double z1, double x, double y, double z)
{
	// Calculate distance^2 from stright line P0P1 to point P
	const double ax = x-x0, ay = y-y0, az = z-z0;
	const double bx = x1-x0, by = y1-y0, bz = z1-z0;
	// d^2 = a^2 - (a*b/|b|)^2
	const double ab = ax*bx+ay*by+az*bz;
	return ax*ax+ay*ay+az*az - ab*ab/(bx*bx+by*by+bz*bz);
}

bool NCUnit::Defined()
{
	return GCD.IsDefined;
}

BOOL NCUnit::GetWordValue(const CString &key, CString & val) const
{
	val = ""; 
	return GCD.SWordDef.Lookup(key, val);
}

// Get first elem in GCD.SFuncDef according to key
BOOL NCUnit::GetFuncNum(const CString& key, CString & val)
{
	val = "";
	struct FDef *pFDef;
	if(!GCD.SFuncDef.Lookup(key, pFDef))
		return FALSE;
	val = pFDef->Num;
	return TRUE;
}

BOOL NCUnit::GetMiscFuncValue(const CString& key, CString & val)
{
	val = ""; 
	return GCD.SMiscFuncDef.Lookup(key, val);
}

int NCUnit::CalcComp(const NCUnitStatePrev& St, bool NextGeomFlag, const NCadrGeom &NextGeom)
{
	if(St.CurLengthCompNum != State.CurLengthCompNum)
	{
		State.ToolDisplChanged.x = TRUE;
		State.ToolDisplChanged.z = TRUE;
	}
	if(State.ToolDisplChanged.x || State.ToolDisplChanged.z)
		State.PrevToolDisplacement = State.CurToolDisplacement;

	if(State.CoordCompCode != 0)
	{
		ProcessCoordComp(St);
		return 0;
	}


	if(State.Geom.IsUndef())
		return 0;
	if(St.RetractNext)
		State.CompCancel = TRUE;
	bool RetractDone = false;
	if(State.CompCancel)
	{// CompCancel in this cadr
		if (ChangedX(St) || !PartialCompCancel)
		{
			State.MashcX = State.MashX;
			RetractDone = true;
		}
		if(ChangedY(St) || !PartialCompCancel)
		{
			State.MashcY = State.MashY;
			RetractDone = true;
		}
		if(ChangedZ(St) || !PartialCompCancel)
		{
			State.MashcZ = State.MashZ;
			RetractDone = true;
		}
		if (RetractDone)
			State.RetractNext = FALSE;

		State.CompGeom.Set(State.Geom.GetType(),
				St.MashcX,St.MashcY,St.MashcZ,
				State.MashcX,State.MashcY,State.MashcZ,
				0.,0.,0.,XY);
	}
	else // if(State.CompCancel)
	{
		if(St.CurDirComp == LR_OFF)
		{
			// No compensation in this cadr
			State.MashcX = State.MashX;
			State.MashcY = State.MashY;
		}
		else 
		{
			// CompCancel in this cadr or compensation will be processed later
			State.MashcX = State.MashX;
			State.MashcY = State.MashY;
		}
		State.MashcZ = State.MashZ;
		State.CompGeom = State.Geom;
		if(!State.CompGeom.HaveGeom())
		{
			State.MashcX = St.MashcX;
			State.MashcY = St.MashcY;
			State.MashcZ = St.MashcZ;
		}
		if(State.Geom.IsLine())
		{
			//Fill cadr comp geom 
			if(State.Geom.HaveGeom())
				State.CompGeom.Set(State.Geom.GetType(),
							St.MashcX,St.MashcY,St.MashcZ,
							State.MashcX,State.MashcY,State.MashcZ,
							0.,0.,0.,XY);
		}
		else
		{
			BPoint r(State.MashcX-State.MashX,State.MashcY-State.MashY,State.MashcZ-State.MashZ,0.);
			State.CompGeom.Move(r);
		}
		// HorVerCompensation
		if(State.CurDiaCompHorVec != 0. || State.CurDiaCompVerVec != 0.)
		{
			State.MashcX = State.MashX + State.CurDiaCompHorVec * State.CurDiamComp;
			State.MashcY = State.MashY + State.CurDiaCompVerVec * State.CurDiamComp;
			if(State.Geom.IsLine())
			{
				State.CompGeom.Set(State.Geom.GetType(),
						St.MashcX,St.MashcY,St.MashcZ,
						State.MashcX,State.MashcY,State.MashcZ,
						0.,0.,0.,XY);
			}
			else
			{
				double bi,bj,bk;
				State.Geom.GetI(&bi,&bj,&bk);
				bi = St.MashX+bi-St.MashcX;
				bj = St.MashY+bj-St.MashcY;
				bk = St.MashZ+bk-St.MashcZ;
				State.CompGeom.Set(State.Geom.GetType(),
						St.MashcX,St.MashcY,St.MashcZ,
						State.MashcX,State.MashcY,State.MashcZ,
						bi,bj,bk,XY);
			}
		}
		else
		{
			if(St.CurDiaCompHorVec != 0. || St.CurDiaCompVerVec != 0.)
			{
				State.MashcX = State.MashX;
				State.MashcY = State.MashY;
				State.CompGeom.Set(State.Geom.GetType(),
						St.MashcX,St.MashcY,St.MashcZ,
						State.MashcX,State.MashcY,State.MashcZ,
						0.,0.,0.,XY);
			}
		}

		// Diam compensation
		bool CompStart = (St.CurDirComp == LR_OFF && State.CurDirComp != LR_OFF);
		if(State.CurDirComp != LR_OFF) // Включена контурная коррекция
		{
			enum CompType CType = GetCompType();
			bool DisableIntersect = false;
//BEGIN коррекция по IJ (A.2)

//			NCadrGeom NextGeom;
//			BOOL NextGeomFlag;
//			NextGeomFlag = GetNextGeom(NextGeom);// This call must be done before the ParToNext check
			double xcomp,ycomp,zcomp;// compensation vector
			if(State.Geom.IsLine())
			{
// Если в кадре не заданы IJK, то они определяются как перемещения по 
// XYZ в текущем кадре
				CString val;
				bool ChamferEnabled = false;
				if(GetOtherValue("EnableOrtChamfer", val))
					if(val == "Yes")
						ChamferEnabled = true;	

				double i,j,k;
				if(ChangedIJK(St) && !ChamferEnabled)
				{
					i=State.CurI;
					j=State.CurJ;
					k=State.CurK;
					DisableIntersect = true;
				}
				else
				{
					i=State.CurX;
					j=State.CurY;
					k=State.CurZ;
					DisableIntersect = false;
				}
// Перпендикулярно к след кадру
				if(State.ParToNext && CompStart)
				{
					if(NextGeomFlag)
					{
						if(NextGeom.HaveGeom())
						{
							if(NextGeom.IsArc())
							{
								double xi,yi,zi;
								NextGeom.GetI(&xi,&yi,&zi);
								i = -yi;
								j = xi;
								k = 0.;
							
								if(NextGeom.IsCCWArc())
								{
									i = -i;
									j = -j;
								}
							}
							else
							{
								double xb,yb,zb,xe,ye,ze;
								NextGeom.GetB(&xb,&yb,&zb);
								NextGeom.GetE(&xe,&ye,&ze);
								i=xe-xb;
								j=ye-yb;
								k=ze-zb;
							}
						}
					}
				}
// END Перпендикулярно к след кадру

				switch(State.CurPlane)
				{
				case XY:
					zcomp=0.;
					xcomp=-j;
					ycomp=i;
					break;
				case XZ:
					ycomp=0.;
					xcomp=k;
					zcomp=-i;
					break;
				case YZ:
					xcomp=0.;
					ycomp=-k;
					zcomp=j;
					break;
				}

				if(State.CurDirComp == RIGHT)
				{
					xcomp *= -1.; ycomp *= -1.;	zcomp *= -1.;
				}
				double d=xcomp*xcomp+ycomp*ycomp+zcomp*zcomp;
				if(d > DMU * DMU) 
				{
					d=sqrt(1./d);
					xcomp *= d*State.CurDiamComp;
					ycomp *= d*State.CurDiamComp;
					zcomp *= d*State.CurDiamComp;
				}
				State.MashcX += xcomp;
				State.MashcY += ycomp;
				State.MashcZ += zcomp;
	// Calculate arc from end point of previos cadr to start point of current cadr
				if(State.EnableExtraArc)
				{
					if(Dist2(St.MashcX,St.MashcY,0.,
								St.MashX+xcomp,
								St.MashY+ycomp,
								0.) > MIN_ARC*MIN_ARC)
					{
						State.AddCompGeom.Set(State.CurDirComp == RIGHT ? ccwarc : cwarc,
								St.MashcX,St.MashcY,St.MashcZ,
								St.MashX+xcomp,
								St.MashY+ycomp,
								St.MashcZ,
								St.MashX-St.MashcX,
								St.MashY-St.MashcY,
								0,XY);
						State.CompGeom.Set(State.Geom.GetType(),
								St.MashX+xcomp,
								St.MashY+ycomp,
								St.MashcZ,
								State.MashcX,State.MashcY,State.MashcZ,
								0.,0.,0.,XY);
					}
					else
						State.CompGeom.Set(State.Geom.GetType(),
				 				St.MashcX,St.MashcY,St.MashcZ,
								State.MashcX,State.MashcY,State.MashcZ,
								0.,0.,0.,XY);
				}
	//END Calculate arc from end point of previous cadr to start point of current cadr
				else //if(State.EnableExtraArc)
				{
					if(State.Geom.HaveGeom())
					State.CompGeom.Set(State.Geom.GetType(),
				 			St.MashcX,St.MashcY,St.MashcZ,
							State.MashcX,State.MashcY,State.MashcZ,
							0.,0.,0.,XY);
				}
			}

		
			if(State.Geom.IsArc())
			{
				State.CompGeom.Offset(State.CurDirComp,State.CurDiamComp);
				if(State.CompGeom.HaveGeom())
					State.CompGeom.GetE(&State.MashcX,&State.MashcY,&State.MashcZ);
			}
//END коррекция по IJ

// С дополнительными дугами
			if((CType == Arc || State.IntNotFound) && !(State.ParToNext && CompStart) && !DisableIntersect)
			{
				const NCUnitStatePrevGeom &PrevGeomState = St.Geom.HaveGeom() ? St : LastGeomState;
				if (State.IntNotFound)
					State.IntNotFound = false;
				if(State.Geom.IsLine())
					if(State.Geom.HaveGeom())
					State.CompGeom.Set(State.Geom.GetType(),
					PrevGeomState.MashX + xcomp,
					PrevGeomState.MashY + ycomp,
					PrevGeomState.MashcZ,
							State.MashcX,State.MashcY,State.MashcZ,
							0.,0.,0.,XY);
				
				if (PrevGeomState.IsAngleConcave(State.Geom))
				{// change start point
					State.CompGeom.SetB(PrevGeomState.MashcX, PrevGeomState.MashcY, PrevGeomState.MashcZ);
				}
				else
				{// if previos angle is convex -> add extra arc
					if(State.Geom.HaveGeom())
						AddExtraArc(PrevGeomState);
				}
				if(NextGeomFlag && !State.RetractComp)
				{
					if(State.IsAngleConcave(NextGeom))
					{
						//enum curve	cType = State.Geom.GetType(),
						//			nType = NextGeom.GetType();
						if(State.Geom.HaveGeom() && NextGeom.HaveGeom())
						{
							BOOL t0,t1;// TRUE if otr, FALSE if arc
							t0 = State.Geom.IsLine();
							t1 = NextGeom.IsLine();

							if(	t0 && t1)
							{ // otr otr
								CompOtrOtr(NextGeom);
							}
							else
							{// at least one arc
								if(!t0 && !t1)
								{// arc arc
									CompArcArc(NextGeom);
								}
								else
								{// one arc and one line
									CompOtrArc(NextGeom, t0);
								}
							}
							State.CompGeom.SetE(State.MashcX, State.MashcY, State.MashcZ); 
						}
					}
				}
			}
// END С дополнительными дугами

// До пересечения
			if((CType == Int || CType == InternalInt) && !(State.ParToNext && CompStart) && !DisableIntersect)
			{
// This if was added for Pribor from SPb especially (29.06.07)
// It processes the case when Z movement occurs while compensation is active
				if(State.CurX == 0. && State.CurY == 0.) 
				{
					State.MashcX = St.MashcX;
					State.MashcY = St.MashcY;
					State.MashcZ = St.MashcZ + State.CurZ;

					State.CompGeom.Set(State.CurCurveType,
						St.MashcX,St.MashcY,St.MashcZ,
						State.MashcX,State.MashcY,State.MashcZ);
				}
				else
				{
					if(!State.RetractComp)
					{
						if(NextGeomFlag)
						{
							if(CType == InternalInt)
							{
								if(State.IsAngleConcave(NextGeom))
									CompToInters(NextGeom, St);
							}
							else
								CompToInters(NextGeom, St);

						}
					}
				}
			}
// END До пересечения
		}// END if(State.CurDirComp != LR_OFF) // Включена контурная коррекция
	}// End if(State.CompCancel) else

	//Length offset compensation (g45-g48)
	if(State.Offset != LO_UNDEF)
	{ 
		double xo,yo,zo,ro;
		ro = xo = yo = zo = State.CurDiamComp;
		int dxsign = State.CurX > 0 ? 1 : -1;
		if(fabs(State.CurX) < DMUO)
			dxsign = 0;
		int dysign = State.CurY > 0 ? 1 : -1;
		if(fabs(State.CurY) < DMUO)
			dysign = 0;
		int dzsign = State.CurZ > 0 ? 1 : -1;
		if(fabs(State.CurZ) < DMUO)
			dzsign = 0;
// КОРРЕКЦИЯ ИЗМЕНЯЕТ НЕ ПЕРЕМЕЩЕНИЕ, А КООРДИНАТУ
		dxsign = abs(dxsign);
		dysign = abs(dysign);
		dzsign = abs(dzsign);
		switch(State.Offset)
		{
		case INCR:
			ro *= 1;
			xo *= 1*dxsign;
			yo *= 1*dysign;
			zo *= 1*dzsign;
			break;
		case DECR:
			ro *= -1;
			xo *= -1*dxsign;
			yo *= -1*dysign;
			zo *= -1*dzsign;
			break;
		case INCR2X:
			ro *= 2;
			xo *= 2*dxsign;
			yo *= 2*dysign;
			zo *= 2*dzsign;
			break;
		case DECR2X:
			ro *= -2;
			xo *= -2*dxsign;
			yo *= -2*dysign;
			zo *= -2*dzsign;
			break;
		}
		if(State.Geom.IsArc())
		{
			double r=State.CurI*State.CurI+State.CurJ*State.CurJ;
			if(r>0.) r=sqrt(r);
			double d = (r+ro)/r;
			double bi,bj,bk;
			State.Geom.GetI(&bi,&bj,&bk);
			State.MashcX = d*(State.MashX + (St.MashcX - St.MashX + bi*(d-1.))) + (1.-d)*(St.MashcX + bi*d);
			State.MashcY = d*(State.MashY + (St.MashcY - St.MashY + bj*(d-1.))) + (1.-d)*(St.MashcY + bj*d);
			State.CompGeom.Set(State.Geom.GetType(),
					St.MashcX,St.MashcY,St.MashcZ,
					State.MashcX,State.MashcY,State.MashcZ,
					d*bi,d*bj,0.,XY);
		}
		else
		{
			// 2.7 UPDATE START
			/*
			State.MashcX = St.MashcX + State.CurX;
			State.MashcY = St.MashcY + State.CurY;
			State.MashcZ = St.MashcZ + State.CurZ;
			if(xo != 0.)
				State.MashcX = St.MashX + State.CurX + xo;
			if(yo != 0.)
				State.MashcY = St.MashY + State.CurY + yo;
			if(zo != 0.)
				State.MashcZ = St.MashZ + State.CurZ + zo;
			State.CompGeom.Set(State.CurCurveType,
					St.MashcX,St.MashcY,St.MashcZ,
					State.MashcX,State.MashcY,State.MashcZ,
					0.,0.,0.,XY);
					*/
			double d = sqrt(State.CurX*State.CurX + State.CurY*State.CurY + State.CurZ*State.CurZ);
			if(d > 0.) d = (d + ro)/d;
			State.MashcX = St.MashcX + State.CurX*d;
			State.MashcY = St.MashcY + State.CurY*d;
			State.MashcZ = St.MashcZ + State.CurZ*d;
			State.CompGeom.Set(State.Geom.GetType(),
					St.MashcX,St.MashcY,St.MashcZ,
					State.MashcX,State.MashcY,State.MashcZ,
					0.,0.,0.,XY);
			// 2.7 UPDATE START (ЮВ утвердил 14.05.05)
		}
	}
	return 0;
}

bool NCUnit::GetDValue(CString& text, double* val)
{
	return GetDValue(text, val, GCD.DecimalConversion);
}

BOOL NCUnit::ProcessCycle(NCadr *pCadr, bool ResetMatrs)
{
	BOOL ng = CycleNeedsGeom();
	BOOL dopc = FALSE;
	CString str;
	if(GCD.SOthers.Lookup("CycleNeedsParam",str))
		if(str == "Yes")
			dopc = TRUE;
	BOOL XYchanged = State.CurX != 0. || State.CurY != 0.  || State.CurZ != 0. || State.CurA != 0. || State.CurB != 0. || State.CurC != 0.;
	BOOL ZRchanged = St.CycleZ != State.CycleZ ||
						St.R1 != State.R1 ||
						St.R2 != State.R2;
	BOOL FirstCadr = (St.CurCycle != State.CurCycle);
	if (State.CurCycle == TURNROUGH ||
		(FirstCadr && !ng) || XYchanged || (!FirstCadr && ZRchanged && dopc) || State.CurCycle == TURNCUT || State.CurCycle == TURNFACECUT || State.CurCycle == TURNCLEAR)
	{// cycle defined
		pCadr->pCycle = new NCycle;

		if(true /*State.CurX != 0. || State.CurY != 0.*/)// X or Y changes in current cadr
		{
			if(pCadr->pCycle == NULL)
			{
				Error = 1087;// level 1
				return FALSE;
			}
			CString pc("No"); 
			GCD.SOthers.Lookup("ParamCycle",pc);
			if(pc == "Yes")
				SubstCycleParam();
			switch(State.CurCycle)
			{
			case DRILL_OLD:
			case DRILL_X:
			case DRILL_Y:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILL:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILL_MP81:
				if(CrMP81Cycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILL_HM:
				if(CrHMCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILL_HMC:
				if(CrHMCCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILL_HN:
				if(CrHNCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILLDWELL:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case RHTAPPINGX:
			case RHTAPPINGZ:
			case RHTAPPINGPLANE:
				if(CrThreadingTapCycle(pCadr->pCycle, State.CurCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case DRILLPECK:
			case DRILLPECKZ:
			case DRILLPECKX:
				if(CrMillDrillPeckCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case CUTTING:
				if (CrCuttingCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case GROOVE:
				if (CrGrooveCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case LEFTCUTTING:
				if(CrCuttingCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TAPPING:
				if(CrTappingCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case LEFTTAPPING:
				if(CrTappingCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case NUMERICON:
				if(CrNumerCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case RIGHTRHTAPPING:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case REAM:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case BORE:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case BACKBORE:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case BOREDWELL:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case REAMDWELL:
				if(CrDrillCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case RECTANGLE:
				if(CrRectCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case CIRCULARPOKET:
				if(CrCircleCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case RECTPOKET:
				if(CrRectangCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNROUGH:
				if(CrTurnRoughCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNLOOP:
				if(CrTurnLoopCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNCLEAR:
				if(CrTurnClearCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNGROOVX:
			case TURNGROOVZ:
				if(CrTurnGroovCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNTHREADCUT:
				if(CrTurnThreadCutCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNCHAMFER:
				if(CrTurnChamfCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNROUND:
				if(CrTurnRoundCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNCUTROUGH:
			case TURNFACECUTROUGH:
				if(CrTurnCutRoughCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNTHREAD:
				if(CrTurnThreadCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNTHREADAUTO:
				if(State.CurX != 0. || State.CurZ != 0.)
				{// Second cadr
					if(CrTurnThreadACycle(pCadr->pCycle) != 0)
					{
						Error = 1087;// level 1
						return FALSE;
					}
				}
				else
				{// First cadr
					State.CurCycle = CYCLE_OFF;
				}
				break;
			case TURNTHREADC:
				if(CrTurnThreadCCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case TURNFACECUT:
			case TURNCUT:
				if(CrTurnCutCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case APPRCT:
			case APPRLT:
			case APPRLN:
			case APPRLCT:
			case DEPCT:
			case DEPLT:
			case DEPLN:
			case DEPLCT:
				if(CrApprCycle(pCadr->pCycle) != 0)
				{
					// Error is defined in CrApprCycle
					return FALSE;
				}
				break;
			case MILLRECTCENTER:
			case MILLRECTCORNER:
				if(CrMillRectCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case MILLRECTCENTERIN:
			case MILLRECTCORNERIN:
			case MILLRECTCORNERINCLEAR:
			case MILLRECTCENTERINCLEAR:
			case MILLSLOTIN:
				if(CrMillRectInCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case MILLRECTCENTEROUT:
			case MILLRECTCORNEROUT:
				if(CrMillRectOutCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case MILLCIRC:
				if(CrMillCircCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case MILLCIRCIN:
			case MILLHOLECLEAR:
			case MILLCIRCINCLEAR:
				if(CrMillCircInCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case MILLCIRCOUT:
				if(CrMillCircOutCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case MILLCONT:
				if(CrMillContCycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			case SPLINEA:
				if(CrSplineACycle(pCadr->pCycle) != 0)
				{
					Error = 1087;// level 1
					return FALSE;
				}
				break;
			default:
				return 0;
				break;
			}
			RunCycle(pCadr, ResetMatrs);
		}
	}
	return TRUE;
}
BOOL NCUnit::ProcessCycleParam(CString &text, CString CurWord)
{
// При обработке цикла координаты XYZ IJK R обрабатываются в этом
// модуле, а не по общим правилам

	double val=0;
	if(!GetDValue(text,&val))
	{
		if (StoreError(1029))
			return FALSE;
		else
			return TRUE;
	}
	CString num;
	num.Format("%lf", val);
	val *= GCD.LinearScale;

	const BOOL BufAC = State.AbsoluteCoord;
	if (CurWord == "HorizontalAxisInc" ||// X-coord
		CurWord == "VerticalAxisInc" ||// Y-coord
		CurWord == "SpindleAxisInc")// Z-coord
	{
		CurWord.Delete(CurWord.GetLength() - 3, 3);
		State.AbsoluteCoord = FALSE;
	}
	else if (CurWord == "HorizontalAxisAbs" ||// X-coord
		CurWord == "VerticalAxisAbs" ||// Y-coord
		CurWord == "SpindleAxisAbs")// Z-coord
	{
		CurWord.Delete(CurWord.GetLength() - 3, 3);
		State.AbsoluteCoord = TRUE;
	}

	if(	CurWord == "HorizontalAxis")// X-coord
	{
 		if(State.IsNotSpecCycle() && State.CurPlane == YZ)
		{
			State.CycleDepthZ = val * GCD.XScale * State.CurXScale
				+ State.CurBase.x0 + (State.AbsoluteCoord ? 0. : State.MashX);
			if(BufAC == State.AbsoluteCoord)
				State.CycleZ = num;
			else
			{
				if (State.AbsoluteCoord)
				{
					double NewVal = (State.CycleDepthZ - State.CurBase.x0 - State.MashX) / (GCD.LinearScale * GCD.XScale * State.CurXScale);
					State.CycleZ.Format("%lf", NewVal);
				}
				else
				{
					double NewVal = (State.CycleDepthZ - State.CurBase.x0) / (GCD.LinearScale * GCD.XScale * State.CurXScale);
					State.CycleZ.Format("%lf", NewVal);
				}
			}
		}
		else
		{
			if(State.FixX)
			{
				if (StoreError(1011))
					return FALSE;
			}
			val *= GCD.XScale * State.CurXScale;
			if(State.ZeroPreset) // G92 is active
			{
 				if(State.ZeroPresetType == ZPT_REL)
					State.CurBase.x0 = State.MashX - val;
				else if (State.ZeroPresetType == ZPT_SHT)
					State.CurBase.x0 += val;
				else
				{
					if(State.AbsoluteCoord)
						State.CurBase.x0 = State.GetActiveUCS().x0 + val;
					else
						State.CurBase.x0 += val;
				}
			}
			else
			{
				bool Used = false;
				if(State.PolarCoordActive)
				{
					switch(State.CurPlane)
					{
					case XY:
						Used = true;
						State.PolarDist = val;
						break;
					case XZ:
						Used = true;
						State.PolarAngle = val;
						break;
					}
				}
				if(!Used)
				{
					val += State.CurBase.x0;
					if(State.AbsoluteCoord)
						State.CurX = val - State.MashX;
					else
						State.CurX = val;
					State.MashX += State.CurX;
				}
			}
		}
	}
	if(	CurWord == "VerticalAxis")// Y-coord
	{
		if(State.IsNotSpecCycle() && State.CurPlane == XZ)
		{
			State.CycleDepthZ = val * GCD.YScale * State.CurYScale
				+ State.CurBase.y0 + (State.AbsoluteCoord ? 0. : State.MashY);
			if (BufAC == State.AbsoluteCoord)
				State.CycleZ = num;
			else
			{
				if (State.AbsoluteCoord)
				{
					double NewVal = (State.CycleDepthZ - State.CurBase.y0 - State.MashY) / (GCD.LinearScale * GCD.YScale * State.CurYScale);
					State.CycleZ.Format("%lf", NewVal);
				}
				else
				{
					double NewVal = (State.CycleDepthZ - State.CurBase.y0) / (GCD.LinearScale * GCD.YScale * State.CurYScale);
					State.CycleZ.Format("%lf", NewVal);
				}
			}
		}
		else
		{
			if(State.FixY)
			{
				if (StoreError(1012))
					return FALSE;
			}
			val *= GCD.YScale * State.CurYScale;
			if(State.ZeroPreset) // G92 is active
			{
 				if(State.ZeroPresetType == ZPT_REL)
					State.CurBase.y0 = State.MashY - val;
				else if (State.ZeroPresetType == ZPT_SHT)
					State.CurBase.y0 += val;
				else
				{
					if(State.AbsoluteCoord)
						State.CurBase.y0 = State.GetActiveUCS().y0 + val;
					else
						State.CurBase.y0 += val;
				}
			}
			else
			{
				bool Used = false;
				if(State.PolarCoordActive)
				{
					switch(State.CurPlane)
					{
					case XY:
						Used = true;
						State.PolarAngle = val;
						break;
					case YZ:
						Used = true;
						State.PolarDist = val;
						break;
					}
				}
				if(!Used)
				{
					val += State.CurBase.y0;
					if(State.AbsoluteCoord)
							State.CurY = val - State.MashY;
					else
							State.CurY = val;
					State.MashY += State.CurY;
				}
			}
		}
	}
	if(	CurWord == "SpindleAxis")// Z-coord
	{
		if(State.IsNotSpecCycle() && State.CurPlane == XY)
		{
			State.CycleDepthZ = val * GCD.ZScale * State.CurZScale
				+ State.CurBase.z0 + (State.AbsoluteCoord ? 0. : State.MashZ);
			if (BufAC == State.AbsoluteCoord)
				State.CycleZ = num;
			else
			{
				if (State.AbsoluteCoord)
				{
					double NewVal = (State.CycleDepthZ - State.CurBase.z0 - State.MashZ) / (GCD.LinearScale * GCD.ZScale * State.CurZScale);
					State.CycleZ.Format("%lf", NewVal);
				}
				else
				{
					double NewVal = (State.CycleDepthZ - State.CurBase.z0) / (GCD.LinearScale * GCD.ZScale * State.CurZScale);
					State.CycleZ.Format("%lf", NewVal);
				}
			}
		}
		else
		{
			if(State.FixZ)
			{
				if (StoreError(1013))
					return FALSE;
			}
			val *= GCD.ZScale * State.CurZScale;
			if(State.ZeroPreset) // G92 is active
			{
 				if(State.ZeroPresetType == ZPT_REL)
					State.CurBase.z0 = State.MashZ - val;
				else if (State.ZeroPresetType == ZPT_SHT)
					State.CurBase.z0 += val;
				else
				{
					if(State.AbsoluteCoord)
						State.CurBase.z0 = State.GetActiveUCS().z0 + val;
					else
						State.CurBase.z0 += val;
				}
			}
			else
			{
				bool Used = false;
				if(State.PolarCoordActive)
				{
					switch(State.CurPlane)
					{
					case XZ:
						Used = true;
						State.PolarDist = val;
						break;
					case YZ:
						Used = true;
						State.PolarAngle = val;
						break;
					}
				}
				if(!Used)
				{
					val += State.CurBase.z0;
					if(State.AbsoluteCoord)
							State.CurZ = val - State.MashZ;
					else
							State.CurZ = val;
					State.MashZ += State.CurZ;
				}
			}
		}
	}
	State.AbsoluteCoord = BufAC;

	if(	CurWord == "CircleDepthCenter")// K-coord
	{
		State.CycleTappingZ = num;
	}
	if(	CurWord == "CircleHorzCenter")// I-coord
	{
		State.CycleI = num;
	}
	if(	CurWord == "CircleVerticalCenter")// J-coord
	{
		State.CycleJ = num;
	}

	if(	CurWord == "CircleRadius" || CurWord == "RapidClearanceLevel")// R-coord
// Above line was TEMPORARY changed due to FANUC6M
// R1 - last R ; R2 - current R
	{
		if(State.R1 == CYCLE_UNDEF)
		{
			State.R1 = num;
			if(State.CurCycleRet == TOREF)
				State.R2 = State.R1;
		}
		else
		{
			if(State.CurCycleRet == TOREF)
				State.R2 = num;
		}
/*			S
		{
		case TOSTART:
			State.R2.Format("%.6lf",State.MashZ/(GCD.LinearScale*GCD.ZScale));
			break;
		case TOREF:
			break;
		}
		*/
	}
	if( CurWord == "RapidClearanceLevel1")// R-coord
	{
		State.R1 = num;
		if(State.CurCycleRet == TOREF)
			State.R2 = State.R1;
	}
	if(	CurWord == "RapidClearanceLevel2")// R-coord
	{
		if(State.CurCycleRet == TOREF)
			State.R2 = num;
	}

	return TRUE;
}

void NCUnit::JumpToHCadrRel(const BProgram *pDestProg, int DestHCadr, bool AbsHCadr)
{//Jump to the specified Hcadr of the specified program from the current pos
	CWaitCursor w;
	if(!pDestProg || DestHCadr < 0)
		return;
	CMapStringToNProgram *pPList = GetChannel().NGetProgramListCn();
	cadrID DestHID(pPList->GetIndex(pDestProg), DestHCadr);
	JumpForToHCadr(DestHID, AbsHCadr);

	GetChannel().SetCurProgrTDTCn();
}

bool NCUnit::JumpForToHCadr(cadrID CadrHID, bool AbsHCadr /* = false */)
{// Jump to CadrHID not only forward
	if (CadrHID.Prog < 0)
		return false;
	int Ind = GetStates().FindMaxLowerInd(CadrHID, AbsHCadr);
	const NCUnitState *St, *AddSt;
	const NCUnitStatePrevGeom *GeomSt;
	GetStates().GetStatePair(Ind, St, AddSt, GeomSt);
	Jump2State(*St, *AddSt, *GeomSt);
	//	JumpForToHCadr(CadrHID);
//Jump forward from current position until CadrIDEx == CadrHID or AbsCadrIDEx == CadrHID
	if(CadrHID <= (AbsHCadr ?
		++State.GetCurHCadrAbsEx() : State.GetCurCadrIDEx())// -
		)
	{
		pDoc->GetMachTool().ProcCadrStep(*this, State);
		return true;
	}
	bool Continue = true;
	for(int i = 0; i < WHOLE_PROG && Continue; ++i)
	{
		if (!IsCurCadrProtected())
		{
			if (CadrHID <= (AbsHCadr ? ++State.GetCurHCadrAbsEx() : State.GetCurCadrIDEx()))// -
				return true;
		}

		int ret = EvaluateCurrent(false);

		if(ret < -1)
			return false;

		if(ret == -1)// Program ended
			Continue = JumpToNextProg();
	}
	return false;
}


CString NCUnitBase::GetConfPath() const
{
	return NCM_PROJECT.GetUnitsPath();
}

int NCUnitBase::LookupSGeneral(const char *key, CString &res)
{
	return GCD.SGeneral.Lookup(key,res);
}

bool NCUnitBase::GetLabel(CString &text, CString &Label) const
{
	// Get label
	CString LabelFirst;
	if(!GCD.SWordDef.Lookup(_T("LabelFirst"), LabelFirst))
		return false;//error
	if(text.Left(1) != LabelFirst)
		return false;//error
	text.Delete(0);
	CString LabelLast;
	if(!GCD.SWordDef.Lookup(_T("LabelLast"), LabelLast))
		return false;//error
	const int EndInd = text.Find(LabelLast);
	if(EndInd < 0)
	{
		text.Empty();
		return false;//error
	}
	Label = text.Left(EndInd);
	text.Delete(0, EndInd + 1);
	return true;
	//END: Get label
}

bool NCUnitBase::FindCallFile(CString &text, cadrID *res)
{
	*res = State.CadrID;
	CString title;
	GCD.SWordDef.Lookup("ProgramNumber", title);
	if(title != text.Left(1))
	{
		Error = 1068;// level 1

		return false;//error
	}
	text.Delete(0);
	CString Label;
	if(!GetLabel(text, Label))
	{
		Error = 1068;// level 1
		return false;
	}
	int ProgNum = GetChannel().NGetProgramListCn()->GetIndexByShortName(Label);
	if(ProgNum < 0)
	{
		Error = 1071;// level 1
		return false;//error
	}
	res->Prog = ProgNum;
	res->Cadr = 0;
	return true;
}

bool NCUnitBase::FindNextCadr(CString &text, cadrID *res, bool FindByLineNumber)
{
	*res = State.CadrID;

	CString title;
	int num;
	CString format;
	//format: N[x1]L[x2] where	x1 - one digit number of digits in program number
	//							x2 --//-- of repetitions
	// or L[x2]Nx1
	if(!GCD.SOthers.Lookup("SubrCallFormat",format))
		format = "NL";
	if(format.GetLength() == 0)
		format = "NL";
	if(format[0] == 'L')
	{
		int k = format.Find('N');
		format.Delete(0, k + 1);
		if(format.GetLength() > 0 && k > 0)
		{
			CString tmptext = text;
			tmptext.Delete(0);
			CString tmp = tmptext.SpanIncluding("0123456789");
			tmptext.Delete(0, tmp.GetLength());
			if(isdigit(format[0]))
			{
				CString tmp1 = tmp.Left(tmp.GetLength() - atoi(format.Left(1)));
				tmp.Delete(0, tmp1.GetLength());
				text = text.Left(1) + tmp + tmp1 + tmptext;
				format = "N" + format.Left(1) + "L";
			}
			else
				format = "NL";
		}
		else
			format = "NL";
	}

	if(format[0] == 'N' && format.GetLength()>1)
		format.Delete(0);
	GCD.SWordDef.Lookup("ProgramNumber", title);
//	CString Word = GetWord(text);
	if(title != text.Left(1))
		if(State.ProgramNumber == "")
		{
			Error = 1068;// level 1
			return false;//error
		}
		else
			text = title + State.ProgramNumber;
	text.Delete(0);
// 
	CString btext;
	CString r_text = text;
	if(isdigit(format[0]))
	{
		btext = text.Left(atoi(format.Left(1)));
		format.Delete(0,1);
		r_text = btext;
		if(r_text.IsEmpty())
		{
			Error = 1068;// level 1
			return false;//error	
		}
	}
	text.Delete(0, r_text.GetLength());
	bool StringName = (r_text[0] == '"');
	if(StringName)
	{
		CString MacroName;
		if(!GetLabel(r_text, MacroName))
		{
			Error = 1070;// level 1
			return false;//error	
		}
		GetHashNum(MacroName, &num);
	}
	else
	{
		if(!GetIValue(r_text, &num))
		{
			Error = 1070;// level 1
			return false;//error	
		}
	}
	text = r_text + text;
	if(format.GetLength() > 0)
		if(format[0] == 'L')
		{
			CString RepNum;
			if(GCD.SWordDef.Lookup("RepeatNumber", RepNum))
				if(text.Left(1) == RepNum)
					text.Delete(0,1);
		}


	
// Now text contains repetitions number if any
		
	CString Word;
	CString key;
	CString Curtext;
	NProgram *pProg;

	const CMapStringToNProgram *pProgList = GetChannel().NGetProgramListCn();
	int CadrNum = -1;
	if(pProgList->GetAt(res->Prog, key, (CObject *&)pProg))
		CadrNum = FindCadrInProg(pProg, num, FindByLineNumber);
	if(CadrNum < 0)
	{
		for(int i = 0; i < pProgList->GetSize(); ++i)
		{
			if(i == res->Prog)
				continue;
			pProgList->GetAt(i, key, (CObject *&)pProg);
			CadrNum = FindCadrInProg(pProg, num, FindByLineNumber);
			if(CadrNum >= 0)
			{
				res->Prog = i;
				break;
			}
		}
	}
	res->Cadr = CadrNum;
	if(CadrNum >= 0)
		return true;
	Error = 1071;// level 1
	return false;
}

bool NCUnit::JumpForTo(cadrID ID)
{
//Jump forward from current position until CurCadrID == ID
	bool Continue = true;
	for(int i = 0; i < WHOLE_PROG && Continue; ++i)
	{
		if(ID <= State.GetCurCadrID())
			return true;

		int ret = EvaluateCurrent(false);

		if(ret < -1)
			return false;

		if(ret == -1)// Program ended
			Continue = JumpToNextProg();
	}
	return false;
}

int NCUnit::FindStartCadr()
{
	CString sp;
	if(!GCD.SOthers.Lookup("StartProgram",sp))
		return 0;//StartProgram = First by default
	BOOL last = (sp.Find("Last")>=0);
	BOOL cadr1 = (sp.Find("Cadr1")>=0);
	if( !last && !cadr1)
		return 0;//StartProgram = First by default
	if(last)
	{
// Find last subprogram title
		int cadr_num=0;
		CString Word;
		CString key;
		CString Curtext;
		BProgram *pProg;
		if(State.CadrID.Prog < 0)
			return 0;
		if(!GetChannel().NGetProgramListCn()->
				GetAt(State.CadrID.Prog,key,(CObject *&)pProg))
				return 0;
		for(int i=0;i<pProg->GetSize();i++)
		{
			Curtext = (*pProg)[i]->GetReText();
//			DoReplace(Curtext);
			Word = GetWord(Curtext);
			if(Word == "ProgramStartEnd")
			{
				cadr_num = i;
			}
		}
		return cadr_num;
	}
	if(cadr1)
	{
// Find N1 cadr
		int cadr_num=0;
		int num;
		CString Word;
		CString key;
		CString Curtext;
		BProgram *pProg;
		if(State.CadrID.Prog < 0)
			return 0;
		if(!GetChannel().NGetProgramListCn()->
				GetAt(State.CadrID.Prog,key,(CObject *&)pProg))
				return 0;
		for(int i=0;i<pProg->GetSize();i++)
		{
			Curtext = (*pProg)[i]->GetReText();
//			DoReplace(Curtext);
			Word = GetWord(Curtext);
			if(Word == "LineNumber")
			{
				GetIValue(Curtext,&num);
				if(num == 1)
					return i;
			}
		}
		return 0;
	}
	return 0;
}


BOOL NCUnit::GetFileExt(CString &ext)
{
	return GCD.SOthers.Lookup("FileExtension",ext);
}

double NCUnit::DecodeFeed(double val)
{
// feed is coded according to these rules
// if Fabcd is code then Feed = 10^(b-5)*(cd),
// where (b) and (cd) is the number constructed
// by digits b (or c and d)
	int cod = (int)val;
	int frt = cod/100;// All digits except last two ones
	int deg = frt - (frt/10)*10; // b digit only
	return (cod-frt*100)*pow(10.,deg-5);
}

int NCUnit::ProcessRegNum(CString &text)
{
	int RegNum = -1;
	if (text[0] == '[')
	{// RegNum is an expression
		int count = 1;
		// Find paired bracket
		int ind = 1;
		for (; ind < text.GetLength() && count > 0; ++ind)
		{
			if (text[ind] == '[')
				++count;
			else if (text[ind] == ']')
				--count;
		}
		if (count != 0)
		{
			return -1;
		}
		CString exp = text.Left(ind);
		NEvalExp expression(this, UseIntegerArithm, ConsecutiveCalc);
		double val;
		if (!expression.Evaluate(exp, &val))
		{
			return -1;
		}
		text.Delete(0, ind);
		RegNum = int(val + 0.5);
	}
	else
	{
		CString ConstCode = text.SpanIncluding("0123456789.");
		if (ConstCode.GetLength() == 0)
		{
			return -1;
		}
		// remove code from text
		text.Delete(0, ConstCode.GetLength());

		RegNum = atoi(ConstCode);
	}
	if (RegNum < 0 || RegNum >= MAX_REG)
	{
		return -1;
	}

	// added for 2P32M (incremental register operations) 4.05.04
	//! @10 - "Ri=Ко"
	//! @12 - "++"
	//! @13 - "--"
	//! @14 - "*(-1)"
	// @20 - "cos"
	// @21 - "Ri=arctg(Rj=1/Rj)"
	//! @4 - "+="
	//! @5 - "-="
	//! @6 - "*="
	//! @7 - "/="
	//! @28 - "R[Ri] = Rj"
	if (text.GetLength() >= 2 && text[0] == '@')
	{
		text.Delete(0);
		int op;
		if (!GetIValue(text, &op))
		{
			return -1;
		}
		CString code;
		code.Format(_T("%d"), RegNum);
		CString tmp = "=R" + code;
		switch (op)
		{
		case 4:
			tmp = tmp + "+";
			break;
		case 5:
			tmp = tmp + "-";
			break;
		case 6:
			tmp = tmp + "*";
			break;
		case 7:
			tmp = tmp + "/";
			break;
		case 12:
			tmp = "=1+";
			break;
		case 13:
			tmp = "=" + CString(NEvalExp::OpenBC()) + "-1" + CString(NEvalExp::CloseBC()) + "+";
			break;
		case 14:
			tmp = "=" + CString(NEvalExp::OpenBC()) + "-1" + CString(NEvalExp::CloseBC()) + "*";
			break;
		case 10:
			tmp = "=";
			break;
		case 28:
			tmp = "=";
			RegNum = (int)State.GetRegister(RegNum);
			if (RegNum < 0 || RegNum >= MAX_REG)
			{
				return -1;
			}
			break;
		}
		text = tmp + text;
	}
	// END added for 2P32M (incremental register operations) 4.05.04
	return RegNum;
}

BOOL NCUnit::ProcessRegister(CString &text, bool Round)
{
	if(text.GetLength() < 1)
	{
		Error = 1016;// level 1
		return FALSE;
	}
	int RegNum = ProcessRegNum(text);
	if (RegNum < 0 || RegNum >= MAX_REG)
	{
		Error = 1017;// level 1
		return FALSE;
	}
	CString code;
	code.Format(_T("%d"), RegNum);
	// added for MAJAK (implicit IF statement and incremental operations) 25.11.05
	if (text.GetLength() >= 1 && (text[0] == '>' || text[0] == '<'))
	{// implicit IF
		text = "IFR" + code + text;
		return TRUE;
	}
	CString str;
	if (GCD.SOthers.Lookup("IncrRegOp", str))
		if (str == "Yes")
		{
			if (text.GetLength() >= 1 && (text[0] == '-' || text[0] == '+' || text[0] == '*' || text[0] == '/'))
			{// incremental operations
				text = "=R" + code + text;
			}
		}
	// END added for MAJAK (implicit IF statement) 25.11.05
	double RegVal;
	if(!GetDValue(text,&RegVal))
	{
		if (StoreError(1019))
			return FALSE;
		else
			return TRUE;
	}
	if (Round)
		RegVal = floor(RegVal + 0.5);
	State.SetRegister(RegNum, RegVal);

	return TRUE;
}

BOOL NCUnit::ProcessDwell(CString &text)
{
	double step = DWELL_STEP;
	CString Sstep;
	if(GCD.SOthers.Lookup("DwellTimeDiscr",Sstep))
		step = atoi(Sstep) * 0.001;

	if(text.GetLength() <= 0)
	{
		State.CompGeom.SetWorkTimeS(State.GlobalDwell * step);
		return TRUE;// Dwell time GlobalDwell or 0 assumed
	}
	double DwellVal = 0.;
	bool DTPresent = GetDIParamValSoft(text, _T("DwellTime"), &DwellVal);
	double DwellValSec = 0.;
	bool DTPresentSec = GetDIParamValSoft(text, _T("DwellTimeSec"), &DwellValSec);
	if (!DTPresent && !DTPresentSec)
	{
		State.CompGeom.SetWorkTimeS(State.GlobalDwell * step);
		return TRUE;// If DwellSym is not defined it is assumed that DwellTime = GlobalDwell
	}
	if (DTPresentSec)
	{
		State.CompGeom.SetWorkTimeS(DwellValSec);
		return TRUE;
	}
	State.CompGeom.SetWorkTimeS(DwellVal * step);
	return TRUE;
}

double NCUnit::GetLinearDecimalZero()
{
	double ret = -1.;
	CString SZero;
	if(GCD.SOthers.Lookup("LinearDecimalZero", SZero))
	{
		int deg = atoi(SZero);
		if(deg >= 0)
		{
			ret = 1.;
			for(int i = 0; i < deg; ++i)
				ret *= 10.;
		}
	}
	return ret;
}

double NCUnit::GetScale(const CString &str)
{
// Calculate Scale = 1./(10^LinearConversion)
// If any error => Scale = 1.
	CString SScale;
	if(!GCD.SOthers.Lookup(str, SScale))
		return 1.;

	if(SScale.Find('.') >= 0)
		return atof(SScale);
// This format is deprecated since v.3.8 30.11.09
	int deg;
	double ret = 1.;
	deg = atoi(SScale);
	if( deg >= 0)
	{
		for(int i=0;i<deg;i++)
			ret *= 10.;
		return 1./ret;
	}
	else
	{
		deg = -deg;

		for(int i=0;i<deg;i++)
			ret *= 10.;
		return ret;
	}
}

double NCUnit::GetFeedScale()
{
// Calculate Scale = 1./(10^FeedDecimalConversion)
// If any error => Scale = 1.
	return GetScale("FeedDecimalConversion");
}

double NCUnit::GetSpeedScale()
{
// Calculate Scale = 1./(10^SpeedDecimalConversion)
// If any error => Scale = 1.
	return GetScale("SpeedDecimalConversion");
}

double NCUnit::GetRotaryScale()
{
// Calculate Scale = 1./(10^RotaryDecimalConversion)
// If any error => Scale = 1.
	return GetScale("RotaryDecimalConversion");
}

int NCUnit::CreateRArc(const NCUnitStatePrev &St, NCadrGeom *pGeom)
{
	enum Plane CurPlane = State.CurPlane;
	int res = -1;
	switch(CurPlane)
	{
	case XY:
		res = CreateRArcXY(St.MashX, St.MashY, St.MashZ,
			State.MashX, State.MashY, St.MashZ, State.MashZ, pGeom);
		break;
	case XZ:
		State.CurPlane = XY;
		res = CreateRArcXY(St.MashX, -St.MashZ, St.MashY,
			State.MashX, -State.MashZ, St.MashY, State.MashY, pGeom);
		pGeom->Tr(BMatr().RotX(BPoint(0.,0.,0.,1.), -90.));
		State.CurPlane = CurPlane;
		break;
	case YZ:
		State.CurPlane = XY;
		res = CreateRArcXY(-St.MashZ, St.MashY, St.MashX,
			-State.MashZ, State.MashY, St.MashX, State.MashX, pGeom);
		pGeom->Tr(BMatr().RotY(BPoint(0.,0.,0.,1.), 90.));
		State.CurPlane = CurPlane;
		break;
	}
	return res;
}
int NCUnit::CreateRArcXY(double x1, double y1, double z1,
		double x2, double y2, double z2, double zst, NCadrGeom *pGeom)
{
	double R = fabs(State.CurR);
	double dist2 = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1);
	if(dist2 == 0.)
		return 0;
	double dist = sqrt(dist2);
	double s2 = R - 0.5*dist;
	if(fabs(s2) < GCD.MinArcDist)
	{
		pGeom->Set(State.CurCurveType,
			x1,y1,z1,x2,y2,zst,
			0.5*(x2-x1),0.5*(y2-y1),0.5*(z2-z1),State.CurPlane);
		return 0;
	}
	if(s2 < 0.)
		return -1;
	double s = sqrt(fabs(R * R - 0.25 * dist2));
	double	x = x2 - x1,
			y = y2 - y1,
			z = z2 - z1;// P2 - P1 vector

	x = y1 - y2;
	y = x2 - x1;

	double d = s/dist;
	if(State.CurCurveType == cwarc)
		d = -d;
	if(State.CurR < 0.)
		d = -d;
	x *= d;
	y *= d;
	z *= d;
	double	xc = (x1 + x2)*0.5 + x,
			yc = (y1 + y2)*0.5 + y,
			zc = (z1 + z2)*0.5 + z;
	pGeom->Set(State.CurCurveType,
		x1,y1,z1,x2,y2,zst,
		xc - x1,yc - y1,zc - z1,
		State.CurPlane);

	return 0;
}

double NCUnit::GetMinArcDist()
{
	return GCD.MinArcDist;
}
double NCUnit::CalcMinArcDist()
{
	double eps = DMUO;
	CString SZero;
	if(GCD.SOthers.Lookup("MinArcDist",SZero))
	{
		int deg;
		double ret = 1.;
		deg = atoi(SZero);
		for(int i = 0; i < deg; ++i)
			ret *= 10.;
		eps = 1. / ret;
	}
	return eps;
}



NProgram * NCUnit::MakeMainInitProg(const CString &text)
{
	if (text.IsEmpty())
		return nullptr;
	CString PerCent;
	if(!GCD.SWordDef.Lookup("ProgramBeginEnd", PerCent))
		return NULL;
	CString M;
	if(!GCD.SWordDef.Lookup("MiscFunctionCode", M))
		return NULL;
	CString PrEnd;
	if(!GCD.SMiscFuncDef.Lookup("EndProgram", PrEnd))
		return NULL;
	const CString *pStr = &text;
	NProgram *pProg = pDoc->NewProgram();
	pProg->SetInitProg();
	pProg->AddCadr(new NCadr(PerCent));
	for(int Pos = 0, Pos1 = 0; Pos < pStr->GetLength(); Pos = Pos1 + 1)
	{
		Pos1 = pStr->Find('\n', Pos + 1);
		if(Pos1 < 0)
			Pos1 = pStr->GetLength();
		pProg->AddCadr(new NCadr(pStr->Mid(Pos, Pos1 - Pos)));
	}
	pProg->AddCadr(new NCadr(M + PrEnd));
	return pProg;
}

void NCUnit::ProcDiaCompOffset(int n)
{
// DiaCompOffset processing
// n - diameter corrector number
	double val;
	if(!State.DComp.Lookup(n,val))
	{
		StartState.DComp[n] = 0.;
		val = 0.;
	}
	State.SetRegister(4107, n);
	State.CurDiamComp = val * GCD.LinearScale;
	CString str;
	if(GCD.SOthers.Lookup("DHCompEQ", str))
		if(str == "Yes")
			ProcLengthCompOffset(n);
}

void NCUnit::ProcLengthComp4Cycle()
{
	// Process length compensation
	if (CycleGeom.GetSize() == 0)
		return;
	if (State.CurZComp != UD_OFF)// to do
	{
		BMatr DisplM = BMatr().Trans(St.Geom.GetE(), St.CompGeom.GetE());
		if (CycleGeomComp.GetSize() == 0)
		{// Copy CycleGeom to CycleGeomComp
			for (int k = 0; k < CycleGeom.GetSize(); ++k)
				CycleGeomComp.AddElement(*CycleGeom[k]);
		}
		for (int k = 0; k < CycleGeomComp.GetSize(); ++k)
			CycleGeomComp[k]->Tr(DisplM);
	}
	// END:Process length compensation
}

void NCUnit::ProcLengthCompOffset(int n)
{
// LengthCompOffset processing
// n - length corrector number
	double val;
	if(!State.HComp.Lookup(n, val))
	{
		StartState.HComp[n] = 0.;
		val = 0.;
	}
	State.SetCurLengthComp(val);

	if(!State.DComp.Lookup(1000 + n, val))
	{
		StartState.DComp[1000 + n] = 0.;
		val = 0.;
	}
	State.CurCodeComp = val;
	State.CurLengthCompNum = n;
}




//int NCUnit::CrLasCycle(NCycle *pCycle)
//{
///*	NCadr *pcCadr;
//	CString text,g,g0,g1,b,z,beg,g91,g90;
//	struct FDef *pFDef;
//	if( !GCD.SWordDef.Lookup("FunctionCode",g) ||
//		!GCD.SWordDef.Lookup("SpindleAxis",z) ||
//		!GCD.SWordDef.Lookup("ProgramBeginEnd",beg) ||
//		!GCD.SFuncDef.Lookup("Rapid",pFDef))   
//		return -1;
//	g0 = g + pFDef->Num;
//	if( !GCD.SFuncDef.Lookup("Linear",pFDef))   
//		return -1;
//	g1 = g + pFDef->Num;
//	if( !GCD.SFuncDef.Lookup("IncrementalUnit",pFDef))   
//		return -1;
//	g91 = g + pFDef->Num;
//	if( !GCD.SFuncDef.Lookup("AbsoluteUnit",pFDef))   
//		return -1;
//	g90 = g + pFDef->Num;
//	
//	if(State.R2 == CYCLE_UNDEF)
//		return -1;
//		State.CycleX = 0.;
//	if(State.CycleY == CYCLE_UNDEF)
//		State.CycleY = 0.;
//	text = beg + "LasCycle";pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
//	text = g91 + g0 + X + State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
//	text = g1 + z + State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
//
//	text = g0;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
//	*/
//	return 0;
//}

CString NCUnit::GetStartProgram()
{
	CString sp;
	if(!GCD.SOthers.Lookup("StartProgram",sp))
		return "";
	return sp;
}

BOOL NCUnit::GetRegTypedValue(const CString& name, double* value, bool &Integer)
{
	*value = 0.;
	if (GCD.RegisterSymbol == '0')
		return FALSE;
	if (name[0] != GCD.RegisterSymbol)
		return FALSE;
	CString num = name;
	num.Delete(0);
	if (num.GetLength() == 0)
		return FALSE;
	int RegNum;
	if (!GetIValue(num, &RegNum))
		return false;
	Integer = IsRegInteger(RegNum);
	return GetRegValue(RegNum, value);
}

bool NCUnit::IsRegInteger(int RegNum) const
{
	return State.GetRegisters().IsInteger(RegNum);
}

BOOL NCUnit::GetRegValue(const CString &name, double *value)
{
	*value = 0.;
	if(GCD.RegisterSymbol == '0')
		return FALSE;
	if(name[0] != GCD.RegisterSymbol)
		return FALSE;
	CString num = name;
	num.Delete(0);
	if(num.GetLength() == 0)
		return FALSE;
	int RegNum;
	if(!GetIValue(num, &RegNum))
			return false;
	
	return GetRegValue(RegNum, value);
}
BOOL NCUnit::GetRegValue(int RegNum, double *value)
{
	if(RegNum >= State.DComp2RegStart && RegNum <= State.DComp2RegEnd)
	{// Get D-compensation value
		if(!State.DComp.Lookup(RegNum - State.DComp2RegStart, *value))
			*value = 0.;
	}
	else
		*value = State.GetRegister(RegNum);
	return TRUE;
}

double NCUnit::GetMaxWorkFeed()
{
	double ret=100000.;
	CString Str;
	if(!GCD.SOthers.Lookup("MaxFeedRate",Str))
		return ret;
	ret = atof(Str);
	if(ret == 0.)
		ret = 100000.;
	return ret;
}

BOOL NCUnit::FindCopy(CString &text,cadrID *res,int *end, enum SearchDir SDir)
{
	// Find start cadr of program copy
	int num;
	double dval;
	switch(GCD.LType)
	{
	case LT_EMPTY:
		if(!GetIParamVal(text, "ProgramCopyStart", &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	case LT_EXPRESSION:
		if(!GetDParamVal(text, "ProgramCopyStart", &dval))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		num = int(dval + 0.5);
		break;
	case LT_BALTSYS:
	case LT_SINUMERIK:
		if(!GetLParamVal(text, "ProgramCopyStart", &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	}

	cadrID cadr = findMark(num, SDir);
	if (cadr.Prog == -1)
	{
		Error = 1022;// level 1
		return FALSE;// Error
	}
	*res = cadr;

// find end cadr number
	switch(GCD.LType)
	{
	case LT_EMPTY:
		if(!GetIParamVal(text, "ProgramCopyEnd", &num))
		{
			return false;//error	
		}
		break;
	case LT_EXPRESSION:
		if(!GetDParamVal(text, "ProgramCopyEnd", &dval))
		{
			return false;//error	
		}
		num = int(dval + 0.5);
		break;
	case LT_BALTSYS:
	case LT_SINUMERIK:
		if(!GetLParamVal(text, "ProgramCopyEnd", &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	}
	*end = num;
	return TRUE;
}

BOOL NCUnit::FindCopy1L(CString& text, cadrID* res, enum SearchDir SDir)
{
	// Find start cadr of program copy
	int num;
	double dval;
	switch (GCD.LType)
	{
	case LT_EMPTY:
		if (!GetIParamVal(text, "ProgramCopyStart", &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	case LT_EXPRESSION:
		if (!GetDParamVal(text, "ProgramCopyStart", &dval))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		num = int(dval + 0.5);
		break;
	case LT_BALTSYS:
	case LT_SINUMERIK:
		if (!GetLParamVal(text, "ProgramCopyStart", &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	}

	cadrID cadr = findMark(num, SDir);
	if (cadr.Prog == -1)
	{
		Error = 1022;// level 1
		return FALSE;// Error
	}
	*res = cadr;
	return TRUE;
}

BOOL NCUnit::FindCopy(int num, cadrID *res)
{
	// Find start cadr of program copy
	cadrID cadr = State.CadrID;
	cadr.Cadr = 100000000;
	CString key;
	BProgram *pProg;
	GetChannel().NGetProgramListCn()->
		GetAt(cadr.Prog, key, (CObject *&)pProg);
	if(!pProg->GetGotoList().isCreated)
		MakeGotoList(pProg);
	int IntCadr = pProg->GetGotoList().findMark(num, -1, false);

	if(IntCadr == -1)
	{
		Error = 1022;// level 1
		return FALSE;// Error
	}
	cadr.Cadr = IntCadr;
	*res = cadr;
	return TRUE;
}

BOOL NCUnit::ReturnFromSub(BOOL cont)
{
// returns from subroutine
	if (State.ActiveCopyEnd != -1)
	{
		// program copy is active
		// pop extra element
		cadrID p;
		int count;
		State.CallStack.Pop(p, count);
	}
	cadrID p;
	int count;
	if(State.CallStack.Pop(p,count) < 0)
	{
		if(!cont)
			State.EndProgram = true;// Added for laser ???????????
		return TRUE;
	}
	if (State.ActiveCopyEnd != -1)
		State.ActiveCopyEnd = count;
	State.OrderChanged = true;
	State.NextCadr = p;
	if(State.CallStack.IsEmpty())
		State.Pop133Registers();
	State.GetVarSet().PopRegsMap();
	return TRUE;
}

BOOL NCUnit::ProcessForStartSinum(CString& text)
{
	if (text.GetLength() < 7)
	{
		Error = 1207;// ERROR wrong for statement
		return FALSE;
	}
	bool First = false;
	int count;
	cadrID p;
	if (State.WhileStack.Pop(p, count) < 0)
		First = true;
	else if (p != State.GetCurCadrID())
	{
		First = true;
		State.WhileStack.Push(p, count);
	}
	const CString CyclePar = text.SpanExcluding(_T("="));
	if (First)
	{
		if (!ProcessAssignmentStatement(text, true))
			return FALSE;//Error defined in ProcessAssignmentStatement
	}
	else
	{
		CString TmpText = CyclePar + _T("=") + CyclePar + _T("+1");
		if (!ProcessAssignmentStatement(TmpText, true))
			return FALSE;//Error defined in ProcessAssignmentStatement
	}
	const auto TOPos = text.Find(_T("TO"));
	if (TOPos < 0)
	{
		Error = 1206;// ERROR "TO" word missing
		return FALSE;
	}
	text.Delete(0, TOPos + 2);
	CString IfString = CyclePar + _T(">") + text;
	text.Empty();
	bool Res = false;
	if (!ProcessCondition(IfString, Res))
		return FALSE;//Error defined in ProcessIF
	if (Res) 
	{// cycle is over
		BOOL b = DoGoToGroupEnd(State.CadrID);
		if (b)
			State.NextCadr.Cadr++;
		else
			return FALSE;//Error defined in DoGoToGroupEnd
		return TRUE;
	}
	else
	{// cycle continues
		int count = 0;
		cadrID p = State.CadrID;
		State.WhileStack.Push(p, count);
		return TRUE;
	}
}

BOOL NCUnit::ProcessWhileStartSinum(CString& text)
{
	bool Res;
	if (!ProcessCondition(text, Res))
		return FALSE;//Error defined in ProcessIF
	int count = 0;
	cadrID p = State.CadrID;
	if (State.WhileStack.Pop(p, count) < 0)
		;
	else if (p != State.GetCurCadrID())
	{
		State.WhileStack.Push(p, count);
	}
	if (!Res)
	{// cycle is over
		BOOL b = DoGoToGroupEnd(State.CadrID);
		if (b)
			State.NextCadr.Cadr++;
		else
			return FALSE;//Error defined in DoGoToGroupEnd
		return TRUE;
	}
	else
	{// cycle continues
		State.WhileStack.Push(p, count);
		return TRUE;
	}
}

BOOL NCUnit::ProcessRepeatSinum()
{
	int count = 0;
	cadrID p = State.CadrID;
	State.WhileStack.Push(p, count);
	return TRUE;
}

BOOL NCUnit::ProcessUntilSinum(CString& text)
{
	bool Res;
	if (!ProcessCondition(text, Res))
		return FALSE;//Error defined in ProcessIF
	if (Res)
	{// cycle is over
		return TRUE;
	}
	else
	{// cycle continues
		return ProcessCycleEndSinum();
	}
}

BOOL NCUnit::ProcessLoopStartSinum(CString& text)
{
	bool First = false;
	int count;
	cadrID p;
	if (State.WhileStack.Pop(p, count) < 0)
		;
	else if (p != State.GetCurCadrID())
	{
		State.WhileStack.Push(p, count);
	}

	p = State.GetCurCadrID();
	State.WhileStack.Push(p, 0);

	return TRUE;
}

BOOL NCUnit::ProcessIfSinum(CString& text)
{
	bool Res = false;
	if (!ProcessCondition(text, Res))
		return FALSE;//Error defined in ProcessIF
	if (!Res)
	{
		BOOL b = DoGoToGroupEnd(State.CadrID);
		if (!b)
			return FALSE;//Error defined in DoGoToGroupEnd
	}
	return TRUE;
}

BOOL NCUnit::ProcessElseSinum(CString& text)
{
	if (!St.GotoElse)
		return DoGoToGroupEnd(State.GetCurCadrID());
	return TRUE;
}

BOOL NCUnit::ProcessElseIfSinum(CString& text)
{
	if (St.GotoElse)
		return ProcessIfSinum(text);
	return DoGoToGroupChainEnd(State.GetCurCadrID());
}

BOOL NCUnit::ProcessCycleEndSinum()
{
	int count;
	cadrID p;
	if (State.WhileStack.Pop(p, count) < 0)
	{
		return TRUE;
	}
	State.WhileStack.Push(p, count);
	State.OrderChanged = true;
	State.NextCadr = p;
	return TRUE;
}

BOOL NCUnit::ProcessAssignmentStatement(CString& text, bool Round)
{
	const auto CurWord = GetWord(text);
	if (CurWord == "Register")// Register
	{
		if (!ProcessRegister(text, Round))
			return FALSE;//Error defined in ProcessRegister
	}
	else if (CurWord == _T("VariablePrefix"))
	{
		ProcessVariable(text, false);
		text.Delete(0, 1); // register symbol
		if (!ProcessRegister(text, Round))
			return FALSE;//Error defined in ProcessRegister
	}
	else if (CurWord == _T("VariablePrefixLoc"))
	{
		ProcessVariable(text, true);
		text.Delete(0, 1); // register symbol
		if (!ProcessRegister(text, Round))
			return FALSE;//Error defined in ProcessRegister
	}
	else
	{
		Error = 1205;// ERROR wrong cycle parameter
		return FALSE;
	}
	return TRUE;
}

BOOL NCUnit::ProcessTechTable(CString &text)
{
	if (text.GetLength() == 0)
	{
		Error = 1002;// level 1
		return FALSE;
	}
	if(text[0] != NEvalExp::OpenBC())
	{
		Error = 1002;// level 1
		return FALSE;
	}
	text.Delete(0);
	int num = 0;
	if(!GetIValue(text,&num))
	{
		Error = 1002;// level 1
		return FALSE;
	}
	State.TechTable = num;
	if (text.GetLength() == 0)
	{
		Error = 1002;// level 1
		return FALSE;
	}
	if(text[0] != NEvalExp::CloseBC())
	{
		Error = 1002;// level 1
		return FALSE;
	}
	text.Delete(0);
	return TRUE;
}
BOOL NCUnitBase::CycleNeedsGeom() const
{
// Returns FALSE if CycleNeedsGeom == NO
// and TRUE otherwise
	CString str;
	if(!GCD.SOthers.Lookup("CycleNeedsGeom",str))
		return TRUE;
	if(str != "No")
		return TRUE;
	return FALSE;
}

BOOL NCUnit::ChangedIJKR(void) const
{
	return State.ChangedCoord.i ||
		State.ChangedCoord.j ||
		State.ChangedCoord.k ||
		State.ChangedCoord.r;
}
BOOL NCUnit::ChangedIJK(const NCUnitStatePrev &St)
{
	if(IsArcCenterAbs())
	{
		if(State.CurI == St.CurI && State.CurJ == St.CurJ /*&& State.CurK == St.CurK*/)
			return FALSE;
	}
	else
	{// mode == "Incremental"
		if(State.CurI == 0. && State.CurJ == 0. /*&& State.CurK == 0.*/)
			return FALSE;
	}
	return TRUE;
}



BOOL NCUnit::ChangedX(const NCUnitStatePrev &St)
{
	return State.ChangedCoord.x;
}
BOOL NCUnit::ChangedY(const NCUnitStatePrev &St)
{
	return State.ChangedCoord.y;
}
BOOL NCUnit::ChangedZ(const NCUnitStatePrev &St)
{
	return State.ChangedCoord.z;
}

//NCUnit::NCUnit(const NCUnit& In) : FixedDef(0)
//{
//	Copy(In);
//}
//NCUnit & NCUnit::operator =(const NCUnit &In)
//{
//	Copy(In);
//	return *this;
//}
void NCUnit::Copy(const NCUnit &In)
{
	State = In.State;
	GCDV.XScale = In.GCD.XScale;
	GCDV.IScale = In.GCD.IScale;
	GCDV.YScale = In.GCD.YScale;
	GCDV.ZScale = In.GCD.ZScale;
	GCDV.IScale = In.GCD.IScale;
	GCDV.JScale = In.GCD.JScale;
	GCDV.KScale = In.GCD.KScale;
	GCDV.LinearScale = In.GCD.LinearScale;
	//State.CallStack = In.State.CallStack;
	//CycleStack = In.CycleStack;
	WordDef = In.WordDef;
}

bool NCUnit::GetNextGeom(NCadrGeom &NextGeom)
{// Get next cadr geom
	if(State.GetNextGeomActive) // To prevent recursive call
		return false;

	NCUnit tmp(pDoc, *this);//NCUnit constructor
	tmp.Copy(*this);
	if(tmp.State.OrderChanged)
	{
		if(tmp.State.CadrID.Prog != State.NextCadr.Prog)
		{
			return false;
		}
		tmp.State.SetCurCadr(State.NextCadr);
	}
	else
		tmp.State.SetCurCadr(State.GetCurCadr() + 1);

	bool MS = pDoc->MileStones.IsActive();
	pDoc->MileStones.DeActivate();
	bool CS = pDoc->CheckStones.IsActive();
	pDoc->CheckStones.DeActivate();
	tmp.State.GetNextGeomActive = true;
	for(int i = 0; tmp.EvaluateCurrent(true) > -1 && i < 128; ++i)
	{
		if((!tmp.State.CadrSkiped && tmp.State.Geom.HaveGeom())
			|| (tmp.State.RetractComp && tmp.State.CurDirComp == LR_OFF))
		{
			NextGeom = tmp.State.Geom;
			break;
		}
	}
	tmp.State.GetNextGeomActive = false;
	if(MS)
		pDoc->MileStones.Activate();
	if(CS)
		pDoc->CheckStones.Activate();
	if(tmp.State.RetractComp && tmp.State.CurDirComp == LR_OFF)// Added 6.02.04
	{
		// compensation will be canceled in the next cadr and RetractComp will be specified
		// RetractComp is used to disable CompToIntersect
		State.RetractComp = TRUE;
	}
	return true;
}
bool NCUnit::GetNextState(NCUnitState &NextState)
{// Get next cadr geom
	static int Busy = 0;// to prevent recursive call of this proc
	if(Busy)
		return false;
	++Busy;

	NCUnit tmp(pDoc, *this);//NCUnit constructor
	tmp.Copy(*this);
	if(tmp.State.OrderChanged)
	{
		if(tmp.State.CadrID.Prog != State.NextCadr.Prog)
		{
			--Busy;
			return false;
		}
		tmp.State.SetCurCadr(State.NextCadr);
	}
	else
		tmp.State.SetCurCadr(State.GetCurCadr() + 1);

	bool MS = pDoc->MileStones.IsActive();
	pDoc->MileStones.DeActivate();
	bool CS = pDoc->CheckStones.IsActive();
	pDoc->CheckStones.DeActivate();
	tmp.State.GetNextGeomActive = true;

	int Res = tmp.EvaluateCurrent(true);

	tmp.State.GetNextGeomActive = false;
	if(MS)
		pDoc->MileStones.Activate();
	if(CS)
		pDoc->CheckStones.Activate();

	NextState = *tmp.GetState();

	--Busy;
	return Res < -1 ? false : true;
}

void NCUnit::CompOtrOtr(const NCadrGeom &NextGeom)
{
// Найти точку пересечения отрезков, отстоящих от заданных на расстояние d

	double d = State.CurDiamComp;// offset d>0 if the tool is to left from path
	if(State.CurDirComp == RIGHT)
		d = -d;

	BPoint p;
	if (NCUnitStatePrevGeom::DIntOtrOtr(State.Geom, NextGeom, d, p))
	{
		State.MashcX = p.GetX();
		State.MashcY = p.GetY();
		State.MashcZ = State.Geom.GetB().GetZ();//p.GetZ();
	}

}
bool NCUnit::CompOtrArc(const NCadrGeom &NextGeom, BOOL OtrFirst)
{
// Найти точку пересечения отрезка и дуги, отстоящих от заданных на расстояние d

	double d = State.CurDiamComp;// offset d>0 if the tool is to left from path
	if(State.CurDirComp == RIGHT)
		d = -d;
	NCadrGeom Geom0 = OtrFirst ? State.Geom : NextGeom;
	NCadrGeom Geom1 = OtrFirst ?  NextGeom : State.Geom;
	BPoint p;
	if (NCUnitStatePrevGeom::DIntOtrArc(Geom0, Geom1, d, OtrFirst, p))
	{
		State.MashcX = p.GetX();
		State.MashcY = p.GetY();
		State.MashcZ = p.GetZ();
		return true;
	}
	return false;
}

void NCUnit::CompArcArc(const NCadrGeom &NextGeom)
{
// Найти точку пересечения дуг, отстоящих от заданных на расстояние d
	double d = State.CurDiamComp;// offset d>0 if the tool is to left from path
	if (State.CurDirComp == RIGHT)
		d = -d;
	
	BPoint p;
	if (NCUnitStatePrevGeom::DIntArcArc(State.Geom, NextGeom, d, p))
	{
		State.MashcX = p.GetX();
		State.MashcY = p.GetY();
		State.MashcZ = p.GetZ();
	}
}

BOOL NCUnit::GetParamValue(const CString &name, double* value, bool &Integer)
{
	return (GetRegTypedValue(name, value, Integer) == TRUE);
}

void NCUnit::AptExport(const CString &ProgName)
{
	CString filename;
  	CString ext="apt"; // описываем переменные которые будут использоваться при запросе имени исходного файла APTCL
	CString filter;
	filter = "*." + ext +"|*." + ext + "|"; // создание фильтра для ограничения на расширение сходящего APTCL файла
	CFileDialog fd(FALSE,ext,ProgName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter); //диалгоговое окно запроса имеени исходящего файла
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;
	NProgram *pProg;
	
	int index = GetChannel().NGetProgramListCn()->GetIndex(ProgName);
	if(index < 0 || index >= GetChannel().NGetProgramListCn()->GetSize())
		return;
	
	pAptOut = new APTCLOut(ProgName, filename);

	if(!pAptOut)
		return;

	Reset();
	pAptOut->Init(GetState());

	CString key;
	GetChannel().NGetProgramListCn()->GetAt(index,key,(CObject *&)pProg);
	pProg->ClearEvaluated();
	SetCurProg(key);
	State.SetCurCadrEx(cadrID(index, 0));
	RunProgram(pProg);
	pProg->SetGeomReady();
	pProg->SetEvaluated();

	delete pAptOut;
	pAptOut = NULL;

}

int NCUnitBase::GetToolNumByName(const CString &Name) const
{
	for each (const auto & el in *pDoc->NGetToolList())
	{
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);;
		if(pTool->GetName() == Name)
		{
			if(State.CurTurret == pTool->Turret)
			{
				pTool->SetIsUsed(true);
				return pTool->Position;
			}
		}
	}
	return -1;
}

int NCUnitBase::MakeNewToolByName(const CString &Name) const
{
	int MaxToolNum = 0;
	for each (const auto & el in *pDoc->NGetToolList())
	{
		const NToolID& ToolID = el.first;
		NToolCombined* pTool = static_cast<NToolCombined*>(el.second);;
		if(State.CurTurret == pTool->Turret)
			if(pTool->Position > MaxToolNum)
				MaxToolNum = pTool->Position;
	}
	++MaxToolNum;
	NToolID ToolID;
	ToolID.Set(MaxToolNum, State.CurTurret);
	pDoc->MakeNewTool(ToolID, Name);
	return MaxToolNum;
}

BOOL NCUnit::ProcessToolCode(CString &text)
{
	if(GCD.AllowToolString)
	{
		if(text.IsEmpty())
		{
			Error = 1151;// level 1
			return FALSE;
		}
		if(text[0] != '"')
		{
			Error = 1151;// level 1
			return FALSE;
		}
		text.Delete(0);
		int Ind = text.Find('"');
		if(Ind < 0)
		{
			Error = 1151;// level 1
			return FALSE;
		}
		CString Name = text.Left(Ind);
		text.Delete(0, Ind + 1);
		State.ThisCadrToolN = GetToolNumByName(Name);
		if(State.ThisCadrToolN < 0)
			State.ThisCadrToolN = MakeNewToolByName(Name);
		return TRUE;
	}
	CString toitn;
	if(!GCD.SOthers.Lookup("ToolOffsetInToolNumber", toitn))
		toitn = "No";

	if(!GetIValue(text, &State.ThisCadrToolN))
	{
		if(text.GetLength() <= 0)
		{
			if (StoreError(1042))
				return FALSE;
			else
				return TRUE;
		}
		bool SpecCase = false;
		if(text.Left(1) == '#')
		{
			if(text.GetLength() < 2)
				SpecCase = true;
			else
				if(!isdigit(text.GetAt(1)))
					SpecCase = true;
		}
		if(SpecCase)
		{
// Special case when instrument number is in the register N1
// Added 16.03.04
			State.ThisCadrToolN = int(State.GetRegister(1));
			text.Delete(0);
		}
		else
		{
			CString NewText = text.SpanExcluding(_T("."));
			int OrigL = NewText.GetLength();
			double val;
			if(GetDIValue(NewText, &val))
			{
				text.Delete(0, OrigL - NewText.GetLength());
				State.ThisCadrToolN = int(val + 0.5);
			}
			else
			{
				if (StoreError(1043))
					return FALSE;
				else
					return TRUE;
			}
		}
	}
// No
	if(toitn == "No")
		return TRUE;

// Yes
	if(toitn == "Yes")
	{
		int n = State.ThisCadrToolN;
		double val;
		if(!State.DComp.Lookup(n,val))
		{
			StartState.DComp[n] = 0.;
			val = 0.;
		}
		State.CurDiamComp = val;
		ProcLengthCompOffset(n);

// Add HComp for tool displacement in turn
		if(!State.DComp.Lookup(n + 1000,val))
			StartState.DComp[n + 1000] = 0.;
		if(!State.DComp.Lookup(n + 2000,val))
			StartState.DComp[n + 2000] = 0.;
		if(!State.DComp.Lookup(n + 3000,val))
			StartState.DComp[n + 3000] = 0.;
		return TRUE;
	}
//For
	if(toitn == "For")
	{
		toitn = "Com";
		CString TNF;
		if(!GCD.SOthers.Lookup("ToolNumFormat",TNF))
			TNF = "N6L0";
		CString Digits;
		Digits.Format("%d", State.ThisCadrToolN);
		int X2 = 0;
		if(TNF.GetLength() == 4)
			X2 = atoi(TNF.Right(1));
		if(Digits.GetLength() < X2 + 1)
		{// Assume 00 as compensator number
			for(int k = 0; k < X2; ++k)
				Digits += "0";
		}
		CString tmp = Digits.Left(Digits.GetLength() - X2);
		if(!GetIValue(tmp, &State.ThisCadrToolN))
		{
			if (StoreError(1042))
				return FALSE;
			else
				return TRUE;
		}
		text = "." + Digits.Right(X2) + text;
	}
// Com
	if(toitn == "Com")
	{
		if(text.GetLength() <= 0)
		{
			if (StoreError(1044))
				return FALSE;
			else
				return TRUE;
		}
		if(text[0] != '.')
		{
			if (StoreError(1045))
				return FALSE;
			else
				return TRUE;
		}
		text.Delete(0);
		double dn;
		if(!GetDIValue(text, &dn))
		{
			if (StoreError(1045))
				return FALSE;
			else
				return TRUE;
		}
		int n = int(dn + 0.5);
		double val;
		if(!State.DComp.Lookup(n, val))
		{
			StartState.DComp[n] = 0.;
			val = 0.;
		}
		State.CurDiamComp = val;
		ProcLengthCompOffset(n);

// Add HComp for tool displacement in turn
		if(!State.DComp.Lookup(n + 1000,val))
			StartState.DComp[n + 1000] = 0.;
		if(!State.DComp.Lookup(n + 2000,val))
			StartState.DComp[n + 2000] = 0.;
		if(!State.DComp.Lookup(n + 3000,val))
			StartState.DComp[n + 3000] = 0.;

		return TRUE;
	}
	return TRUE;
}

const CString & NCUnit::GetConfFileName()
{
	return GCD.ConfFileName;
}

int NCUnit::CrTappingCycle(NCycle *pCycle)
{
	CString z, beg;
	if (!GCD.SWordDef.Lookup("SpindleAxis", z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg))
		return -1;
	CString g0 = GetGCodeText(_T("Rapid"));
	CString g1 = GetGCodeText(_T("Linear"));
	CString g2 = GetGCodeText(_T("CircularCW"));
	CString g3 = GetGCodeText(_T("CircularCCW"));

	CString g90 = GetGCodeText(_T("AbsoluteUnit"));
	CString g91 = GetGCodeText(_T("IncrementalUnit"));
	if (g0.IsEmpty() || g1.IsEmpty() || g2.IsEmpty() || g3.IsEmpty() || g90.IsEmpty() || g91.IsEmpty())
		return -1;

	CString I,J;
	I = (State.CycleI == CYCLE_UNDEF) ? "" : "I"+State.CycleI;
	J = (State.CycleJ == CYCLE_UNDEF) ? "" : "J"+State.CycleJ;

	CString text;
	NCadr* pcCadr = nullptr;
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g2 + I + J;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g91; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	int c;
	for(c = 0; c < State.CycleThreadParm; ++c)
	{
		text = g1 + z + State.CycleTappingZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		text = g2 + I + J;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}
	State.CurCycle = CYCLE_OFF;
	return 0;
}

int NCUnit::CrCuttingCycle(NCycle *pCycle)
{
	CString z, beg;
	if (!GCD.SWordDef.Lookup("SpindleAxis", z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg))
		return -1;
	CString g0 = GetGCodeText(_T("Rapid"));
	CString g1 = GetGCodeText(_T("Linear"));
	CString g2 = GetGCodeText(_T("CircularCW"));
	CString g3 = GetGCodeText(_T("CircularCCW"));

	CString g90 = GetGCodeText(_T("AbsoluteUnit"));
	CString g91 = GetGCodeText(_T("IncrementalUnit"));
	if (g0.IsEmpty() || g1.IsEmpty() || g2.IsEmpty() || g3.IsEmpty() || g90.IsEmpty() || g91.IsEmpty())
		return -1;

	CString I,J;
	I = (State.CycleI == CYCLE_UNDEF) ? "" : "I"+State.CycleI;
	J = (State.CycleJ == CYCLE_UNDEF) ? "" : "J"+State.CycleJ;
	CString X,Y;
	X = (State.CycleI == CYCLE_UNDEF) ? "" : "X"+InvertSign(State.CycleI);
	Y = (State.CycleJ == CYCLE_UNDEF) ? "" : "Y"+InvertSign(State.CycleJ);

	CString text;
	NCadr* pcCadr = nullptr;
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g91;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + X + Y;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g2 + I + J;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	X = (State.CycleI == CYCLE_UNDEF) ? "" : "X"+State.CycleI;
	Y = (State.CycleJ == CYCLE_UNDEF) ? "" : "Y"+State.CycleJ;
	text = g1 + X + Y;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	State.CurCycle = CYCLE_OFF;
	return 0;
}

int NCUnit::CrGrooveCycle(NCycle *pCycle)
{
	//M57
	//	M52C = #17
	//	M10
	//	G1Z = #26F = #9M50


	NCadr *pcCadr;
	CString text, g, m, g0, g1, n, g3, b, z, beg;
	struct FDef *pFDef;
	if (!GCD.SWordDef.Lookup("FunctionCode", g) ||
		!GCD.SWordDef.Lookup("MiscFunctionCode", m) ||
		!GCD.SWordDef.Lookup("SpindleAxis", z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg) ||
		!GCD.SFuncDef.Lookup("Rapid", pFDef))
		return -1;
	text = beg; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	if (!GCD.SMiscFuncDef.Lookup("RestartRotAngles", n))
		return -1;
	text = m + n; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	if (State.Spindle[SPINDLE_TURN].SpindleDir == R_UNDEF)
		return -1;
	if (State.Spindle[SPINDLE_TURN].SpindleDir == R_CCW)
	{
		if (!GCD.SMiscFuncDef.Lookup("TRTAlwaysCCW", n))
			return -1;
	}
	else
	{
		if (!GCD.SMiscFuncDef.Lookup("TRTAlwaysCW", n))
			return -1;
	}
	text = m + n; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	CString C;
	if (!GCD.SWordDef.Lookup("SpindleRotationAxis", C))
		return -1;
	CString Val;
	if (!GetTParamVal(State.CycleZ, _T("StartAngleGrooveCycle"), Val))
	{
		Val = _T("0");
	}
	text = C + Val; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	if (!GCD.SMiscFuncDef.Lookup("GrooveOn", n))
		return -1;
	text = m + n; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	if (!GCD.SMiscFuncDef.Lookup("TRTDoNotChange", n))
		return -1;
	if (!GCD.SFuncDef.Lookup("Linear", pFDef))
		return -1;
	g1 = g + pFDef->Num;
	text = m + n + g1; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	text = State.CycleZ; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	State.CycleZ.Empty();
	State.CurCycle = CYCLE_OFF;
	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}


CString NCUnit::InvertSign(const CString &str)
{
	CString ret = str;
	ret.TrimLeft();
	if(ret[0] == '-')
		ret.SetAt(0,'+');
	else
		if(ret[0] == '+')
			ret.SetAt(0,'-');
		else
			ret = "-" + ret;
	return ret;
}

int NCUnit::CreatePArc(const NCUnitStatePrev &St, NCadrGeom *pGeom)
{
	BPoint Pc(St.CurI,St.CurJ,St.MashZ,1.);
	double R;
	if(St.CurR == 0.)
		R = sqrt((St.CurI - State.MashX) * (St.CurI - State.MashX)
		+ (St.CurJ - State.MashY) * (St.CurJ - State.MashY));
	else
		R = St.CurR;
	BPoint Px(Pc.GetX() + fabs(R),Pc.GetY(),Pc.GetZ(),1.);
	BMatr T;
	T.RotZ(Pc,State.MashC);
	State.MashC = 0.;
	BPoint Pe = Px * T;

	State.CurX = Pe.GetX() - State.MashX;
	State.CurY = Pe.GetY() - State.MashY;
	State.CurZ = Pe.GetZ() - State.MashZ;

	State.MashX = Pe.GetX();
	State.MashY = Pe.GetY();
	State.MashZ = Pe.GetZ();

	State.Geom.Set(State.CurCurveType,
					St.MashX,St.MashY,St.MashZ,
					Pe.GetX(),Pe.GetY(),Pe.GetZ(),
					St.CurI-St.MashX,
					St.CurJ-St.MashY,
					0.,
					State.CurPlane);

	return 0;
}
int NCUnit::CrNumerCycle(NCycle *pCycle) 
{
	NCadr *pcCadr;
	CString text,g,g0,g1,b,z,beg;
	struct FDef *pFDef;
	if( !GCD.SWordDef.Lookup("FunctionCode",g) ||
		!GCD.SWordDef.Lookup("SpindleAxis",z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd",beg) ||
		!GCD.SFuncDef.Lookup("Rapid",pFDef))   
		return -1;
	g0 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Linear",pFDef))   
		return -1;
	g1 = g + pFDef->Num;
	
	if(State.CycleZ == CYCLE_UNDEF)
		return -1;
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + CString(NEvalExp::OpenBC()) + State.CycleZ + "-#8-#9" + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);



	text = g0;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	return 0;
}
int NCUnit::CrHMCCycle(NCycle *pCycle)
{
	NCadr *pcCadr;
	CString text,g,g0,g1,b,z,beg,reg;
	struct FDef *pFDef;
	if( !GCD.SWordDef.Lookup("FunctionCode",g) ||
		!GCD.SWordDef.Lookup("SpindleAxis",z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd",beg) ||
		GCD.RegisterSymbol == '0' ||
		!GCD.SFuncDef.Lookup("Rapid",pFDef))   
		return -1;
	g0 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Linear",pFDef))   
		return -1;
	g1 = g + pFDef->Num;
	
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	if(State.GetRegister(343) != 1)
		{text = g1 + z + CString(NEvalExp::OpenBC()) + reg + "203" + "+" + reg + "201" + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
	else
		{text = g1 + z + CString(NEvalExp::OpenBC()) + reg + "203" + "-" + reg + "344*0.5*т" + CString(NEvalExp::OpenBC()) + reg + "8105*57.2957796" + CString(NEvalExp::CloseBC()) + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
	if(State.GetRegister(204) != 0)
		{text = g1 + z + CString(NEvalExp::OpenBC()) + reg + "203" + "+" + reg + "204" + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
	else
		{text = g1 + z + CString(NEvalExp::OpenBC()) + reg + "203" + "+" + reg + "200" + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}

	text = g0;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	return 0;
}
int NCUnit::CrHMCycle(NCycle *pCycle)
{
	NCadr *pcCadr;
	CString text,g,g0,g1,b,z,beg,reg(GCD.RegisterSymbol);
	struct FDef *pFDef;
	if( !GCD.SWordDef.Lookup("FunctionCode",g) ||
		!GCD.SWordDef.Lookup("SpindleAxis",z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd",beg) ||
		GCD.RegisterSymbol == '0' ||
		!GCD.SFuncDef.Lookup("Rapid",pFDef))   
		return -1;
	g0 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Linear",pFDef))   
		return -1;
	g1 = g + pFDef->Num;
	
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + CString(NEvalExp::OpenBC()) + reg + "203" + "+" + reg + "201" + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + CString(NEvalExp::OpenBC()) + reg + "203" + "+" + reg + "204" + "+" + reg + "200" + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);

	text = g0;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	return 0;
}
int NCUnit::CrMP81Cycle(NCycle *pCycle)
{
	NCadr *pcCadr;
	CString text, g, g0, g1, g4, b, z, beg, reg(GCD.RegisterSymbol), dwell;
	struct FDef *pFDef;
	if( !GCD.SWordDef.Lookup("FunctionCode", g) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg) ||
		GCD.RegisterSymbol == '0' ||
		!GCD.SWordDef.Lookup("DwellTime", dwell) ||
		!GCD.SFuncDef.Lookup("Rapid", pFDef))   
		return -1;
	g0 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Linear", pFDef))   
		return -1;
	g1 = g + pFDef->Num;
	
	if( !GCD.SFuncDef.Lookup("Dwell", pFDef))   
		return -1;
	g4 = g + pFDef->Num;

	switch(State.CurPlane)
	{
	default:
	case XY:
		if(!GCD.SWordDef.Lookup("SpindleAxis", z))
			return -1;
		break;
	case XZ:
		if(!GCD.SWordDef.Lookup("VerticalAxis", z))
			return -1;
		break;
	case YZ:
		if(!GCD.SWordDef.Lookup("HorizontalAxis", z))
			return -1;
		break;
	}


	text = beg; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g0 + z + CString(NEvalExp::OpenBC()) + State.CycleZ + "+" + reg + "201" + CString(NEvalExp::CloseBC()); pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + CString(NEvalExp::OpenBC()) + State.CycleZ + "+" + reg + "203" + CString(NEvalExp::CloseBC()); pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g4 + dwell + CString(NEvalExp::OpenBC()) + reg + "205" + CString(NEvalExp::CloseBC()); pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	text = g1 + z + CString(NEvalExp::OpenBC()) + State.CycleZ + "+" + reg + "201" + CString(NEvalExp::CloseBC()); pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	if(State.GetRegister(204) > State.GetRegister(201) || State.GetRegister(204) != 0)
	{text = g0 + z + CString(NEvalExp::OpenBC()) + State.CycleZ + "+" + reg + "204" + CString(NEvalExp::CloseBC()); pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
	return 0;
}
int NCUnit::CrHNCycle(NCycle *pCycle)
{
	CrHMCycle(pCycle);
	State.NonModalCycleCall = true;

	return 0;
}
int NCUnit::CrSplineACycle(NCycle *pCycle)
{
	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);
	CycleGeom.GetAt(0)->SetAttr(1);
	CycleGeom.GetAt(0)->SetSpline(true);
	//State.MashcX = St.MashX;
	//State.MashcY = St.MashY;
	//State.MashcZ = St.MashZ;
	//State.CurX = 0.;
	//State.CurX = 0.;
	//State.CurZ = 0.;
	State.Geom.SetType(undef);
	State.CompGeom = State.Geom;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	return 0;
}

void NCUnit::CompToInters(const NCadrGeom & NextGeom, const NCUnitStatePrev & St)
{
	//enum curve	cType = State.Geom.GetType(),
	//			nType = NextGeom.GetType();
	if(State.Geom.HaveGeom() && NextGeom.HaveGeom())
	{
		BOOL t0,t1;// TRUE if otr, FALSE if arc
		t0 = State.Geom.IsLine();
		t1 = NextGeom.IsLine();

		if(	t0 && t1)
		{ // otr otr
			CompOtrOtr(NextGeom);
		}
		else
		{// at least one arc
			if(!t0 && !t1)
			{// arc arc
				CompArcArc(NextGeom);
			}
			else
			{// one arc and one line
				State.IntNotFound = !CompOtrArc(NextGeom, t0);
			}
		}
		if(t0)
			State.CompGeom.SetE(State.MashcX,State.MashcY,State.MashcZ); //COMP
		else
		{
			double sx = St.MashcX, sy = St.MashcY, sz = St.MashcZ;
			if(State.AddCompGeom.HaveGeom())
			{
				State.AddCompGeom.GetE(&sx, &sy, &sz);
			}
			BPoint I = State.Geom.GetC() - BPoint(sx, sy, sz, 1.);
			State.CompGeom.Set(State.CurCurveType,
				sx, sy, sz,
				State.MashcX,State.MashcY,State.MashcZ,
				I.GetX(),I.GetY(),0.,XY);
		}
	}
}

void NCUnit::AddExtraArc(const NCUnitStatePrevGeom & St)
{
	double x,y,z;
	State.CompGeom.GetB(&x,&y,&z);
	if(Dist2(St.MashcX,St.MashcY,0.,
				x, y, 0.) < MIN_ARC*MIN_ARC)
		return;
	State.AddCompGeom.Set(State.CurDirComp == RIGHT ? ccwarc : cwarc,
			St.MashcX,St.MashcY,St.MashcZ,
			x,y,z,
			St.MashX-St.MashcX,
			St.MashY-St.MashcY,
			0,XY);
}



BOOL NCUnit::ProcessSubrCall(CString & text, bool CallByLineNumber)
{
		cadrID p;
		int count = 1;
		p = State.CadrID;
		++p.Cadr;
		State.GetVarSet().PushRegsMap();
		if(State.CallStack.Push(p,count) != 0)
		{
			Error = 1067;// level 1
			return FALSE;
		}
		State.OrderChanged = true;
		cadrID tmp;
		if(!FindNextCadr(text, &tmp, CallByLineNumber))
			return FALSE; // Error defined in FindNextCadr
		int nbuf = 0;
		if(State.AutoCycleCount > 0)
			nbuf = State.AutoCycleCount;
		else
		{
			double b = 0.;
			if (GetDIValue(text, &b))// May contain repetitions number
				nbuf = int(b + 0.5);
		}

		for(int kr=0;kr<nbuf-1;kr++)
		{// Push first subroutine cadr nbuf-1 times
			State.GetVarSet().PushRegsMap();
			if(State.CallStack.Push(tmp,count) !=0)
				return FALSE;
		}
		State.NextCadr = tmp;
		return TRUE;
}

BOOL NCUnit::ProcessFileCall(CString & text)
{
		cadrID p;
		int count = 1;
		p = State.CadrID;
		++p.Cadr;
		State.GetVarSet().PushRegsMap();
		if(State.CallStack.Push(p,count) != 0)
		{
			Error = 1067;// level 1
			return FALSE;
		}
		State.OrderChanged = true;
		cadrID tmp;
		if(!FindCallFile(text, &tmp))
			return FALSE; // Error defined in FindCallFile
		int nbuf = 0;
		if(State.AutoCycleCount > 0)
			nbuf = State.AutoCycleCount;
		else
		{
			double b = 0.;
			if (GetDIValue(text, &b))// May contain repetitions number
				nbuf = int(b + 0.5);
		}

		for(int kr=0;kr<nbuf-1;kr++)
		{// Push first subroutine cadr nbuf-1 times
			State.GetVarSet().PushRegsMap();
			if(State.CallStack.Push(tmp,count) !=0)
				return FALSE;
		}
		State.NextCadr = tmp;
		return TRUE;
}
BOOL NCUnit::ProcessVariablesDecl(CString & text, bool LocalVar)
{
	CString Exp;
	if (!GetOtherValue(LocalVar ? _T("VarRegExpLoc") : _T("VarRegExp"), Exp))
	{
		Error = 1160;// level 1
		return FALSE;
	}
	const int LimPos = Exp.Find(_T(";;"), 0);
	if (LimPos < 0)
	{
		Error = 1164;// level 1
		return FALSE;
	}
	CString RegExp = Exp.Left(LimPos);
	Exp.Delete(0, LimPos + 2);
	if(Exp.IsEmpty() || RegExp.IsEmpty())
	{
		Error = 1188;// level 1
		return FALSE;
	}
	for (; !text.IsEmpty();)
	{
		RemoveLeadBlanks(text);
		// Get Lexem
		bool SkipComma = false;
		int k = 0;
		for (; k < text.GetLength(); ++k)
		{
			if (text[k] == ',' && !SkipComma)
				break;
			else if (text[k] == '[' || text[k] == '(')
					SkipComma = true;
			else if(text[k] == ']' || text[k] == ')')
				SkipComma = false;
		}
		CString Lexem = text.Left(k);
		State.VariablesDecl = (k == text.GetLength()) ? false : true;
		text.Delete(0, k + 1);
		// Process Lexem
		if (Lexem.IsEmpty())
			continue;
		CString Dimens;
		int OpenPosS = Lexem.Find('[');
		if (OpenPosS >= 0)
		{ // Array 
			int ClosePosS = Lexem.Find(']');
			if (ClosePosS < 0)
			{
				Error = 1161;// level 1
				return FALSE;
			}
			Dimens = Lexem.Mid(OpenPosS + 1, ClosePosS - OpenPosS - 1);
			Lexem.Delete(OpenPosS, ClosePosS - OpenPosS + 1);
		}
		int EqPos = Lexem.Find('=');
		if (EqPos >= 0)
			Lexem.Delete(EqPos, 1);
		CString InitList;
		if (EqPos >= 0)
		{
			int OpenPosR = Lexem.Find('(');
			if (OpenPosR >= 0)
			{ // Array 
				int ClosePosR = Lexem.Find(')');
				if (ClosePosR < 0)
				{
					Error = 1161;// level 1
					return FALSE;
				}
				InitList = Lexem.Mid(OpenPosR + 1, ClosePosR - OpenPosR - 1);
				Lexem.Delete(OpenPosR, ClosePosR - OpenPosR + 1);
			}
			else
			{
				InitList = Lexem.Mid(EqPos);
				Lexem.Delete(EqPos, InitList.GetLength());
			}
		}
		// Lexem is variable identifier now
		if (Lexem.IsEmpty())
		{
			Error = 1165;// level 1
			return FALSE;
		}
		NVarSet& VarSet = GetState()->GetVarSet();
		if (Dimens.IsEmpty())
		{// Simple variable
			if (!InitList.IsEmpty())
			{
				const int Index = VarSet.StoreName(Lexem.GetBuffer(), LocalVar);
				State.SetRegister(Index, atof(InitList));
			}
		}
		else
		{// Array
			int CommaPos = Dimens.Find(',');
			if (CommaPos < 0)
			{// Single dimension
				const int Index = VarSet.StoreArrName(Lexem.GetBuffer(), LocalVar, atoi(Dimens));
				if (!InitList.IsEmpty())
				{
					for (int k = Index; !InitList.IsEmpty(); ++k)
					{
						CString SVal = InitList.SpanExcluding(",");
						InitList.Delete(0, SVal.GetLength() + 1);
						State.SetRegister(k, atof(SVal));
					}
				}
			}
			else
			{// 2 dimensions

			}
		}
		if (regex_match(std::string(Lexem), VarTestExp))
		{ // invalid variable name
			if (StoreError(1162))
				return FALSE;
		}
		else
		{
			CString Tmp;
			Tmp.Format(RegExp, Lexem);
			State.AddReplacePair(Tmp, Exp, LocalVar);
		}
	}
	return TRUE;
}

BOOL NCUnit::ProcessDefinesDecl(CString & text)
{
	RemoveComment(text);
	int Pos = text.Find(',');
	if (Pos <= 0)
	{
		Error = 1163;// level 1
		return false;
	}
	CString Src = text.Left(Pos);
	CString Dest = text.Mid(Pos + 1, text.GetLength() - Pos - 1);
	State.GetVarSet().AddDefinePair(Src, Dest);
	text.Empty();
	return TRUE;
}

BOOL NCUnit::ProcessReadMTSpecPos(CString & text)
{
	// Prep maching tool
	pDoc->GetMachTool().ProcCadrStep(*this, State);
	BBox StockGabar = pDoc->GetModifStocksGabar();
	const NToolCombined *pTool = pDoc->GetActiveTool();
	if (pTool)
		pDoc->GetMachTool().SetComp(*this, pTool->GetGabar().Zmax, pTool->GetCompVect(), pTool->Turret, pTool->Attach, StockGabar);
	else
		pDoc->GetMachTool().SetComp(*this, 0., BPoint(0., 0., 0., 0.), 0, 0, StockGabar);
	// END: Prep maching tool

	int ival = 0;
	GetIParamVal(text, "MTSpecPosInd", &ival);
	SCoordsDef CDef(0);
	SMachState MachState(nullptr);
	pDoc->GetMachTool().GetSpecState(ival, MachState, CDef, GetChannelInd());
	return SetMachState2Regs(MachState);
}

BOOL NCUnit::ProcessReadMTSpecPos4Tool(CString & text)
{
	// Prep maching tool
	pDoc->GetMachTool().ProcCadrStep(*this, State);
	BBox StockGabar = pDoc->GetModifStocksGabar();
	const NToolCombined *pTool = pDoc->GetActiveTool();
	if (pTool)
		pDoc->GetMachTool().SetComp(*this, pTool->GetGabar().Zmax, pTool->GetCompVect(), pTool->Turret, pTool->Attach, StockGabar);
	else
		pDoc->GetMachTool().SetComp(*this, 0., BPoint(0., 0., 0., 0.), 0, 0, StockGabar);
	// END: Prep maching tool

	int ival = 0;
	if(!GetIParamVal(text, "MTSpecPosInd", &ival))
		if (StoreError(1201))
			return FALSE;
	SCoordsDef CDef(0);
	SMachState MachState(nullptr);
	pDoc->GetMachTool().GetSpecState4Tool(ival, MachState, CDef, GetChannelInd());
	return SetMachState2Regs(MachState);
}

BOOL NCUnit::StoreStockStdParams(CString & text)
{
	double b;
	int StockInd = -1;
	if (!GetDIParamValSoft(text, _T("SSPNum"), &b))
		StockInd = -1;
	else
		StockInd = int(b);
	BBox Box;
	if (StockInd == -1)
	{
		Box = pDoc->NGetStockList()->GetGabar();
	}
	else
	{
		NStock *pStock = pDoc->GetStock(StockInd);
		if (pStock == nullptr)
		{
			Error = 1175;// level 1
			return FALSE;
		}
		Box = pStock->GetGabar();
	}
	if(!Box.IsDefined())
	{
		Error = 1047;// level 1
		return FALSE;
	}
	SetRegister(State, _T("SSPMinX"), Box.Xmin);
	SetRegister(State, _T("SSPMinY"), Box.Ymin);
	SetRegister(State, _T("SSPMinZ"), Box.Zmin);
	SetRegister(State, _T("SSPMaxX"), Box.Xmax);
	SetRegister(State, _T("SSPMaxY"), Box.Ymax);
	SetRegister(State, _T("SSPMaxZ"), Box.Zmax);
	return TRUE;
}

BOOL NCUnit::StoreAddCoord(CString& text)
{
	CString Name;
	if (!GetTextParamVal(text, _T("ACName"), Name))
		return FALSE;
	CString CurWord = GetWord(Name);
	if (IsAddCoordWord(CurWord))
	{
		if (CurWord.GetLength() < 2)
		{
			Error = 1139;// level 1
			return FALSE;
		}
		CString NumStr = CurWord.Left(2);
		int Ind = atoi(NumStr);
		if (Ind < 0 || Ind >= AddCoordsScale.GetSize())
		{
			Error = 1139;// level 1
			return FALSE;
		}
		return SetRegister(State, _T("ACValue"), State.AddCoords.GetCoord(Ind), 100) ? TRUE : FALSE;
	}
	Error = 1139;// level 1
	return FALSE;
}

BOOL NCUnit::SetMachState2Regs(const SMachState & MachState)
{
	if (State.PolarInterp.IsActive() || State.CylInterp.IsActive())
	{
		if (StoreError(1172)) // Полярная или цилиндрическая интерполяция
			return FALSE;
		else
			return TRUE;
	}
	if (State.PolarCoordActive)
	{
		if (StoreError(1173)) // Полярные координаты
			return FALSE;
		else
			return TRUE;
	}
	if (State.CurDirComp != LR_OFF || State.CurZComp != UD_OFF)// new done
	{
		if (StoreError(1174)) // Контурная или коррекция на длину
			return FALSE;
		else
			return TRUE;
	}
	BPoint XYZ;
	XYZ.Set(MachState.GetCoord(FixedDef.GetName(0)), MachState.GetCoord(FixedDef.GetName(1)), MachState.GetCoord(FixedDef.GetName(2)), 1.);
	XYZ = XYZ * State.CSMatr.invr();
	SetRegister(State, _T("MTSpecPosX"), XYZ.GetX());
	SetRegister(State, _T("MTSpecPosY"), XYZ.GetY());
	SetRegister(State, _T("MTSpecPosZ"), XYZ.GetZ());
	SetRegister(State, _T("MTSpecPosA"), MachState.GetCoord(FixedDef.GetName(3)));
	SetRegister(State, _T("MTSpecPosB"), MachState.GetCoord(FixedDef.GetName(4)));
	SetRegister(State, _T("MTSpecPosC"), MachState.GetCoord(FixedDef.GetName(5)));

	CString AddCoord(_T("AddCoord"));
	CString InitStr;
	for (int i = 0; i < 100; ++i)
	{
		CString Num;
		Num.Format("%02u", i);
		CString MachAxisName;
		int Ind = atoi(Num);
		if (Ind >= 0 && Ind < AddCoordsScale.GetSize() - 1)
			MachAxisName = AddCoordsScale.GetAName(Ind);
		else
			MachAxisName = AddCoord + Num;
		const double CoordVal = MachState.GetCoord(MachAxisName);
		SetRegister(State, _T("MTSpecPosAC") + Num, CoordVal);
	}
	return TRUE;
}

BOOL NCUnit::ProcessRegSubst(CString & text, int Base)
{// The values of symbols from "addrs" list must be placed in the registers
	// with numbers from "regs" list.
	// Additionally, if "DiaCompOffset" or "LengthCompOffset" symbols present than correspondent correctors must be activated
	CString addrs = "ABCIJKDEFHMQRSTUVWXYZ";
	int regs[] = {1,2,3,4,5,6,7,8,9,11,13,17,18,19,20,21,22,23,24,25,26};
	CString tmp("");
	CString Ds;// For D and H processing
	GCD.SWordDef.Lookup("DiaCompOffset", Ds);
	CString Hs;// For D and H processing
	GCD.SWordDef.Lookup("LengthCompOffset", Hs);
	while(!text.IsEmpty())
	{
		int Ind = text.Find('"');
		if(Ind >= 0)
		{
			Ind = text.Find('"', Ind + 1);
			if(Ind < 0)
			{
				Error = 1194;// level 1
				return FALSE;
			}
			tmp += text.Left(Ind + 1);
			text.Delete(0, Ind + 1);
		}
		CString str = text.SpanExcluding(addrs);
		tmp += str;
		text.Delete(0,str.GetLength());
		if(text.IsEmpty())
			continue;
		CString FSim = text.Left(1);
		int ind = addrs.Find(FSim);
		text.Delete(0);
		double val;
		if(!GetDValue(text,&val))
		{
			if (StoreError(1080))
				return FALSE;
			else
				continue;
		}
		State.SetRegister(regs[ind] + Base, val);
		State.SetRegister(8300 + regs[ind], 1.);
		// Process D and H
		if(FSim == Ds)
		{
			double b;
			int n = int(val + 0.5);
			if(!State.DComp.Lookup(n,b))
				StartState.DComp[n] = 0.;
		}	
		if(FSim == Hs)
		{
			double b;
			int n = int(val + 0.5);
			if(!State.HComp.Lookup(n,b))
				StartState.HComp[n] = 0.;
		}	
	}
	text = tmp;
	return TRUE;
}

BOOL NCUnit::ProcessReposStock(CString& text)
{
	double Src;
	auto SrcPresent = GetDIParamValSoft(text, _T("SourceStPos"), &Src);
	double Dst;
	auto DstPresent = GetDIParamValSoft(text, _T("DestStPos"), &Dst);
	if (!SrcPresent || !DstPresent)
	{
		Error = 1208;// Level 1
	}
	int SrcInt = int(Src + 0.5);
	int DstInt = int(Dst + 0.5);
	const MTMachineTool& MT = pDoc->GetMachTool();
	const std::set<int>& PosSet = MT.GetStPosSet();
	if (PosSet.find(SrcInt) == PosSet.end() || PosSet.find(DstInt) == PosSet.end())
	{
		Error = 1209;// Level 1
		return FALSE;
	}

	MStGr2StPos& Gr2Pos = State.MachLink.GetStGr2StPos();
	auto FoundStGr = Gr2Pos.Find(SrcInt);
	if (FoundStGr.size() == 0)
	{
		Error = 1210;// Level 1
		return FALSE;
	}
	for each (auto Ind in FoundStGr)
		Gr2Pos.Set(Ind, DstInt);
	return TRUE;
}

BOOL NCUnit::ProcessReposStockByGr(CString& text)
{
	double Src;
	auto SrcPresent = GetDIParamValSoft(text, _T("SourceStGroup"), &Src);
	double Dst;
	auto DstPresent = GetDIParamValSoft(text, _T("DestStPos"), &Dst);
	if (!SrcPresent || !DstPresent)
	{
		Error = 1208;// Level 1
		return FALSE;
	}
	int SrcInt = int(Src + 0.5);
	int DstInt = int(Dst + 0.5);
	const MTMachineTool& MT = pDoc->GetMachTool();
	const std::set<int>& PosSet = MT.GetStPosSet();
	if (PosSet.find(DstInt) == PosSet.end())
	{
		Error = 1209;// Level 1
		return FALSE;
	}
	MStGr2StPos& Gr2Pos = State.MachLink.GetStGr2StPos();
	BStockGrID ID;
	ID.Set(SrcInt);
	Gr2Pos.Set(ID, DstInt);
	return TRUE;
}

BOOL NCUnit::DefineRect(CString & text)
{
	CString addrs = "ABCIJKRXYZ";
	int regs[] = {1,2,3,4,5,6,18,24,25,26};
	CString tmp("");
	while(text.GetLength() > 0)
	{
		CString str = text.SpanExcluding(addrs);
		tmp += str;
		text.Delete(0,str.GetLength());
		if(text.IsEmpty())
			continue;
		int ind = addrs.Find(text.Left(1));
		text.Delete(0);
		double val;
		if(!GetDValue(text,&val))
		{
			if (StoreError(1080))
				return FALSE;
			else
				continue;
		}
		State.SetRegister(regs[ind], val);
	}
	text = tmp;
	return TRUE;
}
BOOL NCUnit::DefineRectQ(CString & text)
{
	CString addrs = "ABCIQKRXYZ";
	int regs[] = {1,2,3,4,5,6,18,24,25,26};
	CString tmp("");
	while(text.GetLength() > 0)
	{
		CString str = text.SpanExcluding(addrs);
		tmp += str;
		text.Delete(0,str.GetLength());
		if(text.IsEmpty())
			continue;
		int ind = addrs.Find(text.Left(1));
		text.Delete(0);
		double val;
		if(!GetDValue(text,&val))
		{
			if (StoreError(1080))
				return FALSE;
			else
				continue;
		}
		State.SetRegister(regs[ind], val);
	}
	text = tmp;
	return TRUE;
}

void NCUnit::SubstCycleParam(void)
{
	State.CycleZ.Format("%lf",State.GetRegister(202));
	State.R1.Format("%lf",State.GetRegister(200));
	if(State.CurCycleRet != TOSTART)
		State.R2 = State.R1;
}

BOOL NCUnit::GetOneScaleValue(CString & text, const CString & axis, double Korr, double & Scale)
{
	int ind = text.Find(axis);
	if(ind < 0)
		return TRUE;
	Scale = 1.;
	CString buf = text.Mid(ind + 1);
	int l = buf.GetLength();
	double val = 1.;
	if(!GetDValue(buf, &val))
		return FALSE;
	text.Delete(ind, 1 + l - buf.GetLength());
	Scale = val * Korr;
	return TRUE;
}

BOOL NCUnit::ProcessScalingOn(CString & text, double Korr)
{
	CString axis;
	if(GCD.SWordDef.Lookup("HorizontalAxis", axis))
	{
		if(!GetOneScaleValue(text, axis, Korr, State.CurXScale))
		{
			if (StoreError(1083))
				return FALSE;
		}
	}
	if(GCD.SWordDef.Lookup("VerticalAxis", axis))
	{
		if(!GetOneScaleValue(text, axis, Korr, State.CurYScale))
		{
			if (StoreError(1084))
				return FALSE;
		}
	}
	if(GCD.SWordDef.Lookup("SpindleAxis", axis))
	{
		if(!GetOneScaleValue(text, axis, Korr, State.CurZScale))
		{
			if (StoreError(1085))
				return FALSE;
		}
	}
	if(GCD.SWordDef.Lookup("CircleHorzCenter", axis))
	{
		if(!GetOneScaleValue(text, axis, Korr, State.CurIScale))
		{
			if (StoreError(1083))
				return FALSE;
		}
	}
	if(GCD.SWordDef.Lookup("CircleVerticalCenter", axis))
	{
		if(!GetOneScaleValue(text, axis, Korr, State.CurJScale))
		{
			if (StoreError(1084))
				return FALSE;
		}
	}
	if(GCD.SWordDef.Lookup("CircleDepthCenter", axis))
	{
		if(!GetOneScaleValue(text, axis, Korr, State.CurKScale))
		{
			if (StoreError(1085))
				return FALSE;
		}
	}
	return TRUE;
}

int NCUnit::CrRectCycle(NCycle *pCycle)
{
//	CString addrs = "ABCIJKDEFHMQRSTUVWXYZ";
//	int regs[] = {1,2,3,4,5,6,7,8,9,11,13,17,18,19,20,21,22,23,24,25,26};

	CString z, beg;
	if (!GCD.SWordDef.Lookup("SpindleAxis", z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg))
		return -1;
	CString g0 = GetGCodeText(_T("Rapid"));
	CString g1 = GetGCodeText(_T("Linear"));
	CString g2 = GetGCodeText(_T("CircularCW"));
	CString g3 = GetGCodeText(_T("CircularCCW"));

	CString g90 = GetGCodeText(_T("AbsoluteUnit"));
	CString g91 = GetGCodeText(_T("IncrementalUnit"));
	if (g0.IsEmpty() || g1.IsEmpty() || g2.IsEmpty() || g3.IsEmpty() || g90.IsEmpty() || g91.IsEmpty())
		return -1;

	CString text;
	NCadr* pcCadr = nullptr;
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g90 + g0 + "Z#2+"+State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	int NZstep = 1;
	if(State.GetRegister(6) != 0)
	{
		NZstep = int(State.GetRegister(26)/State.GetRegister(6));
		if(NZstep * State.GetRegister(6) > State.GetRegister(26))
			NZstep++;
	}
	double ToolDiam = 2.;
	State.DComp.Lookup(State.CurToolN.GetToolNum(), ToolDiam);
	ToolDiam *= 2.;
	double SideStep = ToolDiam*0.75;
	if(State.GetRegister(4) != 0)
		SideStep = ToolDiam * State.GetRegister(4)/100.;
	int Nstep = int((max(State.GetRegister(24),State.GetRegister(25))-ToolDiam)/(2*SideStep))+1;
	double XStep = (State.GetRegister(24)-ToolDiam)/(2*Nstep);
	double YStep = (State.GetRegister(25)-ToolDiam)/(2*Nstep);
	CString SStep;
	SStep.Format("X%fY%f",-XStep,YStep);
	for(int i=0 ; i < NZstep; ++i)
	{
		text = g91 + g1 +"Z-#2";pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		if(i==NZstep-1)
			{text = g90 + "Z#26+"+State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
		else
			{text = "Z#6";pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
		for(int ik=0 ; ik < Nstep ; ++ik)
		{
			text = g91 + SStep;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
			text.Format("X%f",(ik+1)*XStep*2);pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
			text.Format("Y%f",-(ik+1)*YStep*2);pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
			text.Format("X%f",-(ik+1)*XStep*2);pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
			text.Format("Y%f",(ik+1)*YStep*2);pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		}
		text = "Z#2";pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		text.Format(g0+"X%fY%f",Nstep*XStep,-Nstep*YStep);pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}

	text = g90 + g0 + "Z#2+"+State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);


//	State.CurCycle = CYCLE_OFF;
	return 0;
}


int NCUnit::CrCircleCycle(NCycle *pCycle)
{
	CString z, beg;
	if( !GCD.SWordDef.Lookup("SpindleAxis", z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg))   
		return -1;
	CString g0 = GetGCodeText(_T("Rapid"));
	CString g1 = GetGCodeText(_T("Linear"));
	CString g2 = GetGCodeText(_T("CircularCW"));
	CString g3 = GetGCodeText(_T("CircularCCW"));
	
	CString g90 = GetGCodeText(_T("AbsoluteUnit"));
	CString g91 = GetGCodeText(_T("IncrementalUnit"));
	if (g0.IsEmpty() || g1.IsEmpty() || g2.IsEmpty() || g3.IsEmpty() || g90.IsEmpty() || g91.IsEmpty())
		return -1;

	CString I,J,K;
	I = (State.CycleI == CYCLE_UNDEF) ? "0" : State.CycleI;
	J = (State.CycleJ == CYCLE_UNDEF) ? "0" : State.CycleJ;
	K = (State.CycleTappingZ == CYCLE_UNDEF) ? "0" : State.CycleTappingZ;
	CString X,Y;
	X = (State.CycleI == CYCLE_UNDEF) ? "" : "X" + InvertSign(State.CycleI);
	Y = (State.CycleJ == CYCLE_UNDEF) ? "" : "Y" + InvertSign(State.CycleJ);
	const double dI = atof(I);
	if(dI < State.CurDiamComp)
		return -1;
	const double dJ = atof(J);
	const double dK = atof(K);
	CString BRD;
	BRD.Format("%f", State.CurDiamComp);
	CString text;
	NCadr* pcCadr = nullptr;
	text = beg; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	text = g91; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	CString str = CString(NEvalExp::OpenBC()) + I + "-" + BRD + "+" + J + CString(NEvalExp::CloseBC());
	CString mstr = CString(NEvalExp::OpenBC()) + BRD + "-" + I + "-" + J + CString(NEvalExp::CloseBC());
	if(K == "0")
	{
		if(J != "0")
			return -1;
		text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		text = g2 + "X0.Y0.J" + mstr; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		text = g1 + "Y" + mstr; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}
	else
	{
		if(J == "0")
			return -1;
		const int StepsNum = int((dK - dI)/dJ);
		if(StepsNum == 0)
		{
			str.Format("%f",dK - State.CurDiamComp);
			text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		}
		else
		{
			for(int i = 0 ; i < StepsNum; ++i)
			{
				if(i == 0)
				{
					text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
				}
				else
				{
					text = g1 + "Y" + J; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
				}
				text = g2 + "X0.Y0.J" + mstr; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
				mstr = mstr + "-" + J;
			}
			str.Format("%f",dK - dI - dJ * StepsNum);
			text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		}
		mstr = CString(NEvalExp::OpenBC()) + BRD + "-" + K + CString(NEvalExp::CloseBC());
		text = g2 + "X0.Y0.J" + mstr; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		text = g1 + "Y" + mstr; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}
	if(State.AbsoluteCoord)
	{
		text = g90;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}

	State.NonModalCycleCall = true;

	return 0;
}
int NCUnit::CrRectangCycle(NCycle *pCycle)
{
	CString z, beg;
	if (!GCD.SWordDef.Lookup("SpindleAxis", z) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd", beg))
		return -1;
	CString g0 = GetGCodeText(_T("Rapid"));
	CString g1 = GetGCodeText(_T("Linear"));
	CString g2 = GetGCodeText(_T("CircularCW"));
	CString g3 = GetGCodeText(_T("CircularCCW"));

	CString g90 = GetGCodeText(_T("AbsoluteUnit"));
	CString g91 = GetGCodeText(_T("IncrementalUnit"));
	if (g0.IsEmpty() || g1.IsEmpty() || g2.IsEmpty() || g3.IsEmpty() || g90.IsEmpty() || g91.IsEmpty())
		return -1;

	CString I,J,K;
	I = (State.CycleI == CYCLE_UNDEF) ? "0" : State.CycleI;
	J = (State.CycleJ == CYCLE_UNDEF) ? "0" : State.CycleJ;
	K = (State.CycleTappingZ == CYCLE_UNDEF) ? "0" : State.CycleTappingZ;
	CString X,Y;
	X = (State.CycleI == CYCLE_UNDEF) ? "0" : InvertSign(State.CycleI);
	Y = (State.CycleJ == CYCLE_UNDEF) ? "0" : InvertSign(State.CycleJ);
	double dJ = State.GetRegister(5);
	if(dJ == 0.)
		return -1;
	CString BRD;
	BRD.Format("%f",State.CurDiamComp);
	CString text;
	NCadr* pcCadr = nullptr;
	text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g91;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	CString str = CString(NEvalExp::OpenBC()) + I + "-" + BRD + "+" + J + CString(NEvalExp::CloseBC());
	CString mstr = CString(NEvalExp::OpenBC()) + BRD + "-" + I + "-" + J + CString(NEvalExp::CloseBC());
	int XStepsNum = int((State.GetRegister(24)-State.CurDiamComp*2)/(2*dJ));
	int YStepsNum = int((State.GetRegister(25)-State.CurDiamComp*2)/(2*dJ));
	int StepsNum = max(XStepsNum, YStepsNum);
	double CurX = 0,CurY = 0;
	for(int i=0 ; i<StepsNum; ++i)
	{
		if(dJ * (i + 1) * 2 <= State.GetRegister(24)-State.CurDiamComp*2)
			CurX = dJ * (i + 1)*2;
		if(dJ * (i + 1) * 2 <= State.GetRegister(25)-State.CurDiamComp*2)
		{
			str.Format("%f",dJ);
			text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
			CurY = dJ * (i + 1)*2;
		}
		str.Format("%f",-CurX/2.);
		text = g1 + "X" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		str.Format("%f",-CurY);
		text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		str.Format("%f",CurX);
		text = g1 + "X" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		str.Format("%f",CurY);
		text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
		str.Format("%f",-CurX/2.);
		text = g1 + "X" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}
	str.Format("%f",(State.GetRegister(25) - CurY)/2-State.CurDiamComp);
	text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	CurX = State.GetRegister(24)-State.CurDiamComp*2;
	CurY = State.GetRegister(25)-State.CurDiamComp*2;
	str.Format("%f",-CurX/2.);
	text = g1 + "X" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	str.Format("%f",-CurY);
	text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	str.Format("%f",CurX);
	text = g1 + "X" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	str.Format("%f",CurY);
	text = g1 + "Y" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	str.Format("%f",-CurX/2.);
	text = g1 + "X" + str; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);

	if(State.AbsoluteCoord)
	{
		text = g90;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}

	State.NonModalCycleCall = true;

	return 0;
}

NProgram * NCUnit::GetCurRunProg(void)
{// Current running prog
	NProgram *pProg;
	if(!GetChannel().NGetProgramListCn()->Lookup(State.GetCurProg(), pProg))
		return NULL;

	return pProg;
}
NProgram * NCUnit::GetCurLoaProg(void)
{// Current loaded prog
	return (NProgram *)GetChannel().NGetProgramListCn()->GetProg(State.GetCurCadrID());
}
int NCUnit::GetErrStringNum(void)
{
	return Error;
}

bool NCUnit::JumpToNextProg(void)
{// Without execution
	// Find current prog
	CMapStringToNProgram *pPList = GetChannel().NGetProgramListCn();
	int i = pPList->GetIndex(State.GetCurProg());
	if(i < 0 || i >= pPList->GetMainProgCount() - 1) 
		return false;
	CString key;
	BProgram *pProg;
	// Set next program
	pPList->GetAt(i + 1, key, (CObject *&)pProg);
	SetCurProg(key);
	return true;
}

bool NCUnit::JumpToPrevBreakpoint(void)
{
	int CHCadr = State.GetCurHCadr();
	int StartProgIndex = GetChannel().NGetProgramListCn()->GetIndex(State.GetCurProg());
	if(StartProgIndex < 0)
		return false;
	JumpToProjStart();
	int i = 0;
	for(; ; ++i)
	{
		JumpForToBreakpoint();
		int CurProgIndex = GetChannel().NGetProgramListCn()->GetIndex(State.GetCurProg());
		if(CurProgIndex > StartProgIndex)
			break;
		if(CurProgIndex == StartProgIndex)
			if(State.GetCurHCadr() >= CHCadr)
				break;
	}
	JumpToProjStart();
	for(int k = 0 ; k < i ; ++k)
		JumpForToBreakpoint();
	return true;
}

int NCUnit::GetCallStackSize(void)
{
	return int(State.CallStack.GetSize());
}

BOOL NCUnit::ProcessCoordComp(const NCUnitStatePrev & St)
{
	int code = int(State.CoordCompCode/100);
	if(code < 1 || code > 7)
		return FALSE;
	int CompNum = State.CoordCompCode - code*100;
	double CompVal;
	if(!State.DComp.Lookup(CompNum,CompVal))
	{
		StartState.DComp[CompNum] = 0.;
		CompVal = 0.;
	}
	CompVal *= GCD.LinearScale;
	if( State.CoordCompSignum != 0)
		CompVal = fabs(CompVal) * State.CoordCompSignum;
	if(State.CurCurveType == line || State.CurCurveType == fast)
	{
		State.MashcX = St.MashcX + State.CurX;
		if(code & 1)
			State.MashcX += CompVal;
		State.MashcY = St.MashcY + State.CurY;
		if(code & 2)
			State.MashcY += CompVal;
		State.MashcZ = St.MashcZ + State.CurZ;
		if(code & 4)
			State.MashcZ += CompVal;
		State.CompGeom.Set(State.CurCurveType,
				St.MashcX,St.MashcY,St.MashcZ,
				State.MashcX,State.MashcY,State.MashcZ,
				0.,0.,0.,XY);
	}
	else if(State.CurCurveType == cwarc || State.CurCurveType == ccwarc)
	{
		double CurcI,CurcJ,CurcK;
		State.Geom.GetI(&CurcI,&CurcJ,&CurcK);
		if(code == 2)
		{
			CurcJ += CompVal*CurcJ/fabs(CurcJ);
			State.MashcX = St.MashcX + State.CurX + CompVal*State.CurX/fabs(State.CurX);
			State.MashcY = St.MashcY + State.CurY + CompVal*State.CurY/fabs(State.CurY);
		}
		else if(code == 1)
		{
			CurcI += CompVal*CurcI/fabs(CurcI);
			State.MashcX = St.MashcX + State.CurX + CompVal*State.CurX/fabs(State.CurX);
			State.MashcY = St.MashcY + State.CurY + CompVal*State.CurY/fabs(State.CurY);
		}
		State.CompGeom.Set(State.CurCurveType,
				St.MashcX,St.MashcY,St.MashcZ,
				State.MashcX,State.MashcY,State.MashcZ,
				CurcI,CurcJ,0.,XY);

	}
	return TRUE;
}

// NCUnitBase implementation
BOOL NCUnitBase::GetOtherValue(const CString &key, CString & val) const
{
	val = "";
	return GCD.SOthers.Lookup(key, val);
}

BOOL NCUnitBase::GetOtherRegExp(const CString& key, CString& patt, CString& repl) const
{
	CString Exp;
	if (!GetOtherValue(key, Exp))
	{
		return FALSE;
	}
	const int LimPos = Exp.Find(_T(";;"), 0);
	if (LimPos < 0)
	{
		return FALSE;
	}
	CString RegExp = Exp.Left(LimPos);
	Exp.Delete(0, LimPos + 2);
	if (Exp.IsEmpty() || RegExp.IsEmpty())
	{
		return FALSE;
	}
	patt = RegExp;
	repl = Exp;
	return TRUE;
}

int NCUnitBase::GetRegIndexesValue(const CString &key)
{
	int val = 0;
	GCD.SRegIndexes.Lookup(key, val);
	return val;
}

bool NCUnitBase::SetRegister(NCUnitState &State, const CString &key, double Val, int DefaultInd /*= 0*/)
{
	int Ind = GetRegIndexesValue(key);
	if (Ind == 0)
		Ind = DefaultInd;
	if (Ind == 0)
		return false;
	return State.SetRegister(Ind, Val);
}

CString NCUnitBase::GetMultiNum(CString &text)
{
	int Ind = atoi(text);
	int Val = 1 + State.MultiCounter.GetValue(Ind);
	if(Val < 1)
		return "0";
	State.MultiCounter.Increment(Ind);
	CString Res;
	Res.Format("%d", Val);
	return Res;
}
CString NCUnitBase::GetWord(CString &text)
{
	if(text.GetLength() <= 0)
		return "";
// extract first character and find appropriate key in GCD.SWordDef 
// return that key or "" if key was not found
	// Favorite concepts processing
	CString FCon;
	if(State.FavorConcepts.GetStr(CString(text[0]), FCon))
	{
		text.Delete(0);
		return FCon;
	}
	//END: Favorite concepts processing
	CString ch = text.Left(1);// assume that word may be 1 length only
	CString key = WordDef.GetKey(ch, *this);
	if(!key.IsEmpty())
	{
		text.Delete(0,ch.GetLength());
		//MultiParameter processing (14 characters in this word)
		if(key.GetLength() == 14 + 1)
			if(key.Left(14) == "MultiParameter")
			{
				ch += GetMultiNum(key.Right(1));
				if(State.FavorConcepts.GetStr(ch, key))
					return key;
				if(!GCD.SWordDefInv.Lookup(ch, key))
					return "";
			}
		//END: MultiParameter processing 
	}
	return key;
}

void NCUnitBase::DoReplace(CString &text)
{
	// Make upper if needed
	CString YesNo;
	if(GCD.SOthers.Lookup("MakeUpper", YesNo))
		if(YesNo == _T("Yes"))
			text.MakeUpper();


	if(GCD.SWordOrder.GetSize() > 0)
	{
		if(GCD.SWordOrder[0] == "NCEStyle")
		{
			//text.Remove(' ');
			//text.Remove('\n');
			//text.Remove('\t');
			//text.Remove('\r');
			//CString f = text.Left(1);
			//if(f != "-" && f != "+")
			int n = text.GetLength(),m=0;

			char* f = text.GetBuffer();
			for(int i = 0; i < n; ++i)
			{
				if(f[i] == '\n' || f[i] == '\t' || f[i] == '\r' || f[i] == ' ')
					++m;
				else
					f[i - m] = f[i];
			}
			text.Delete(n - m, m);
			char FirstChar = text[0];
			if(FirstChar != '-' && FirstChar != '+')
			{// The first character IS NOT sign
				CString Code = text.SpanExcluding("-+");
				text.Delete(0,Code.GetLength());
				CString key;
				CString rep;
				for( int ind = 0; ind < GCD.SWordOrder.GetSize(); ind++)
				{
					key = GCD.SWordOrder[ind];
					rep = GCD.SWordRep[ind];
					if(Code == key)
						Code = rep;
				}
				text = Code + text;
			}
			else
			{// The first character IS sign
				//Calculate the number of signes in the string
				int ks = 0;
				for(int l = 0; l < text.GetLength();++l)
					if(text[l] == '+' || text[l] == '-')
						ks++;
				if(ks%2 != 0)
				{// Replace last sign if any		
					CString t = text.Right(1);
					if(t == "-" || t == "+")
					{
						text.Delete(text.GetLength()-1);
						text = text + ((t=="-") ? "G2" : "G3");
					}
				}
				// Replace signs
				char coord[4] = {'X','Y','I','J'};
				for(int i=0,j=0;j<4;i++)
				{
					if(i >= text.GetLength())
						break;
					if(text[i] == '+' || text[i] == '-')
					{
						text.Insert(i++,coord[j++]);
						text.Insert(++i,'0');
					}
				}
			}
			return;
		}
	}
	CString str;
	if(GCD.SOthers.Lookup("PutFirst",str))
	{// Move codes mentioned in the str to the beginning of the text
		CStringArray StrArr;
		int i = 0;
		for( i = 0; str.GetLength() > 0; ++i)
		{
			CString code = str.SpanExcluding(",");
			str.Delete(0,code.GetLength() + 1);
			StrArr.Add(code);
		}
		int pos;
		for(int k = 0; k < i; ++k)
		{
			CString code = StrArr[k];
			if((pos = text.Find(code)) >= 0)
			{
				text.Delete(pos,code.GetLength());
				text = code + text;
			}
		}
	}
// Add comment char if IgnoreCadrContaining is defined and corresponding string
// presents in this cadr
	if(GCD.SOthers.Lookup("IgnoreCadrContaining",str))
	{
		if(text.Find(str) >= 0)
		{
			CString com;
			GCD.SWordDef.Lookup("CommentStart",com);
			text = com + text;
		}
	}
// replace all words from SWordRep map and remove all blanks
	//if(!SinumericStyle)
	//	text.Remove(' ');
	//text.Remove('\n');
	//text.Remove('\t');
	//text.Remove('\r');
	int n = text.GetLength();
	int m = 0;
    char* f = text.GetBuffer();
	for(int i = 0; i < n; ++i)
	{
		if(f[i]=='\n' || f[i]=='\t' || f[i]=='\r')
			++m;
		else if(!GCD.SinumericStyle && f[i]==' ')
			++m;
		else
		{ 
			f[i - m] = f[i];
		}
	}
	text.Delete(n - m, m);
/*	// Sinumeric 
	// MCALL CYCLExx -> Gxx
	static BOOL CycleActive = FALSE;// CYCLE without MCALL was called in the previos cadr
	static BOOL ng = CycleNeedsGeom();
	if(CycleActive)
	{

		if(text[0] == 'N')
		{
			text = "G80" + text;
			CycleActive = FALSE;
			GCD.SOthers.SetAt("CycleNeedsGeom", ng ? "Yes" : "No");
		}
	}
	int pos = text.Find("MCALLCYCLE");
	if(pos >= 0)
	{
		// Parsing parameter string
		text.Replace("MCALLCYCLE","G");
	}
	else // MCALLCYCLE not found
	{
		pos = text.Find("CYCLE");
		if(pos >= 0)
		{
			// Parsing parameter string
			text.Replace("CYCLE","G");
			CycleActive = TRUE;
			GCD.SOthers.SetAt("CycleNeedsGeom","No");
		}
	}
	// MCALL -> G80 (standard)
	// CYCLExx ->
	// %_N_L -> L (standard)
		
*/	// End Sinumeric
	// BaltSysStyle
	if(GCD.BaltSysStyle)
	{
		if(!ProcBaltSysStyle(text))
			return;// Error in ProcBaltSysStyle
	}
	// END:BaltSysStyle
	// SinumericStyle
	if(GCD.SinumericStyle)
	{
		if(!ProcSinumericStyle(text))
			return;// Error in ProcSinumericStyle
		text.Replace("  "," ");
	}
	// END:SinumericStyle
	for( int ind = 0; ind < GCD.SWordOrder.GetSize(); ind++)
	{
		CString key = GCD.SWordOrder[ind];
		if(key.GetLength() >= 2)
		{
			if(key[0] == '_' && key[1] == '#')
			{// Conditional replace
				key.Delete(0,2);
				CString cond = key.SpanExcluding("_");
				if(cond.GetLength() < 2)
					return;// error in cond
				key.Delete(0, cond.GetLength());
				key.Delete(0);// delete "_"
				CString condstr = key.SpanExcluding("_");
				key.Delete(0, condstr.GetLength());
				key.Delete(0);// delete "_"
				if(cond == "IFFIRST")
				{
					if(text.Left(condstr.GetLength()) == condstr)
						text.Replace(key, GCD.SWordRep[ind]);
				}
				continue;
			}
		}
		// Unconditional replace ( conditional replace ends with "continue"
		text.Replace(key, GCD.SWordRep[ind]);
	}
	// Regular expressions 
	GCDV.ReplaceSet.Apply(text);
	// Replace functions names
	if(GCD.UseStdFuncNames)
	{
		for( int ind = 0; ind < FUNC_NUMBER; ind++)
		{
			CString key = FUNC_TITLES[ind];
			text.Replace(key, CString(FUNC_NAMES[ind]));
		}
	}

}

bool NCUnit::GetDIValue(CString &text, double *val)
{
// Get double value from text WITHOUT "DecimalConversion" usage

// text may contain a number, register or +-register
	CString CharSet;
	CharSet = ExprCharSet + FUNC_NAMES;
	if(text.GetLength() > 0)
		if(text[0] == '=')
			text.Delete(0);
	CString exp("");
	CString tmp;
	ReplaceVariables(text);
	int n = text.GetLength();
	for(int i=0; i<n; i++)
	{
		tmp = text.SpanIncluding(CharSet);
		text.Delete(0,tmp.GetLength());
		exp += tmp;
		if(text.GetLength() == 0)
			break;
		if(text[0] == GCD.RegisterSymbol)
		{
			if(Sign.Find(exp.Right(1)) >= 0)// Sign before register
			{
				text.Delete(0);
				exp += GCD.RegisterSymbol;
			}
			else
				break;
		}
		else
			break;
	}
// This block added to speed up, may be removed
// The goal is to prevent usage of NEvalExp if expression consists of 
// one constant only
	tmp = exp.SpanIncluding("0123456789.-+");
	if(tmp.GetLength() == exp.GetLength())
	{
		if(exp.Find('+') < 1 && exp.Find('-') < 1)
		{
			if(sscanf(exp,"%lf",val) == 1)
			{
				return true;
			}
			else
			{
				CString str;
				if(!GCD.SOthers.Lookup("EmptyZero",str))
					return false;
				if(str != "Yes")
					return false;
				if(tmp.GetLength() == 0) //  (Zero may be missed)
				{
					*val = 0.;
					return true;
				}
				return false;
			}
		}
	}
//End: This block added to speed up, may be removed

	NEvalExp expression( (NCUnit *)this, UseIntegerArithm);
	return expression.Evaluate( exp, val );	
}

bool NCUnitBase::GetTValue(CString &text, CString *val)
{
	*val = text.SpanIncluding("0123456789");
	text.Delete(0,val->GetLength());
	return true;
}

bool NCUnitBase::GetIValue(CString &text, int *val) const
{
	CString sign = text.SpanIncluding("+-");
	if(sign.GetLength() > 1)
		return false;
	text.Delete(0,sign.GetLength());
	CString num = text.SpanIncluding("0123456789");
	text.Delete(0,num.GetLength());
	if(sscanf(sign + num,"%ld",val)!=1)
		return false;
	return true;

}

bool NCUnitBase::GetFullLValue(CString &text, int *val) const
{
	CString LabelFirst;
	if(!GCD.SWordDef.Lookup(_T("LabelFirst"), LabelFirst))
		return false;//error	
	if(text.Left(1) != LabelFirst)
		return false;//error	
	text.Delete(0);
	if(!GetLValue(text, val))
		return false;//error
	return true;
}
bool NCUnitBase::GetLValue(CString &text, int *val) const
{
	CString LabelLast;
	switch(GCD.LType)
	{
	case LT_BALTSYS:
		if(!GCD.SWordDef.Lookup(_T("LabelLast"), LabelLast))
			return false;
		break;
	case LT_SINUMERIK:
			LabelLast = _T(":");
		break;
	}
	CString Label = text.SpanExcluding(LabelLast);
	if(Label.GetLength() == text.GetLength())
	{
		text.Empty();
		return false;
	}
	text.Delete(0, Label.GetLength() + 1);
	GetHashNum(Label, val);
	return true;

}
bool NCUnitBase::GetHashNum(const CString &text, int *val)
{
	*val = CStringRefElementTraits<CString>::Hash(text);
	return true;

}

void NCUnit::SetUCSAutoNames()
{
	// Set auto names for UCSs
	for (auto it = NCUnit::GetUsedUCSs().begin(); it != NCUnit::GetUsedUCSs().end(); it++)
	{
		CString WordStr;
		GetWordValue(_T("FunctionCode"), WordStr);
		if (StartState.UCSs[*it].GetName().IsEmpty())
		{
			CString FuncStr, FuncKey;
			FuncKey.Format(_T("WorkCoordSys%d"), *it + 1);
			if (GetFuncNum(FuncKey, FuncStr))
				StartState.UCSs[*it].SetName(WordStr + FuncStr, true);
		}
	}
}

int NCUnit::AskStartState(int StartPage, int CurZeroPos)
{
	SetUCSAutoNames();
	NCUnit::GetUsedUCSs().swap(StartState.UCSs.GetUsed());
	int Ret = StartState.AskUser(StartPage, CurZeroPos);
	NCUnit::GetUsedUCSs().swap(StartState.UCSs.GetUsed());
	return Ret;
}
int NCUnitBase::CrDrillCycle(NCycle *pCycle)
{
	NCadr *pcCadr;
	CString g, z, beg;
	struct FDef *pFDef;
	double MashZ = 0.;
	if( !GCD.SWordDef.Lookup("FunctionCode",g) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd",beg) ||
		!GCD.SFuncDef.Lookup("Rapid",pFDef))   
		return -1;
	switch(State.CurPlane)
	{
	default:
	case XY:
		MashZ = State.MashZ;
		if(!GCD.SWordDef.Lookup("SpindleAxis",z))
			return -1;
		break;
	case XZ:
		MashZ = State.MashY;
		if(!GCD.SWordDef.Lookup("VerticalAxis",z))
			return -1;
		break;
	case YZ:
		MashZ = State.MashX;
		if(!GCD.SWordDef.Lookup("HorizontalAxis",z))
			return -1;
		break;
	}
	CString g0 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Linear",pFDef))   
		return -1;
	CString g1 = g + pFDef->Num;
	
	if(State.CycleZ == CYCLE_UNDEF)
		return -1;
	if(State.R1 == CYCLE_UNDEF)
		return -1;
	CString text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g0 + z + State.R1;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);

	if(State.CurCycle == DRILLDWELL)
	{
		if( !GCD.SFuncDef.Lookup("Dwell",pFDef))   
			return -1;
		CString g4 = g + pFDef->Num;
		CString DwellSym;
		if( !GCD.SWordDef.Lookup("DwellTime", DwellSym))
			return -1;
		CString Time;
		Time.Format(_T("%d"), State.CycleDwellTime);
		text = g4 + DwellSym + Time; pcCadr = new NCadr(text); pCycle->AddCadr(pcCadr);
	}

	if(State.R2 != CYCLE_UNDEF)
	{
		{text = g0 + z + State.R2;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}
	}
	else // R2 undefined
	{
		CString bR;
		bR.Format("%lf", MashZ);
		text = g0 + z + bR;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	}
	text = g0;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	return 0;
}

bool NCUnitBase::SetCurProg(const CString &str)
{// without execution
	State.EndProgram = false;
	State.SetCurCadr(0);
	State.ClearCurHCadr();
	State.ClearCurHCadrAbs();
	State.SetCurProg(str);
	State.ProgramStarted = false;
	State.CadrID.Prog = GetChannel().NGetProgramListCn()->GetIndex(str);
	State.CadrIDEx.Prog = State.CadrID.Prog;
	if(State.CadrID.Prog < 0)
		return false;
	else
		return true;
}

void NCUnitBase::MakeGotoList(BProgram *pProg)
{
	for(int i = 0; i < pProg->GetSize(); ++i)
	{
		CString Curtext = (*pProg)[i]->GetReText();
//		DoReplace(Curtext);
		switch(GCD.LType)
		{
		case LT_EMPTY:
		case LT_EXPRESSION:
			{
				CString Word = GetWord(Curtext);
				if(Word == _T("LineNumber"))
				{
					int cnum;
					if(!GetIValue(Curtext, &cnum))
						continue;
					pProg->GetGotoList().addMark(cnum, i);
				}
			}
			break;
		case LT_BALTSYS:
			{
				CString Word = GetWord(Curtext);
				if(Word == _T("BlockDebug"))
					Word = GetWord(Curtext);
				if(Word == _T("LabelLast"))
				{
					CString CLast;
					GCD.SWordDef.Lookup(_T("LabelLast"), CLast);
					CString CFirst;
					GCD.SWordDef.Lookup(_T("LabelFirst"), CFirst);
					if(CFirst == CLast)
						Word = _T("LabelFirst");
				}
				if(Word == _T("LabelFirst"))
				{
					int cnum;
					if(!GetLValue(Curtext, &cnum))
						continue;
					pProg->GetGotoList().addMark(cnum, i);
				}
			}
			break;
		case LT_SINUMERIK:
			if(Curtext.Left(1) == GCDV.SWordDef[_T("LineNumber")])
			{
				CString Word = GetWord(Curtext);
				int cnum;
				GetIValue(Curtext, &cnum);
			}
			if(Curtext.GetLength() >= 3)
			{
				if(isletter(Curtext[0]) && isletter(Curtext[1]))
				{// Label
					int EndInd = Curtext.Find(':');
					if(EndInd < 0)
						continue;
					int cnum;
					if(!GetLValue(Curtext, &cnum))
						continue;
					pProg->GetGotoList().addMark(cnum, i);
				}
			}
			break;
		}
	}
	pProg->GetGotoList().isCreated = true;
}

int NCUnit::MakeProgGroupList(BProgram* pProg, int ProgInd, const CString& ProgName)
{
	if (!GCD.SinumericStyle)
		return 0;
	for (int i = 0; i < pProg->GetSize(); ++i)
	{
		CString Curtext = (*pProg)[i]->GetReText();
		Curtext.Remove(' ');
		//		DoReplace(Curtext);
		switch (GCD.LType)
		{
		case LT_EMPTY:
		case LT_EXPRESSION:
		{
		}
		break;
		case LT_BALTSYS:
		{
		}
		break;
		case LT_SINUMERIK:
			if (Curtext.Left(1) == GCDV.SWordDef[_T("LineNumber")])
			{
				CString Word = GetWord(Curtext);
				int cnum;
				GetIValue(Curtext, &cnum);
			}
			if (Curtext.Left(1) == GCDV.SWordDef[_T("ProgramStartEnd")])
			{
				const int Res = ProgGroupList.AddSubStart(cadrID(ProgInd, i));
				//if (Res < 0)
				//	return Res; // error
			}
			else if (Curtext.GetLength() >= 3)
			{
				if (Curtext[0] == '{')
				{// may be a word from ProgGroupList
					Curtext.Delete(0);
					CString Word = Curtext.SpanExcluding("}");
					const cadrID CurCadr(ProgInd, i);
					const int Res = ProgGroupList.AddWord(Word, CurCadr);
				}
			}
			break;
		}
	}
	const int Out = ProgGroupList.CloseOneProg(cadrID(ProgInd, pProg->GetSize()));
	// add name to errors if any
	ProgGroupList.SetErrorProgName(ProgInd, ProgName);
	return 0;
}

void NCUnit::AddGroupErrors2Prog(BProgram* pProg, int ProgInd) const
{
	for (int i = 0;; ++i)
	{
		const NProgError* pErr = ProgGroupList.GetProgError(ProgInd, i);
		if (pErr == nullptr)
			break;
		pProg->AddErrorPos(-i - 1, *pErr);
	}
}

BOOL NCUnit::DoGoTo(CString &text, enum SearchDir SDir)
{
	CString title;
	GCD.SWordDef.Lookup("ProgramNumber",title);
	if(title != text.Left(1))
		if(State.ProgramNumber == "")
		{
			Error = 1119;// level 1
			return false;//error
		}
		else
			text = title + State.ProgramNumber;
	text.Delete(0);

	int num;
	double dval;
	switch(GCD.LType)
	{
	case LT_EMPTY:
		if(!GetIValue(text, &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	case LT_EXPRESSION:
		if(!GetDIValue(text, &dval))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		num = int(dval + 0.5);
		break;
	case LT_BALTSYS:
		if(!GetFullLValue(text, &num))
		{
			Error = 1065;// level 1
			return false;//error	
		}
		break;
	case LT_SINUMERIK:
		{
			CString Label = text.SpanExcluding(_T(" "));
			text.Delete(0, Label.GetLength() + 1);
			GetHashNum(Label, &num);
		}
		break;
	}

	
	cadrID cadr = findMark(num, SDir);

	if(cadr.Prog == -1)
	{
		Error = 1066;// level 1
		return FALSE;// Error
	}
	State.OrderChanged = true;
	State.NextCadr = cadr;
	return TRUE;
}

BOOL NCUnit::DoGoToGroupEnd(cadrID CurCadr)
{
	cadrID LastCadr = ProgGroupList.GetEndOfGroup(CurCadr);
	if (LastCadr.IsEmpty())
	{
		Error = 1066;// level 1
		return FALSE;// Error
	}
	State.GotoElse = true;
	State.OrderChanged = true;
	State.NextCadr = LastCadr;
	return TRUE;
}

BOOL NCUnit::DoGoToGroupChainEnd(cadrID CurCadr)
{
	cadrID LastCadr = ProgGroupList.GetEndOfGroupChain(CurCadr);
	if (LastCadr.IsEmpty())
	{
		Error = 1066;// level 1
		return FALSE;// Error
	}
	State.OrderChanged = true;
	State.NextCadr = LastCadr;
	return TRUE;
}

cadrID NCUnit::findMark(int num, enum SearchDir SDir)
{
	cadrID cadr = State.CadrID;
	cadr.Cadr = 0;
	CString key;
	BProgram *pProg;
	GetChannel().NGetProgramListCn()->
		GetAt(cadr.Prog, key, (CObject *&)pProg);

	//если первый просмотр кода
	if (!pProg->GetGotoList().isCreated)
		MakeGotoList(pProg);

	int strNum = -1;
	switch (SDir)
	{
	case SD_EMPTY: // search whole text from the beginning
		strNum = pProg->GetGotoList().findMark(num, -1, false);
		break;
	case SD_FOR:
		strNum = pProg->GetGotoList().findMark(num, State.GetCurCadr(), false);
		break;
	case SD_BACK:
		strNum = pProg->GetGotoList().findMark(num, State.GetCurCadr(), true);
		break;
	case SD_FORBACK:
		strNum = pProg->GetGotoList().findMark(num, State.GetCurCadr(), false);
		if(strNum == -1)
			strNum = pProg->GetGotoList().findMark(num, State.GetCurCadr(), true);
		break;
	case SD_BACKFOR:
		strNum = pProg->GetGotoList().findMark(num, State.GetCurCadr(), true);
		if (strNum == -1)
			strNum = pProg->GetGotoList().findMark(num, State.GetCurCadr(), false);
		break;
	}

	if (strNum < 0)
		cadr.Prog = -1;
	else
		cadr.Cadr = strNum;
	if (!CheckGotoPair(State.GetCurCadrID(), cadr))
		cadr.Prog = -1;
	return cadr;
}

bool NCUnit::CheckGotoPair(cadrID Src, cadrID Dest)
{
	if (ProgGroupList.empty())
		return true;
	CString sval;
	if(GetOtherValue(_T("GotoCheck"), sval))
		if (sval == _T("Yes"))
		{
			if (Src.Prog != Dest.Prog)
				return false;
			const auto DestGroup = ProgGroupList.GetSmallestGroup(Dest);
			if (Src.Cadr > DestGroup.first && Src.Cadr <= DestGroup.second.first)// Src belongs the same group
			{
				// pop WhileStack for every leaving cycle group
				for (auto SrcGroup = ProgGroupList.GetSmallestGroup(Src); SrcGroup.first != DestGroup.first;
					SrcGroup = ProgGroupList.GetSmallestGroup(cadrID(Src.Prog, SrcGroup.first)))
				{
					if (ProgGroupList.IsGroupCycle(SrcGroup.second))
					{
						int count = 0;
						cadrID p = cadrID(-1);
						State.WhileStack.Pop(p, count);
					}
				}

				return true;
			}
			return false;
		}
	return true;
}

BOOL NCUnit::ProcessIF(CString & text, BOOL IfNot)// 16.03.04 added
{
	text.Delete(0, 2);
	if (!GCD.SinumericStyle)
	{
		text.Replace("LT", "<");
		text.Replace("LE", "?");
		text.Replace("GT", ">");
		text.Replace("GE", "!");
		text.Replace("EQ", "=");
		text.Replace("NE", ":");
		text.Replace("AND", "&");
		text.Replace("XOR", ":");
		text.Replace("OR", "|");
	}
	CString RegSym = (GCD.RegisterSymbol == '0') ? _T("") : CString(GCD.RegisterSymbol);

	CString CharSet = RegSym + ExprCharSet + LogCharSet + FUNC_NAMES;

	ReplaceVariables(text);
	CString exp = text.SpanIncluding(CharSet);
	if( exp.GetLength() == 0)
	{
		Error = 1024;// level 1
		return FALSE;
	}
	NEvalExp expression((NCUnit *)this, UseIntegerArithm);
	double val;
	if(!expression.Evaluate(exp, &val))
	{
		Error = 1025;// level 1
		return FALSE;
	}
	text.Delete(0,exp.GetLength());

	bool IfStrProc = true;
	if(GCD.SinumericStyle)
	{ // Обработка случая с двумя IF в одном кадре
		if(!text.IsEmpty())
		{
			CString M;
			if(!GCD.SWordDef.Lookup(_T("MiscFunctionCode"), M))
			{
				Error = 1025;// level 1
				return FALSE;
			}
			if(text[0] == M[0])
			{
				IfStrProc = false;
				int IFInd = text.Find(_T("IF"));
				if(IFInd > 0)
				{
					if( (val == 0.) == !IfNot )
					{
						text.Delete(0, IFInd);
					}
					else
					{
						text.Delete(IFInd, text.GetLength());
					}
				}
				else
				{
					if( (val == 0.) == !IfNot )
					{
						text.Empty();
					}
				}
			}
		}
	}
	if(IfStrProc)
	{// Ordinary processing
		if( (val == 0.) == !IfNot )
		{
			CString buf;
			if(!GCD.SOthers.Lookup("IfNotStrAdd",buf))
				buf = "";
			text = buf;
		}
		else
		{
			CString buf;
			if(!GCD.SOthers.Lookup("IfStrAdd",buf))
				buf = "";
			text = text + buf;
		}
	}
	return TRUE;
}

BOOL NCUnit::ProcessCondition(CString& text, bool& Res)
{
	if (!GCD.SinumericStyle)
	{
		text.Delete(0, 2);
		text.Replace("LT", "<");
		text.Replace("LE", "?");
		text.Replace("GT", ">");
		text.Replace("GE", "!");
		text.Replace("EQ", "=");
		text.Replace("NE", ":");
		text.Replace("AND", "&");
		text.Replace("XOR", ":");
		text.Replace("OR", "|");
	}
	CString RegSym = (GCD.RegisterSymbol == '0') ? _T("") : CString(GCD.RegisterSymbol);

	CString CharSet = RegSym + ExprCharSet + LogCharSet + FUNC_NAMES;

	ReplaceVariables(text);
	CString exp = text.SpanIncluding(CharSet);
	if (exp.GetLength() == 0)
	{
		Error = 1024;// level 1
		return FALSE;
	}
	NEvalExp expression((NCUnit*)this, UseIntegerArithm);
	double val;
	if (!expression.Evaluate(exp, &val))
	{
		Error = 1025;// level 1
		return FALSE;
	}
	text.Delete(0, exp.GetLength());

	Res = val != 0.;
	return TRUE;
}

int NCUnit::RunCycle(NCadr *pCadr, bool ResetMatrs)
{
	int bufCurCadrNum = State.CurCadrNum;
	enum CycleType buf = State.CurCycle;
	State.CurCycle = CYCLE_OFF;
	bool  NonModalCycleCall = State.NonModalCycleCall;
	cadrID bufCadr = State.GetCurCadrID();
	bool bufOC = State.OrderChanged;
	cadrID bufNC = State.NextCadr;
	BOOL bufAC = State.AbsoluteCoord;
	State.AbsoluteCoord = TRUE;
	NCadrGeom bAddCompGeom = State.AddCompGeom;
	NCadrGeom bCompGeom = State.CompGeom;
	NCadrGeom bGeom = State.Geom;
	NCadrGeom bAddGeom = AddState.Geom;
	double bTime = State.Time;
	State.AllowBreak = false;

	BProgram *pParentProg = GetCurRunProg();
	RunProgram(pCadr->pCycle, pParentProg, -1, ResetMatrs);

	State.AllowBreak = true;
	// Компенсируем рабочий ход на выходе из цикла
//	State.Time -= 0.5 * (State.Time - bTime);

	State.AddCompGeom = bAddCompGeom;
	State.CompGeom = bCompGeom;
	AddState.Geom = bAddGeom;
	State.Geom = bGeom;
	if(NonModalCycleCall)
		State.CurCycle = CYCLE_OFF;
	else
		State.CurCycle = buf;
	State.SetCurCadr(bufCadr.Cadr);
	State.CurCadrNum = bufCurCadrNum;
	State.OrderChanged = bufOC;
	State.NextCadr = bufNC;
	State.AbsoluteCoord = bufAC;
	return 0;
}
int NCUnit::CrTurnFaceCutCycle(NCycle *pCycle)
{
	return 0;
}
int NCUnit::CrTurnCutCycle(NCycle *pCycle)
{
	bool NeedClear = true;
	CString tmp;
	if(GCD.SOthers.Lookup(_T("TurnCutCycleMod"), tmp))
		if(tmp == _T("Yes"))
			NeedClear = false;
		if(NeedClear)
		{
			NTurnCutCycle::stat_dX = 0.; 
			NTurnCutCycle::stat_R = 0.; 
			NTurnCutCycle::stat_dZ = 0.; 
		}


	double dX = State.CurX;
	double dZ = State.CurZ;

	if(State.ChangedCoord.x)
		NTurnCutCycle::stat_dX = dX;
	if(State.ChangedCoord.z)
		NTurnCutCycle::stat_dZ = dZ;
	if(State.ChangedCoord.r)
		NTurnCutCycle::stat_R = State.TurnThreadR2;

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	//Compensation processing
	TurnCutCompProc();


	State.MashX -= State.CurX;
	State.MashZ -= State.CurZ;
	State.MashcX -= State.CurX;
	State.MashcZ -= State.CurZ;
	State.CurX = 0.;
	State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	NTurnCutCycle CutCycle(CycleGeom, State.CurCycle == TURNFACECUT);

	if(!CutCycle.IsOK())
		return -1;

	NCadrGeom *pLast = CycleGeom[CycleGeom.GetSize() - 1];
	NCadrGeom *pNew = new NCadrGeom;
	pNew->SetAttr(1);
	pNew->SetType(fast);
	pNew->SetB(pLast->GetE());
	pNew->SetE(CycleGeom[0]->GetB());
	CycleGeom.Add(pNew);

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}
int NCUnit::CrTurnRoughCycle(NCycle *pCycle)
{
	double U = State.GetRegister(5132), R = State.GetRegister(5133);
	if(U <= 0.)
	{
		Error = 1087;// level 1
		return -7;
	}

	double ShX = State.CurX, ShZ = State.CurZ;
	State.MashX -= ShX;
	State.MashZ -= ShZ;
	State.MashcX -= ShX;
	State.MashcZ -= ShZ;
	State.CurX = 0.;
	State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	if(CycleGeom.GetSize() <= 1)
		return -2;
	if(!CycleGeom[0]->IsLine())
		return -2;
	BPoint St = CycleGeom[0]->GetStartDir();
	NTurnCycle RoughCycle(CycleGeom, U, R, St.GetX() > 0. ? -1. : 1.);

	if(!RoughCycle.IsOK())
		return -1;
	BMatr Shift;
	Shift.Trans(ShX, 0., ShZ);
	for(int i = 0; i < CycleGeom.GetSize(); ++i)
		CycleGeom[i]->Tr(Shift);
	CycleGeom.InsertAt(0, new NCadrGeom);
	CycleGeom[0]->SetAttr(1);
//	CycleGeom[0]->Set(fast, State.MashX, State.MashY, State.MashZ,  State.MashX + ShX, State.MashY, State.MashZ + ShZ);
	CycleGeom[0]->Set(fast, State.MashX, State.MashY, State.MashZ,  State.MashX + ShX, State.MashY, State.MashZ + ShZ);
	CycleGeom[0]->SetE(CycleGeom[1]->GetB());
	NCadrGeom *pLast = CycleGeom[CycleGeom.GetSize() - 1];
	NCadrGeom *pNew = new NCadrGeom;
	pNew->SetAttr(1);
	pNew->Set(fast, 0.,0.,0., State.MashX, State.MashY, State.MashZ);
	pNew->SetB(pLast->GetE());
	CycleGeom.Add(pNew);

	State.NonModalCycleCall = true;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}
int NCUnit::CrTurnLoopCycle(NCycle *pCycle)
{
	double U = State.GetRegister(5135), W = State.GetRegister(5136);
    int R = int(State.GetRegister(5137) + 0.5);
	if(R <= 0)
	{
		Error = 1087;// level 1
		return -7;
	}
	double ShX = State.CurX, ShZ = State.CurZ;
	State.MashX -= ShX;
	State.MashZ -= ShZ;
	State.MashcX -= ShX;
	State.MashcZ -= ShZ;
	State.CurX = 0.;
	State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	double dU = (R == 1) ? 0. : U / (R - 1);
	double dW = (R == 1) ? 0. : W / (R - 1);

	NCadrGeom *pLast = new NCadrGeom;
	pLast->SetAttr(1);
	pLast->SetType(fast);
	pLast->SetB(CycleGeom[CycleGeom.GetSize() - 1]->GetE());
	pLast->SetE(CycleGeom[0]->GetB() - BPoint(dU, 0., dW, 0.));
	CycleGeom.Add(pLast);
	BMatr Shift;
	Shift.Trans(dU * R + ShX, 0., dW * R + ShZ);
	for(int i = 0; i < CycleGeom.GetSize(); ++i)
		CycleGeom[i]->Tr(Shift);

	BGeomArray Path;
	for(int k = 0; k < R ; ++k)
	{
		for(int i = 0; i < CycleGeom.GetSize(); ++i)
		{
			Path.AddElement(*CycleGeom[i]);
			CycleGeom[i]->Tr(BMatr().Trans(-dU, 0., -dW));
		}
	}
	CycleGeom.RemoveAll();

	Path.InsertAt(0, new NCadrGeom);
	Path[0]->SetAttr(1);
	Path[0]->Set(fast, State.MashX, State.MashY, State.MashZ,  State.MashX + ShX + R * dU, State.MashY, State.MashZ + ShZ + R *dW);
	CycleGeom.Append(Path);
	Path.SetSize(0);
	pLast = CycleGeom[CycleGeom.GetSize() - 1];
	NCadrGeom *pNew = new NCadrGeom;
	pNew->SetAttr(1);
	pNew->Set(fast, 0.,0.,0., State.MashX, State.MashY, State.MashZ);
	pNew->SetB(pLast->GetE());
	CycleGeom.Add(pNew);

	State.NonModalCycleCall = true;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}
int NCUnit::CrTurnClearCycle(NCycle *pCycle)
{
	if(!State.ActiveCopyEnding)
		return 0;
	// Last cadr
	State.CurCycle = CYCLE_OFF;
	State.MashX = State.GetRegister(9002);
	State.MashZ = State.GetRegister(9003);
	State.MashcX = State.GetRegister(9004);
	State.MashcZ = State.GetRegister(9005);
	State.SetRegister(9002, 0);
	State.SetRegister(9003, 0);
	State.SetRegister(9004, 0);
	State.SetRegister(9005, 0);


	NCadrGeom ReturnCadr;
	ReturnCadr.SetType(fast);
	ReturnCadr.SetAttr(1);
	ReturnCadr.SetB(State.Geom.GetE());
	ReturnCadr.SetE(BPoint(State.MashX, 0, State.MashZ, 1.));
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(ReturnCadr);
	ReturnCadr.SetB(State.CompGeom.GetE());
	ReturnCadr.SetE(BPoint(State.MashcX, 0, State.MashcZ, 1.));
	CycleGeomComp.RemoveAll();
	CycleGeomComp.AddElement(ReturnCadr);

//	State.CurX = ReturnCadr.GetE().GetX() - ReturnCadr.GetB().GetX();
//	State.CurZ = ReturnCadr.GetE().GetZ() - ReturnCadr.GetB().GetZ();


	return 0;
}
int NCUnit::CrTurnGroovCycle(NCycle *pCycle)
{
	double step = DWELL_STEP;
	CString Sstep;
	if(GCD.SOthers.Lookup("DwellTimeDiscr",Sstep))
		step = atoi(Sstep) * 0.001;
	double DwellTime = 0.;
	CString SDwellTime;
	if(GetOtherValue("TurnGroovDwell", SDwellTime))
		DwellTime = atof(SDwellTime) * step;

	double P = State.GetRegister(9000);
	double Q = State.GetRegister(4321);
	if (Q == 0)
		Q = 1000000.;
    double R = State.GetRegister(9001);
	if(State.ChangedCoord.x)
	{
		double buf = State.GetRegister(4322);
		if(buf != 0.)
			R = buf;
	}
	State.SetRegister(9000, 0.);
	State.SetRegister(4321, 0.);
	State.SetRegister(4322, 0.);
//	State.SetRegister(9001, 0.);
	double dX = State.CurX;
	double dZ = State.CurZ;

	if(P < 0. || R < 0.
		|| State.CurCycle == TURNGROOVZ && P == 0. && fabs(dX) > 0.
		|| State.CurCycle == TURNGROOVX && P == 0. && fabs(dZ) > 0.
		)
	{
		Error = 1087;// level 1
		return -7;
	}

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	State.MashX -= State.CurX;
	State.MashZ -= State.CurZ;
	State.MashcX -= State.CurX;
	State.MashcZ -= State.CurZ;
	State.CurX = 0.;
	State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	if(CycleGeom.GetSize() <= 0)
		return -2;
	double QR = Q;
	switch(State.CurCycle)
	{
	case TURNGROOVZ:
		QR *= GCD.ZScale * State.CurZScale;
		break;
	case TURNGROOVX:
		QR *= GCD.XScale * State.CurXScale;
		break;
	}
	NTurnGroovCycle GroovCycle(CycleGeom, P, QR, State.CurCycle != TURNGROOVZ, DwellTime);

	if(!GroovCycle.IsOK())
		return -1;

	NCadrGeom *pLast = CycleGeom[CycleGeom.GetSize() - 1];
	pLast->SetAttr(1);
	pLast->SetType(fast);
	pLast->SetE(CycleGeom[0]->GetB());

	// Set time
	double StateFeed = State.Feed;
	State.Feed *= (1./(1. + 2. * R / Q));
	for(int i = 0; i < CycleGeom.GetSize(); ++i)
	{
		State.SetCadrTime(CycleGeom[i]);
	}
	State.Feed = StateFeed;

	State.NonModalCycleCall = true;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}
int NCUnit::CrTurnThreadCutCycle(NCycle *pCycle)
{
	CycleGeom.RemoveAll();

	NTurnThreadCutCycle ThreadCutCycle(CycleGeom, State.CPar);

	if(!ThreadCutCycle.IsOK())
		return -1;

	if(CycleGeom.GetSize() < 1)
		return -2;
	NCadrGeom App;
	App.Set(fast, State.MashX, State.MashY, State.MashZ, 0., 0., 0.);
	App.SetE(CycleGeom[0]->GetB());

	State.CurX = App.GetE().GetX() - State.MashX;
	State.CurZ = App.GetE().GetZ() - State.MashZ;
	State.MashX = App.GetE().GetX();
	State.MashZ = App.GetE().GetZ();
	State.MashcX = State.MashX;
	State.MashcZ = State.MashZ;
	State.Geom = App;
	State.CompGeom = App;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	// Calculate feed
	double Feed = State.CPar.PIT;
	if(Feed <= 0.)
		Feed = GetThreadPitch(State.CPar.MPIT);
	if(Feed <= 0.)
		return -3;
	State.CPar.PIT = Feed;
	double Dx = fabs(State.CPar.DM2 - State.CPar.DM1) / 2.;
	double Dz = fabs(State.CPar.FPL - State.CPar.SPL);
	double TanA = Dx < Dz ? Dx / Dz : Dz / Dx;
	double CosA = cos(atan(TanA));
	Feed /= CosA;

	// Set time
	double StateFeed = State.Feed;
	State.Feed = Feed;
	for(int i = 0; i < CycleGeom.GetSize(); ++i)
	{
		State.SetCadrTime(CycleGeom[i]);
	}
	State.Feed = StateFeed;


	NCadrGeom *pLast = CycleGeom[CycleGeom.GetSize() - 1];
	pLast->SetAttr(1);
	pLast->SetType(fast);
	pLast->SetE(CycleGeom[0]->GetB());

	State.NonModalCycleCall = true;

	return 0;
}
int NCUnit::CrTurnCutRoughCycle(NCycle *pCycle)
{
	double P = State.GetRegister(9000);
    double R = State.GetRegister(9001);
	State.SetRegister(9000, 0.);
	State.SetRegister(9001, 0.);
	double dX = State.CurX;
	double dZ = State.CurZ;

	if(P <= 0. || R < 0.)
	{
		Error = 1087;// level 1
		return -7;
	}

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	if(State.CurCycle == TURNCUTROUGH)
	{
		State.MashZ -= State.CurZ;
		State.MashcZ -= State.CurZ;
		State.CurZ = 0.;
		if(State.CurCycleRet != TOREF)
		{
			State.MashX -= State.CurX;
			State.MashcX -= State.CurX;
			State.CurX = 0.;
		}
	}
	else
	{
		State.MashX -= State.CurX;
		State.MashcX -= State.CurX;
		State.CurX = 0.;
		if(State.CurCycleRet != TOREF)
		{
			State.MashZ -= State.CurZ;
			State.MashcZ -= State.CurZ;
			State.CurZ = 0.;
		}
	}
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	NTurnCutRoughCycle CutRoughCycle(CycleGeom, P, R, State.CurCycle == TURNFACECUTROUGH);

	if(!CutRoughCycle.IsOK())
		return -1;

	NCadrGeom *pNew = new NCadrGeom;
	pNew->SetAttr(1);
	pNew->SetType(fast);
	pNew->SetB(CycleGeom[CycleGeom.GetSize() - 1]->GetE());
	pNew->SetE(State.MashX, 0., State.MashZ);
	CycleGeom.Add(pNew);

	State.NonModalCycleCall = true;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}

int NCUnit::CrTurnThreadCycle(NCycle *pCycle)
{
	if(State.FeedM != MMpREV && State.FeedM != INCHpREV)
		return -1;

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	//State.MashX -= State.CurX;
	//State.MashZ -= State.CurZ;
	//State.MashcX -= State.CurX;
	//State.MashcZ -= State.CurZ;
	//State.CurX = 0.;
	//State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	NTurnThreadCycle ThreadCycle(CycleGeom, State.Feed);

	if(!ThreadCycle.IsOK())
		return -1;

	State.NonModalCycleCall = true;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}

int NCUnit::CrTurnThreadACycle(NCycle *pCycle)
{
	if(State.FeedM != MMpREV && State.FeedM != INCHpREV)
		return -1;

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	State.MashX -= State.CurX;
	State.MashZ -= State.CurZ;
	State.MashcX -= State.CurX;
	State.MashcZ -= State.CurZ;
	State.CurX = 0.;
	State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	double SyncroTime = 0.;
	CString Val;
	if(GetOtherValue("SyncroTime", Val))
		SyncroTime = atof(Val);

	double Q1 = State.GetRegister(5140);
	double R1 = State.GetRegister(5141);
	double P1 = State.GetRegister(5142);

	double Q2 = State.GetRegister(9900);
	double P2 = State.GetRegister(9901);
	State.SetRegister(9900, 0.);
	State.SetRegister(9901, 0.);

	NTurnThreadACycle ThreadACycle(CycleGeom, Q1, R1, P1, Q2, State.TurnThreadR2, P2, State.Feed, SyncroTime);

	if(!ThreadACycle.IsOK())
		return -1;

	NCadrGeom *pLast = CycleGeom[CycleGeom.GetSize() - 1];
	NCadrGeom *pNew = new NCadrGeom;
	pNew->SetAttr(1);
	pNew->SetType(fast);
	pNew->SetB(pLast->GetE());
	pNew->SetE(CycleGeom[0]->GetB());
	CycleGeom.Add(pNew);

	// Set time
	double StateFeed = State.Feed;
	double Sc = ThreadACycle.CalcPassesNum();
	if(Sc <= 0.)
		Sc = 1.;
	State.Feed /= Sc;
	for(int i = 0; i < CycleGeom.GetSize(); ++i)
	{
		if(CycleGeom[i]->IsTimeDefined())
			CycleGeom[i]->SetWorkTimeS(CycleGeom[i]->GetWorkTimeS() * Sc);
		else
			State.SetCadrTime(CycleGeom[i]);
	}
	State.Feed = StateFeed;

	State.NonModalCycleCall = true;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}
int NCUnit::CrTurnThreadCCycle(NCycle *pCycle)
{
	if(State.FeedM != MMpREV && State.FeedM != INCHpREV)
		return -1;

	double dX = State.CurX;
	double dZ = State.CurZ;

	if(State.ChangedCoord.z)
		NTurnThreadACycle::stat_dZ = dZ;

	dZ = NTurnThreadACycle::stat_dZ;

	State.Geom.SetE(State.Geom.GetB() + BPoint(dX, 0., dZ, 0.));

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	State.MashX -= State.CurX;
	State.MashZ -= State.CurZ;
	State.MashcX -= State.CurX;
	State.MashcZ -= State.CurZ;
	State.CurX = 0.;
	State.CurZ = 0.;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	double SyncroTime = 0.;
	CString Val;
	if(GetOtherValue("SyncroTime", Val))
		SyncroTime = atof(Val);

	NTurnThreadACycle ThreadACycle(CycleGeom, 0., 0., 1., 0., 0., 0., State.Feed, SyncroTime);

	if(!ThreadACycle.IsOK())
		return -1;

	for(int i = 0; i < CycleGeom.GetSize(); ++i)
	{
		if(CycleGeom[i]->IsTimeDefined())
			State.SetCadrTime(CycleGeom[i]);// To add SyncroTime to State.Time
	}

	NCadrGeom *pLast = CycleGeom[CycleGeom.GetSize() - 1];
	NCadrGeom *pNew = new NCadrGeom;
	pNew->SetAttr(1);
	pNew->SetType(fast);
	pNew->SetB(pLast->GetE());
	pNew->SetE(CycleGeom[0]->GetB());
	CycleGeom.Add(pNew);

	State.NonModalCycleCall = false;

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}

BOOL NCUnit::ProcessLoopCycle(CString & text)
{
	if(!MakeCycleGeom(text, CString(_T("LoopCycleSearch"))))
		return FALSE;

	return TRUE;
}
BOOL NCUnit::MakeCycleGeom(CString & text, CString &key)
{
	bool CheckStonesActive = pDoc->CheckStones.IsActive();
	pDoc->CheckStones.DeActivate();

	auto BState = State;
	auto BSt = St;
	cadrID p;
	int count = 1;
	p = State.CadrID;
	++p.Cadr;
	if(State.CallStack.Push(p,count) != 0)
	{
		Error = 1022;// level 1
		return FALSE;
	}
	if(State.CallStack.Push(p,count) != 0)
	{
		Error = 1022;// level 1
		return FALSE;
	}

	SearchDir SDir = GetSDir(key, false);
	cadrID tmp;
	if(!FindCopy(text, &tmp, &State.ActiveCopyEnd, SDir))
		return FALSE;// Error defined in FindCopy
	State.CurCycle = CYCLE_OFF;
	State.SetCurCadr(tmp.Cadr);
	cadrID NextCadr;
	bool FirstCadrXZ = false;
	BGeomArray BCycleGeom;
	for(int i = 0; EvaluateCurrent(true) > -1 && i < 4096 ; ++i)
	{
		if(i == 0 && State.ChangedCoord.x && State.ChangedCoord.z)
			FirstCadrXZ = true;
		State.AddGeom2CycleGeom(BCycleGeom);
		AddState.AddGeom2CycleGeom(BCycleGeom);
		if(State.ActiveCopyEnding)
			break;
		NextCadr = State.GetCurCadrID();
	}



	if(CheckStonesActive)
		pDoc->CheckStones.Activate();
	State = BState;
	St = BSt;
	State.OrderChanged = true;
	State.NextCadr = ++NextCadr;
	State.CurCurveType = fast;


	for(int i = 0; i < BCycleGeom.GetSize(); ++i)
	{
		if(BCycleGeom[0]->HaveGeom())
			break;
		else
		{
			delete BCycleGeom[0];
			BCycleGeom.RemoveAt(0);
		}
	}
	CycleGeom.RemoveAll();
	CycleGeom.Append(BCycleGeom);
	BCycleGeom.SetSize(0);
	if(CycleGeom.GetSize() <= 0)
		return FALSE;
	if(FirstCadrXZ)
		CycleGeom[0]->SetAttr(4);
	return TRUE;
}
BOOL NCUnit::MakeSubrGeom(CString & text, BGeomArray &Cont)
{
	Cont.RemoveAll();

	auto BState = State;
	auto BSt = St;

	cadrID p;
	int count = 1;
	p = State.CadrID;
	++p.Cadr;
	if(State.CallStack.Push(p,count) != 0)
	{
		Error = 1067;// level 1
		return FALSE;
	}
	bool CallByLineNumber = false;
	State.OrderChanged = true;
	cadrID tmp;
	CString title;
	if(!GCD.SWordDef.Lookup("ProgramNumber",title))
	{
		Error = 1195;// level 1
		return FALSE;
	}
	if(!FindNextCadr(title + text, &tmp, CallByLineNumber))
		return FALSE; // Error defined in FindNextCadr

	State.CurCycle = CYCLE_OFF;
	SetCurCadr(tmp);

	for(int i = 0;EvaluateCurrent(true) > -1 && i < 4096 ; ++i)
	{
		State.AddGeom2CycleGeom(Cont);
		AddState.AddGeom2CycleGeom(Cont);
		if(State.OrderChanged == true && State.NextCadr == p)
			break;
	}

	State = BState;
	St = BSt;
	State.OrderChanged = false;
	State.NextCadr.Cadr = 0;
	State.CurCurveType = fast;


	for(int i = 0; i < Cont.GetSize(); ++i)
	{
		if(Cont[0]->HaveGeom())
			break;
		else
		{
			delete Cont[0];
			Cont.RemoveAt(0);
		}
	}
	if(Cont.GetSize() <= 0)
		return FALSE;

	return TRUE;
}

int NCUnit::ProcRoughDParam(CString & text)
{
	double D;
	if(!GetDValue(text,&D))
	{
		if (StoreError(1015))
			return -6;
		else
			D = 0.;
	}
	State.SetRegister(5132, D);
	State.SetRegister(5133, D);
	return 0;
}
int NCUnit::ProcLoopDParam(CString & text)
{
	int D;
	if(!GetIValue(text, &D))
	{
		if (StoreError(1015))
			return -6;
		else
			D = 0;
	}
	State.SetRegister(5137, D);
	return 0;
}
int NCUnit::ProcThreadRParam(CString & text)
{
	if(!GetDValue(text, &State.TurnThreadR2))
	{
		if (StoreError(1015))
			return -6;
		else
			State.TurnThreadR2 = 0.;
	}
	State.ChangedCoord.r = true;
	return 0;
}

const BMatr NCUnit::CalcMRCS(void)
{
	pDoc->GetMachTool().ProcCadrStep(*this, State);
	BPoint C(State.MashX, State.MashY, State.MashZ, 1.);
	BPoint V(State.CurI, State.CurJ, State.CurK, 0.);
	if (V.D2() < MIND * MIND)
	{
		switch(State.CurPlane)
		{
		case XY:
			V.Set(0., 0., 1., 0.);
			break;
		case XZ:
			V.Set(0., 1., 0., 0.);
			break;
		case YZ:
			V.Set(1., 0., 0., 0.);
			break;
		}
	}
	return
	BMatr().rotg(State.MoveRotateCSAngle
		, C
		, V);
}
int NCUnit::CrTurnChamfCycle(NCycle *pCycle)
{
	State.NonModalCycleCall = true;

	if(!State.Geom.IsLine())
		return 0;

	NCadrGeom BufG = State.Geom;
	BufG.SetAttr(1);
	double dXn = 0., dZn = 0.;
	if(State.CurX > 0.)
		dXn = - fabs(State.CurK);
	if(State.CurX < 0.)
		dXn = fabs(State.CurK);
	if(State.CurZ > 0.)
		dZn = - fabs(State.CurI);
	if(State.CurZ < 0.)
		dZn = fabs(State.CurI);

	BufG.SetE(BPoint(State.MashX + dXn, State.MashY, State.MashZ + dZn, 1.));

	State.MashX += State.CurI;
	State.MashZ += State.CurK;
	State.MashcX = State.MashX;
	State.MashcZ = State.MashZ;
	State.CurX += State.CurI;
	State.CurZ += State.CurK;

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(BufG);
	BufG.SetB(BufG.GetE());
	BufG.SetE(BPoint(State.MashX, State.MashY, State.MashZ, 1.));
	CycleGeom.AddElement(BufG);

	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}
int NCUnit::CrTurnRoundCycle(NCycle *pCycle)
{
	State.NonModalCycleCall = true;

	if(!State.Geom.IsLine())
		return 0;

	NCadrGeom BufG = State.Geom;
	BufG.SetAttr(1);
	double dXn = 0., dZn = 0.;
	if(State.CurX > 0.)
		dXn = - fabs(State.CurR);
	if(State.CurX < 0.)
		dXn = fabs(State.CurR);
	if(State.CurZ > 0.)
		dZn = - fabs(State.CurR);
	if(State.CurZ < 0.)
		dZn = fabs(State.CurR);

	BufG.SetE(BPoint(State.MashX + dXn, State.MashY, State.MashZ + dZn, 1.));

	double i = 0, k = 0.;
	if(State.CurX != 0.)
	{
		State.MashZ += State.CurR;
		State.CurZ += State.CurR;
		k = State.CurR;
	}
	else
	{
		if(State.CurZ != 0.)
		{
			State.MashX += State.CurR;
			State.CurX += State.CurR;
			i = State.CurR;
		}
	}
	State.MashcX = State.MashX;
	State.MashcZ = State.MashZ;

	enum curve arctype = cwarc;

	if(i != 0.)
	{
		if(State.CurZ * i > 0)
			arctype = ccwarc;
	}
	else
	{
		if(State.CurX * k < 0)
			arctype = ccwarc;
	}

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(BufG);
	BufG.SetB(BufG.GetE());
	BufG.SetE(BPoint(State.MashX, State.MashY, State.MashZ, 1.));
	BufG.SetI(i, 0., k);
	BufG.SetType(arctype);
	CycleGeom.AddElement(BufG);

	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	return 0;
}

BOOL NCUnit::ProcCancelCycle()
{
	if(StoredPlane != P_UNDEF)
	{
		State.CurPlane = StoredPlane;
		StoredPlane = P_UNDEF;
	}
	if( !State.SetCurCycle(CYCLE_OFF) )
	{
		if (StoreError(1078))
			return FALSE;
	}
	State.R1 = State.R2 = State.CycleZ = CYCLE_UNDEF;
	State.CycleDepthZ = 0.;
	State.CycleDwellTime = 0;
	State.FavorConcepts.RemoveAll();
	return TRUE;
}

bool NCUnit::ProcessCurAngle(void)
{
	switch(State.CurPlane)
	{
	case XY:
		if(State.CurX != 0. && State.CurY == 0.)
		{
			double Ang = State.CurAngle * PI / 180.;
			if(fabs(PI/2. - fabs(Ang)) < AMU)
			{
				Error = 1101;// level ???
				return false;
			}
			State.CurY = State.CurX * tan(Ang);
			State.MashY += State.CurY;
			return true;
		}
		else if(State.CurX == 0. && State.CurY != 0.)
		{
			double Ang = PI / 2. - State.CurAngle * PI / 180.;
			if(fabs(PI/2. - fabs(Ang)) < AMU)
			{
				Error = 1101;// level ???
				return false;
			}
			State.CurX = State.CurY * tan(Ang);
			State.MashX += State.CurX;
			return true;
		}
		else 
		{
			Error = 1099;// level ???
			return false;
		}
		break;
	case XZ:
		if(State.CurX != 0. && State.CurZ == 0.)
		{
			double Ang = PI / 2. - State.CurAngle * PI / 180.;
			if(fabs(PI/2. - fabs(Ang)) < AMU)
			{
				Error = 1101;// level ???
				return false;
			}
			State.CurZ = State.CurX * tan(Ang);
			State.MashZ += State.CurZ;
			return true;
		}
		else if(State.CurX == 0. && State.CurZ != 0.)
		{
			double Ang = State.CurAngle * PI / 180.;
			if(fabs(PI/2. - fabs(Ang)) < AMU)
			{
				Error = 1101;// level ???
				return false;
			}
			State.CurX = State.CurZ * tan(Ang);
			State.MashX += State.CurX;
			return true;
		}
		else 
		{
			Error = 1099;// level ???
			return false;
		}
		break;
	case YZ:
		if(State.CurZ != 0. && State.CurY == 0.)
		{
			double Ang = State.CurAngle * PI / 180.;
			if(fabs(PI/2. - fabs(Ang)) < AMU)
			{
				Error = 1101;// level ???
				return false;
			}
			State.CurY = State.CurZ * tan(Ang);
			State.MashY += State.CurY;
			return true;
		}
		else if(State.CurZ == 0. && State.CurY != 0.)
		{
			double Ang = PI / 2. - State.CurAngle * PI / 180.;
			if(fabs(PI/2. - fabs(Ang)) < AMU)
			{
				Error = 1101;// level ???
				return false;
			}
			State.CurZ = State.CurY * tan(Ang);
			State.MashZ += State.CurZ;
			return true;
		}
		else 
		{
			Error = 1099;// level ???
			return false;
		}
		break;
	}
	return true;
}

int NCUnit::MakeOrtChamfer(void)
{
	NCUnitState NextState;
	if (!GetNextState(NextState))
	{
		Error = 1137;// level ???
		return NCUnitStatePrev::ECR_ERROR;
	}
	if(State.CurR != 0.)
	{// Orthogonal round
		double dXn = 0., dZn = 0.;
		if(State.CurX > 0.)
			dXn = - fabs(State.CurR);
		if(State.CurX < 0.)
			dXn = fabs(State.CurR);
		if(State.CurZ > 0.)
			dZn = - fabs(State.CurR);
		if(State.CurZ < 0.)
			dZn = fabs(State.CurR);
		double dXc = 0., dZc = 0.;// смещения для конца дуги
		if (NextState.CurX > 0.)
			dXc = fabs(State.CurR);
		if (NextState.CurX < 0.)
			dXc = -fabs(State.CurR);
		if (NextState.CurZ > 0.)
			dZc = fabs(State.CurR);
		if (NextState.CurZ < 0.)
			dZc = -fabs(State.CurR);

		State.CurX += dXn;
		State.CurZ += dZn;
		State.MashX += dXn;
		State.MashZ += dZn;
		State.Geom.Set(State.CurCurveType,
					State.MashX-State.CurX,
					State.MashY-State.CurY,
					State.MashZ-State.CurZ,
					State.MashX,
					State.MashY,
					State.MashZ,
					0.,0.,0.,State.CurPlane);

		AddState = State;
		double i = 0., k = 0.;
		if(AddState.CurX != 0.)
		{
			AddState.MashZ += dZc;
			k = dZc;
		}
		else
		{
			if(AddState.CurZ != 0.)
			{
				AddState.MashX += dXc;
				i = dXc;
			}
		}
		
		enum curve arctype = cwarc;

		if(i != 0.)
		{
			if(AddState.CurZ * i > 0)
				arctype = ccwarc;
		}
		else
		{
			if(AddState.CurX * k < 0)
				arctype = ccwarc;
		}

		AddState.MashX -= dXn;
		AddState.MashZ -= dZn;
		AddState.CurX = AddState.MashX - State.MashX;
		AddState.CurZ = AddState.MashZ - State.MashZ;

		AddState.Geom.Set(arctype,
					State.MashX,
					State.MashY,
					State.MashZ,
					AddState.MashX,
					AddState.MashY,
					AddState.MashZ,
					i, 0., k, XZ);
		return NCUnitStatePrev::ECR_OK;
	}
	// Orthogonal chamfer
	double dXn = 0., dZn = 0.;// смещения для первого отрезка
	if (State.CurX > 0.)
		dXn = -fabs(State.CurK);
	if(State.CurX < 0.)
		dXn = fabs(State.CurK);
	if(State.CurZ > 0.)
		dZn = - fabs(State.CurI);
	if(State.CurZ < 0.)
		dZn = fabs(State.CurI);
	double dXc = 0., dZc = 0.;// смещения для отрезка фаски
	if (NextState.CurX > 0.)
		dXc = fabs(State.CurI);
	if (NextState.CurX < 0.)
		dXc = -fabs(State.CurI);
	if (NextState.CurZ > 0.)
		dZc = fabs(State.CurK);
	if (NextState.CurZ < 0.)
		dZc = -fabs(State.CurK);

	State.CurX += dXn;
	State.CurZ += dZn;
	State.MashX += dXn;
	State.MashZ += dZn;
	State.Geom.Set(State.CurCurveType,
				State.MashX-State.CurX,
				State.MashY-State.CurY,
				State.MashZ-State.CurZ,
				State.MashX,
				State.MashY,
				State.MashZ,
				0.,0.,0.,State.CurPlane);

	AddState = State;

	AddState.MashX -= dXn;
	AddState.MashZ -= dZn;

	AddState.MashZ += dZc;
	AddState.MashX += dXc;

	AddState.CurX = AddState.MashX - State.MashX;
	AddState.CurZ = AddState.MashZ - State.MashZ;

	AddState.Geom.Set(State.CurCurveType,
				State.MashX,
				State.MashY,
				State.MashZ,
				AddState.MashX,
				AddState.MashY,
				AddState.MashZ,
				0.,0.,0.,State.CurPlane);
	return NCUnitStatePrev::ECR_OK;
}

void NCUnit::TurnCutCompProc(void)
{
	if(State.CurDirComp == LR_OFF)
		return;
	// Compensation processing
	if(fabs(NTurnCutCycle::stat_dZ) < DMU)
		return;
	//Calculate displacement
	double Alf = - NTurnCutCycle::stat_R / NTurnCutCycle::stat_dZ;
	double R = State.CurDiamComp;
	R *= State.CurDirComp == RIGHT ? 1. : -1.;
	R *= NTurnCutCycle::stat_dZ > 0. ? 1. : -1.;
	R *= NTurnCutCycle::stat_dX > 0. ? 1. : -1.;
	BPoint E = State.GetToolDisplHAAS(true);// E is in XY plane
	double DeltaX = E.GetX() + Alf * E.GetY() - R * sqrt(1. + Alf * Alf);
	// Apply calculation results
	NTurnCutCycle::stat_dX += DeltaX;
	double b = NTurnCutCycle::stat_dX;
	State.Geom.SetE(State.Geom.GetE() + BPoint(DeltaX, 0., 0., 0.));

	return;
}

void NCUnit::ClearCompArrays(void)
{
	StartState.DComp.RemoveAll();
	StartState.HComp.RemoveAll();
	GCDV.ConfParNames.clear();
}

int NCUnit::RecalcRotCoords(const NCUnitStatePrev & StartSt, NCUnitStatePrev & EndSt, BPoint & EndABC)
{
	double A = EndSt.MashA;
	double B = EndSt.MashB;
	double C = EndSt.MashC;
	if(CyclicA)
	{
		A = RecalcOneRotCoord(StartSt.MashA, EndSt.MashA);
		EndSt.MashA = A;
	}
	if(CyclicB)
	{
		B = RecalcOneRotCoord(StartSt.MashB, EndSt.MashB);
		EndSt.MashB = B;
	}
	if(CyclicC)
	{
		C = RecalcOneRotCoord(StartSt.MashC, EndSt.MashC);
		EndSt.MashC = C;
	}
	EndABC.Set(A, B, C, 1.);
	return 0;
}
double NCUnit::RecalcOneRotCoord(double Start, double End)
{
	switch(State.RotType)
	{
	case Linear:
		return Fit360(End);
	case DoNotChange:
		return End;
	case ShortestDistance:
		{
			End = Fit360(End);
			double Start360 = Fit360(Start);
			double Delta = End - Start360;
			if(fabs(Delta) > 180.)
			{
				if(Delta > 0.)
					Delta -= 360.;
				else
					Delta += 360.;
			}
			return Start + Delta;
		}
	case AlwaysCW:
		return ACWVal(Start, End);
	case AlwaysCCW:
		return ACCWVal(Start, End);
	case PositiveCW:
		if(End < 0.)
			return ACCWVal(Start, fabs(End));
		else
			return ACWVal(Start, fabs(End));
	case PositiveCCW:
		if(End >= 0.)
			return ACCWVal(Start, fabs(End));
		else
			return ACWVal(Start, fabs(End));
	}
	return End;
}
double NCUnit::Fit360(double val)
{
	const double FullA = 360.;
	val -= floor(val / FullA) * FullA;
	return val;
}
double NCUnit::ACCWVal(double Start, double End)
{
	End = Fit360(End);
	double k = floor((End - Start) / 360.);
	return End - k * 360.;
}
double NCUnit::ACWVal(double Start, double End)
{
	End = Fit360(End);
	double k = ceil((End - Start) / 360.);
	return End - k * 360.;
}
int NCUnit::InitExecCadr(const NCUnitStatePrev & St)
{
	// initialize current state

	State.CurX = 0.;
	State.CurY = 0.;
	State.CurZ = 0.;
	State.CurA = 0.;
	State.CurB = 0.;
	State.CurC = 0.;
	State.CurR = 0.;
	State.CurAngle = 0.;
	State.CurAngleDefined = false;
	State.CurDiaCompHorVec = 0.;
	State.CurDiaCompVerVec = 0.;
	State.EnableExtraArc = State.EnableExtraArcModal;
	State.ParToNext = FALSE;
	State.RetractComp = FALSE;
	State.CompCancel = FALSE;
	State.RetractNext = FALSE;
	State.ChangedCoord.byte = 0;
	State.CycleI = CYCLE_UNDEF;
	State.CycleJ = CYCLE_UNDEF;
	
	State.ActiveCopyEnding = false;
	State.ActiveCopyStarting = false;

	State.MoveRotateCS = FALSE;
	State.MoveRotateCSRel = FALSE;
	State.RotateCSABC = FALSE;
	State.MoveRotateCSCancel = FALSE;
	State.ZeroPreset = FALSE;
	State.ProgStop = false;
	State.OptionalStop = false;
	State.CoordCompCode = 0;
	State.AutoCycleCount = -1;
	State.ThreadCadr = false;
	State.TurnThreadR2 = 0.;
	State.FavorConcepts.RemoveNonModal();
	State.Spindle[SPINDLE_MILL].SetMaxSpeed = FALSE;
	State.Spindle[SPINDLE_TURN].SetMaxSpeed = FALSE;
	State.TLFlag = TL_EMPTY;
// Added for NCE400
	CString ModalArc;
	if(GCD.SOthers.Lookup("ModalArc",ModalArc))
		if(ModalArc == "No")
			if(State.CurCurveType == cwarc || State.CurCurveType == ccwarc)
				State.CurCurveType = line;
// End Added for NCE400

// Added for НЦ31
	CString ModalRapid;
	if(GCD.SOthers.Lookup("ModalRapid",ModalRapid))
		if(ModalRapid == "No")
			if(State.CurCurveType == fast)
				State.CurCurveType = line;
// End Added for НЦ31

	if(!IsArcCenterAbs())
	{
		State.CurI = 0;
		State.CurJ = 0;
		State.CurK = 0;
	}
	State.Offset = LO_UNDEF;
	if( St.RetractNext )
	{
		State.SetCurDirComp(LR_OFF);
		State.CompCancel = TRUE;
		State.RetractNext = TRUE;
	}
	Error = 0;
	CString FCycle;
	if(GCD.SOthers.Lookup(_T("ForcedCycles"),FCycle))
		if(FCycle == _T("Yes"))
		{
			if(State.CurCycle != CYCLE_OFF)
				HiddenCycle = State.CurCycle;
			State.SetCurCycle(CYCLE_OFF);
		}
	State.HhLen = 0.;
	State.HhCca = 0.;
	State.HhR = 0.;
	State.NonModalCycleCall = false;
	State.CurCurveMod = NotDefCM;
	CycleGeom.RemoveAll();
	CycleGeomComp.RemoveAll();
	State.MultiCounter.Clear();
	State.LeadingAxis = -1;// Axis not defined
	State.AddCoordsChanged = false;
	State.AddCoordsFlags.ClearValues();
	State.ProbeCadr = false;
	State.TouchCadr = false;
	State.ProbeCollision = true;
	State.ProbeExpNoTouch = false;
	State.ProbeExpTouch = false;
	State.DraftCode.Clear();
	State.CPar.Reset();
	TiltPlane.SetChanging(false);
	State.PlaceTool = PT_EMPTY;
	State.PlaceToolAxes = RCP_AC;
	State.CircleFullTurns = 0;
	if(GCD.PolarCoordRecalc)
		State.CalcPolarCoord(State.MashX, State.MashY, State.MashZ);
	State.ArcCenterFromPolarCenter = FALSE; 
	State.DefinePolarCenter = FALSE;
	State.PolarHelix = false;
	if(!State.AbsoluteCoord)
	{
		State.PolarDist = 0.;
		State.PolarAngle = 0.;
	}
//	State.SetRegister(222, 0.);

	CheckPoints.clear();
	return 0;
}
int NCUnit::InitNonExecCadr(const NCUnitStatePrev & St)
{
	// initialize current state
	State.CadrTime = 0.;
	State.AddCompGeom.Clear();
	State.CompGeom.Clear();
	State.Geom.Clear();
	AddState.Geom.Clear();
	State.OrderChanged = false;
	State.GotoElse = false;
	State.CadrSkiped = false;
	State.CurCadrNum = -1;
	State.Chamfer.Clear();
	// end of initialising
	return 0;
}
int NCUnit::CrMillCircCycle(NCycle *pCycle)
{
	// U R Z Q K P J F S - modal
	// W - non modal
	double U = State.GetRegister(223);
	double R = State.GetRegister(203) + State.CurBase.z0;
	double Z = State.GetRegister(201) + ((State.CycleZAbs) ? State.CurBase.z0 : 0.);
	int Q = int(State.GetRegister(9200));
	double K = State.GetRegister(202);
	double P = State.GetRegister(370);
	double J = State.GetRegister(369);
	double W = State.GetRegister(204);
	State.SetRegister(204, 0.);

// HEIDENHAIN special
		CString NCType;
		if(GCD.SGeneral.Lookup("Type", NCType))
			if(NCType.MakeUpper() == "HEIDENHAIN")
				if(!State.CycleZAbs)
					Z *= -1.;

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	double Dtool = State.GetRegister(8000 + State.CurToolN.GetToolNum()) + State.GetRegister(108) * 2.;
	double XYdist = -P * Dtool * 0.01;
	double FAC_XY_CLR = 0.;
	CString tmp;
	if(GetOtherValue("FAC_XY_CLR", tmp))
		FAC_XY_CLR = atof(tmp);
	double Dmax = U + Dtool + 2.* FAC_XY_CLR;

	double YStartPoint = U / 2. + Dtool / 2. + XYdist;
	double XStartPoint = State.MashX - sqrt(fabs(Dmax * Dmax * 0.25 - YStartPoint * YStartPoint));
	YStartPoint += State.MashY;
	double ZStartPoint = R;
	if(Q == 4 || Q == 5 || Q == 14 || Q == 15)
	{
		ZStartPoint = (State.CycleZAbs ? Z : R - Z) + J; 
	}
	BPoint StartPoint = BPoint(XStartPoint, YStartPoint, ZStartPoint, 1.);
	BPoint EndPoint(State.MashX, State.MashY, R + W, 1.);
	BPoint CDir(1., 0., 0., 0.);

	double Zdist = StartPoint.GetZ() - (State.CycleZAbs ? Z : R - Z);
	if(Q == 2 || Q == 12)
	{
		Zdist -= J;
	}
	if(Q == 4 || Q == 5 || Q == 14 || Q == 15)
	{
		Zdist = J;
	}

	bool OneDirCut = (Q >= 10 && Q <= 15);
	NMillCircCycle MillCircCycle(CycleGeom
		, StartPoint
		, EndPoint
		, BPoint(State.MashX, State.MashY, State.MashZ, 1.)
		, CDir
		, XYdist
		, Zdist
		, K
		, 0.1
		, OneDirCut); 

	if(!MillCircCycle.IsOK())
		return -1;

	if (State.Geom.IsUndef())
		State.Geom.SetType(fast);
	if (State.CompGeom.IsUndef())
		State.CompGeom.SetType(fast);

	SetStateFromCompGeom();
	return 0;
}

void NCUnit::SetStateFromCompGeom()
{
	CycleGeom.GetLast()->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	State.MashcX = St.MashcX + State.CurX;
	State.MashcY = St.MashcY + State.CurY;
	State.MashcZ = St.MashcZ + State.CurZ;

	const auto pStartCadr = CycleGeom.GetAt(0);
	const BPoint StartPoint = pStartCadr->GetB();
	State.Geom.SetE(StartPoint.GetX(), StartPoint.GetY(), StartPoint.GetZ());
	State.CompGeom.SetE(StartPoint + (State.CompGeom.GetB() - State.Geom.GetB()));
	pDoc->GetMachTool().ProcCadrStep(*this, State);
}

int NCUnit::CrMillRectCycle(NCycle *pCycle)
{
	// W - non modal
	// U V O R Z Q K P J - modal
	// 
	double U = State.GetRegister(218);
	double V = State.GetRegister(219);
	double O = State.GetRegister(224);
	double R = State.GetRegister(203) + State.CurBase.z0;
	double Z = State.GetRegister(201) + ((State.CycleZAbs) ? State.CurBase.z0 : 0.);
	int Q = int(State.GetRegister(9200));
	double K = State.GetRegister(202);
	double P = State.GetRegister(370);
	double J = State.GetRegister(369);
	double W = State.GetRegister(204);
	State.SetRegister(204, 0.);

// HEIDENHAIN special
		CString NCType;
		if(GCD.SGeneral.Lookup("Type", NCType))
			if(NCType.MakeUpper() == "HEIDENHAIN")
				if(!State.CycleZAbs)
					Z *= -1.;

	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	double Dtool = State.GetRegister(8000 + State.CurToolN.GetToolNum()) + State.GetRegister(108) * 2.;

	double FAC_XY_CLR = 0.;
	CString tmp;
	if(GetOtherValue("FAC_XY_CLR", tmp))
		FAC_XY_CLR = atof(tmp);

	int NPas = int(min(U, V) / (P * Dtool * 0.01) + 0.999);
	double XYdist = min(U, V) / NPas;
	double XStartPoint = State.MashX - U / 2. - Dtool / 2. - FAC_XY_CLR;
	double YStartPoint = State.MashY + V / 2. + Dtool / 2. - XYdist;
	if(V > U)
	{
		XStartPoint = State.MashX - U / 2. - Dtool / 2. + XYdist;
		YStartPoint = State.MashY + V / 2. + Dtool / 2. + FAC_XY_CLR;
	}
	if(State.CurCycle == MILLRECTCORNER)
	{
		XStartPoint = State.MashX - Dtool / 2. - FAC_XY_CLR;
		YStartPoint = State.MashY + V + Dtool / 2. - XYdist;
		if(V > U)
		{
			XStartPoint = State.MashX - Dtool / 2. + XYdist;
			YStartPoint = State.MashY + V + Dtool / 2. + FAC_XY_CLR;
		}
	}
	double ZStartPoint = R;
	if(Q == 4 || Q == 5 || Q == 14 || Q == 15)
	{
		ZStartPoint = (State.CycleZAbs ? Z : R - Z) + J; 
	}
	BPoint StartPoint = BPoint(XStartPoint, YStartPoint, ZStartPoint, 1.);
	BPoint EndPoint(State.MashX, State.MashY, R + W, 1.);
	BPoint CDir(U + Dtool + 2. * FAC_XY_CLR, 0., 0., 0.);
	BPoint BDir(0., -XYdist, 0., 0.);
	if(V > U)
	{
		CDir = BPoint(0., -V  - Dtool - 2. * FAC_XY_CLR, 0., 0.);
		BDir = BPoint(XYdist, 0., 0., 0.);
	}
	double Zdist = StartPoint.GetZ() - (State.CycleZAbs ? Z : R - Z);
	if(Q == 2 || Q == 12)
	{
		Zdist -= J;
	}
	if(Q == 4 || Q == 5 || Q == 14 || Q == 15)
	{
		Zdist = J;
	}

	double DZdown = K;
	bool OneDirCut = (Q >= 10 && Q <= 15);

	if(O != 0.)
	{
		BPoint BasePoint(State.MashX, State.MashY, State.MashZ, 1.);
		BMatr M;
		M.RotZ(BasePoint, O);
		StartPoint = StartPoint * M;
		CDir = CDir * M;
		BDir = BDir * M;
	}

	NMillRectCycle MillRectCycle(CycleGeom
		, StartPoint
		, EndPoint
		, CDir
		, BDir
		, NPas + 1
		, Zdist
		, DZdown
		, 0.1
		, OneDirCut); 
	if(!MillRectCycle.IsOK())
		return -1;



	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.Geom.SetE(StartPoint.GetX(), StartPoint.GetY(), State.Geom.GetE().GetZ());
	State.CompGeom = State.Geom;
	return 0;
}

int NCUnit::CrMillRectOutCycle(NCycle *pCycle)
{
	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);
	// U V O R Z ,R Z Q J K P I F S - modal
	// W - non modal
	double U = State.GetRegister(218);
	double V = State.GetRegister(219);
	double O = State.GetRegister(224);
	double R = State.GetRegister(203) + State.CurBase.z0;
	double Z = State.GetRegister(201) + ((State.CycleZAbs) ? State.CurBase.z0 : 0.);
	double CR = State.GetRegister(220);
	int Q = int(State.GetRegister(9200));
	double J = State.GetRegister(369);
	double K = State.GetRegister(202);
	double P = State.GetRegister(370);
	double I = State.GetRegister(368);
	double W = State.GetRegister(204);
	State.SetRegister(204, 0.);


	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	double Dtool = State.GetRegister(8000 + State.CurToolN.GetToolNum()) + State.GetRegister(108) * 2.;

	double FAC_XY_CLR = 0.;
	CString tmp;
	if(GetOtherValue("FAC_XY_CLR", tmp))
		FAC_XY_CLR = atof(tmp);

	BPoint Center(State.MashX, State.MashY, R, 1.);
	if(State.CurCycle == MILLRECTCORNEROUT)
	{
		Center = Center + BPoint(U / 2., V / 2., 0., 0.); 
	}
	BPoint EX(1., 0., 0., 0.);
	BPoint EY(0., 1., 0., 0.);

	BPoint EndPoint(State.MashX, State.MashY, R + W, 1.);

// HEIDENHAIN special
		CString NCType;
		if(GCD.SGeneral.Lookup("Type", NCType))
			if(NCType.MakeUpper() == "HEIDENHAIN")
			{
				if(!State.CycleZAbs)
					Z *= -1.;

				P = 0.;

				Center.Set(State.GetRegister(216), State.GetRegister(217), R, 1.);
				double sZ = (W != 0.) ? W : State.GetRegister(200);
				EndPoint = Center + BPoint(0., 0., sZ, 0.);
			}
	BPoint StartPoint = Center - EX * (U / 2. + J + Dtool / 2. + FAC_XY_CLR) - EY * (V / 2. + J + Dtool / 2. + FAC_XY_CLR);
	BPoint BasePoint(State.MashX, State.MashY, R, 1.);
	BMatr M;
	M.RotZ(BasePoint, O);
	Center = Center * M;
	EX = EX * M;
	EY = EY * M;
	EndPoint = EndPoint * M;
	StartPoint = StartPoint * M;

	double DXrough = U + 2. * I + Dtool;
	double DYrough = V + 2. * I + Dtool;
	double DXfinish = U + Dtool;
	double DYfinish = V + Dtool;

	bool CCWDir = false;

	if(State.Spindle[SPINDLE_MILL].SpindleDir == R_CW && (Q >= 10 && Q <= 15) ||
		State.Spindle[SPINDLE_MILL].SpindleDir == R_CCW && (Q >= 0 && Q <= 5))
		CCWDir = true;

	bool NeedRough = !(Q == 4 || Q == 5 || Q == 14 || Q == 15);

	bool NeedFinish = (Q == 0 || Q == 1 || Q == 4 || Q == 5 || Q == 10 || Q == 11 || Q == 14 || Q == 15);

	double XYdist = P * Dtool * 0.01;

	double ZdistRo = StartPoint.GetZ() - (State.CycleZAbs ? Z : R - Z);

	double ZdistFi = ZdistRo;

	double DZdownRo = K;

	double DZdownFi = K;
	if(!(Q == 1 || Q == 5 || Q == 11 || Q == 15))
		DZdownFi = ZdistFi;

	double CornerR = CR;
	if(CornerR != 0.)
		CornerR += Dtool / 2.;

	double DZup = 0.1;

	NMillRectOutCycle MillRectOutCycle(CycleGeom
		, StartPoint
		, Center
		, EndPoint
		, EX 
		, EY 
		, DXrough
		, DYrough
		, DXfinish
		, DYfinish
		, CCWDir
		, NeedRough
		, NeedFinish
		, XYdist
		, ZdistRo
		, ZdistFi
		, DZdownRo
		, DZdownFi
		, CornerR
		, FAC_XY_CLR
		, DZup);



	if(!MillRectOutCycle.IsOK())
		return -1;


	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	StartPoint = MillRectOutCycle.GetStartPoint();

	State.Geom.SetE(StartPoint.GetX(), StartPoint.GetY(), State.Geom.GetE().GetZ());
	State.CompGeom = State.Geom;

	return 0;
}

int NCUnit::CrMillCircInCycle(NCycle *pCycle)
{
	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	// U R Z Q K P I J J1 F S - modal
	// W L - non modal
	double U = State.GetRegister(223);
	double R = State.GetRegister(203) + State.CurBase.z0;
	double Z = State.GetRegister(201) + ((State.CycleZAbs) ? State.CurBase.z0 : 0.);
	int Q = int(State.GetRegister(9200));
	double K = State.GetRegister(202);
	double P = State.GetRegister(370);
	double I = State.GetRegister(368);
	double J = State.GetRegister(369);
	double J1 = State.GetRegister(342);
	double W = State.GetRegister(204);
	State.SetRegister(204, 0.);
	double L = State.GetRegister(366);
	State.SetRegister(366, 0.);


	double Dtool = State.GetRegister(8000 + State.CurToolN.GetToolNum()) + State.GetRegister(108) * 2.;
	double XYdist = P * Dtool * 0.01;

// HEIDENHAIN special
	CString NCType;
	if(GCD.SGeneral.Lookup("Type", NCType))
		if(NCType.MakeUpper() == "HEIDENHAIN")
		{
			if(!State.CycleZAbs)
				Z *= -1.;

			if(State.CurCycle == MILLHOLECLEAR)
				P = 0;

			XYdist = P * Dtool / 2.;

			if(State.CurCycle == MILLCIRCINCLEAR)
			{
				U = State.GetRegister(335);
				J1 = (U - State.GetRegister(342)) / 2.;
			}
		}


	double FAC_XY_CLR = 0.;
	CString tmp;
	if(GetOtherValue("FAC_XY_CLR", tmp))
		FAC_XY_CLR = atof(tmp);

	double DRough = U - Dtool - 2. * I;
	double XStartPoint = State.MashX - 0.3 * Dtool;
	if(State.CurCycle == MILLCIRCINCLEAR)
	{
		XStartPoint = State.MashX - U / 2. + (Dtool / 2. + J1 + FAC_XY_CLR);
	}
	else
	{
		if(0.3 * Dtool > DRough / 2.)
		{
			XStartPoint = State.MashX - DRough / 2.;
		}
	}
	double YStartPoint = State.MashY;
	double ZStartPoint = R;
	if((Q == 4 || Q == 5 || Q == 14 || Q == 15) && State.CurCycle != MILLCIRCINCLEAR)
	{
		ZStartPoint = (State.CycleZAbs ? Z : R - Z) + J; 
	}
	BPoint StartPoint = BPoint(XStartPoint, YStartPoint, ZStartPoint, 1.);

	BPoint EndPoint(State.MashX, State.MashY, R + W, 1.);

	BPoint Center(State.MashX, State.MashY, State.MashZ, 1.);

	if(State.CurCycle == MILLHOLECLEAR)
	{
		Center.Set(State.GetRegister(216), State.GetRegister(217), R, 1.);
		StartPoint = Center;
		double sZ = (W != 0.) ? W : State.GetRegister(200);
		EndPoint = Center + BPoint(0., 0., sZ, 0.);
	}

	bool CCWDir = false;

	if(State.Spindle[SPINDLE_MILL].SpindleDir == R_CW && (Q >= 10 && Q <= 15) ||
		State.Spindle[SPINDLE_MILL].SpindleDir == R_CCW && (Q >= 0 && Q <= 5))
		CCWDir = true;

	bool NeedAppr = (L == 0);
	if(State.CurCycle == MILLCIRCINCLEAR || State.CurCycle == MILLHOLECLEAR)
		NeedAppr = false;

	bool NeedRough = !(Q == 4 || Q == 5 || Q == 14 || Q == 15);

	bool NeedFinish = (Q == 0 || Q == 1 || Q == 4 || Q == 5 || Q == 10 || Q == 11 || Q == 14 || Q == 15);

	double DAppr = (State.MashX - XStartPoint) * 2.;

	double DFinish = DRough + 2. * I;

	double ZdistRo = StartPoint.GetZ() - (State.CycleZAbs ? Z : R - Z);

	double ZdistFi = ZdistRo;

	if((Q == 2 || Q == 12 || Q == 4 || Q == 5 || Q == 14 || Q == 15) && State.CurCycle != MILLCIRCINCLEAR)
	{
		ZdistRo -= J;
	}

	double DZdownRo = K;

	double DZdownFi = K;
	if(!(Q == 1 || Q == 5 || Q ==11 || Q == 15))
		DZdownFi = ZdistFi;

	double DZup = 0.1;

	NMillCircInCycle MillCircInCycle(CycleGeom
		, StartPoint
		, EndPoint
		, Center
		, CCWDir
		, NeedAppr
		, NeedRough
		, NeedFinish
		, DAppr
		, DRough
		, DFinish
		, XYdist
		, ZdistRo
		, ZdistFi
		, DZdownRo
		, DZdownFi
		, DZup); 

	if(!MillCircInCycle.IsOK())
		return -1;


	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.Geom.SetE(StartPoint.GetX(), StartPoint.GetY(), State.Geom.GetE().GetZ());
	State.CompGeom = State.Geom;
	return 0;
}

int NCUnit::CrMillCircOutCycle(NCycle *pCycle)
{
	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	// U R Z Q K P I J J1 F S - modal
	// W L - non modal
	double U = State.GetRegister(223);
	double R = State.GetRegister(203) + State.CurBase.z0;
	double Z = State.GetRegister(201) + ((State.CycleZAbs) ? State.CurBase.z0 : 0.);
	int Q = int(State.GetRegister(9200));
	double K = State.GetRegister(202);
	double P = State.GetRegister(370);
	double I = State.GetRegister(368);
	double J = State.GetRegister(369);
	double J1 = State.GetRegister(342);
	double W = State.GetRegister(204);
	State.SetRegister(204, 0.);
	double L = State.GetRegister(366);
	State.SetRegister(366, 0.);

// HEIDENHAIN special
		CString NCType;
		if(GCD.SGeneral.Lookup("Type", NCType))
			if(NCType.MakeUpper() == "HEIDENHAIN")
				if(!State.CycleZAbs)
					Z *= -1.;

	if(State.CurCycle == MILLRECTCORNERINCLEAR || State.CurCycle == MILLRECTCENTERINCLEAR)
		J = J1;

	double Dtool = State.GetRegister(8000 + State.CurToolN.GetToolNum()) + State.GetRegister(108) * 2.;

	double FAC_XY_CLR = 0.;
	CString tmp;
	if(GetOtherValue("FAC_XY_CLR", tmp))
		FAC_XY_CLR = atof(tmp);

	double DRough = U + Dtool + 2. * I;
	double XStartPoint = State.MashX - U / 2. - (Dtool / 2. + J1 + FAC_XY_CLR);
	double YStartPoint = State.MashY;
	double ZStartPoint = R;
	//if((Q == 4 || Q == 5 || Q == 14 || Q == 15) && State.CurCycle != MILLCIRCINCLEAR)
	//{
	//	ZStartPoint = (State.CycleZAbs ? Z : R - Z) + J; 
	//}
	BPoint StartPoint = BPoint(XStartPoint, YStartPoint, ZStartPoint, 1.);

	BPoint EndPoint(State.MashX, State.MashY, R + W, 1.);

	BPoint Center(State.MashX, State.MashY, State.MashZ, 1.);

	bool CCWDir = false;

	if(State.Spindle[SPINDLE_MILL].SpindleDir == R_CW && (Q >= 10 && Q <= 15) ||
		State.Spindle[SPINDLE_MILL].SpindleDir == R_CCW && (Q >= 0 && Q <= 5))
		CCWDir = true;

	bool NeedAppr = (L == 0);
//	if(State.CurCycle == MILLCIRCINCLEAR)
		NeedAppr = false;

	bool NeedRough = !(Q == 4 || Q == 5 || Q == 14 || Q == 15);

	bool NeedFinish = (Q == 0 || Q == 1 || Q == 4 || Q == 5 || Q == 10 || Q == 11 || Q == 14 || Q == 15);

	double DAppr = (State.MashX - XStartPoint) * 2.;

	double DFinish = DRough - 2. * I;

	double XYdist = P * Dtool * 0.01;

	double ZdistRo = StartPoint.GetZ() - (State.CycleZAbs ? Z : R - Z);

	double ZdistFi = ZdistRo;

	//if((Q == 2 || Q == 12) && State.CurCycle != MILLCIRCINCLEAR)
	//{
	//	ZdistRo -= J;
	//}

	double DZdownRo = K;

	double DZdownFi = K;
	if(!(Q == 1 || Q == 5 || Q ==11 || Q == 15))
		DZdownFi = ZdistFi;

	double DZup = 0.1;

	NMillCircInCycle MillCircInCycle(CycleGeom
		, StartPoint
		, EndPoint
		, Center
		, CCWDir
		, NeedAppr
		, NeedRough
		, NeedFinish
		, DAppr
		, DRough
		, DFinish
		, XYdist
		, ZdistRo
		, ZdistFi
		, DZdownRo
		, DZdownFi
		, DZup); 

	if(!MillCircInCycle.IsOK())
		return -1;


	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.Geom.SetE(StartPoint.GetX(), StartPoint.GetY(), State.Geom.GetE().GetZ());
	State.CompGeom = State.Geom;
	return 0;
}

bool NCUnit::SetVal2Reg(CString & text, int RegNum)
{
	double val;
	if(!GetDValue(text, &val))
	{
		if (StoreError(1014))
			return false;
		else
			return true;
	}
	State.SetRegister(RegNum, val);
	return true;
}

bool NCUnit::GetIParamVal(CString& text, const CString& PName, int* pVal)
{
	CString title;
	if(!GCD.SWordDef.Lookup(PName, title))
	{
		*pVal = 0;
		TempError.Values[0] = PName;
		TempError.SetErrNum(1198);
		StoreError(1198);
		return false;
	}
	int ParamPos = text.Find(title);
	if(ParamPos < 0)
	{
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1199);
		StoreError(1199);
		return false;
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	if(!GetIValue(text, pVal))
	{
		text = pref + text;
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1200);
		StoreError(1200);
		return false;
	}
	text = pref + text;

	return true;
}
bool NCUnit::GetLParamVal(CString& text, const CString& PName, int* pVal)
{
	CString title;
	if(!GCD.SWordDef.Lookup(PName, title))
	{
		return false;//error
	}
	int ParamPos = text.Find(title);
	if(ParamPos < 0)
	{
		return false;//error
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	switch(GCD.LType)
	{
	case LT_BALTSYS:
	default:
		if(!GetFullLValue(text, pVal))
		{
			return false;//error	
		}
		break;
	case LT_SINUMERIK:
		{
			CString Label = text.SpanExcluding(_T(":"));
			text.Delete(0, Label.GetLength() + 1);
			GetHashNum(Label, pVal);
		}
		break;
	}
	text = pref + text;

	return true;
}
bool NCUnit::GetDParamVal(CString& text, const CString& PName, double* pVal)
{
	CString title;
	if (!GCD.SWordDef.Lookup(PName, title))
	{
		*pVal = 0;
		TempError.Values[0] = PName;
		TempError.SetErrNum(1198);
		StoreError(1198);
		return false;
	}
	int ParamPos = text.Find(title);
	if (ParamPos < 0)
	{
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1199);
		StoreError(1199);
		return false;
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	if (!GetDValue(text, pVal))
	{
		text = pref + text;
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1200);
		StoreError(1200);
		return false;
	}
	text = pref + text;
	return true;
}

bool NCUnit::GetDIParamVal(CString& text, const CString& PName, double* pVal)
{
	CString title;
	if (!GCD.SWordDef.Lookup(PName, title))
	{
		*pVal = 0;
		TempError.Values[0] = PName;
		TempError.SetErrNum(1198);
		StoreError(1198);
		return false;
	}
	int ParamPos = text.Find(title);
	if (ParamPos < 0)
	{
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1199);
		StoreError(1199);
		return false;
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	if (!GetDIValue(text, pVal))
	{
		text = pref + text;
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1200);
		StoreError(1200);
		return false;
	}
	text = pref + text;
	return true;
}

bool NCUnit::GetDParamValSoft(CString& text, const CString& PName, double* pVal)
{
	CString title;
	if (!GCD.SWordDef.Lookup(PName, title))
	{
//		*pVal = 0;
		return false;
	}
	int ParamPos = text.Find(title);
	if (ParamPos < 0)
	{
//		*pVal = 0;
		return false;
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	if (!GetDValue(text, pVal))
	{
		text = pref + text;
//		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1200);
		StoreError(1200);
		return false;
	}
	text = pref + text;
	return true;
}

bool NCUnit::GetDIParamValSoft(CString& text, const CString& PName, double* pVal)
{
	// full copy of GetDParamVal with replacing GetDValue на GetDIValue
	CString title;
	if (!GCD.SWordDef.Lookup(PName, title))
	{
		*pVal = 0;
		return false;
	}
	int ParamPos = text.Find(title);
	if (ParamPos < 0)
	{
		*pVal = 0;
		return false;
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	if (!GetDIValue(text, pVal))
	{
		text = pref + text;
		*pVal = 0;
		TempError.Values[0] = title;
		TempError.Values[1] = PName;
		TempError.SetErrNum(1200);
		StoreError(1200);
		return false;
	}
	text = pref + text;
	return true;
}

bool NCUnit::GetTParamVal(CString& text, const CString& PName, CString& Val)
{
	CString title;
	if (!GCD.SWordDef.Lookup(PName, title))
	{
		return false;//error
	}
	int ParamPos = text.Find(title);
	if (ParamPos < 0)
	{
		return false;//error
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	Val = text.SpanIncluding(_T("+-.0123456789"));
	text.Delete(0, Val.GetLength());
	text = pref + text;

	return true;
}

bool NCUnit::GetTextParamVal(CString& text, const CString& PName, CString& Val)
{
	CString title;
	bool titleOK = GCD.SWordDef.Lookup(PName, title);
	if (titleOK)
		titleOK = !title.IsEmpty();
	if (!titleOK)
	{
		if (StoreError(1180))
			return false;
		else
			return true;
	}
	
	int ParamPos = -1;
	for (int StartPos = 0; StartPos != -1 && ParamPos == -1;)// skip "" substrings
	{
		ParamPos = text.Find(title, StartPos);
		StartPos = text.Find('\"', StartPos);
		if (StartPos == -1)
			break;
		if (ParamPos > StartPos)
			ParamPos = -1;
		StartPos = text.Find('\"', StartPos + 1);
	}
	if (ParamPos < 0)
	{
		Error = 1178;// level 1
		return false;
	}
	CString pref = text.Left(ParamPos);
	text.Delete(0, ParamPos + 1);
	if (text.IsEmpty())
	{
		Error = 1186;// level 1
		return false;
	}
	if(text[0] != '\"')
	{
		Error = 1179;// level 1
		return false;
	}
	text.Delete(0, 1);
	Val = text.SpanExcluding(_T("\""));
	text.Delete(0, Val.GetLength());
	if (text.IsEmpty())
	{
		Error = 1179;// level 1
		return false;
	}
	if (text[0] != '\"')
	{
		Error = 1179;// level 1
		return false;
	}
	text.Delete(0, 1);
	text = pref + text;

	return true;
}

void NCUnitBase::RemoveComment(CString & text) const
{
	CString tmp;
	if(GCD.SWordDef.Lookup("BlockSkip", tmp))
	{
		int BlockSkipPos = text.Find(tmp);
		if(BlockSkipPos >= 0)
			text.Delete(BlockSkipPos, text.GetLength());
	}
		
	if(!GCD.SWordDef.Lookup("CommentStart", tmp))
		return;
	int CStartPos = text.Find(tmp);
	if(CStartPos < 0)
		return;

	int CEndPos = text.GetLength() - 1;
	if(GCD.SWordDef.Lookup("CommentEnd", tmp))
	{
		int Pos = text.Find(tmp);
		if(Pos > CStartPos)
			CEndPos = Pos;
	}
	text.Delete(CStartPos, CEndPos - CStartPos + 1);
	
}

int NCUnit::CrApprCycle(NCycle *pCycle)
{
	static BOOL called = FALSE;// to prevent recursive call of this proc
	if(called)
		return 0;


	NCadrGeom NextGeom = St.Geom;
	NCadrGeom CurGeom = State.Geom;
	if(!CurGeom.HaveGeom())
	{
		CurGeom.SetType(line);
		CurGeom.SetB(State.MashX, State.MashY, State.MashZ);
		CurGeom.SetE(State.MashX, State.MashY, State.MashZ);
	}
	enum LR CurDirComp;
	bool LeftCompDir;
	if(State.CurCycle == APPRLT || State.CurCycle == APPRCT || State.CurCycle == APPRLN || State.CurCycle == APPRLCT)
	{
		CycleType tmp = State.CurCycle;
		State.CurCycle = CYCLE_OFF;
		called = TRUE;
		bool res = GetNextGeom(NextGeom);
		called = FALSE;
		State.CurCycle = tmp;
		if(!res)
		{
			Error = 1106;// level 1
			return -1;
		}
		CurDirComp = State.CurDirComp;
		LeftCompDir = (CurDirComp == LEFT);
		if(State.CurDiamComp == 0.)
			LeftCompDir = false;
	}
	else // Departure
	{
		NextGeom.Reverse();
		CurDirComp = St.CurDirComp;
		if(CurDirComp == LEFT)
			CurDirComp = RIGHT;
		else if(CurDirComp == RIGHT)
			CurDirComp = LEFT;
		LeftCompDir = (CurDirComp != RIGHT);
		if(State.CurDiamComp == 0.)
			LeftCompDir = true;

		CurGeom.Reverse();
	}

	CycleGeom.RemoveAll();
	CycleGeomComp.RemoveAll();
	
	CycleGeom.AddElement(CurGeom);
	CycleGeomComp.AddElement(CurGeom);


	double CompVal = (CurDirComp == LR_OFF) ? 0. : State.CurDiamComp;

	NHHApprCycle HHApprCycle(CycleGeom, CycleGeomComp
		, State.HhLen
		, State.HhCca
		, State.HhR
		, CompVal
		, LeftCompDir
		, NextGeom
		, State.CurCycle);

	if(!HHApprCycle.IsOK())
	{
		Error = HHApprCycle.GetError();// level 1
		return -1;
	}


	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	CycleGeomComp.GetAt(CycleGeomComp.GetSize() - 1)->GetE(&State.MashcX, &State.MashcY, &State.MashcZ);
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.NonModalCycleCall = true;

	State.Geom.SetE(State.Geom.GetB());
	State.CompGeom = State.Geom;
	State.CompGeom.SetB(CycleGeomComp.GetAt(0)->GetB());
	State.CompGeom.SetE(State.CompGeom.GetB());
	return 0;
}

int NCUnit::CrRetrCycle(NCycle *pCycle)
{
	return 0;
}
bool NCUnit::ApprCycleActive(void) const
{
	return State.CurCycle == APPRLT
		|| State.CurCycle == APPRCT
		|| State.CurCycle == APPRLN
		|| State.CurCycle == APPRLCT
		|| State.CurCycle == DEPLT
		|| State.CurCycle == DEPCT
		|| State.CurCycle == DEPLN
		|| State.CurCycle == DEPLCT
		;
}

bool NCUnit::ProcessConfChange(CString& text, bool ResetMatrs)
{
	if (State.MachLink.IsIndW1())
	{// reposition of branch
		CString S, D;
		if (!GetTextParamVal(text, _T("MoveBranchSource"), S))
		{
			return false;
		}
		double SPrefix = 0.;
		if (GetDParamValSoft(text, _T("MoveBranchSourcePrefix"), &SPrefix))
		{
			CString Prefix;
			Prefix.Format(IDS_VIRT_TOOL_NUM_FORMAT, int(SPrefix));
			S = Prefix + S;
		}
		if (!GetTextParamVal(text, _T("MoveBranchDest"), D))
		{
			return false;
		}
		double DPrefix = 0.;
		if (GetDParamValSoft(text, _T("MoveBranchDestPrefix"), &DPrefix))
		{
			CString Prefix;
			Prefix.Format(IDS_VIRT_TOOL_NUM_FORMAT, int(DPrefix));
			D = Prefix + D;
		}
		if (ResetMatrs)
		{
			MTConfig& NewConf = pDoc->GetMachTool().AddNewConfig();
			if (!NewConf.MoveBranch(S, D))
			{
				TempError.Values[0] = S;
				TempError.SetErrNum(1181);
				StoreError(1181);// level 1
				return false;
			}
			State.MachLink = pDoc->GetMachTool().GetMTLink();
		}
		else
		{
			State.MachLink.SetConfInd(St.MachLink.GetConfInd() + 1);
		}
	}
	else if (State.MachLink.IsIndW2())
	{	// swap axes names
		CString F, S;
		if (!GetTextParamVal(text, _T("FirstSwapName"), F) || !GetTextParamVal(text, _T("SecondSwapName"), S))
		{
			return false;
		}
		if (ResetMatrs)
		{
			pDoc->GetMachTool().AddNewConfig();
			int Res = pDoc->GetMachTool().SwapAxisNames(F, S);
			if (0 != Res)
			{
				if(Res == -1)
					TempError.Values[0] = F;
				else if(Res == -2)
					TempError.Values[0] = S;
				TempError.SetErrNum(1181);
				StoreError(1181);// level 1
				return false;
			}
			State.MachLink = pDoc->GetMachTool().GetMTLink();
		}
		else
		{
			State.MachLink.SetConfInd(St.MachLink.GetConfInd() + 1);
		}
	}
	return true;
}

// Added because of incorrect results of original version with cyrillic
bool NCUnitBase::isdigit( char c )
{
	return	c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
			c == '5' ||	c == '6' || c == '7' || c == '8' || c == '9';
}
bool NCUnitBase::isletter( char c )
{
	return	c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' ||
			c == 'F' ||	c == 'G' || c == 'H' || c == 'I' || c == 'J' ||
			c == 'K' ||	c == 'L' || c == 'M' || c == 'N' || c == 'O' ||
			c == 'P' ||	c == 'Q' || c == 'R' || c == 'S' || c == 'T' ||
			c == 'U' ||	c == 'V' || c == 'W' || c == 'X' || c == 'Y' ||
			c == 'Z' ||	c == '_';
}
int NCUnit::ProcessCurCurvMod(void)
{
	switch(State.CurCurveMod)
	{
	case NotDefCM:
		break;
	case HhArcTang:
		{
			if(!St.Geom.HaveGeom())
			{
				Error = 1108;// level 1
				return -1;
			}
			double L = St.Geom.Length();
			if(L < 1.e-7)
			{
				Error = 1108;// level 1
				return -1;
			}
			BPoint SDir = St.Geom.GetEndDir() * (1. / L);
			BPoint SPoint(St.Geom.GetE());
			BPoint EPoint(State.MashX, State.MashY, State.MashZ, 1.);
			BPoint V = (EPoint - SPoint) * BMatr().rotg(90., BPoint(0.,0.,0.,1.), St.Geom.GetN());
			double Sp = V * SDir;
			if(fabs(Sp) < 1.e-7)
			{
				Error = 1109;// level 1
				return -1;
			}
			double t = (SPoint - (EPoint + SPoint) * 0.5) * SDir / Sp;
			BPoint C = (EPoint + SPoint) * 0.5 + V * t;
			State.Geom = St.Geom;
			State.Geom.SetType(((SDir % (EPoint - SPoint)) * St.Geom.GetN()) > 0. ? ccwarc : cwarc);
			State.Geom.SetB(SPoint);
			State.Geom.SetE(EPoint);
			State.Geom.SetI(C - SPoint);



		}
		break;
	}
	return 0;
}
BOOL NCUnit::ProcessSubrCallEnd(CString & text)
{
	cadrID p;
	int count;
	if(State.CallStack.Pop(p, count) >= 0)
	{
		State.CallStack.Push(p, count);
		if(count == State.GetCurCadr())
			return ReturnFromSub(true);
	}
	
	p = State.CadrID;
	++p.Cadr;
	if(State.CallStack.Push(p, State.GetCurCadr()) != 0)
	{
		Error = 1067;// level 1
		return FALSE;
	}
	State.OrderChanged = true;
	cadrID tmp;
	if(!FindNextCadr(text, &tmp, false))
		return FALSE; // Error defined in FindNextCadr
	int nbuf = 0;
	if(State.AutoCycleCount > 0)
		nbuf = State.AutoCycleCount;
	else
	{
		if (!text.IsEmpty())
		{
			double dnbuf = 0.;
			if(GetDIValue(text, &dnbuf))// May contain repetitions number
				nbuf = int(dnbuf + 0.5);
		}
	}
	for(int kr = 0; kr < nbuf - 1; ++kr)
	{// Push first subroutine cadr nbuf-1 times
		if(State.CallStack.Push(tmp, State.GetCurCadr()) != 0)
			return FALSE;
	}
	State.NextCadr = tmp;
	return TRUE;
}

int NCUnit::CrMillContCycle(NCycle *pCycle)
{
	// Z J R K Q V - modal
	double Z = State.GetRegister(1);
	double J = State.GetRegister(3);
	double R = State.GetRegister(5) + State.CurBase.z0;
	double K = State.GetRegister(10);
	int Q = int(State.GetRegister(15));
	double V = State.GetRegister(7) + State.CurBase.z0;

	double XStartPoint = State.MashX;
	double YStartPoint = State.MashY;
	double ZStartPoint = R;
	BPoint StartPoint = BPoint(XStartPoint, YStartPoint, ZStartPoint, 1.);

	BPoint EndPoint(State.MashX, State.MashY, V, 1.);

	bool NeedRough = true;

	bool NeedFinish = (J != 0.);

	double ZdistRo = Z;

	double DZdownRo = K * ((Z > 0.) ? 1. : -1.);
	double ZdistFi = ZdistRo;


	double DZdownFi = DZdownRo;

	double Dist = J;

	LR DirComp = LEFT;
	if(State.Spindle[SPINDLE_MILL].SpindleDir == R_CW && Q == -1. ||
		State.Spindle[SPINDLE_MILL].SpindleDir == R_CCW && Q == 1. )
		DirComp = RIGHT;
	if(Q == 0.)
		DirComp = LR_OFF;

	bool OneDirCut = (Q != 0.);

	BGeomArray ContProg;
	BGeomArray ContComp;
	BGeomArray ContRough;
	State.CurDirComp = DirComp;
	double CompValue = State.CurDiamComp;
	if(NeedRough)
	{
		State.CurDiamComp = Dist + CompValue;
		if(!MakeSubrGeom(State.ProgramNumberStore, ContRough))
			return -1;
	}
	if(NeedFinish)
	{
		State.CurDiamComp = CompValue;
		if(!MakeSubrGeom(State.ProgramNumberStore, ContComp))
			return -1;
	}
	State.CurDiamComp = 0.;
	if(!MakeSubrGeom(State.ProgramNumberStore, ContProg))
		return -1;

	State.CurDiamComp = CompValue;
	State.CurDirComp = LR_OFF;

	NMillContCycle MillContCycle(CycleGeom, CycleGeomComp
		, ContProg, ContComp, ContRough
		, StartPoint
		, EndPoint
		, NeedRough
		, NeedFinish
		, State.CurDiamComp
		, ZdistRo
		, ZdistFi
		, DZdownRo
		, DZdownFi
		, Dist
		, DirComp
		, OneDirCut); 

	if(!MillContCycle.IsOK())
		return -1;


	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	NCadrGeom *pCadr;
	pCadr = new NCadrGeom;
	pCadr->SetAttr(1);
	pCadr->SetType(fast);
	pCadr->SetB(State.Geom.GetE());
	pCadr->SetE(CycleGeom[0]->GetB());
	CycleGeom.InsertAt(0, pCadr);
	NCadrGeom *pCCadr = new NCadrGeom;
	*pCCadr = *pCadr;
	CycleGeomComp.InsertAt(0, pCCadr);


//	State.Geom.SetE(StartPoint.GetX(), StartPoint.GetY(), State.Geom.GetE().GetZ());
//	State.CompGeom = State.Geom;
	return 0;
}
class NToolCombined *NCUnit::GetActiveTool(void) const
{
	NToolCombined *pTool;
	if (!pDoc->NGetToolList()->Lookup(GetState()->CurToolN, pTool))// There is no current tool
		return nullptr;
	return pTool;
}

void NCUnit::FillRegChanges(BProgram::ProbePos::NRegChanges & RegChanges)
{
	typedef std::pair<int, double> RegPair;
	int Ind = 0;
	if ((Ind = GetRegIndexesValue("ProbeX")) != 0)
		RegChanges.push_back(RegPair(Ind, GetState()->MashXUCS));
	if ((Ind = GetRegIndexesValue("ProbeY")) != 0)
		RegChanges.push_back(RegPair(Ind, GetState()->MashYUCS));
	if ((Ind = GetRegIndexesValue("ProbeZ")) != 0)
		RegChanges.push_back(RegPair(Ind, GetState()->MashZUCS));
	if ((Ind = GetRegIndexesValue("ProbeA")) != 0)
		RegChanges.push_back(RegPair(Ind, GetState()->MashA - GetState()->CurBase.a0));
	if ((Ind = GetRegIndexesValue("ProbeB")) != 0)
		RegChanges.push_back(RegPair(Ind, GetState()->MashB - GetState()->CurBase.b0));
	if ((Ind = GetRegIndexesValue("ProbeC")) != 0)
		RegChanges.push_back(RegPair(Ind, GetState()->MashC - GetState()->CurBase.c0));
}

void NCUnit::ProcAutoFix(NCadrGeom* pGeom)
{
	if (!GCD.AutoFixRotAxes)
		return;
	bool RotMov = pGeom->IsRotate();
	bool LineMov = pGeom->HaveGeom();
	if (State.AutoFixed && RotMov)
	{
		State.AutoFixed = false;
		CString Val;
		if (GetOtherValue("AutoUnfixTime", Val))
			pGeom->SetWorkTimeS(pGeom->GetWorkTimeS() + atof(Val));
	}
	else if (!State.AutoFixed && LineMov)
	{
		State.AutoFixed = true;
		CString Val;
		if (GetOtherValue("AutoFixTime", Val))
			pGeom->SetWorkTimeS(pGeom->GetWorkTimeS() + atof(Val));
	}
}

NChannel& NCUnitBase::GetChannel()
{
	return *pDoc->GetChannel(pCommonData->ChannelInd);
}

const NChannel& NCUnitBase::GetChannel() const
{
	return *pDoc->GetChannel(pCommonData->ChannelInd);
}

bool NCUnit::StoreActiveToolStdParams(void)
{
	NToolCombined *pTool = GetActiveTool();
	if(pTool == nullptr)// There is no current tool
		return false;
	double Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height;
	pTool->GetStdParams
		(Diameter, CornerRadius, ELength, FLength, BottomAngle, SideAngle, Height);
	SetRegister(State, _T("STDDiam"), Diameter, 8101);
	SetRegister(State, _T("STDCornerRadius"), CornerRadius, 8102);
	SetRegister(State, _T("STDELength"), ELength, 8103);
	SetRegister(State, _T("STDFLength"), FLength, 8104);
	SetRegister(State, _T("STDBottomAngle"), BottomAngle, 8105);
	SetRegister(State, _T("STDSideAngle"), SideAngle, 8106);
	SetRegister(State, _T("STDHeight"), Height, 8107);
	return true;
}
bool NCUnit::GetNextDir(BPoint &NextDir)
{// Get next cadr direction
	static int Busy = 0;
	if(Busy)
		return 0;
	++Busy;

	NCUnit tmp(pDoc, *this);//NCUnit constructor
	tmp.Copy(*this);
	tmp.State.SetCurCadr(State.GetCurCadr() + 1);
	bool BCheckNext = tmp.CheckNext;
	tmp.CheckNext = false;

	int Res = tmp.EvaluateCurrent(true);

	tmp.CheckNext = BCheckNext;
	if(Res < 0)
	{
		--Busy;
		return false;
	}

	NextDir.Set(tmp.State.CurX, tmp.State.CurY, tmp.State.CurZ, 0.);

	--Busy;
	return true;
}
int NCUnit::MakeChamfer(void)
{
	static int Busy = 0;
	if(Busy || State.GetNextGeomActive)
	{
		State.Geom.Set(State.CurCurveType,
					State.MashX-State.CurX,
					State.MashY-State.CurY,
					State.MashZ-State.CurZ,
					State.MashX,
					State.MashY,
					State.MashZ,
					State.CurI,State.CurJ,State.CurK,State.CurPlane);
		return 0;
	}
	++Busy;
	BPoint Vp(-State.CurX, -State.CurY, -State.CurZ, 0.);
	NCadrGeom NextGeom;
	if(!GetNextGeom(NextGeom))
	{
		Error = 1114;// level 1
		--Busy;
		return -1;
	}
	BPoint Vn = NextGeom.GetStartDir();
	BPoint P0(State.MashX, State.MashY, State.MashZ, 1.);
	NCadrGeom Geom;
	if(!State.Chamfer.MakeGeomParams(State.Geom, NextGeom, P0, Vp, Vn, Geom, State.CurPlane)) 
	{
		Error = 1115;// level 1
		--Busy;
		return -1;
	}
	State.Coord2ChamfCoord();
	State.MashX = Geom.GetB().GetX();
	State.MashY = Geom.GetB().GetY();
	State.MashZ = Geom.GetB().GetZ();
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	State.Geom.Set(State.CurCurveType,
		State.MashX - State.CurX,
		State.MashY - State.CurY,
		State.MashZ - State.CurZ,
		State.MashX,
		State.MashY,
		State.MashZ,
		State.CurI, State.CurJ, State.CurK, State.CurPlane);

	AddState = State;

	AddState.MashX = Geom.GetE().GetX();
	AddState.MashY = Geom.GetE().GetY();
	AddState.MashZ = Geom.GetE().GetZ();
	AddState.CurX = AddState.MashX - State.MashX;
	AddState.CurY = AddState.MashY - State.MashY;
	AddState.CurZ = AddState.MashZ - State.MashZ;
	AddState.Geom = Geom;
	AddState.CurCurveType = Geom.GetType().type;

	--Busy;
	return 0;
}
int NCUnit::CrMillRectInCycle(NCycle *pCycle)
{
	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);
	// U V O R Z ,R Q J J1 K P I F S - modal
	// W L - non modal
	double U = State.GetRegister(218);
	double V = State.GetRegister(219);
	double O = State.GetRegister(224);
	double R = State.GetRegister(203) + State.CurBase.z0;
	double Z = State.GetRegister(201) + ((State.CycleZAbs) ? State.CurBase.z0 : 0.);
	double CR = State.GetRegister(220);
	int Q = int(State.GetRegister(9200));
	double J = State.GetRegister(369);
	double J1 = State.GetRegister(342);
	double K = State.GetRegister(202);
	double P = State.GetRegister(370);
	double I = State.GetRegister(368);
	double W = State.GetRegister(204);
	State.SetRegister(204, 0.);
	double L = State.GetRegister(366);
	State.SetRegister(366, 0.);


	CycleGeom.RemoveAll();
	CycleGeom.AddElement(State.Geom);

	double Dtool = State.GetRegister(8000 + State.CurToolN.GetToolNum()) + State.GetRegister(108) * 2.;

	double XYdist = P * Dtool * 0.01;

	double FAC_XY_CLR = 0.;
	CString tmp;
	if(GetOtherValue("FAC_XY_CLR", tmp))
		FAC_XY_CLR = atof(tmp);

	BPoint Center(State.MashX, State.MashY, R, 1.);
	if(State.CurCycle == MILLRECTCORNERIN || State.CurCycle == MILLRECTCORNERINCLEAR)
	{
		Center = Center + BPoint(U / 2., V / 2., 0., 1.); 
	}

	BPoint EndPoint(State.MashX, State.MashY, R + W, 1.);

// HEIDENHAIN special
	CString NCType;
	if(GCD.SGeneral.Lookup("Type", NCType))
		if(NCType.MakeUpper() == "HEIDENHAIN")
		{
			if(!State.CycleZAbs)
				Z *= -1.;

			switch(int(State.GetRegister(367)))
			{
			case 1:
				Center = Center + BPoint(U / 2., V / 2., 0., 0.);
				break;
			case 2:
				Center = Center + BPoint(- U / 2., V / 2., 0., 0.);
				break;
			case 3:
				Center = Center + BPoint(- U / 2., - V / 2., 0., 0.);
				break;
			case 4:
				Center = Center + BPoint(U / 2., - V / 2., 0., 0.);
				break;
			}

			if(State.CurCycle == MILLRECTCENTERINCLEAR)
			{
				Center.Set(State.GetRegister(216),  State.GetRegister(217), R, 1.);
				O = 0.;
			}

			if(State.CurCycle == MILLRECTCENTERINCLEAR)
				P = 0.;

			XYdist = P * Dtool / 2.;

			if(W == 0.)
				EndPoint = EndPoint + BPoint(0., 0., State.GetRegister(200), 0.);
		}

	if(State.CurCycle == MILLSLOTIN)
	{
		Center.Set(State.MashX, State.MashY, R, 1.);
		switch(int(State.GetRegister(367)))
		{
		case 1:
			Center = Center + BPoint(U / 2., 0., 0., 0.);
			break;
		case 2:
			Center = Center + BPoint(U / 2. - V / 2., 0., 0., 0.);
			break;
		case 3:
			Center = Center + BPoint(V / 2. - U / 2., 0., 0., 0.);
			break;
		case 4:
			Center = Center + BPoint(- U / 2., 0., 0., 0.);
			break;
		}
	}

	BPoint EX(1., 0., 0., 0.);
	BPoint EY(0., 1., 0., 0.);
	if(U < V)
	{
		EX.Set(0., 1., 0., 0.);
		EY.Set(-1., 0., 0., 0.);
	}
	BPoint StartPoint1 = Center - EX * (fabs(U - V) / 2.);
	BPoint StartPoint2 = Center + EX * (fabs(U - V) / 2.);

	if(U == V)
	{
		StartPoint1 = Center - EX * (0.3 * Dtool / 2.);
		StartPoint2 = Center + EX * (0.3 * Dtool / 2.);
	}


	if(State.CurCycle == MILLRECTCORNERINCLEAR || State.CurCycle == MILLRECTCENTERINCLEAR)
	{
		StartPoint1 = Center - EX * (max(U, V) / 2. - Dtool / 2. - J1);
		StartPoint2 = Center + EX * (max(U, V) / 2. - Dtool / 2. - J1);
	}
	BPoint BasePoint(State.MashX, State.MashY, R, 1.);
	BMatr M;
	M.RotZ(BasePoint, O);
	Center = Center * M;
	EX = EX * M;
	EY = EY * M;
	EndPoint = EndPoint * M;
	StartPoint1 = StartPoint1 * M;
	StartPoint2 = StartPoint2 * M;

	int ApprType = 4;
	double ApprAngle = 0;
	if(L > 0)
	{
		ApprType = 2;
		ApprAngle = L;
	}
	else if(L == 0)
		ApprType = 1;
	else
		ApprType = 4;

	if(U == V)
		ApprType = 3;

	double DXrough = max(U, V) - 2. * I - Dtool;
	double DYrough = min(U, V) - 2. * I - Dtool;
	double DXfinish = max(U, V) - Dtool;
	double DYfinish = min(U, V) - Dtool;

	bool CCWDir = false;

	if(State.Spindle[SPINDLE_MILL].SpindleDir == R_CW && (Q >= 10 && Q <= 15) ||
		State.Spindle[SPINDLE_MILL].SpindleDir == R_CCW && (Q >= 0 && Q <= 5))
		CCWDir = true;

	bool NeedAppr = true;
	if(L == -1 && U == V)
		NeedAppr = false;
	if(State.CurCycle == MILLRECTCENTERINCLEAR || State.CurCycle == MILLRECTCORNERINCLEAR)
		NeedAppr = false;

	bool NeedRough = !(Q == 4 || Q == 5 || Q == 14 || Q == 15);

	bool NeedFinish = (Q == 0 || Q == 1 || Q == 4 || Q == 5 || Q == 10 || Q == 11 || Q == 14 || Q == 15);

	double ZdistRo = StartPoint1.GetZ() - (State.CycleZAbs ? Z : R - Z);

	double ZdistFi = ZdistRo;

	if((Q == 2 || Q == 12 || Q == 4 || Q == 5 || Q == 14 || Q == 15) && !(State.CurCycle == MILLRECTCENTERINCLEAR || State.CurCycle == MILLRECTCORNERINCLEAR))
	{
		ZdistRo -= J;
	}

	double DZdownRo = K;

	double DZdownFi = K;
	if(!(Q == 1 || Q == 5 || Q == 11 || Q == 15))
		DZdownFi = ZdistFi;

	double CornerR = CR;
	if(CornerR != 0.)
		CornerR -= Dtool / 2.;

	double DZup = 0.1;

	NMillRectInCycle MillRectInCycle(CycleGeom
		, StartPoint1
		, StartPoint2
		, Center
		, EndPoint
		, EX // единичный вектор длинной стороныВектор рабочего хода
		, EY // единичный вектор короткой стороны
		, DXrough
		, DYrough
		, DXfinish
		, DYfinish
		, CCWDir
		, NeedAppr
		, NeedRough
		, NeedFinish
		, XYdist
		, ZdistRo
		, ZdistFi
		, DZdownRo
		, DZdownFi
		, CornerR
		, ApprAngle
		, ApprType
		, DZup);



	if(!MillRectInCycle.IsOK())
		return -1;


	CycleGeom.GetAt(CycleGeom.GetSize() - 1)->GetE(&State.MashX, &State.MashY, &State.MashZ);
	State.MashcX = State.MashX;
	State.MashcY = State.MashY;
	State.MashcZ = State.MashZ;
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	State.Geom.SetE(StartPoint2.GetX(), StartPoint2.GetY(), State.Geom.GetE().GetZ());
	State.CompGeom = State.Geom;
	return 0;
}

bool NCUnit::FindEndCadr(cadrID *res)
{
	*res = State.GetCurCadrID();

	CString Word;
	CString key;
	CString Curtext;
	BProgram *pProg;
	GetChannel().NGetProgramListCn()->
		GetAt(res->Prog, key, (CObject *&)pProg);
	for(int i = res->Cadr + 1; i < pProg->GetSize(); ++i)
	{
		Curtext = (*pProg)[i]->GetReText();
//		DoReplace(Curtext);
		Word = GetWord(Curtext);
// Ignore line number
// Line number (line number may contain "line number" symbol or stay as a first number in the cadr)
		if(Word == "LineNumber" ||
			(Word.GetLength() == 0 && Curtext.GetLength() > 0 && isdigit(Curtext[0])))
		{
			Word = GetWord(Curtext);
		}
		if(Word == "MiscFunctionCode")
		{
			CString key;
			if(!GetMiscKey(Curtext, key))
			{
				return false;// Error code in GetMiscKey
			}
			if(key == "SubroutineEnd" || key == "SubroutineEndCont")
			{
				res->Cadr = i;
				return true;
			}
		}
	}
	Error = 1118;// level 1
	return false;
}
BOOL NCUnit::GoToSubEndNext(void)
{
	cadrID cadr;
	if(!FindEndCadr(&cadr))
		return FALSE;
	State.OrderChanged = true;
	State.NextCadr = cadr;// + 1;
	return TRUE;
}
bool NCUnit::SetInterpChanged(void)
{
	State.ChangedCoord.interp = true;
	State.Spline.DeActivate();
	// Cancel cycle if needed
	if(State.CurCycle != CYCLE_OFF 
		&& State.CurCycle != TURNCLEAR
		&& State.CurCycle != TURNROUGH
		&& State.CurCycle != TURNLOOP
		&& State.CurCycle != TURNGROOVZ
		&& State.CurCycle != TURNGROOVX
		&& State.CurCycle != TURNTHREADAUTO
		&& State.CurCycle != TURNTHREAD
		)
		{
		// Cancel cycle by interpolation type change
			CString Iccc = "No";
			if(GetOtherValue("InterChangeCancelCycle", Iccc))
				if(Iccc == "Yes")
					if(!ProcCancelCycle())
						return false;
		}

	return true;
}

bool NCUnit::SetPolarInterp(bool On)
{
	if (State.PolarInterp.IsActive() == On)
		return true;
	if(On)
	{
		// Attach Y to C
		WordDef.SetTemp(_T("VerticalAxis"), _T("C"));// "C" coordinate (not axis) name
		BPoint CDir(0., 0., -1., 0.);
		MTAxis* pCAxis = pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(5));
		MTAxis* pZAxis = pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(2));
		if (pCAxis != nullptr && pZAxis != nullptr)
		{
			double kc = pDoc->GetMachTool().IsAnyStockAncestor(pCAxis) ? -1. : 1.;
			double kz = pDoc->GetMachTool().IsAnyStockAncestor(pZAxis) ? -1. : 1.;
			CDir *= (pCAxis->GetVector().GetZ() * pZAxis->GetVector().GetZ() * kc * kz > 0.) ? 1. : -1.;
		}
		State.PolarInterp.Activate(
			State.CurPlane
			, XY
			, BPoint(0., 0., 0., 1.)
			, BPoint(1., 0., 0., 0.)
			, BPoint(1., 0., 0., 0.)
			, CDir
			, State.CurBase
			, State.MashC);
		State.CurPlane = XY;
		State.CurBase.y0 = 0;
		if (State.PolarInterp.GetFanucStyle())
		{
			State.CurBase.c0 = State.MashC;
		}
		BSpacePos::AddPolar(State.PolarInterp);
		return true;
	}
	else
	{
		State.PolarInterp.DeActivate();
		State.CurPlane = State.PolarInterp.GetPreviousPlane();
		WordDef.RemoveTemp("C");// "C" coordinate (not axis) name
		return true;
	}

}
bool NCUnit::SetCylInterp(CString &text)
{
	double R = 0.;
	if(!GetDParamVal(text, "SpindleRotationAxis", &R))
	{
		if (StoreError(1143))
			return false;
	}
	if(R == 0.)
	{// Cancel cylindrical interpolation
		State.CurPlane = State.CylInterp.GetPreviousPlane();
		State.CylInterp.DeActivate();
		WordDef.RemoveTemp("C");// "C" coordinate (not axis) name
	}
	else
	{// Activate cylindrical interpolation
		// Attach Y to C
		WordDef.SetTemp("VerticalAxis", "C");// "C" coordinate (not axis) name
		State.CylInterp.Activate(
			State.CurPlane
			, XY
			, BPoint(0., 0., 0., 1.)
			, BPoint(1., 0., 0., 0.)
			, BPoint(1., 0., 0., 0.)
			, R
			, State.CurBase
			, State.MashC);
		State.CurPlane = YZ;
		BSpacePos::AddCyl(State.CylInterp);
	}
	return true;
}

int NCUnit::MakeCurve4CylInt(NCadrGeom & Geom)
{
	const int MAX_Otrs = 64;
	BCurve *Curves[MAX_Otrs];
	int NOtrs = min(MAX_Otrs, Geom.GetNumAppr(0.1));
	BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.), XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
	Geom.CalcCurCoord(0., XYZ0, ABC0);
	BCurve *pCurve = NULL;
	int Kp = 0;
	for(int i = 0; i < NOtrs; ++i)
	{
		Geom.CalcCurCoord(double(i + 1) / NOtrs , XYZ1, ABC1);
		const BStockToolInds STInds = GetCurSTInds();
		pCurve = pDoc->GetMachTool().Make5xLine(State.RCPMask, pDoc->GetpMUPair(), XYZ0, ABC0, XYZ1, ABC1, BPoint(0., 0., 0., 1.), STInds);
		if(pCurve)
			Kp += (pCurve->GetSize() - 1) / 3;
		Curves[i] = pCurve;
		XYZ0 = XYZ1;
		ABC0 = ABC1;
	}
	if(NOtrs > 1)
	{
		pCurve = BCurve::NewBCurve(Kp);
		for(int k = 0, rk = 1; k < NOtrs; ++k)
		{
			if(!Curves[k])
				continue;
			--rk; // First point of new curve overwrites last point of previous curve
			for(int j = 0; j < Curves[k]->GetSize(); ++j)
			{
				pCurve->SetPoint(rk, Curves[k]->GetPoint(j));
				++rk;
			}
//			delete Curves[k];
		}
	}
	Geom.SetCurve(pCurve);
	return 0;
}

NCUnit::UWordDef::UWordDef(void)// : GCD.SWordDefInv(SWDI)
{
}

NCUnit::UWordDef::~UWordDef()
{
}

const CString &NCUnit::UWordDef::GetKey(const CString &Value, const NCUnitBase& ThisUnit)
{
	if(Value.GetLength() == 1)
	{
		unsigned char key = Value[0];
		if(!WordDefInv[key].IsEmpty())
			return WordDefInv[key];
	}
	
	RKey.Empty();
	ThisUnit.GCD.SWordDefInv.Lookup(Value, RKey);
	return RKey;
}
void NCUnit::UWordDef::SetTemp(const CString &Key, const CString &Value)
{
	if(Value.GetLength() != 1)
		return;

	WordDefInv[unsigned char(Value[0])] = Key;
}
void NCUnit::UWordDef::RemoveTemp(const CString &Value)
{
	if(Value.GetLength() != 1)
		return;

	WordDefInv[unsigned char(Value[0])] = CString("");
}

void NCUnit::UWordDef::Clear(void)
{
	for(int i = 0; i < USIZE; ++i)
		WordDefInv[i].Empty();
}

void NCUnit::SetFlags(void)
{
	_int32 flags = 0x0;// Same as Curve.flags

	flags |= (State.CurCurveType == fast) ? CU_FAST : 0x0;
	flags |= (State.CurCycle == CYCLE_OFF) ? 0x0 : CU_CYCL;
	flags |= (State.Coolant == COOLOFF) ? 0x0 : CU_COOL;
	flags |= (State.Spindle[SPINDLE_MILL].SpindleDir != R_UNDEF) ? CU_SPMI : 0x0;
	flags |= (State.Spindle[SPINDLE_TURN].SpindleDir != R_UNDEF) ? CU_SPTU : 0x0;
	flags |= (State.CallStack.IsEmpty()) ? 0x0 : CU_SUBP;
	flags |= (State.CurDirComp == LR_OFF) ? 0x0 : CU_DCOM;
	flags |= (State.AddCoordsChanged) ? CU_ADDC : 0x0;
	flags |= (State.Spline.IsActive()) ? CU_SPLN : 0x0;
	flags |= (State.ProbeCadr) ? CU_PRBE : 0x0;
	flags |= (State.TouchCadr) ? CU_TOUC : 0x0;
	flags |= (State.MachLink != St.MachLink) ? CU_CONF : 0x0;
	flags |= ((State.RCPMask & 0x1) ? CU_RCP0 : 0) + ((State.RCPMask & 0x2) ? CU_RCP1 : 0) + ((State.RCPMask & 0x4) ? CU_RCP2 : 0);
	if (State.CurZCompChanging)
		flags |= (State.CurZComp == UD_OFF) ? CU_LCOMP_BEG : CU_LCOMP_END;
	else
		flags |= (State.CurZComp == UD_OFF) ? 0x0 : CU_LCOMP_BEG | CU_LCOMP_END;

	if(State.GrooveON)
		flags |= CU_GROV;
	else 
		flags &= ~CU_GROV;
	State.Geom.SetFlags(flags);
	State.CompGeom.SetFlags(flags);
	State.AddCompGeom.SetFlags(flags);
	AddState.Geom.SetFlags(flags);
	AddState.CompGeom.SetFlags(flags);
	AddState.AddCompGeom.SetFlags(flags);
}
NCUnitBase::CompType NCUnit::GetCompType(void)
{
	CString Val;
	if(GCD.SOthers.Lookup("CompType", Val))
	{
		if(Val == "Arc")
			return Arc;
		if(Val == "Int")
			return Int;
		if(Val == "InternalInt")
			return InternalInt;
		return Default;
	}
	if(GCD.SOthers.Lookup("CompWithArcs", Val))
	{
		if(Val == "Yes")
			return Arc;
	}
	if(GCD.SOthers.Lookup("CompToIntersect", Val))
	{
		if(Val == "Yes")
			return Int;
	}
	return Default;
}

bool NCUnit::SetLimit(int Num, TravelLimit TL, CString &text)
{
	double val = 0.;
	if(!GetDValue(text, &val))
	{
		if (StoreError(1029))
			return false;
		else
			return true;
	}
	switch(TL)
	{
	case TL_POSITIVE:
		State.GetCoordLimits().SetMax(Num, val);
		break;
	case TL_NEGATIVE:
		State.GetCoordLimits().SetMin(Num, val);
		break;
	case TL_ACTIVATE:
		State.GetCoordLimits().SetActive(Num, true);
		break;
	}
	State.TLFlag = TL_EMPTY;
	return true;
}

bool NCUnit::CheckTravelLimits()
{
	NCoordLimits &CL = State.GetCoordLimits();
	if(State.TLFlag != TL_EMPTY)
	{
		for(int i = 0; i < LIMITS_NUM; ++i)
		{
			switch(State.TLFlag)
			{
			case TL_POSITIVE:
				CL.SetMax(i, StartState.GetCoordLimits().GetMax(i));
				break;
			case TL_NEGATIVE:
				CL.SetMin(i, StartState.GetCoordLimits().GetMin(i));
				break;
			case TL_ACTIVATE:
				CL.SetActive(i, false);
				break;
			}
		}
	}
	State.TLFlag = TL_EMPTY;
	if(!CL.IsValid(0, State.MashcX))
	{
		if (StoreError(1120))
			return false;
	}
	if(!CL.IsValid(1, State.MashcY))
	{
		if (StoreError(1121))
			return false;
	}
	if(!CL.IsValid(2, State.MashcZ))
	{
		if (StoreError(1122))
			return false;
	}
	if(!CL.IsValid(3, State.MashA))
	{
		if (StoreError(1123))
			return false;
	}
	if(!CL.IsValid(4, State.MashB))
	{
		if (StoreError(1124))
			return false;
	}
	if(!CL.IsValid(5, State.MashC))
	{
		if (StoreError(1125))
			return false;
	}
	return true;
}

bool NCUnit::SetCurCycle(const CString &CycleDef, CycleType Type, CString &text)
{
	if( !State.SetCurCycle(Type) )
	{
		if (StoreError(1078))
			return false;
	}
	// Process "(,,)" list
	if(!ProcParenthesesList(CycleDef, text))
		return false;
	return true;
}

bool NCUnit::ProcParenthesesList(const CString &CycleDef, CString &text)
{
	CString val;
	if(!GetOtherValue(CycleDef, val))
		return true;
	if(val.GetLength() < 3)
	{
		Error = 1126;// level 1
		return false;
	}
	char First = val.GetAt(0);
	char Last = val.GetAt(val.GetLength() - 1);
	if(text.GetLength() < 3)
	{
		Error = 1127;// level 1
		return false;
	}
	if(text.GetAt(0) != First)
	{
		Error = 1127;// level 1
		return false;
	}
	CString NewText;
	while(val.GetLength() > 1)
	{
		val.Delete(0);
		text.Delete(0);
		if(text.IsEmpty())
		{
			Error = 1128;// level 1
			return false;
		}
		CString ExStr(CString(_T(",")) + CString(Last));
		CString ParName = val.SpanExcluding(ExStr);
		val.Delete(0, ParName.GetLength());
		CString ParValue = text.SpanExcluding(ExStr);
		text.Delete(0, ParValue.GetLength());
		if(ParValue.IsEmpty() || ParName.IsEmpty())
			continue;
		NewText = NewText + ParName + ParValue;
	}
	if(text.GetAt(0) != Last)
	{
		Error = 1128;// level 1
		return false;
	}
	text.Delete(0);
	text = NewText + text;
	
	return true;
}

bool NCUnit::IsAddCoordWord(CString &CurWord) const
{
	const CString AddCoord(_T("AddCoord"));
	const int AddLength = AddCoord.GetLength();
	int CurLength = CurWord.GetLength();
	if(CurLength < AddLength + 2)
		return false;

	for(int i = 0; i < AddLength; ++i)
		if(CurWord[i] != AddCoord[i])
			return false;

	if(!isdigit(CurWord[AddLength]))
			return false;
	if(!isdigit(CurWord[AddLength + 1]))
			return false;

	CurWord.Delete(0, AddLength);

	return true;
}

bool NCUnit::ProcessAddCoord(CString &text, CString &CurWord)
{
	// CurWord has 2 digits in the first 2 positions
	// it may have "Inc" suffix after digits
	if(CurWord.GetLength() < 2)
	{
		Error = 1139;// level 1
		return false;
	}
	CString NumStr = CurWord.Left(2);
	CurWord.Delete(0, 2);
	bool Increment = false;
	if(CurWord == _T("Inc"))
	{
		Increment = true;
		CurWord.Delete(0, 3);
	}
	if(!CurWord.IsEmpty())
	{
		Error = 1139;// level 1
		return false;
	}

	double Val = 0;
	if(!GetDValue(text, &Val))
	{
		if (StoreError(1138))
			return false;
	}

	int Ind = atoi(NumStr);
	if(Ind < 0 || Ind >= AddCoordsScale.GetSize())
	{
		Error = 1139;// level 1
		return false;
	}

	Val *= AddCoordsScale.GetCoord(Ind);

	if(Increment)
		Val += State.AddCoords.GetCoord(Ind);

	State.AddCoords.SetCoord(Ind, Val);
	State.AddCoordsFlags.SetCoord(Ind, Increment ? 2. : 1.);// 1. - fixed, 2. - non fixed, 0. - not changed
	State.AddCoordsChanged = true;

	return true;
}
bool NCUnit::ProcessRCPOn(CString& text)
{
	State.RCPActive = true;
	State.RCPStartABC.Set(State.MashA, State.MashB, State.MashC, 1.);
	State.RCPcX = 0.;
	State.RCPcY = 0.;
	State.RCPcZ = 0.;
	double DVal = 0.;
	if (GetDIParamValSoft(text, _T("RCPArg"), &DVal))
	{
		State.RCPMask = unsigned int(DVal + 0.5);
	}
	else
	{
		CString SVal;
		if (GetOtherValue(_T("RCPCode"), SVal))
			State.RCPMask = atoi(SVal);
		else
			State.RCPMask = 7;
	}
	if (State.RCPMask > 7)
	{
		const MTMachineTool& Mach = pDoc->GetMachTool();
		int iA = Mach.GetCurConfig().GetAxisIndex(FixedDef.GetName(3));
		int iB = Mach.GetCurConfig().GetAxisIndex(FixedDef.GetName(4));
		int iC = Mach.GetCurConfig().GetAxisIndex(FixedDef.GetName(5));
		bool ToolOnly = (State.RCPMask == 10);
		bool StockOnly = (State.RCPMask == 11);
		if (!ToolOnly && !StockOnly)
		{
			State.RCPMask = 7;
			if (StoreError(1212))
				return false;
			else
				return true;
		}
		const MTConfig& Conf = Mach.GetCurConfig();
		int iT = Conf.GetAxisIndex(Mach.GetCurConfig().GetToolAxisName(State.CurToolN.GetTurretNum(), State.CurToolN.GetToolNum()));
		const MTAxis* pToolAxis = Conf.GetAxis(iT);
		int iS = Conf.GetAxisIndex(Mach.GetStockMovedAxisName(GetCurStockPosition()));
		const MTAxis* pStockAxis = Conf.GetAxis(iS);
		if (pToolAxis == nullptr || pStockAxis == nullptr)
		{
			State.RCPMask = 7;
			return true;
		}
		State.RCPMask = 0;
		if (iA >= 0)
		{
			if (Conf.IsDescendant(*(Conf.GetAxis(iA)), *pToolAxis))
				State.RCPMask |= ToolOnly ? 1 : 0;
			else
				State.RCPMask |= StockOnly ? 1 : 0;
		}
		if (iB >= 0)
		{
			if (Conf.IsDescendant(*(Conf.GetAxis(iB)), *pToolAxis))
				State.RCPMask |= ToolOnly ? 2 : 0;
			else
				State.RCPMask |= StockOnly ? 2 : 0;
		}
		if (iC >= 0)
		{
			if (Conf.IsDescendant(*(Conf.GetAxis(iC)), *pToolAxis))
				State.RCPMask |= ToolOnly ? 4 : 0;
			else
				State.RCPMask |= StockOnly ? 4 : 0;
		}
	}
	return true;
}
double NCUnit::GetThreadPitch(double ThreadNum)
{
	if(ThreadNum < 3 || ThreadNum > 60)
		return 0.;
	return ThreadNum * 0.1;
}

void NCUnit::State2StartState()
{
	StartState = State;
	StartState.EndProgram = false;
	StartState.SetCurCadr(0);
	StartState.ClearCurHCadr();
	StartState.ClearCurHCadrAbs();
	StartState.SetCurProg(_T("Remote"));
	State.ProgramStarted = false;
	State.CadrID.Prog = 0;
}

//void NCUnit::FillWeldCadrs(NProgramWeld *pProgWeld, BGeomArray *pGeomArray)
//{
//	pProgWeld->FillWeldCadrs(St, State, *pGeomArray);
//}

void NCUnit::JumpHere(void)
{
	cadrID id = GetState()->GetCurCadrIDEx();
	bool MS = pDoc->MileStones.IsActive();
	pDoc->MileStones.DeActivate();
	JumpForToHCadr(id);	
	if(MS)
		pDoc->MileStones.Activate();
}

BOOL NCUnit::ProcessTiltPlane(CString &text, bool AnglesCase)
{
	bool IncAngleDefined = false;
	bool Reset = false;
	double A = 0.;
	double B = 0.;
	double C = 0.;
	double Ai = 0.;
	double Bi = 0.;
	double Ci = 0.;
	BPoint XVec(1., 0., 0., 0.);
	BPoint ZVec(0., 0., 1., 0.);
	if (AnglesCase)
	{
		bool AbsAngleDefined = false;
		AbsAngleDefined |= GetDParamValSoft(text, _T("TiltA"), &A);
		AbsAngleDefined |= GetDParamValSoft(text, _T("TiltB"), &B);
		AbsAngleDefined |= GetDParamValSoft(text, _T("TiltC"), &C);

		IncAngleDefined |= GetDParamValSoft(text, _T("TiltAI"), &Ai);
		IncAngleDefined |= GetDParamValSoft(text, _T("TiltBI"), &Bi);
		IncAngleDefined |= GetDParamValSoft(text, _T("TiltCI"), &Ci);
		if (AbsAngleDefined && IncAngleDefined)
		{
			Error = 1145;// level ???
			return FALSE;
		}
		Reset = !AbsAngleDefined && !IncAngleDefined;
	}
	else
	{ // vectors case
		double X0 = 0.;
		bool X0Defined = GetDParamValSoft(text, _T("TiltX0"), &X0);
		double X1 = 0.;
		bool X1Defined = GetDParamValSoft(text, _T("TiltX1"), &X1);
		double X2 = 0.;
		bool X2Defined = GetDParamValSoft(text, _T("TiltX2"), &X2);

		bool IncAngleDefined = false;
		double Z0 = 0.;
		bool Z0Defined = GetDParamValSoft(text, _T("TiltZ0"), &Z0);
		double Z1 = 0.;
		bool Z1Defined = GetDParamValSoft(text, _T("TiltZ1"), &Z1);
		double Z2 = 0.;
		bool Z2Defined = GetDParamValSoft(text, _T("TiltZ2"), &Z2);
		if (!X0Defined && !X1Defined && !X2Defined && !Z0Defined && !Z1Defined && !Z2Defined)
			Reset = true;// no coordinates present
		else if (!X0Defined || !X1Defined || !X2Defined || !Z0Defined || !Z1Defined || !Z2Defined)
		{// not all coordinates present
			Error = 1209;// level 1 // new error // not all coordinates present
			return FALSE;
		}
		XVec.Set(X0, X1, X2, 0.);
		ZVec.Set(Z0, Z1, Z2, 0.);
	}

	double Buf = 27;// 27 (XYZ), 57 (ZYX) кодирование по 2 бита
	GetDParamValSoft(text, _T("TiltSecCode"), &Buf);
	int SecCode = int(Buf + 0.5);
	Buf = 1.;
	GetDParamValSoft(text, _T("TiltDecision"), &Buf);
	int Decision = int(Buf + 0.5);

	double Reg = -1.;
	if (GetDParamValSoft(text, _T("TiltAReg"), &Reg))
	{
		TiltPlane.SetStartRegNum(int(Reg));
	}

	ErrorMinor = 0;// clear errors established in GetDParamVal
	if (Error != 0)
		return FALSE;


	double TiltType = 0.;
	if (!Reset)
	{
		if (!GetDParamVal(text, _T("TiltType"), &TiltType))
		{
			if (Error != 0)
				return FALSE;
		}
		TiltPlane.SetActive(true);
	}
	else
	{
		RemoveCSMatr();
		State.CurBase = StartState.UCSs[State.ActiveUCS];
		TiltPlane.Reset();
		return TRUE;
	}
	// Calculate new coord system in the stock space
	if (AnglesCase)
	{
		BMatr OldSystem;
		OldSystem.SetE();
		if (IncAngleDefined)
		{
			OldSystem = State.TiltNewSys;
			A = Ai;
			B = Bi;
			C = Ci;
		}
		BPoint NullPoint = State.CurBase.GetBPoint() * State.TiltNewSys;
		State.TiltNewSys = OldSystem * BMatr().Trans(BPoint(0., 0., 0., 1), NullPoint);
		// Make rotation matr
		for (int i = 0; i < 3; ++i)
		{
			switch ((SecCode >> 2 * (2 - i)) & 3)
			{
			case 1:
				State.TiltNewSys = State.TiltNewSys * BMatr().RotX(NullPoint, A);
				break;
			case 2:
				State.TiltNewSys = State.TiltNewSys * BMatr().RotY(NullPoint, B);
				break;
			case 3:
				State.TiltNewSys = State.TiltNewSys * BMatr().RotZ(NullPoint, C);
				break;
			}
		}
	}
	else
	{

		if (fabs(XVec.D2() - 1.) > MIND || fabs(ZVec.D2() - 1.) > MIND)
		{
			XVec.Unit();
			ZVec.Unit();
			CString Format(_T("%10.4lf"));
			TempError.Values[0].Format(Format, XVec.GetX());
			TempError.Values[1].Format(Format, XVec.GetY());
			TempError.Values[2].Format(Format, XVec.GetZ());
			TempError.Values[3].Format(Format, ZVec.GetX());
			TempError.Values[4].Format(Format, ZVec.GetY());
			TempError.Values[5].Format(Format, ZVec.GetZ());
			TempError.SetErrNum(1208);
			StoreError(1208);
		}
		else
		{
			XVec.Unit();
			ZVec.Unit();
		}
		// check angle
		if(fabs(fabs(XVec.Angle0_180(ZVec)) - 90.) > MINAD)
		{// vectors are not orthogonal
			Error = 1210;// level 1 // new error // vectors are not orthogonal
			return FALSE;
		}
		BPoint YVec = ZVec % XVec;
		State.TiltNewSys.SetRow(0, XVec);
		State.TiltNewSys.SetRow(1, YVec);
		State.TiltNewSys.SetRow(2, ZVec);
		State.TiltNewSys.SetRow(3, BPoint(0., 0., 0., 1.));
	}
	// Set the machine tool to original position of controlled axes and current position of the other axis
	double BufA = State.MashA;
	double BufB = State.MashB;
	double BufC = State.MashC;
	// Set axes names
	CString AName1, AName2;
	RCoordPair Axes = State.PlaceToolAxes;
	if (pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(3)) == NULL)
		Axes = RCP_BC;
	if (pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(4)) == NULL)
		Axes = RCP_AC;
	if (pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(5)) == NULL)
		Axes = RCP_AB;
	switch (Axes)
	{
	case RCP_AB:
		AName1 = GetFixedDef().GetName(3);
		AName2 = GetFixedDef().GetName(4);
		State.MashA = 0.;
		State.MashB = 0.;
		break;
	case RCP_BC:
		AName1 = GetFixedDef().GetName(4);
		AName2 = GetFixedDef().GetName(5);
		State.MashB = 0.;
		State.MashC = 0.;
		break;
	case RCP_AC:
	default:
		AName1 = GetFixedDef().GetName(3);
		AName2 = GetFixedDef().GetName(5);
		State.MashA = 0.;
		State.MashC = 0.;
		break;
	}
	pDoc->GetMachTool().ProcCadrStep(*this, State);
	if (!TiltPlane.CalculateParams(pDoc->GetpMUPair(), State.TiltNewSys, AName1, AName2, 0, 0, Decision))// Ins TSInds
	{
		Error = 1146;// level 1
		return FALSE;
	}
	// Restore machine tool state
	State.MashA = BufA;
	State.MashB = BufB;
	State.MashC = BufC;
	pDoc->GetMachTool().ProcCadrStep(*this, State);

	switch (int(TiltType))
	{
	case 0:
		TiltPlane.SetChanging(false);
		{
			double ResA = 0., ResB = 0., ResC = 0.;
			TiltPlane.GetResAngles(GetFixedDef(), &ResA, &ResB, &ResC);
			int Reg = -1;
			if ((Reg = TiltPlane.GetStartRegNum()) >= 0)
			{
				State.SetRegister(Reg, TiltPlane.GetCSMatr().GetRow(3).GetX());
				State.SetRegister(Reg + 1, TiltPlane.GetCSMatr().GetRow(3).GetY());
				State.SetRegister(Reg + 2, TiltPlane.GetCSMatr().GetRow(3).GetZ());
				State.SetRegister(Reg + 3, ResA);
				State.SetRegister(Reg + 4, ResB);
				State.SetRegister(Reg + 5, ResC);
			}
		}
		break;
	case 1:
	case 2:
		TiltPlane.SetChanging(true);
		//		TiltPlane.GetResAngles(GetFixedDef(), &State.MashA, &State.MashB, &State.MashC);
		break;
	}
	if (AnglesCase)
		State.FavorConcepts.AddConcepts(
			CString("TiltA\nTiltB\nTiltC\nTiltAI\nTiltBI\nTiltCI\nTiltSecCode\nTiltDecision\nTiltAReg\nTiltType"), GCD.SWordDef, false);
	else
		State.FavorConcepts.AddConcepts(
			CString("TiltX0\nTiltX1\nTiltX2\nTiltZ0\nTiltZ1\nTiltZ2\nTiltSecCode\nTiltDecision\nTiltAReg\nTiltType"), GCD.SWordDef, false);


	// Calculate CSMatr
	//State.CSMatr = TiltPlane.GetCSMatr();
//	State.MoveRotateCS = TRUE;
	return TRUE;
}

BOOL NCUnit::ProcessCreateMTool(CString& text)
{
	NAutoToolGenParams& tmp = AutoToolGenParams;
	tmp.Clear();
	double buf;
	GetDParamVal(text, _T("CrToolType"), &tmp.CrToolType);
	buf = 0;
	if (GetDParamVal(text, _T("CrToolCode"), &buf))
		tmp.CrToolCode = int(buf + 0.5);
	buf = 0;
	if (GetDParamVal(text, _T("CrAxisNum"), &buf))
		tmp.CrAxisNum = int(buf + 0.5);
	buf = 0;
	if (GetDParamVal(text, _T("CrTurretNum"), &buf))
		tmp.CrTurretNum = int(buf + 0.5);
	GetDParamVal(text, _T("CrMillDiameter"), &tmp.CrMillDiameter);
	GetDParamVal(text, _T("CrMillHeight"), &tmp.CrMillHeight);
	GetDParamVal(text, _T("CrMillRadius"), &tmp.CrMillRadius);
	GetDParamVal(text, _T("CrMillAngle"), &tmp.CrMillAngle);
	GetDParamVal(text, _T("CrMillLength"), &tmp.CrMillLength);
	GetDParamVal(text, _T("CrShankD"), &tmp.CrShankD);
	GetDParamVal(text, _T("CrShankH"), &tmp.CrShankH);
	GetDParamVal(text, _T("CrHolderD"), &tmp.CrHolderD);
	GetDParamVal(text, _T("CrHolderH"), &tmp.CrHolderH);

	ErrorMinor = 0;// clear errors established in GetDParamVal
	Error = 0;

	tmp.AToolID.Set(tmp.CrToolCode, tmp.CrTurretNum);
	tmp.Cool = true;
	return TRUE;
}

bool NCUnit::PlaceTool(const BPoint &P, const BPoint &N)
{
	RCoordPair Axes = State.PlaceToolAxes;
	if(pDoc->GetMachTool().GetCurConfig().GetAxis(FixedDef.GetName(3)) == NULL)
		Axes = RCP_BC;
	if(pDoc->GetMachTool().GetCurConfig().GetAxis(FixedDef.GetName(4)) == NULL)
		Axes = RCP_AC;
	if(pDoc->GetMachTool().GetCurConfig().GetAxis(FixedDef.GetName(5)) == NULL)
		Axes = RCP_AB;
	SMachState NewMachState(&FixedDef);
	if(!CalcPlaceTool(NewMachState
		, P
		, N
		, Axes
		, 1
		, true))
		return false;
	State.CurX = NewMachState.GetCoord(0) - State.MashX;
	State.CurY = NewMachState.GetCoord(1) - State.MashY;
	State.CurZ = NewMachState.GetCoord(2) - State.MashZ;
	State.MashX += State.CurX;
	State.MashcX = State.MashX;
	State.MashcY = State.MashY = NewMachState.GetCoord(1);
	State.MashcZ = State.MashZ = NewMachState.GetCoord(2);
	State.MashA = NewMachState.GetCoord(3);
	State.MashB = NewMachState.GetCoord(4);
	State.MashC = NewMachState.GetCoord(5);
	return true;
}

int NCUnit::PlaceTool(const BMatr &CS, int Solution)
{
	std::vector<SMachState*> NewMachState;
	int Res = CalcPlaceTool(NewMachState, CS);
	if(Res <= 0)
		return Res;
	if(Solution >= Res)
		Solution = 0;
	State.CurX = NewMachState[Solution]->GetCoord(0) - State.MashX;
	State.CurY = NewMachState[Solution]->GetCoord(1) - State.MashY;
	State.CurZ = NewMachState[Solution]->GetCoord(2) - State.MashZ;
	State.MashX += State.CurX;
	State.MashcX = State.MashX;
	State.MashcY = State.MashY = NewMachState[Solution]->GetCoord(1);
	State.MashcZ = State.MashZ = NewMachState[Solution]->GetCoord(2);
	State.MashA = NewMachState[Solution]->GetCoord(3);
	State.MashB = NewMachState[Solution]->GetCoord(4);
	State.MashC = NewMachState[Solution]->GetCoord(5);
	return Res;
}

bool NCUnit::ProcessPlaceTool(void)
{
	PlaceTool( BPoint(State.MashX, State.MashY, State.MashZ, 1.)
			, BPoint(State.CurI, State.CurJ, State.CurK, 0.));
	return true;
}

bool NCUnit::CalcPlaceTool(SMachState &Result, const BPoint &ToolPos, const BPoint &ToolDir, RCoordPair Axes,
		int Decision/* = 1*/, bool PreferRot/* = true*/, int Type/* = 0*/)
{
	// Type: 0 - absolute in stock UCS, 1 - relative angles, 2 - relative dist, 3 - absolute in current UCS

	// ToolPos and Tool dir should be given in the stock coordinate system
	NToolID ToolID;
	ToolID.Set(State.CurToolN.GetToolNum(), State.CurTurret);
	NToolCombined *pTool = pDoc->GetTool(ToolID);
	int Turret = 0;
	int Attach = 0;
	if(pTool)
	{
		Turret = pTool->Turret;
		Attach = pTool->Attach;
	}
	pDoc->GetMachTool().MakeActual();
	NTiltPlane LocTiltPlane;
	LocTiltPlane.SetActive(true);
	SMachState OriginalState(pDoc->GetMachTool().GetFixState(&GetFixedDef()));
	const BMTLinkage OriginalConfNum = pDoc->GetMachTool().GetMTLink();
	LocTiltPlane.FixState = OriginalState;
	BMatr PrevS2T = pDoc->GetMachTool().GetStock2ToolMatr(0, Turret, Attach);
	pDoc->GetMachTool().Reset(*Result.GetDef());
	BPoint NewToolDir(ToolDir);
	BPoint CorrToolPos(ToolPos);
	if(Type != 0)
	{
		BPoint PrevToolDir = BPoint(0., 0., 1., 0.) * PrevS2T;
		CorrToolPos = PrevS2T.GetRow(3) - PrevToolDir * State.ElectrT1;
		if(Type == 1)
		{
			BPoint Y(0., 1., 0., 0.);
			NewToolDir = PrevToolDir
				* BMatr().rotg(ToolDir.GetX(), BPoint(0., 0., 0., 1.), PrevToolDir % (Y % PrevToolDir))
				* BMatr().rotg(ToolDir.GetZ(), BPoint(0., 0., 0., 1.), PrevToolDir);
		}
		else if(Type == 2)
		{
			NewToolDir = PrevToolDir;
			Decision = 0;
			CorrToolPos = CorrToolPos + PrevToolDir * ToolDir.GetZ();
		}
		else if(Type == 3)
		{
			CorrToolPos = ToolPos + State.GetActiveUCS().GetBPoint() - BPoint(0., 0., 0., 1.);
		}
	}
	double d = sqrt(NewToolDir.D2());
	if(d < MIND)
		NewToolDir.Set(0., 0., 1., 0.);
	else
		NewToolDir = NewToolDir * (1. / d);
	CorrToolPos = CorrToolPos + NewToolDir * State.ElectrT1;

	BPoint OldToolDir = BPoint(0., 0., 1., 0.) * pDoc->GetMachTool().GetStock2ToolMatr(0, Turret, Attach);
	BMatr NewSystem;
	NewSystem.e0cong(OldToolDir, NewToolDir);
	NewSystem.SetRow(3, CorrToolPos);
	
	CString AName1, AName2;
	switch(Axes)
	{
	case RCP_AB:
		AName1 = GetFixedDef().GetName(3);
		AName2 = GetFixedDef().GetName(4);
		break;
	case RCP_BC:
	default:
		AName1 = GetFixedDef().GetName(4);
		AName2 = GetFixedDef().GetName(5);
		break;
	case RCP_AC:
		AName1 = GetFixedDef().GetName(3);
		AName2 = GetFixedDef().GetName(5);
		break;
	}
	if(!LocTiltPlane.CalculateParams(pDoc->GetpMUPair(),
		NewSystem, AName1, AName2, Turret, Attach, Decision))
		return false;
	Result = LocTiltPlane.GetFixState();
	pDoc->GetMachTool().SetState(OriginalConfNum, OriginalState);
	return true;
}

int NCUnit::CalcPlaceTool(std::vector<SMachState*> &Result, const BMatr &CS)
{
	// Type: 0 - absolute in stock UCS, 1 - relative angles, 2 - relative dist, 3 - absolute in current UCS

	// ToolPos and Tool dir should be given in the stock coordinate system
	NToolID ToolID;
	ToolID.Set(State.CurToolN.GetToolNum(), State.CurTurret);
	NToolCombined *pTool = pDoc->GetTool(ToolID);
	int Turret = 0;
	int Attach = 0;
	if(pTool)
	{
		Turret = pTool->Turret;
		Attach = pTool->Attach;
	}
	pDoc->GetMachTool().MTBase::Reset();
	pDoc->GetMachTool().PositionTool2CS(CS, GetCurStockPosition(), Turret, Attach, Result, GetFixedDef());
	return int(Result.size());
}

const CString NCUnit::MakePointString(const BPoint &P, const BPoint &N, const CString &text)
{
	// Text format X<Px>Y<Py>Z<Pz>I<Vx>J<Vy>K<Vz>L<AxesPair>W<Decision>U<PreferRot>V<CS>
	double X, Y, Z, I, J, K;
	int L, W, U, V;
	// V - 0 - absolute in stock UCS, 1 - relative angles, 2 - relative dist, 3 - absolute in current UCS
	if(sscanf(text, "X%lfY%lfZ%lfI%lfJ%lfK%lfL%dW%dU%dV%d", &X, &Y, &Z, &I, &J, &K, &L, &W, &U, &V)
		!= 10)
		return  _T("");
	BPoint Pc(P);
	if(V == 3)
		Pc = Pc - State.GetActiveUCS().GetBPoint();

	CString PointString;
	PointString.Format("X%lfY%lfZ%lfI%lfJ%lfK%lf", 
		Pc.GetX(), Pc.GetY(), Pc.GetZ(), 
		N.GetX(), N.GetY(), N.GetZ());
	return PointString;
}
const CString NCUnit::MakePlaceString(const CString &text)
{
	// Text format X<Px>Y<Py>Z<Pz>I<Vx>J<Vy>K<Vz>L<AxesPair>W<Decision>U<PreferRot>V<CS>
	CString CadrStr;
	double X, Y, Z, I, J, K;
	int L, W, U, V;
	// V - 0 - absolute in stock UCS, 1 - relative angles, 2 - relative dist, 3 - absolute in current UCS
	if(sscanf(text, "X%lfY%lfZ%lfI%lfJ%lfK%lfL%dW%dU%dV%d", &X, &Y, &Z, &I, &J, &K, &L, &W, &U, &V)
		!= 10)
		return  _T("");
	SMachState Result(&GetFixedDef());
	RCoordPair Axes;
	switch(L)
	{
	case 1:
		Axes = RCP_AB;
		break;
	case 2:
	default:
		Axes = RCP_BC;
		break;
	case 3:
		Axes = RCP_AC;
		break;

	}
	if(!CalcPlaceTool(Result
			, BPoint(X, Y, Z, 1.)
			, BPoint(I, J, K, 0.)
			, Axes
			, W
			, (U != 0)
			, V
			))
		return _T("");
	
	return MakeCadr(Result);
}
const CString NCUnit::MakeCadr(const SMachState &MState) const
{
//	MState is fixed state
	CString CadrStr;
	for(int i = 0; i < MState.GetSize() - 1; ++i)
	{
		CString Name = MState.GetAName(i);
		double Base = 0.;
		double Scale = 1.;
		switch(i)
		{
		case 0:
			Base = State.GetActiveUCS().x0;
			Scale = 1. / (GCD.LinearScale * GCD.XScale * State.CurXScale);
			break;
		case 1:
			Base = State.GetActiveUCS().y0;
			Scale = 1. / (GCD.LinearScale * GCD.YScale * State.CurYScale);
			break;
		case 2:
			Base = State.GetActiveUCS().z0;
			Scale = 1. / (GCD.LinearScale * GCD.ZScale * State.CurZScale);
			break;
		case 3:
			Base = State.GetActiveUCS().a0;
			Scale = 1. / GCD.RotaryScale;
			break;
		case 4:
			Base = State.GetActiveUCS().b0;
			Scale = 1. / GCD.RotaryScale;
			break;
		case 5:
			Base = State.GetActiveUCS().c0;
			Scale = 1. / GCD.RotaryScale;
			break;
		}
		CString Val;
		Val.Format("%lf", MState.GetCoord(i) * Scale - Base);
		CadrStr = CadrStr + Name + Val;
	}
	return CadrStr;
}

void NCUnit::AddStartMatr(bool FirstRegen, int StPos)
{
	BPoint ABCE(StartState.MashA, StartState.MashB, StartState.MashC, 0.);
	pDoc->GetMachTool().SetState(BMTLinkage().SetIndW1(), SMachState(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.), &GetFixedDef()));
	BMatr Q = pDoc->GetMachTool().GetMainCoordsMatr(GetFixedDef());
	BMatr StockMatr = pDoc->GetMachTool().GetStockMatr(StPos, false);
	Q = Q * StockMatr.invr();
	if (FirstRegen && ABCE.D2() == 0. && Q.IsE())
		return;
	BSpacePos::AddRotMatr(Q, ABCE);
}

BOOL NCUnit::ProcessCoordRot(CString &text, const CString &CurWord)
{
	double angle;
	if(!GetDValue(text, &angle))
	{
		if (StoreError(1030))
			return FALSE;
		else
			return TRUE;
	}
	angle *= GCD.RotaryScale;
	switch(CurWord[0])
	{
	case 'H':
		if(State.LeadingAxis < 0)
			State.LeadingAxis = 3;
		if(State.ZeroPreset) // G92 is active
		{
			if(State.ZeroPresetType == ZPT_REL)
				State.CurBase.a0 = State.MashA - angle;
			else if (State.ZeroPresetType == ZPT_SHT)
				State.CurBase.a0 += angle;
			else
			{
				if (State.AbsoluteCoord)
					State.CurBase.a0 = State.GetActiveUCS().a0 + angle;
				else
					State.CurBase.a0 += angle;
			}
		}
		else
		{
			if(State.FixA)
			{
				if (StoreError(1073))
					return -2;
			}
			State.ChangedCoord.a = TRUE;
			if(State.AbsoluteCoord)
				State.MashA = angle + State.CurBase.a0;
			else
				State.MashA += angle;
		}
	break;
	case 'V':
		if(State.LeadingAxis < 0)
			State.LeadingAxis = 4;
		if(State.ZeroPreset) // G92 is active
		{
			if(State.ZeroPresetType == ZPT_REL)
				State.CurBase.b0 = State.MashB - angle;
			else if (State.ZeroPresetType == ZPT_SHT)
				State.CurBase.b0 += angle;
			else
			{
				if (State.AbsoluteCoord)
					State.CurBase.b0 = State.GetActiveUCS().b0 + angle;
				else
					State.CurBase.b0 += angle;
			}
		}
		else
		{
			if(State.FixB)
			{
				if (StoreError(1074))
					return -2;
			}
			State.ChangedCoord.b = TRUE;
			if(State.AbsoluteCoord)
				State.MashB = angle + State.CurBase.b0;
			else
				State.MashB += angle;
		}
	break;
	case 'S':
		if(State.LeadingAxis < 0)
			State.LeadingAxis = 5;
		if(State.ZeroPreset) // G92 is active
		{
			if(State.ZeroPresetType == ZPT_REL)
				State.CurBase.c0 = State.MashC - angle;
			else if (State.ZeroPresetType == ZPT_SHT)
				State.CurBase.c0 += angle;
			else
			{
				if (State.AbsoluteCoord)
					State.CurBase.c0 = State.GetActiveUCS().c0 + angle;
				else
					State.CurBase.c0 += angle;
			}
		}
		else
		{
			if(State.FixC)
			{
				if (StoreError(1075))
					return -2;
			}
			State.ChangedCoord.c = TRUE;
			{
				if(State.AbsoluteCoord)
					State.MashC = angle + State.CurBase.c0;
				else
					State.MashC += angle;
			}
		}
	break;
	}
	return TRUE;
}

int NCUnit::SetSpindleSpeed(CString &text, int SpinNum)
{
	double Val;
	if(State.Spindle[SpinNum].SetMaxSpeed)
	{
		if(!GetDValue(text, &Val, _T("SpeedDotDecimalConversion")))
		{
			if (StoreError(1040))
				return -5;//error
			else
				return 0;
		}
		State.Spindle[SpinNum].MaxSpeed = Val * GetSpeedScale();
		State.Spindle[SpinNum].SetMaxSpeed = FALSE;
		return 0;
	}
	if(State.Spindle[SpinNum].FixCutSpeed)
	{
		if(!GetDIValue(text,&Val))
		{
			if (StoreError(1040))
				return -5;//error
			else
				return 0;
		}
		State.Spindle[SpinNum].FixedCutSpeed = Val * 1000.;
		return 0;
	}
	if(!GetDValue(text, &Val, _T("SpeedDotDecimalConversion")))
	{
		if (StoreError(1040))
			return -5;//error
		else
			return 0;
	}
	State.Spindle[SpinNum].Speed = Val * GetSpeedScale();
	return 0;
}

bool NCUnitBase::ProcBaltSysStyle(CString &text) const
{
	int IndS = text.Find('(');
	if(IndS < 0)
		return true;
	int IndE = text.Find(')', IndS);
	if(IndE < 0)
		return false;
	CString CurText = text.Mid(IndS + 1, IndE - IndS);
	if(CurText.Find(',', 0) <= 0)
		return true;
	CString Tokens[8];
	int TokensN = 0;
	for(; !CurText.IsEmpty() && TokensN < 8; ++TokensN)
	{
		Tokens[TokensN] = CurText.SpanExcluding(_T(",)"));
		CurText.Delete(0, Tokens[TokensN].GetLength() + 1);
	}
	const int MaxKeys = 9;
	CString Keys[MaxKeys] = {_T("BNC"), _T("BLE"), _T("BGT"), _T("BLT"), _T("BEQ"), _T("BNE"), _T("BGE")
						,_T("EPP")/*Program copy*/,_T("CLS")/*Subr call(external)*/};
	int KeyNum = 0;
	for(; KeyNum < MaxKeys; ++KeyNum)
		if(Tokens[0] == Keys[KeyNum])
			break;
	if(KeyNum == MaxKeys)
		return true;
	CString NewText;
	switch(KeyNum)
	{
	case 0:// Go to
		{
			if(TokensN != 2)
				return false;
			CString M;
			if(!GCD.SWordDef.Lookup(_T("MiscFunctionCode"), M))
				return false;
			CString Num;
			if(!GCD.SMiscFuncDef.Lookup(_T("GoTo"), Num))
				return false;
			CString P;
			if(!GCD.SWordDef.Lookup(_T("ProgramNumber"), P))
				return false;
			NewText = M + Num + P + _T("\"") + Tokens[1] + _T("\"");
		}
		break;
	case 1:// conditional go to
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		{
			if(TokensN != 4)
				return false;
			CString M;
			if(!GCD.SWordDef.Lookup(_T("MiscFunctionCode"), M))
				return false;
			CString Num;
			if(!GCD.SMiscFuncDef.Lookup(_T("GoTo"), Num))
				return false;
			CString P;
			if(!GCD.SWordDef.Lookup(_T("ProgramNumber"), P))
				return false;
			CString Oper;
			switch(KeyNum)
			{
			case 1: Oper = CString(NEvalExp::CloseBC()) + "?" + CString(NEvalExp::OpenBC()); break;
			case 2: Oper = CString(NEvalExp::CloseBC()) + ">" + CString(NEvalExp::OpenBC()); break;
			case 3: Oper = CString(NEvalExp::CloseBC()) + "<" + CString(NEvalExp::OpenBC()); break;
			case 4: Oper = CString(NEvalExp::CloseBC()) + "=" + CString(NEvalExp::OpenBC()); break;
			case 5: Oper = CString(NEvalExp::CloseBC()) + ":" + CString(NEvalExp::OpenBC()); break;
			case 6: Oper = CString(NEvalExp::CloseBC()) + "!" + CString(NEvalExp::OpenBC()); break;
			}
			NewText = _T("IF") + CString(NEvalExp::OpenBC()) + CString(NEvalExp::OpenBC()) + Tokens[1] + Oper + Tokens[2] + CString(NEvalExp::CloseBC()) + CString(NEvalExp::CloseBC()) + M + Num + P + _T("\"") + Tokens[3] + _T("\"");
		}
		break;
	case 7: // EPP
		{
			if(TokensN != 3)
				return false;
			CString G;
			if(!GCD.SWordDef.Lookup(_T("FunctionCode"), G))
				return false;
			struct FDef * pFDef;
			if(!GCD.SFuncDef.Lookup(_T("ProgramCopy"), pFDef))
				return false;
			CString P;
			if(!GCD.SWordDef.Lookup(_T("ProgramCopyStart"), P))
				return false;
			CString Q;
			if(!GCD.SWordDef.Lookup(_T("ProgramCopyEnd"), Q))
				return false;
			NewText = G + pFDef->Num + P + _T("\"") + Tokens[1] + _T("\"")
									+ Q + _T("\"") + Tokens[2] + _T("\"");
		}
		break;
	case 8:// CLS
		{
			if(TokensN != 2)
				return false;
			CString M;
			if(!GCD.SWordDef.Lookup(_T("MiscFunctionCode"), M))
				return false;
			CString Num;
			if(!GCD.SMiscFuncDef.Lookup(_T("FileCall"), Num))
				return false;
			CString P;
			if(!GCD.SWordDef.Lookup(_T("ProgramNumber"), P))
				return false;
			Tokens[1] = Tokens[1].SpanExcluding(_T("/"));
			NewText = M + Num + P + _T("\"") + Tokens[1] + _T("\"");
		}
		break;
	default:
		return true;
	}
	text = text.Left(IndS) + NewText + text.Mid(IndE + 1);
	return true;
}

bool NCUnitBase::ProcSinumericStyle(CString &text) const
{
	{
		const int MaxKeys = 3;
		CString Keys[MaxKeys] = {_T("TLCH1("), _T("TLCH4H("), _T("TLCH4(")};
		int KeyNum = 0;
		int k = -1;
		for(; KeyNum < MaxKeys; ++KeyNum)
		{
			k = text.Find(Keys[KeyNum]);
			if(k >= 0)
				break;
		}
		if(k >= 0)
		{// Found
			int IndS = text.Find('(');
			if(IndS < 0)
				return false;
			int IndE = text.Find(')', IndS);
			if(IndE < 0)
				return false;
			CString CurText = text.Mid(IndS + 1, IndE - IndS);
			const int MaxTokens = 3;
			CString Tokens[MaxTokens];
			for(int TokensN = 0; !CurText.IsEmpty() && TokensN < MaxTokens; ++TokensN)
			{
				Tokens[TokensN] = CurText.SpanExcluding(_T(",)"));
				CurText.Delete(0, Tokens[TokensN].GetLength() + 1);
			}
			CString NewText;
			switch(KeyNum)
			{
			case 0:// TLCH1
				{
					if(Tokens[1].IsEmpty())
						Tokens[1] = _T("0");
					if(Tokens[2].IsEmpty())
						Tokens[2] = _T("0");
					NewText = _T("T") + Tokens[0] + "M6B" + Tokens[1] + _T("A") + Tokens[2];
				}
				break;
			case 1:// TLCH4H
			case 2:// TLCH4
				{
					NewText = _T("T") + Tokens[0] + _T("M6B") + Tokens[1];
					int ToolNum = GetToolNumByName(Tokens[0]);
					if(Tokens[2] == _T("2"))
						ToolNum += 500;
					if(!Tokens[2].IsEmpty())
					{
						CString Comp;
						Comp.Format(_T("D%d"), ToolNum);
						NewText += Comp;
					}

				}
				break;
			default:
				return true;
			}
			text = text.Left(k) + NewText + text.Mid(IndE + 1);
			return true;
		}
	}
	{
		static CString Keys[] = {_T("UNTIL"), _T("MCALL "), _T("REPEATB") , _T("REPEAT") };
		constexpr int MaxKeys = sizeof(Keys) / sizeof(Keys[0]);

		int KeyNum = 0;
		int k = -1;
		for(; KeyNum < MaxKeys; ++KeyNum)
		{
			k = text.Find(Keys[KeyNum]);
			if(k >= 0)
				break;
		}
		if(k >= 0)
		{// Found
			RemoveComment(text); 

			CString NewText;
			switch(KeyNum)
			{
			case 0:// UNTIL
				{// do nothing
					//CString Pref = text.Left(k);
					//text.Delete(0, 5 + k + 1);// 5 is UNTIL length
					//NewText = _T("IN") + text + _T("GOTOBITERATE");
					//text = Pref + NewText;
				}
				break;
			case 1:// MCALL
				{
					CString Pref = text.Left(k);
					text.Delete(0, 5 + k + 1);// 5 is "MCALL" length
					RemoveLeadBlanks(text);
					if(text.IsEmpty())
					{
						text = _T("G1067");
						break;
					}
					if(text.Left(5) == _T("CYCLE"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(5) == _T("DRILL"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(6) == _T("DRTURN"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(7) == _T("DRBREAK"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(6) == _T("DRDEEP"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(5) == _T("DRTAP"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(5) == _T("DRGAN"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					if(text.Left(5) == _T("DRFCH"))
					{
						text = Pref + _T("MCALL") + text;
						break;
					}
					int BlankInd = text.Find(' ');
					if(BlankInd < 0)
					{
						NewText = text;
						text.Empty();
					}
					else
					{
						NewText = text.Left(BlankInd);
						text.Delete(0, BlankInd + 1);
					}
					NewText = _T("G1066L\"") + NewText + _T("\"");
					text = Pref + NewText + text;
				}
				break;
			case 2:// REPEATB
			{
				CString Pref = text.Left(k);
				text.Delete(0, 7 + k + 1);// 7 is REPEATB length
				RemoveLeadBlanks(text);
				CString Label1 = text.SpanExcluding(_T(" \t"));
				text.Delete(0, Label1.GetLength());
				RemoveLeadBlanks(text);
				CString GProgCopyB = GetGCodeText(_T("ProgramCopyB"));
				CString LProgCopyStart = GetLProgCopyStart();
				CString LProgCopyEnd = GetLProgCopyEnd();
				if (GProgCopyB.IsEmpty() || LProgCopyStart.IsEmpty() || LProgCopyEnd.IsEmpty())
					return false;
				text = GProgCopyB + LProgCopyStart + Label1 + ':' + LProgCopyEnd + Label1 + ':' + text;
				text = Pref + text;
			}
			break;
			case 3:// REPEAT
			{
				CString Pref = text.Left(k);
				text.Delete(0, 6 + k + 1);// 6 is REPEAT length
				if (!ProcSinumRepeat(text))
					return false;
				text = Pref + text;
			}
			break;
			default:
				return true;
			}
			return true;
		}
	}
	// Replace " P<digit>" by "@@P<digit>"
	int i = text.Find(_T(" P"));
	if(i >= 0 && text.GetLength() > i + 1)
	{
		if(isdigit(text[i + 2]))
		{
			text.Delete(i, 1);
			text.Insert(i, _T("@@"));
		}
	}
	return true;
}

bool NCUnitBase::ProcSinumRepeat(CString& text) const
{
	RemoveLeadBlanks(text);
	if (text.GetLength() == 0)
	{
		// REPEAT UNTIL case
		text = _T("REPEAT");
		return true;
	}
	CString Label2;
	// First label present
	CString Label1 = text.SpanExcluding(_T(" \t"));
	text.Delete(0, Label1.GetLength());
	RemoveLeadBlanks(text);
	bool OneLabelCase = (text.GetLength() == 0);
	if (!OneLabelCase)
		OneLabelCase = (text[0] == 'P');
	if (OneLabelCase)
	{
		// One label presents
		CString GProgCopy1L = GetGCodeText(_T("ProgramCopy1L"));
		CString LProgCopyStart = GetLProgCopyStart();
		if (GProgCopy1L.IsEmpty() || LProgCopyStart.IsEmpty())
			return false;
		text = GProgCopy1L + LProgCopyStart + Label1 + ':' + text;
	}
	else
	{
		// Two labels present
		Label2 = text.SpanExcluding(_T(" \t"));
		text.Delete(0, Label2.GetLength());
		RemoveLeadBlanks(text);
		CString GProgCopy = GetGCodeText(_T("ProgramCopy"));
		CString LProgCopyStart = GetLProgCopyStart();
		CString LProgCopyEnd = GetLProgCopyEnd();
		if (GProgCopy.IsEmpty() || LProgCopyStart.IsEmpty() || LProgCopyEnd.IsEmpty())
			return false;
		text = GProgCopy + LProgCopyStart + Label1 + ':' + LProgCopyEnd + Label2 + ':' + text;
	}

	return true;
}

CString NCUnitBase::GetGCodeText(const CString& GName) const
{
	CString G;
	if (!GCD.SWordDef.Lookup(_T("FunctionCode"), G))
		return CString();
	struct FDef* pFDef;
	if (!GCD.SFuncDef.Lookup(GName, pFDef))
		return CString();
	return G + pFDef->Num;
}

CString NCUnitBase::GetLProgCopyStart() const
{
	CString title;
	if (!GCD.SWordDef.Lookup("ProgramCopyStart", title))
	{
		return CString();
	}
	return title;
}

CString NCUnitBase::GetLProgCopyEnd() const
{
	CString title;
	if (!GCD.SWordDef.Lookup("ProgramCopyEnd", title))
	{
		return CString();
	}
	return title;
}

int NCUnitBase::FindCadrInProg(BProgram* pProg, int num, bool FindByLineNumber)
{
	//если первый просмотр кода
	if(!pProg->GetProgList().isCreated)
	{
		for(int i = 0; i < pProg->GetSize(); ++i)
		{
			CString Curtext = (*pProg)[i]->GetReText();
//			DoReplace(Curtext);
			CString Word = GetWord(Curtext);

			if(Word == "LineNumber" || (Word.GetLength() == 0 && Curtext.GetLength() > 0 && isdigit(Curtext[0])))
			{
				int buf;
				GetIValue(Curtext, &buf);
				pProg->GetProgListByLineNumber().addMark(buf, i);
				Word = GetWord(Curtext);
			}
			if(Word == "ProgramStartEnd")
			{
				if(Curtext.IsEmpty())
					continue;
				int cnum;
				bool StringName = (Curtext[0] == '"');
				if(StringName)
				{
					CString MacroName;
					if(!GetLabel(Curtext, MacroName))
						continue;
					GetHashNum(MacroName, &cnum);
				}
				else
				{
					if(!GetIValue(Curtext, &cnum))
						continue;
				}
				pProg->GetProgList().addMark(cnum, i);
			}
		}
		pProg->GetProgList().isCreated = true;
	}
	//поиск процедуры в списке
	int strNum = -1;
	if(FindByLineNumber)
		strNum = pProg->GetProgListByLineNumber().findMark(num, -1, false);
	else
		strNum = pProg->GetProgList().findMark(num, -1, false);
	return strNum;
}

bool NCUnitBase::SetCurCadr(cadrID ID)
{
	State.SetCurCadr(ID);
	return true;
}

BOOL NCUnit::ProcessModalCall(bool ResetMatrs)
{
	BOOL ng = FALSE;
	CString str;
	if(!GCD.SOthers.Lookup("ModalSubNeedsGeom",str))
		ng = TRUE;
	else
	{
		if(str != "No")
			ng = TRUE;
	}
	BOOL dopc = FALSE;
	if(!GCD.SOthers.Lookup("ModalSubNeedsParam",str))
		dopc = TRUE;
	else
	{
		if(str != "No")
			dopc = TRUE;
	}
	if(State.AutoCycleCount > 0)
		State.ModalAutoCycleCount = State.AutoCycleCount;

	BOOL XYchanged = State.CurX != 0. || State.CurY != 0.  || State.CurZ != 0. || State.CurA != 0. || State.CurB != 0. || State.CurC != 0.;
	BOOL Cchanged = (State.ChangedCoord.byte != 0);
	BOOL FirstCadr = (St.ModalSubrID != State.ModalSubrID);
	if(State.ModalSubr2 || (FirstCadr && !ng) || XYchanged || (!FirstCadr && Cchanged && dopc) || State.CurCycle == TURNCUT || State.CurCycle == TURNFACECUT || State.CurCycle == TURNCLEAR)
	{
		cadrID p;
		p = State.CadrID;
		++p.Cadr;
		State.GetVarSet().PushRegsMap();
		if(State.CallStack.Push(p, 1) != 0)
		{
			Error = 1067;// level 1
			return FALSE;
		}
		for (int i = 0; i < State.ModalAutoCycleCount - 1; ++i)
		{
			State.GetVarSet().PushRegsMap();
			if (State.CallStack.Push(State.ModalSubrID, 1) != 0)
			{
				Error = 1067;// level 1
				return FALSE;
			}
		}
		State.OrderChanged = true;

		State.NextCadr = State.ModalSubrID;
		State.ModalSubrIDRet = p;
	}
	return TRUE;
}

int NCUnit::JumpOver()
{
	cadrID DestID = GetState()->GetCurCadrID();
	++DestID.Cadr;
	auto CallStackSize = State.CallStack.GetSize();
	auto CycleStackSize = State.CycleStack.GetSize();

	JumpSteps(1);
	if(GetState()->OrderChanged)
	{
		if(CallStackSize != State.CallStack.GetSize() || CycleStackSize != State.CycleStack.GetSize())
		{
		if(!(DestID.Cadr == 0 && State.GetCurCadr() == 0))
			JumpForTo(DestID);		
		}
	}
	return 0;
}

bool NCUnit::Jump2State(const NCUnitState &TargetState, const NCUnitState &TargetStateAdd, const NCUnitStatePrevGeom &TargetStateGeom)
{
//	int OldProg = State.GetCurCadrIDEx().Prog;
	State.ForceMakeEqualTo(TargetState);
	AddState.ForceMakeEqualTo(TargetStateAdd);
	LastGeomState = TargetStateGeom;
	St.Geom.SetType(undef);
	if (State.PolarInterp.IsActive() || State.CylInterp.IsActive())
		WordDef.SetTemp(_T("VerticalAxis"), _T("C"));// "C" coordinate (not axis) name
	else
		WordDef.RemoveTemp(_T("C"));// "C" coordinate (not axis) name
	return true;
}

void NCUnitBase::RemoveLeadBlanks(CString &text)
{
	CString b = text.SpanIncluding(_T(" "));
	text.Delete(0, b.GetLength());
}

void NCUnitBase::GetConfErrMessage(int ErrNum, const std::vector<CString>& ErrParams, CString& ErrMessage)
{
	CStdioFile f;
	if (!f.Open(NCM_PROJECT.GetSettingsPath() + NCM_PROJECT.ErrConfFile, CFile::typeBinary | CFile::modeRead))
	{
		ErrMessage.Format(_T("Error message file missed (%s)"), NCM_PROJECT.ErrConfFile);
		return;
	}
	CString str;
	bool Found = false;
	while (f.ReadString(str))
	{
		if (atoi(str) == ErrNum)
		{
			Found = true;
			CString Num = str.SpanExcluding(" \t");
			str = str.Mid(Num.GetLength() + 1);
			RemoveLeadBlanks(str);
			switch (ErrParams.size())
			{
			case 0:
				ErrMessage.Format(str
				);
				break;
			case 1:
				ErrMessage.Format(str
					, ErrParams[0]);
				break;
			case 2:
				ErrMessage.Format(str
					, ErrParams[0], ErrParams[1]);
				break;
			case 3:
				ErrMessage.Format(str
					, ErrParams[0], ErrParams[1], ErrParams[2]);
				break;
			case 4:
			default:
				ErrMessage.Format(str
					, ErrParams[0], ErrParams[1], ErrParams[2], ErrParams[3]);
				break;
			}
			break;
		}
	}
	if(!Found)
		ErrMessage.Format(_T("Undefined error message #%d"), ErrNum);
}

int NCUnit::CrMillDrillPeckCycle(NCycle *pCycle)
{
	if(State.CycleZ == CYCLE_UNDEF)
		return -1;
	double step = DWELL_STEP;
	CString Sstep;
	if(GCD.SOthers.Lookup("DwellTimeDiscr",Sstep))
		step = atoi(Sstep) * 0.001;
	double DwellTime = 0.;
	CString SDwellTime;
	if(GetOtherValue("DrillPeckDwell", SDwellTime))
		DwellTime = atof(SDwellTime) * step;

	CycleGeom.RemoveAll();
	NCadrGeom Buf;
	CycleGeom.AddElement(Buf.FullCopy(State.Geom));// Memory leek
	bool FirstCycleCadr = (St.CurCycle == CYCLE_OFF);
	bool RefAbs = true;
	BMatr M0, M1;
	switch(State.CurCycle)
	{
	case DRILLPECK:
		M0.SetE();
		M1.SetE();
		RefAbs = true;
		break;
	case DRILLPECKZ:
		M0.SetE();
		M1.SetE();
		RefAbs = false;
		break;
	case DRILLPECKX:
		M0.RotY(BPoint(0., 0., 0., 1.), -90.);
		M1.RotY(BPoint(0., 0., 0., 1.), 90.);
		RefAbs = false;
		break;
	}
	double PeckIDepth = State.GetRegister(222);
	if(PeckIDepth == 0.)
		PeckIDepth = 1000000.;
	double Ref = State.GetRegister(201);
	if(RefAbs)
	{
		// Recalculate Ref
		switch(State.CurPlane)
		{
		case XY:
			Ref += State.CurBase.z0;
			break;
		case XZ:
			Ref += State.CurBase.y0;
			break;
		case YZ:
			Ref += State.CurBase.x0;
			break;
		}
	}
	NMillDrillCycle MillDrillCycle(CycleGeom, State.Feed, FirstCycleCadr, State.CurCycleRet == TOSTART
		, Ref
		, State.AbsoluteCoord ? PeckIDepth : State.Geom.GetB().GetZ() + PeckIDepth, RefAbs
		, M0
		, M1
		, DwellTime
		, State.CycleDepthZ);

	if(!MillDrillCycle.IsOK())
		return -1;

	if (State.Geom.IsUndef())
		State.Geom.SetType(fast);
	if (State.CompGeom.IsUndef())
	{
		State.CompGeom.SetType(fast); // Must be before SetB
		State.CompGeom.SetB(State.CompGeom.GetE());
	}
//	SetStateFromCompGeom();

	State.MashX = CycleGeom.GetLast()->GetE().GetX();
	State.MashY = CycleGeom.GetLast()->GetE().GetY();
	State.MashZ = CycleGeom.GetLast()->GetE().GetZ();
	State.CurX = State.MashX - St.MashX;
	State.CurY = State.MashY - St.MashY;
	State.CurZ = State.MashZ - St.MashZ;
	State.MashcX = St.MashcX + State.CurX;
	State.MashcY = St.MashcY + State.CurY;
	State.MashcZ = St.MashcZ + State.CurZ;
	State.Geom.SetE(BPoint(State.MashX, State.MashY, State.MashZ, 1.));
	State.CompGeom.SetE(BPoint(State.MashcX, State.MashcY, State.MashcZ, 1.));
	pDoc->GetMachTool().ProcCadrStep(*this, State);
	return 0;
}

int NCUnit::CrThreadingTapCycle(NCycle *pCycle, enum CycleType CType)
{
	//CycleGeom.RemoveAll();
	//NCadrGeom Buf;
	//CycleGeom.AddElement(Buf.FullCopy(State.Geom));// Memory leek
	//bool FirstCycleCadr = (St.CurCycle == CYCLE_OFF);
	//double ReverseTime = 0.;
	//CString Val;
	//if(GetOtherValue("SpindleReverseTime", Val))
	//	ReverseTime = atof(Val);
	//double R = State.GetRegister(201);
	//BMatr M0, M1;
	//switch(CType)
	//{
	//case RHTAPPINGX:
	//	M0.RotY(BPoint(0., 0., 0., 1.), -90.);
	//	M1.RotY(BPoint(0., 0., 0., 1.), 90.);
	//	R *= GCD.XScale * State.CurXScale;
	//	break;
	//case RHTAPPINGZ:
	//	M0.SetE();
	//	M1.SetE();
	//	R *= GCD.ZScale * State.CurZScale;
	//	break;
	//}
	//NThreadingTapCycle ThreadingTapCycle(CycleGeom, State.Feed, FirstCycleCadr, State.CurCycleRet == TOSTART
	//	, R
	//	, ReverseTime,
	//	M0,
	//	M1);

	//if(!ThreadingTapCycle.IsOK())
	//	return -1;

	//for(int i = 0; i < CycleGeom.GetSize(); ++i)
	//{
	//	if(CycleGeom[i]->GetTime() > 0.)
	//		State.SetCadrTime(CycleGeom[i]);// To add SpindleReverseTime to State.Time
	//}

	//State.MashX = CycleGeom.GetLast()->GetE().GetX();
	//State.MashY = CycleGeom.GetLast()->GetE().GetY();
	//State.MashZ = CycleGeom.GetLast()->GetE().GetZ();
	//State.MashcX = CycleGeom.GetLast()->GetE().GetX();
	//State.MashcY = CycleGeom.GetLast()->GetE().GetY();
	//State.MashcZ = CycleGeom.GetLast()->GetE().GetZ();
	//State.CurX = State.MashX - St.MashX;
	//State.CurY = State.MashY - St.MashY;
	//State.CurZ = State.MashZ - St.MashZ;
	//switch(CType)
	//{
	//case RHTAPPINGX:
	//	State.Geom.SetX(State.MashX);
	//	break;
	//case RHTAPPINGZ:
	//	State.Geom.SetZ(State.MashZ);
	//	break;
	//}
	//State.CompGeom = State.Geom;
	//pDoc->GetMachTool().ProcCadrStep(State, GetChannel().NGetProgramListCn()->HaveCompTraj());
	//return 0;
	double ReverseTime = 0.;
	CString SReverseTime;
	if(GetOtherValue("SpindleReverseTime", SReverseTime))
		ReverseTime = atof(SReverseTime);
	State.CycleDwellTime = int(max(State.CycleDwellTime, ReverseTime));
	SReverseTime.Format(_T("%d"), State.CycleDwellTime);
	NCadr *pcCadr;
	CString g, z, beg, DwellSym, RegSym;
	struct FDef *pFDef;
	double MashZ = 0.;
	if( !GCD.SWordDef.Lookup("FunctionCode",g) ||
		!GCD.SWordDef.Lookup("ProgramBeginEnd",beg) ||
		!GCD.SWordDef.Lookup("DwellTime", DwellSym) ||
		!GCD.SWordDef.Lookup("Register", RegSym) ||
		!GCD.SFuncDef.Lookup("Rapid",pFDef))   
		return -1;
	switch(State.CurPlane)
	{
	default:
	case XY:
		MashZ = (State.MashZ - State.CurBase.z0) / GCD.ZScale * State.CurZScale;
		if(!GCD.SWordDef.Lookup("SpindleAxis",z))
			return -1;
		break;
	case XZ:
		MashZ = (State.MashY - State.CurBase.y0) / GCD.YScale * State.CurYScale;
		if(!GCD.SWordDef.Lookup("VerticalAxis",z))
			return -1;
		break;
	case YZ:
		MashZ = (State.MashX - State.CurBase.x0) / GCD.XScale * State.CurXScale;
		if(!GCD.SWordDef.Lookup("HorizontalAxis",z))
			return -1;
		break;
	}
	CString g0 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Linear",pFDef))   
		return -1;
	CString g1 = g + pFDef->Num;
	if( !GCD.SFuncDef.Lookup("Dwell",pFDef))   
		return -1;
	CString g4 = g + pFDef->Num;
	
	if(State.CycleZ == CYCLE_UNDEF)
		return -1;
	CString text = beg;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	CString bR;
	bR.Format("%lf", MashZ);
	CString bbR = bR;
	if(CType == RHTAPPINGPLANE)
		bbR = _T("0.");
	text = g0 + z + CString(NEvalExp::OpenBC()) + RegSym + _T("201+") + bbR + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + State.CycleZ;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g4 + DwellSym + SReverseTime; pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	text = g1 + z + CString(NEvalExp::OpenBC()) + RegSym + _T("201+") + bbR + CString(NEvalExp::CloseBC());pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	if(CType != RHTAPPINGPLANE || State.CurCycleRet == TOSTART)
		{text = g0 + z + bR;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);}

	text = g0;pcCadr = new NCadr(text);pCycle->AddCadr(pcCadr);
	return 0;
}

bool NCUnit::IsArcCenterAbs(void) const
{
	bool AbsC = false;
	if(GCD.CircleCenterMode == CCM_Absolute)
		AbsC = true;
	else if(GCD.CircleCenterMode == CCM_AsCoord)
		AbsC = (State.AbsoluteCoord == TRUE);
	return AbsC;
}

void NCUnit::StoreCurrent2UCS(int UCS)
{
	int ind = UCS;
	StartState.UCSs.SetUsed(ind);
	StartState.UCSs[ind].a0 = State.MashA;
	StartState.UCSs[ind].b0 = State.MashB;
	StartState.UCSs[ind].c0 = State.MashC;
	StartState.UCSs[ind].x0 = State.MashX;
	StartState.UCSs[ind].y0 = State.MashY;
	StartState.UCSs[ind].z0 = State.MashZ;
	StartState.ResetRegisters();
}

void NCUnit::GetUcsToolPos(int i, BPoint &P, BPoint &N)
{
	NCBase B = StartState.UCSs[i];
	BPoint XYZ, ABC;
	pDoc->GetMachTool().GetState(XYZ, ABC, GetFixedDef());
	pDoc->GetMachTool().SetState(BMTLinkage().SetIndW1(), SMachState(BPoint(B.x0, B.y0, B.z0, 1.), BPoint(B.a0, B.b0, B.c0, 1.), &GetFixedDef()));
	BMatr Matr = pDoc->GetMachTool().GetStock2ToolMatr(0, 0, 0);// Ins STInds
	P = BPoint(0., 0., 0., 1.) * Matr;
	N = BPoint(0., 0., 1., 0.) * Matr;
	pDoc->GetMachTool().SetState(BMTLinkage().SetIndW1(), SMachState(XYZ, ABC, &GetFixedDef()));
}

bool NCUnit::GetUcsToolMatr(int i, BMatr &M)
{
	// Return matrix to tranform tool from i'th UCS to 000000 UCS
	const NCBase &B = StartState.UCSs[i];
	if(B.IsEmpty())
	{
		M.SetE();
		return true;
	}
	BPoint XYZ, ABC;
	pDoc->GetMachTool().GetState(XYZ, ABC, GetFixedDef());
	pDoc->GetMachTool().SetState(BMTLinkage().SetIndW1(), SMachState(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.), &GetFixedDef()));
	BMatr Matr0 = pDoc->GetMachTool().GetStock2ToolMatr(0, 0, 0);// Ins STInds
	pDoc->GetMachTool().SetState(BMTLinkage().SetIndW1(), SMachState(BPoint(B.x0, B.y0, B.z0, 1.), BPoint(B.a0, B.b0, B.c0, 1.), &GetFixedDef()));
	BMatr Matr1 = pDoc->GetMachTool().GetStock2ToolMatr(0, 0, 0);// Ins STInds
	M = Matr0 * Matr1.invr();
	pDoc->GetMachTool().SetState(BMTLinkage().SetIndW1(), SMachState(XYZ, ABC, &GetFixedDef()));
	return false;
}

bool NCUnit::RecalcUcs(int i, const BMatr &M)
{
	RCoordPair Axes = State.PlaceToolAxes;
	if(pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(3)) == NULL)
		Axes = RCP_BC;
	if(pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(4)) == NULL)
		Axes = RCP_AC;
	if(pDoc->GetMachTool().GetCurConfig().GetAxis(GetFixedDef().GetName(5)) == NULL)
		Axes = RCP_AB;
	BPoint P, N;
	GetUcsToolPos(i, P, N);
	SMachState NewMachState(&FixedDef);
	if(!CalcPlaceTool(NewMachState
			, P * M
			, N * M
			, Axes
			, 0
			, true
			, 0
			))
			return false;
	NCBase New = StartState.UCSs[i];
	New.x0 = NewMachState.GetCoord(0);
	New.y0 = NewMachState.GetCoord(1);
	New.z0 = NewMachState.GetCoord(2);
	New.a0 = NewMachState.GetCoord(3);
	New.b0 = NewMachState.GetCoord(4);
	New.c0 = NewMachState.GetCoord(5);
	StartState.UCSs[i] = New;
	return true;
}

bool NCUnit::RecalcAllUcss(const BMatr& M)
{
	for each (auto i in GetUsedUCSs())
		RecalcUcs(i, M);
	return false;
}


void NCUnit::FillUnitFile(void)
{
	if (GCD.ConfParNames.empty())// without unit inheritance 
		GCDV.ConfParNames.push_back(GCD.ConfFileName);
	for (int i = 0; i < GCD.ConfParNames.size(); ++i)
	{
		CString buf;
		NCMProject::LoadFile2Str(GetConfPath() + GCD.ConfParNames.at(i), buf);
		CString Name2Store;
		MakeTempName(GCD.ConfParNames.at(i), Name2Store, true);
		NCM_PROJECT.UnitConfs[GetChannelInd()].SetAt(Name2Store, buf);
	}
}


void NCUnit::SetTempUnit(void)
{
	const auto& UnitConfs = NCM_PROJECT.UnitConfs.GetAt(GetChannelInd());

	for (auto Pos = UnitConfs.GetStartPosition(); Pos != NULL; )
	{
		CString Key;
		CString Text;
		UnitConfs.GetNextAssoc(Pos, Key, Text);
		CString buf;
		MakeTempName(Key, buf, false);
		NCMProject::SaveStr2File(GetConfPath() + buf, Text, true);
	}
	NCM_PROJECT.UnitConfs[GetChannelInd()].RemoveAll();
}

void NCUnit::MakeTempName(const CString& FileName, CString& TempName, bool Restore)
{
	static const CString Suffix = _T(".temp");
	if (Restore)
	{
		TempName = FileName;
		TempName.Replace(Suffix, _T(""));
	}
	else
	{
		int Ind = FileName.ReverseFind('.');
		if (Ind < 0)
			TempName = FileName + Suffix;
		else
			TempName = FileName.Left(Ind) + Suffix + FileName.Mid(Ind);
	}
}

bool NCUnit::IsTempName(const CString& FileName)
{
	static const CString Suffix = _T(".temp");
	return FileName.Find(Suffix) >= 0;
}

void NCUnit::ApplyRCP(bool Revert)
{
		if(Revert)
		{
			if (State.IsRCPActive())
			{
				State.MashX += State.RCPcX;
				State.MashY += State.RCPcY; 
				State.MashZ += State.RCPcZ;
				State.CurX += State.RCPcX;
				State.CurY += State.RCPcY;
				State.CurZ += State.RCPcZ;
				State.MashcX += State.RCPcX;
				State.MashcY += State.RCPcY;
				State.MashcZ += State.RCPcZ;
			}
		}
		else
		{
			if (State.IsRCPActive())
			{
				St.MashX -= St.RCPcX;
				St.MashY -= St.RCPcY;
				St.MashZ -= St.RCPcZ;
				St.CurX -= St.RCPcX;
				St.CurY -= St.RCPcY;
				St.CurZ -= St.RCPcZ;
				St.MashcX -= St.RCPcX;
				St.MashcY -= St.RCPcY;
				St.MashcZ -= St.RCPcZ;
			}
		}
}

void NCUnit::ChangeAddCoords(const CMapStringToNProgram& ProgList, cadrID StartPosition, const SMachState& InAddCoord)
{
	// A. change all subsequent non fixed add coords (incremental or start values)

	// set current add coords
	auto pP = ProgList.GetProg(StartPosition);
	// A.1.find current cadr original geom
	NCadrGeom* pCompGeom = pP->GetCompGeomArray(0).GetAt(StartPosition.Cadr);// Last geom
	if (pCompGeom == nullptr)
		return; // internal error
	int StartMSHistInd = pCompGeom->GetMSHistNum();
	auto PrevAddCoord(InAddCoord);
	pP->GetAddCoord(StartMSHistInd, PrevAddCoord);
	pP->SetAddCoord(StartMSHistInd, InAddCoord);
	// A.2. change subsequent add coords
	int NumToDo = InAddCoord.GetSize() - 1;
	bool* FlagsToDo = new bool[NumToDo];
	for (int i = 0; i < NumToDo; ++i)
		FlagsToDo[i] = true;
	auto CurAddCoord(InAddCoord);
	++StartMSHistInd;
	for (cadrID LocID = StartPosition; LocID.Prog < ProgList.GetMainProgCount() && NumToDo > 0; ++LocID.Prog)
	{
		auto pProg = ProgList.GetProg(LocID);
		for (int MSInd = StartMSHistInd; MSInd < pProg->GetMSHistSize() && NumToDo > 0; ++MSInd)
		{
			StartMSHistInd = 0;
			auto& MSPos = pProg->GetMSHistory()[MSInd];
			int CooInd = MSPos.GetIndex();
			if (FlagsToDo[CooInd])
			{
				if (MSPos.IsFixed())
				{
					FlagsToDo[CooInd] = false;
					--NumToDo;
				}
				else
				{
					double Delta = MSPos.GetValue() - PrevAddCoord.GetCoord(CooInd);
					PrevAddCoord.SetCoord(CooInd, MSPos.GetValue());
					MSPos.SetValue(CurAddCoord.GetCoord(CooInd) + Delta, MSPos.IsFixed());
					CurAddCoord.SetCoord(CooInd, MSPos.GetValue());
				}
			}
		}
	}

	// B. change additional coordinates in all subsequent states
	cadrID CurId = StartPosition;
	auto Ind = GetStates().FindMaxLowerInd(StartPosition, true) + 1;// next index
	auto pProg = ProgList.GetProg(CurId);
	for (int k = Ind; k < GetStates().GetSize(); ++k)
	{
		const auto pState = GetStates().States[k];
		CurId = pState->GetCurHCadrAbsEx();
		pProg = ProgList.GetProg(CurId);

		const NCadrGeom* pGeom = pProg->GetCompGeomArray(0).GetAt(CurId.Cadr);
		pProg->GetAddCoord(pGeom->GetMSHistNum(), pState->AddCoords);
		GetStates().AddStates[k]->AddCoords = pState->AddCoords;
	}
}

NCMApplicationType NCUnit::GetToolClass() const
{
	auto ToolClass = AT_UNDEF;
	const NToolCombined* pToolCom = GetActiveTool();
	if (pToolCom)
		ToolClass = pToolCom->GetClass();
	if (ToolClass == AT_UNDEF)
		ToolClass = AT_MILL;
	if (ToolClass == AT_TURN)
	{
		if (GetState()->Geom.IsGroove())
			ToolClass = AT_MILL;
	}
	return ToolClass;
}

bool NCUnit::IsToolTurn() const
{
	return GetToolClass() == AT_TURN;
}

bool NCUnit::IsToolMill() const
{
	return GetToolClass() == AT_MILL;
}

bool NCUnit::StoreError(int ErrCode) noexcept
{
	// returns true if return ECR_ERROR needed and false if processing can continue
	Error = 0;// needed when Error already has a value 
	switch (pDoc->ProgErrHandler.GetErrorReaction(NCUnitStatePrev::ECR_ERROR, ErrCode))
	{
	case BProgErrHandler::ER_CONTINUE:
		if(ErrorMinor == 0)// to store first error code
			ErrorMinor = ErrCode;
		return false;
	case BProgErrHandler::ER_SKIP_LINE:
		if (ErrorMinor == 0)// to store first error code
			ErrorMinor = ErrCode;
		return true;
	case BProgErrHandler::ER_IGNORE:
		return false;
	default:
		Error = ErrCode;
		return true;
	}
}

bool NCUnit::ProgCopyLC()
{
	// program copy last cadr
	cadrID p;
	int count;
	if (State.CallStack.Pop(p, count) < 0)
		return false;
	if (count > 1)
	{
		if (State.CallStack.Push(p, --count) < 0)
			return false;
	}
	else
	{// Pop cadr num after calling cadr and ActiveCopyEnd value
		if (State.CallStack.Pop(p, count) < 0)
			return false;
		State.ActiveCopyEnd = count;
	}
	State.ActiveCopyEnding = true;
	State.OrderChanged = true;
	State.NextCadr = p;
	return true;
}

SearchDir NCUnit::GetSDir(const CString& OtherParam, bool ForREPEATB)
{
	if (GetSinumerikStyle())
	{
		return ForREPEATB ? SD_BACKFOR : SD_BACK;
	}
	CString val;
	SearchDir SDir = SD_EMPTY;
	GetOtherValue(OtherParam, val);
	if (val == _T("ForwardFirst"))
	{
		SDir = SD_FORBACK;
	}
	else if (val == _T("Backward"))
	{
		SDir = SD_BACK;
	}
	else if (val == _T("Forward"))
	{
		SDir = SD_FOR;
	}
	else if (val == _T("BackwardFirst"))
	{
		SDir = SD_BACKFOR;
	}
	return SDir;
}

BStockToolInds NCUnit::GetCurSTInds()
{
	BStockToolInds STInds(GetCurStockPosition(), 0, 0);
	const NToolCombined* pTool = GetActiveTool();
	if (pTool)
	{
		STInds.turret_ = pTool->Turret;
		STInds.attach_ = pTool->Attach;
	}
	return STInds;
}

int NCUnit::GetCadrNum(const CString& str, CString& NumSym) const
{
	if (str.GetLength() < 1)
		return -1;
	const bool NeedSym = (GCD.SWordDef.Lookup("LineNumber", NumSym));
	if (NeedSym && str.Left(1) != NumSym)
		return -1;
	const int StartInd = NeedSym ? 1 : 0;
	if (str.GetLength() < StartInd + 1)
		return -1;
	if (!isdigit(str[StartInd]))
		return -1;
	return atoi(str.Mid(StartInd));
}

int NCUnit::GetStockPos4Turn() const
{
	return GCD.StockPos4Turn;
}

const std::set<int>& NCUnit::GetMyStockPos() const
{
	return GCD.MyStockPos;
}

void NCUnit::AddCheckPointPos(BProgram* pProg, NSyncCheckPoints& CheckPoints)
{
	const int HistPosAbs = pProg->Get1GeomArraySize(GetCurStockPosition()) - 1;
	for (int i = 0; i < CheckPoints.size(); ++i)
		CheckPoints[i].SetCadrHistInd(HistPosAbs, pProg);
	sort(CheckPoints.begin(), CheckPoints.end(), NCheckPoint::Compare);
	GCDV.SyncHistory.insert(GCD.SyncHistory.end(), CheckPoints.begin(), CheckPoints.end());
}


int NCUnit::MakeLengthComp(const NToolCombined& CurTool, const NCadrGeom& Geom, NCadrGeom& LengthCompGeom)
{
	// makes LengthCompGeom from Geom
	// Geom should consider contour compensation 
	// Geom and LengthCompGeom may be references to the single object
	// returns error code or 0
	if (&Geom != &LengthCompGeom)
		LengthCompGeom = Geom;
	if (!Geom.HaveLComp())
		return 0;
	if (!Geom.HaveGeom5())
		return 0;
	MTMachineTool& Mach = pDoc->GetMachTool();
	BMatr MCS = CalcMCS(Geom, &CurTool);
	if (Geom.IsLine() || Geom.IsArc())
	{
		//BPoint XYZ, ABC;
		//Geom.CalcCurCoordMCS(0., MCS, XYZ, ABC);
		//Mach.SetState(Geom.GetMTConfNum(), SMachState(XYZ, ABC, &GetFixedDef()));
		//BPoint ToolVect = BPoint(0., 0., 1., 0.) * Mach.GetStock2ToolMatr(Geom.GetStockPos(), CurTool.Turret, CurTool.Attach);
		//if (!LengthCompGeom.ApplyLCompSimple(ToolVect))
		//	return 1203;
		//BPoint ShiftVect = BPoint(0., 0., 1., 0.) * State.CSMatr;
		//if (!LengthCompGeom.ApplyLCompSimple(ShiftVect))
		//	return 1203;
		BPoint ShiftVect = BPoint(0., 0., 1., 0.);
		if (!LengthCompGeom.ApplyLCompSimple(ShiftVect))
			return 1211;
	}
	else if (Geom.IsRotate())
	{
		if (pDoc->GetMachTool().GetCurConfig().IsDefined())
		{
			//
			//BPoint XYZ, ABC;
			//Geom.CalcCurCoordMCS(0., MCS, XYZ, ABC);
			//Mach.SetState(Geom.GetMTConfNum(), SMachState(XYZ, ABC, &GetFixedDef()));
			BPoint ToolVect = BPoint(0., 0., 1., 0.);// *Mach.GetStock2ToolMatr(Geom.GetStockPos(), CurTool.Turret, CurTool.Attach);
			LengthCompGeom.SetB(LengthCompGeom.GetB() * BMatr().Trans(ToolVect * LengthCompGeom.GetLengthComp()));
			//Geom.CalcCurCoordMCS(0., MCS, XYZ, ABC);
			//Mach.SetState(Geom.GetMTConfNum(), SMachState(XYZ, ABC, &GetFixedDef()));
			//ToolVect = BPoint(0., 0., 1., 0.) * Mach.GetStock2ToolMatr(Geom.GetStockPos(), CurTool.Turret, CurTool.Attach);
			LengthCompGeom.SetE(LengthCompGeom.GetE() * BMatr().Trans(ToolVect * LengthCompGeom.GetLengthComp()));
			
			BPoint ToolBaseComp(0., 0., 0., 1.);
			BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.), XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
			Geom.CalcCurCoord(0., XYZ0, ABC0);
			Geom.CalcCurCoord(1., XYZ1, ABC1);
			//auto pCurveComp = pDoc->GetMachTool()
			//	.Make5xLine(Geom.GetRCPMask(), pDoc->GetpMUPair(), XYZ0, ABC0, XYZ1, ABC1, ToolBaseComp, STInds);
			//LengthCompGeom.SetCurve(pCurveComp);
		}

	}
	else if (Geom.IsCylindric())
	{

	}
	return 0;
}

BMatr NCUnit::CalcMCS(const NCadrGeom& Geom, const NToolCombined* pTool) const
{
	int CMatrNum = Geom.GetMatrNum();
	if (Geom.IsRotate())
		CMatrNum = Geom.GetInterpNum();
	BMatr MCS;
	if (CMatrNum != 0)
	{
		int Tur = 0;
		int Att = 0;
		if (pTool)
		{
			Tur = pTool->Turret;
			Att = pTool->Attach;
		}
		BMatr M = pDoc->GetMachTool().GetRespMatr(BMatr(), BSpacePos::GetABC(CMatrNum), Tur, Att, Geom.GetStockPos(), &GetFixedDef());
		BMatr Q = BSpacePos::GetRotMatr(CMatrNum);
		MCS = Q * M.invr();
	}
	return MCS;
}

void NCUnit::RemoveCSMatr()
{
	BMatr invM = State.CSMatr;
	BPoint Mash = BPoint(St.MashX, St.MashY, St.MashZ, 1.) * invM;
	State.MashX = Mash.GetX();
	State.MashY = Mash.GetY();
	State.MashZ = Mash.GetZ();
	BPoint Mashc = BPoint(St.MashcX, St.MashcY, St.MashcZ, 1.) * invM;
	State.MashcX = Mashc.GetX();
	State.MashcY = Mashc.GetY();
	State.MashcZ = Mashc.GetZ();
	State.CSMatr.SetE();
}

void NCUnit::EnableCSMatr()
{
	BMatr invM = TiltPlane.GetCSMatr().invr();
	BPoint Mash = BPoint(St.MashX, St.MashY, St.MashZ, 1.) * St.CSMatr * invM;
	State.MashX = Mash.GetX();
	State.MashY = Mash.GetY();
	State.MashZ = Mash.GetZ();
	BPoint Mashc = BPoint(St.MashcX, St.MashcY, St.MashcZ, 1.) * St.CSMatr * invM;
	State.MashcX = Mashc.GetX();
	State.MashcY = Mashc.GetY();
	State.MashcZ = Mashc.GetZ();
}

int NCUnit::ProcessLengthComp()
{
	if (!State.CurZCompChanging && State.CurZComp == UD_OFF)
		return 0;
	if (!State.Geom.HaveGeom5())
		return 0;
	double Val = GetLengthComp();
	if (State.CurZCompChanging)
	{
		if (State.Geom.IsArc())
		{
			if (StoreError(1203))
				return NCUnitStatePrev::ECR_ERROR;
		}
		else if (State.Geom.IsLine())
		{
			if (State.CurZComp == UD_OFF)
			{// turning off
				State.CompGeom.SetB(State.CompGeom.GetB() + BPoint(0., 0., Val, 0.));
			}
			else
			{// turning on
				State.MashcZ += Val;
				State.CompGeom.SetE(State.CompGeom.GetE() + BPoint(0., 0., Val, 0.));
			}
		}
	}
	else
	{
		if (State.Geom.IsArc() || State.Geom.IsLine())
		{
			State.MashcZ += Val;
			if (State.AddCompGeom.HaveGeom())
			{
				State.CompGeom.ShiftZ(Val);
				State.AddCompGeom.Set(State.AddCompGeom.GetType(),
					St.MashcX, St.MashcY, St.MashcZ,
					State.CompGeom.GetB().GetX(), State.CompGeom.GetB().GetY(), State.CompGeom.GetB().GetZ());
			}
			else
			{
				if (State.Geom.HaveGeom())
				{
					State.CompGeom.Set(State.Geom.GetType(),
						St.MashcX, St.MashcY, St.MashcZ,
						State.MashcX, State.MashcY, State.MashcZ);
				}
			}
		}
	}
	MTMachineTool& Mach = pDoc->GetMachTool();
	return 0;
}

int NCUnit::ProcessDiamComp(NCadrGeom& NextGeom)
{
	const Plane TmpPlane = State.CurPlane;
	if (TmpPlane != XY)
	{
		State.ChangePlane(TmpPlane, XY);
		St.ChangePlane(TmpPlane, XY);
		LastGeomState.ChangePlane(TmpPlane, XY);
		NextGeom.Tr(NCUnitState::ChangePlaneMatr(TmpPlane, XY));

		const int e = CalcComp(St, true, NextGeom);
		if (e != 0)
		{
			Error = 1058;// level 1
			return NCUnitStatePrev::ECR_ERROR;
		}

		State.ChangePlane(XY, TmpPlane);
		St.ChangePlane(XY, TmpPlane);
		LastGeomState.ChangePlane(XY, TmpPlane);
	}
	else
	{
		const int e = CalcComp(St, true, NextGeom);
		if (e != 0)
		{
			Error = 1058;// level 1
			return NCUnitStatePrev::ECR_ERROR;
		}
	}
	return 0;
}

double NCUnit::GetLengthComp() const
{
	double shift = 0;
	CString sval;
	if (AddZDispToComp)
	{
		const NToolCombined* pTool = GetActiveTool();
		if (pTool != nullptr)
			shift = -pTool->GetZDisp();
	}

	switch (State.CurZComp)// new done
	{
	case UP:
		return State.GetCurLengthComp() + shift;
	case DOWN:
		return -State.GetCurLengthComp() - shift;
	default:
		return 0;
	}
}

int NCUnit::ProcessDisplComp()
{
	bool DisplComp = NCMComData::IsTurnEnabled() && State.CompGeom.HaveGeom();
	if (!DisplComp)
		return 0;
	BPoint Vd = State.GetToolDisplHAAS(true);
	BPoint Ud = State.GetToolDisplHAAS(false);
	if (St.CurDirComp == State.CurDirComp)
	{
		if (State.CurDirComp != LR_OFF)
			State.CurToolDisplacement = Vd;
		else
			State.CurToolDisplacement = Ud;
		bool HasBeenChaged = State.CalcCurToolDispl(PartialCompCancel);
		BMatr TrM;
		TrM.Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + State.CurToolDisplacement);
		State.CompGeom.Tr(TrM);
		State.AddCompGeom.Tr(TrM);
		if (HasBeenChaged)
		{
			State.CompGeom.SetB(State.CompGeom.GetB() + State.PrevToolDisplacement - State.CurToolDisplacement);
		}
	}

	bool CompStart = (St.CurDirComp == LR_OFF && State.CurDirComp != LR_OFF);
	bool CompCancel = (St.CurDirComp != LR_OFF && State.CurDirComp == LR_OFF);
	if (CompStart)
	{
		State.CurToolDisplacement = Vd;
		bool HasBeenChaged = State.CalcCurToolDispl(PartialCompCancel);
		if (HasBeenChaged)
		{
			Ud = State.PrevToolDisplacement;
		}
		BPoint P0 = State.CompGeom.GetB() * BMatr().Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + Ud);
		BPoint P1 = State.CompGeom.GetE() * BMatr().Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + State.CurToolDisplacement);
		State.CompGeom.SetB(P0);
		State.CompGeom.SetE(P1);
	}
	if (CompCancel)
	{
		State.CurToolDisplacement = Ud;
		bool HasBeenChaged = State.CalcCurToolDispl(PartialCompCancel);
		if (HasBeenChaged)
		{
			Vd = State.PrevToolDisplacement;
		}
		BPoint P0 = State.CompGeom.GetB() * BMatr().Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + Vd);
		BPoint P1 = State.CompGeom.GetE() * BMatr().Trans(BPoint(0., 0., 0., 1.), BPoint(0., 0., 0., 1.) + State.CurToolDisplacement);
		State.CompGeom.SetB(P0);
		State.CompGeom.SetE(P1);
	}

	State.MashcX = State.CompGeom.GetE().GetX();
	State.MashcY = State.CompGeom.GetE().GetY();
	return 0;
}

void NCUnit::PrepNewRotMatr(const BPoint& ABCE)
{
	BSpacePos::LastTurret = State.CurTurret;
	BSpacePos::LastCSMatr = State.CSMatr;
	BSpacePos::LastABC = ABCE;
	BSpacePos::pLastMT = &pDoc->GetMachTool();
	BSpacePos::LastMatrUsed = false;
}

int NCUnit::OpenConfFile(CArchive*& f, std::vector<CString>& ErrParams)
{
	ClearConf();
	
	std::filesystem::path Path = GCD.ConfFileName.GetString();
	CString Ext(Path.extension().string().c_str());
	if (Ext.MakeUpper() == ".SMX")
	{
		const HMODULE m = LoadLibrary("PPLicense.dll");
		if (!m)
		{
			AfxMessageBox("License processing DLL file missed");
			return 0;
		}
		CMemFile& mf = *new CMemFile;
		int res = Decrypt::NONE;
		if (NCM_PROJECT.IsPostUserMaster())
			res = Decrypt::DecryptPPFile(GetConfPath() + GCD.ConfFileName, mf);
		else
		{
			CString ClientID;
			ClientID.Format("%d", NCM_PROJECT.GetPostUserID());
			res = Decrypt::DecryptPPFile(GetConfPath() + "License.xml", GetConfPath() + GCD.ConfFileName, ClientID, mf);
		}
		// error messages
		if (res != Decrypt::NONE)
		{
			int ErrMessID = 0;
			switch (res)
			{
			case Decrypt::LICENSE_FILE_NOT_FOUND:
				ErrMessID = 0;
				break;
			case Decrypt::POSTPROCESSOR_FILE_NOT_FOUND:
				ErrMessID = 0;
				break;
			case Decrypt::INVALID_CLIENT_ID_IN_LICENSE:
				ErrMessID = 0;
				break;
			case Decrypt::NO_ACCESS_TO_POSTPROCESSOR_FILE:
				ErrMessID = 0;
				break;
			case Decrypt::INVALID_SIGNATURE:
				ErrMessID = 0;
				break;
			case Decrypt::FILE_READ_FAILED:
				ErrMessID = 0;
				break;
			case Decrypt::DECRYPTION_FAILED:
				ErrMessID = 0;
				break;
			case Decrypt::INVALID_POSTPOCESSOR_FILE:
				ErrMessID = 0;
				break;
			}
			AfxMessageBox(ErrMessID);
			return -7;
		}
		mf.SeekToBegin();
		f = new CArchive(&mf, CArchive::load);
	}
	else
	{
		CStdioFile& sf = *new CStdioFile;
		if (!sf.Open(GetConfPath() + GCD.ConfFileName, CFile::typeText | CFile::modeRead))
		{
			ErrParams.push_back(GCD.ConfFileName);
			return -7;
		}
		f = new CArchive(&sf, CArchive::load);
	}
	return 0;
}

void NCUnit::CloseConfFile(CArchive*& pA) const
{
	pA->Flush();
	CFile* pFile = pA->GetFile();
	pA->Close();
	pFile->Close();
	delete pFile;
	delete pA;
}

void NCUnit::Make3DArcCycle()
{
	if (!State.Geom.Is3DArc())
		return;
	State.Geom.Divide3DArc(CycleGeom);
	State.CompGeom.Divide3DArc(CycleGeomComp);
	State.Geom.SetType(undef);
	State.CompGeom.SetType(undef);
	State.Geom.SetWorkTime(2);
	State.CompGeom.SetWorkTime(2);
	for (int i = 0; i < CycleGeom.GetSize(); ++i)
		CycleGeom.GetAt(i)->SetAttr(1);
	for (int i = 0; i < CycleGeomComp.GetSize(); ++i)
		CycleGeomComp.GetAt(i)->SetAttr(1);
}