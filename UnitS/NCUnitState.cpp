#include "..\Common\NCUnitState.h"
// NCUnitState.cpp: implementation of the NCUnitState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConstDef.h"
#include "NCMComData.h"
#include "NCMProject.h"
#include "NUnitStartStateDlgs.h"
#include "NCBase.h"
#include "NCUnitState.h"
#include "NUnitCompDlg.h"
#include "NUnitWeldDlg.h"
#include "NUnitUCSDlg.h"
#include "NUnitTurnCompDlg.h"
#include "NCadrGeom.h"
#include "curve.h"
#include "BPoint.h"
#include "math.h"
#include "SMachStateFix.h"
#include "SCoordsDef.h"
#include "NWeldCadr.h"
#include "BSpacePos.h"
#include "BIPolar.h"
#include "BICyl.h"
#include "BProgram.h"
#include "ncunitstate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int NCUnitStatePrev::DComp2RegStart = 1;
int NCUnitStatePrev::DComp2RegEnd = 0;

double NCUnitStatePrev::FastFeedX = 12000.;
double NCUnitStatePrev::FastFeedY = 12000.;
double NCUnitStatePrev::FastFeedZ = 12000.;
double NCUnitStatePrev::FastFeedA = 2000.;
double NCUnitStatePrev::FastFeedB = 2000.;
double NCUnitStatePrev::FastFeedC = 2000.;
int NCUnitState::UCSRegsSubst[MAX_UCS];// Register number for the first UCS element
vector<pair<int, int>> NCUnitState::VisibleRegisters;// Visible registers intervals. Register is visible if it belongs to any interval

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NCUnitStatePrevGeom::NCUnitStatePrevGeom()
{
	Reset();
}

void NCUnitStatePrevGeom::ChangePlane(Plane Src, Plane Dest)
{
		if (Src == Dest)
			return;

		BMatr Tr = ChangePlaneMatr(Src, Dest);

		TrCoord(&MashX, &MashY, &MashZ, 1., Tr);
		TrCoord(&MashcX, &MashcY, &MashcZ, 1., Tr);
		Geom.Tr(Tr);
}



void NCUnitStatePrevGeom::Reset()
{
	MashX = 0; MashY = 0; MashZ = 0;
	MashcX = 0; MashcY = 0; MashcZ = 0;
	CurDirComp = LR_OFF;
	Geom.Clear();
	MachLink.Clear();
}

NCUnitStatePrev::NCUnitStatePrev() : NCUnitStatePrevGeom(), AddCoords(nullptr)
{
	DComp2RegStart = 2000;
	DComp2RegEnd = 2199;
}

void NCUnitStatePrev::MakeEqualTo(NCUnitStatePrev &src)
{
	MakeEqualTo((const NCUnitStatePrev &)src);
}

void NCUnitStatePrev::MakeEqualTo(const NCUnitStatePrev &src)
{
	if (DComp.Changed == false && src.DComp.Changed == false)
		DComp.ToChange = false;
	else
		DComp.ToChange = true;

	if (HComp.Changed == false && src.HComp.Changed == false)
		HComp.ToChange = false;
	else
		HComp.ToChange = true;

	if (Registers.Changed == false && src.Registers.Changed == false && Registers.GetSize() == src.Registers.GetSize())
		Registers.ToChange = false;
	else
		Registers.ToChange = true;


	*this = src;

	DComp.Changed = false;
	HComp.Changed = false;
	Registers.Changed = false;
}

////////// NCUnitState ///////////////////////////
NCUnitState::NCUnitState() : NCUnitStatePrev(), PolarCenter(0., 0., 0., 0.), AddCoordsFlags(nullptr)
{
	SetCurProgEmpty();
	Reset();
	DontDrawIfEmpty = true;
}

NCUnitState::NCUnitState(const NCUnitState &InState) : NCUnitStatePrev(InState), AddCoordsFlags(InState.AddCoordsFlags)
{
	*this = InState;
}

NCUnitState::~NCUnitState()
{
}

void NCUnitState::Reset()
{
	DComp.RemoveAll();
	HComp.RemoveAll();
	ResetWOComp();
}

void NCUnitState::ResetWOComp()
{
	NCUnitStatePrev::Reset();
	ChangedCoord.byte = 0;
	CompCancel = FALSE;
	RetractNext = FALSE;
	EnableExtraArc = FALSE;
	EnableExtraArcModal = FALSE;
	ParToNext = FALSE;
	RetractComp = FALSE;
	CurDiaCompHorVec = 0.;
	CurDiaCompVerVec = 0.;
	FixX = FALSE;
	FixY = FALSE;
	FixZ = FALSE;
	FixA = FALSE;
	FixB = FALSE;
	FixC = FALSE;
	MirrorRotate.Reset();
	CommentActive = false;
	CurX = 0; CurY = 0; CurZ = 0;
	MashXUCS = 0; MashYUCS = 0; MashZUCS = 0;
	MashA = 0; MashB = 0; MashC = 0;
	MashX2 = 0.;
	MashcXUCS = 0; MashcYUCS = 0; MashcZUCS = 0;
	CurI = 0; CurJ = 0; CurK = 0;
	CurI2 = 0.;
	CurR = 0;
	PolarDist = 0.;
	PolarAngle = 0.;
	PolarCenter.Set(0., 0., 0., 0.);
	CurToolN = CString("0");
	CurTurret = 0;
	NextToolNum = -1;
	ThisCadrToolN = -1;
	CurCurveType = fast;
	AbsoluteCoord = TRUE;
	CycleZAbs = FALSE;
	MetricCoord = (MM_INCH == M_MM);
	FeedM = MetricCoord ? MMpMIN : INCHpMIN;
	AllowSpiral = TRUE;
	SetCurCadr(0);
	ClearCurHCadr();
	ClearCurHCadrAbs();
	CurPlane = XY;
	Feed = 0;
	Time = 0.;
	Length = 0.;
	CadrTime = 0.;
	Coolant = COOLOFF;
	FeedScale = 1.;
	Rc = 0.;
	Ic = 0.;
	Jc = 0.;
	Kc = 0.;
	Angle = 0.;
	AngleX = 0.;
	AngleY = 0.;
	AngleZ = 0.;
	CadrLength = 0.;
	CurCadrNum = 0;
	ProgramNumber = "";
	ProgramNumberStore = "";
	ProgramName = "";
	Spindle[SPINDLE_MILL].Clear();
	Spindle[SPINDLE_TURN].Clear();
	CurDiamComp = 0.;
	CurCodeComp = 0.;
	CurLengthComp = 0.;
	CurLengthCompNum = 0;
	CurZComp = UD_OFF;
	CurZCompChanging = false;
	CurCycle = CYCLE_OFF;
	R1 = CYCLE_UNDEF;
	R2 = CYCLE_UNDEF;
	CycleZ = CYCLE_UNDEF;
	CycleDepthZ = 0.;
	CycleTappingZ = CYCLE_UNDEF;
	CycleI = CYCLE_UNDEF;
	CycleJ = CYCLE_UNDEF;
	CadrID.Cadr = 0;
	CadrID.Prog = 0;
	CycleThreadParm = 1;
	EndProgram = false;
	OrderChanged = false;
	GotoElse = false;
	NextCadr.Prog = 0;
	NextCadr.Cadr = 0;
	ProgramStarted = false;
	CadrSkiped = false;
	CurCycleRet = TOSTART;
	Offset = LO_UNDEF;
	ActiveUCS = 0;// must be before ResetRegisters
	UCSs.SetUsed(ActiveUCS);
	ResetRegisters();
	Punch = PUNCH_OFF;
	CurPunchStep = 0.;
	AutoCycleCount = -1;
	ModalAutoCycleCount = -1;
	Debug = FALSE;
	MoveRotateCS = FALSE;
	MoveRotateCSRel = FALSE;
	RotateCSABC = FALSE;
	MoveRotateCSCancel = FALSE;
	ZeroPreset = FALSE;
	ZeroPresetType = ZPT_STD;
	ActiveCopyEnd = -1;
	CSMatr.SetE();
	TechTable = 0;
	GlobalDwell = 0.;
	ArcCenterFromPolarCenter = FALSE;
	DefinePolarCenter = FALSE;
	PolarInterp.DeActivate();
	CylInterp.DeActivate();
	CurXScale = 1.;
	CurYScale = 1.;
	CurZScale = 1.;
	CurIScale = 1.;
	CurJScale = 1.;
	CurKScale = 1.;
	CoordCompSignum = 0;
	CoordCompCode = 0;
	CurBase = UCSs[0];
	ThreadCadr = false;
	ToolDisplChanged.byte = 0;
	CurToolDisplacement.Set(0., 0., 0., 0.);
	PrevToolDisplacement.Set(0., 0., 0., 0.);
	TurnThreadR2 = 0.;
	CurAngle = 0.;
	CurAngleDefined = false;
	AllowBreak = true;
	FavorConcepts.RemoveAll();
	RotType = Linear;
	NonExecCount = 0;
	HhLen = 0;
	HhCca = 0;
	HhR = 0;
	DefinedCycleNum = -1;
	Chamfer.Clear();
	IntNotFound = false;
	AddCoords.ClearValues();
	AddCoordsFlags.ClearValues();
	PlaceTool = PT_EMPTY;
	PlaceToolAxes = RCP_AC;
	MachineCSActive = false;
	ModalSubrID.Prog = -1;
	ModalSubrID.Cadr = 0;
	ModalSubr2 = false;
	VariablesDecl = false;
	ModalSubrIDRet.Prog = -1;
	ModalSubrIDRet.Cadr = 0;
	FeedNonModal = false;
	CircleFullTurns = 0;
	GrooveON = false;
	PolarHelix = false;
	Spline.DeActivate();
	CycleDwellTime = 0;
	RCPActive = false;
	RCPMask = 0;
	RCPCanceling = false;
	RCPcX = 0.;
	RCPcY = 0.;
	RCPcZ = 0.;
	RestLifeTime = 0.;
	WorkedLifeTime = 0.;
	ID4LifeTime = IntToolID(-1, -1);
	WorkedLifeTimes.clear();
	RestLifeTimes.clear();
	ChamfCoordStored = false;
	MileStoneFound = false;
	ProbeCadr = false;
	TouchCadr = false;
	ProbeExpNoTouch = false;
	ProbeExpTouch = false;
	MoveRotateCSAngle = 0.;
	VarSet.Clear();
}

const NVarSet & NCUnitState::GetVarSet() const
{
	return VarSet;
}

NVarSet & NCUnitState::GetVarSet()
{
	return VarSet;
}

void NCUnitState::ChamfCoord2Coord()
{
	MashX = ChamfMashX;
	MashY = ChamfMashY;
	MashZ = ChamfMashZ;
}

void NCUnitState::Coord2ChamfCoord()
{
	ChamfMashX = MashX;
	ChamfMashY = MashY;
	ChamfMashZ = MashZ;
//	ChamfCoordStored = true;
}

void NCUnitState::SwapChamfCoord()
{
	double b;
	b = ChamfMashX;
	ChamfMashX = MashX;
	MashX = b;
	b = ChamfMashY;
	ChamfMashY = MashY;
	MashY = b;
	b = ChamfMashZ;
	ChamfMashZ = MashZ;
	MashZ = b;
}

void NCUnitState::ChamfCoord2Prev(NCUnitStatePrev & St)
{
	if (CurCurveType == cwarc || CurCurveType == ccwarc)
	{
		CurI -= ChamfMashX - St.MashX;
		CurJ -= ChamfMashY - St.MashY;
		CurK -= ChamfMashZ - St.MashZ;
	}
	St.MashX = ChamfMashX;
	St.MashY = ChamfMashY;
	St.MashZ = ChamfMashZ;
	ChamfCoordStored = false;
}

int NCUnitState::AskUser(int StartPage, int CurZeroPos)
{
	NUnitPositionDlg UnitPositionDlg;
	UnitPositionDlg.m_x	= MashX;
	UnitPositionDlg.m_y	= MashY;
	UnitPositionDlg.m_z	= MashZ;
	UnitPositionDlg.m_a	= MashA;
	UnitPositionDlg.m_b	= MashB;
	UnitPositionDlg.m_c	= MashC;
	UnitPositionDlg.m_tool_pos = CurToolN;

	NUnitSpeedDlg UnitSpeedDlg;
	UnitSpeedDlg.is_feed_mm_per_min = (FeedM==MMpMIN || FeedM==INCHpMIN) ? true : false;
	UnitSpeedDlg.m_feed.Format("%.0f", Feed);
	UnitSpeedDlg.m_feed_scale.Format("%.0f", FeedScale*100.);

	NUnitModeDlg UnitModeDlg;
	UnitModeDlg.absolute_coord = AbsoluteCoord;
	UnitModeDlg.metric_coord = MetricCoord;
	UnitPositionDlg.m_Debug = Debug;

	NUnitCompDlg UnitCompDlg;
	NMapIntToDouble BufDComp = DComp; // For the changes check only
	NMapIntToDouble BufHComp = HComp;
	switch (NCMComData::GetType())
	{
	case AT_MILL:
	default:
		UnitCompDlg.SetComp(&DComp, &HComp);
		break;
	case AT_TURN:
	case AT_MILL_TURN:
		UnitCompDlg.SetComp(nullptr, &HComp);
		break;
	}
	NUnitWeldDlg UnitWeldDlg;
	UnitWeldDlg.m_ElectrR = NWeldCadr::RootR;
	UnitWeldDlg.m_ElectrT1 = ElectrT1;
	UnitWeldDlg.m_ElectrA = ElectrA;
	UnitWeldDlg.m_ElectrB = ElectrB;
	NUnitUCSDlg UnitUCSDlg(UCSs);
	UnitUCSDlg.m_DontDrawIfEmpty = DontDrawIfEmpty;

	NUnitTurnCompDlg UnitTurnCompDlg;

	if(NCMComData::GetType() != AT_MILL)
	{
		int key;
		double val;
		for(POSITION pos = DComp.GetStartPosition(); pos != NULL;)
		{
			DComp.GetNextAssoc(pos, key, val);
			if(key < 1000)
			{
				NUnitTurnCompDlg::NTurnCompElem elem;
				int num = key;// - int(key / 1000) * 1000;
				if(!UnitTurnCompDlg.Elems.Lookup(num, elem))
				{
					elem.R = DComp[num];
					elem.K = int(DComp[1000 + num] + 0.5);
					elem.X = DComp[2000 + num];
					elem.Z = DComp[3000 + num];
					UnitTurnCompDlg.Elems[num] = elem;
				}
			}
		}
	}

	CPropertySheet UnitStartStateSheet(IDS_UNIT_START_STATE);
	UnitStartStateSheet.m_psh.dwFlags &= ~PSH_HASHELP;
	UnitPositionDlg.m_psp.dwFlags &= ~PSP_HASHELP;
	UnitSpeedDlg.m_psp.dwFlags &= ~PSP_HASHELP;
	UnitModeDlg.m_psp.dwFlags &= ~PSP_HASHELP;
	UnitCompDlg.m_psp.dwFlags &= ~PSP_HASHELP;
	UnitUCSDlg.m_psp.dwFlags &= ~PSP_HASHELP;
	UnitTurnCompDlg.m_psp.dwFlags &= ~PSP_HASHELP;
	UnitStartStateSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	UnitUCSDlg.m_AXIS = (CurZeroPos & ZPS_AXIS) != 0;
	UnitUCSDlg.m_FSYS = (CurZeroPos & ZPS_FSYS) != 0;
	UnitUCSDlg.m_SSYS = (CurZeroPos & ZPS_SSYS) != 0;


	UnitStartStateSheet.AddPage(&UnitPositionDlg);
	UnitStartStateSheet.AddPage(&UnitSpeedDlg);
	UnitStartStateSheet.AddPage(&UnitModeDlg);
 	UnitStartStateSheet.AddPage(&UnitUCSDlg);
	switch(NCMComData::GetType())
	{
	case AT_MILL:
	default:
		UnitStartStateSheet.AddPage(&UnitCompDlg);
		break;
	case AT_TURN:
		UnitStartStateSheet.AddPage(&UnitTurnCompDlg);// The number of this tab is used in CMainFrame::OnUnitInfo	
		UnitStartStateSheet.AddPage(&UnitCompDlg);
		break;
	case AT_MILL_TURN:
		UnitStartStateSheet.AddPage(&UnitTurnCompDlg);// The number of this tab is used in CMainFrame::OnUnitInfo	
		break;
	case AT_WELD:
		UnitStartStateSheet.AddPage(&UnitWeldDlg);
		break;
	}

	UnitStartStateSheet.SetActivePage(StartPage);
	bool ChangesDone = false;
	if( UnitStartStateSheet.DoModal() == IDOK )
	{
		ChangesDone |= (MashX != UnitPositionDlg.m_x);
		MashcX = MashX = UnitPositionDlg.m_x;
		ChangesDone |= (MashY != UnitPositionDlg.m_y);
		MashcY = MashY = UnitPositionDlg.m_y;
		ChangesDone |= (MashZ != UnitPositionDlg.m_z);
		MashcZ = MashZ = UnitPositionDlg.m_z;
		ChangesDone |= (MashA != UnitPositionDlg.m_a);
		MashA = UnitPositionDlg.m_a;
		ChangesDone |= (MashB != UnitPositionDlg.m_b);
		MashB = UnitPositionDlg.m_b;
		ChangesDone |= (MashC != UnitPositionDlg.m_c);
		MashC = UnitPositionDlg.m_c;
		ChangesDone |= (CurToolN != UnitPositionDlg.m_tool_pos);
		CurToolN = UnitPositionDlg.m_tool_pos;
		ChangesDone |= (Debug != UnitPositionDlg.m_Debug);
		Debug = UnitPositionDlg.m_Debug;

		ChangesDone |= (MetricCoord != UnitModeDlg.metric_coord);
		MetricCoord = UnitModeDlg.metric_coord;
		FeedMeasure NewValue = (UnitSpeedDlg.is_feed_mm_per_min) ?
			(MetricCoord ? MMpMIN : INCHpMIN) : (MetricCoord ? MMpREV : INCHpREV);
		ChangesDone |= (FeedM != NewValue);
		FeedM = NewValue;
		ChangesDone |= (Feed != atof(UnitSpeedDlg.m_feed));
		Feed = atof(UnitSpeedDlg.m_feed);
		ChangesDone |= (FeedScale != atof(UnitSpeedDlg.m_feed_scale) / 100.);
		FeedScale = atof(UnitSpeedDlg.m_feed_scale) / 100.;

		ChangesDone |= (AbsoluteCoord != UnitModeDlg.absolute_coord);
		AbsoluteCoord = UnitModeDlg.absolute_coord;

		ChangesDone |= (DontDrawIfEmpty != (UnitUCSDlg.m_DontDrawIfEmpty == TRUE));
		DontDrawIfEmpty = (UnitUCSDlg.m_DontDrawIfEmpty == TRUE);
		ChangesDone |= UnitUCSDlg.UCSChangeDone;

		CurBase = GetActiveUCS();
		ResetRegisters(); // To transfer new UCSs values to registers
		if (NCMComData::GetType() != AT_MILL)
		{
			DComp.RemoveAll();
			for (POSITION pos = UnitTurnCompDlg.Elems.GetStartPosition(); pos != NULL;)
			{
				int key;
				NUnitTurnCompDlg::NTurnCompElem elem;
				UnitTurnCompDlg.Elems.GetNextAssoc(pos, key, elem);
				DComp[key] = elem.R;
				DComp[1000 + key] = elem.K;
				DComp[2000 + key] = elem.X;
				DComp[3000 + key] = elem.Z;
			}
		}
		ChangesDone |= (BufDComp != DComp); // DComp & HComp can be changed in the for cycle above or in the UnitCompDlg
		ChangesDone |= (BufHComp != HComp);
		ChangesDone |= (ElectrT1 != UnitWeldDlg.m_ElectrT1);
		ElectrT1 = UnitWeldDlg.m_ElectrT1;
		ChangesDone |= (ElectrA != UnitWeldDlg.m_ElectrA);
		ElectrA = UnitWeldDlg.m_ElectrA;
		ChangesDone |= (ElectrB != UnitWeldDlg.m_ElectrB);
		ElectrB = UnitWeldDlg.m_ElectrB;
		ChangesDone |= (NWeldCadr::RootR != UnitWeldDlg.m_ElectrR);
		NWeldCadr::RootR = UnitWeldDlg.m_ElectrR;
		int Ret = (ChangesDone ? ZPS_NEED_REGEN : ZPS_EMPTY) | (UnitUCSDlg.m_SSYS ? ZPS_SSYS : ZPS_EMPTY) 
			| (UnitUCSDlg.m_FSYS ? ZPS_FSYS : ZPS_EMPTY) | (UnitUCSDlg.m_AXIS ? ZPS_AXIS : ZPS_EMPTY);
		return Ret;
	}
	return ZPS_NO;
}

void NCUnitState::Serialize(CArchive &ar)
{
	SerializeElements(ar, &AbsoluteCoord, 1);	
	SerializeElements(ar, &MetricCoord, 1);	
	SerializeElements(ar, &MashX, 1);	
	SerializeElements(ar, &MashY, 1);	
	SerializeElements(ar, &MashZ, 1);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 3.8))
	{
		SerializeElements(ar, &MashA, 1);
		SerializeElements(ar, &MashB, 1);
		SerializeElements(ar, &MashC, 1);
		SerializeElements(ar, &ElectrT1, 1);
		SerializeElements(ar, &ElectrA, 1);
		SerializeElements(ar, &ElectrB, 1);
		SerializeElements(ar, &NWeldCadr::RootR, 1);
	}


	if(ar.IsLoading())
	{
		MashcX = MashX;
		MashcY = MashY;
		MashcZ = MashZ;
	}
	DComp.Serialize(ar);
	HComp.Serialize(ar);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 2.6))
	{
		if(ar.IsLoading())
		{
			CString str;
			SerializeElements(ar,&str,1);
			CurToolN = str;
		}
		else
		{
			CString str = CurToolN;
			SerializeElements(ar,&str,1);
		}

		SerializeElements(ar,&FeedM,1);
		SerializeElements(ar,&FeedScale,1);
		SerializeElements(ar,&Spindle[SPINDLE_MILL].Speed,1);
	}
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 4.8))
	{
		UCSs.Serialize(ar);
	}
	else	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 3.7))
	{
		NCBaseOld OldUCSs[6];
		SerializeElements(ar, OldUCSs, 6);
		for (int i = 0; i < 6; ++i)
		{
			UCSs[i].a0 = OldUCSs[i].a0;
			UCSs[i].b0 = OldUCSs[i].b0;
			UCSs[i].c0 = OldUCSs[i].c0;
			UCSs[i].x0 = OldUCSs[i].x0;
			UCSs[i].y0 = OldUCSs[i].y0;
			UCSs[i].z0 = OldUCSs[i].z0;
			UCSs[i].z0 = OldUCSs[i].z0;
			UCSs[i].Visible = OldUCSs[i].Visible;
		}
	}
	else
	{
		class B
		{
		public:
			double x0, y0, z0, a0, b0, c0;
			virtual ~B(){};

		}b[6];
		SerializeElements(ar, &b, 1);
		for(int i = 0; i < 6; ++i)
		{
			UCSs[i].x0 = b[i].x0;
			UCSs[i].y0 = b[i].y0;
			UCSs[i].z0 = b[i].z0;
			UCSs[i].a0 = b[i].a0;
			UCSs[i].b0 = b[i].b0;
			UCSs[i].c0 = b[i].c0;
			UCSs[i].Visible = true;
		}
	}
	ResetRegisters();
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 4.3))
	{
		SerializeElements(ar, &DontDrawIfEmpty, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 3.9))
	{
		SerializeElements(ar, &Feed, 1);
		SerializeElements(ar, &Spindle[SPINDLE_TURN].SpindleDir, 1);
	}
	if(ar.IsLoading())
	{
		// Check enum params
		for(int i = 0; i < MAX_SPINDLE_NUM; ++i)
		{
			switch(Spindle[i].SpindleDir)
			{
			case R_UNDEF:
			case R_CW:
			case R_CCW:
				break;
			default:
				Spindle[i].SpindleDir = R_UNDEF;
				break;
			}
		}
		switch(FeedM)
		{
		case MMpMIN:
		case MMpREV:
		case MMpTOOTH:
		case INCHpMIN:
		case INCHpREV:
		case INCHpTOOTH:
		case REVApMIN:
		case REVBpMIN:
		case REVCpMIN:
			break;
		default:
			FeedM = MMpMIN;
			break;
		}
		//
		CurBase = GetActiveUCS();
	}
}

double NCUnitState::SetCadrTime(NCadrGeom *pGeom) const
{
	bool MMtoDEGinRotation = true;
	double Time = 0.;
	if (!(pGeom->HaveGeom5()))
	{
		//Time = pGeom->GetTime();
	}
	else if (pGeom->IsFast())
	{
		BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.);
		pGeom->CalcCurCoord(0., XYZ0, ABC0);
		BPoint XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
		pGeom->CalcCurCoord(1., XYZ1, ABC1);
		XYZ0 = XYZ1 - XYZ0;
		ABC0 = ABC1 - ABC0;
		double t = fmax(fmax(fmax(fmax(fmax(
			fabs(XYZ0.GetX()) / FastFeedX
			, fabs(XYZ0.GetY()) / FastFeedY)
			, fabs(XYZ0.GetZ()) / FastFeedZ)
			, fabs(ABC0.GetX()) / FastFeedA)
			, fabs(ABC0.GetY()) / FastFeedB)
			, fabs(ABC0.GetZ()) / FastFeedC);
		Time = 60. * t;
	}
	else if (Spindle[SPINDLE_TURN].FixCutSpeed && (FeedM == MMpREV || FeedM == INCHpREV))
	{
		Time = CalcFixedCutTime(pGeom);
	}
	else if (FeedM == REVApMIN || FeedM == REVBpMIN || FeedM == REVCpMIN)
	{
		if (Feed < 1.e-12)
			return -1.;// Incorrect Feed
		if (!pGeom->IsRotate())
			return -1.;// Incorrect movement
		SMachStateFix1 Start;
		if (!pGeom->CalcCurCoordMS(BMatr().SetE(), 0., Start))
			return -1.;
		SMachStateFix1 End;
		if (!pGeom->CalcCurCoordMS(BMatr().SetE(), 1., End))
			return -1.;
		SMachStateFix1 Mov = End - Start;

		double Angle = 0.;
		switch (FeedM)
		{
		case REVApMIN:
			Angle = Mov.GetCoord(3);
			break;
		case REVBpMIN:
			Angle = Mov.GetCoord(4);
			break;
		case REVCpMIN:
			Angle = Mov.GetCoord(5);
			break;
		}
		Time = abs(Angle) * 60. / (360. * Feed * FeedScale);// Feed must be in revolutions per min; Angle in degrees
	}
	else if (MMtoDEGinRotation && pGeom->IsRotate())
	{
		double Feed = CalcFeed();
		double SpecLength = pGeom->GetRotSpecLength();
		double t = SpecLength / Feed;
		Time = 60. * t;
	}
	else
	{
		double f = CalcFeed();
		double CadrLength = CadrLength4Time(*pGeom);
		if (f < 1.e-12 || CadrLength < 1.e-12)
			return -1.;// Incorrect Feed
		Time = CadrLength * 60. / f;
	}
	Time += pGeom->GetWorkTimeS();
	Time = fmax(2 * NCadrGeom::Tick2Sec, Time);// 2 is minimum allowed time for any cadr. Needed for right sequencing 
	pGeom->SetWorkTimeS(Time);
	return Time;
}

double NCUnitState::CadrLength4Time(const NCadrGeom &Geom) const
{
	if (!Geom.IsGroove())
	{
		if (GrooveON)
		{
			switch (Geom.GetType().type)
			{
			case cwarc:
			case ccwarc:
			case curve::rotate:
				return Geom.Length();
			case line:
				if (Geom.IsSpline() && Geom.GetCurve())
					return Geom.Length();
				{
					BPoint V = Geom.GetE() - Geom.GetB();
					V.SetX(fabs(V.GetX()));
					V.SetY(fabs(V.GetY()));
					V.SetZ(fabs(V.GetZ()));
					return max(V.GetX(), max(V.GetY(), V.GetZ()));
				}
			default:
				return 0.;
			}
		}
	}
	else
	{
		NCadrGeom Buf(Geom);
		Buf.SetType(line);
		return Buf.Length();
	}
	return Geom.Length();
}

void NCUnitState::IncrementTimer(NCadrGeom *pGeom)
{
	CalcLength(pGeom);
	CadrTime = fmax(0., SetCadrTime(pGeom));
	Time += CadrTime;
}

double NCUnitState::CalcFeed() const
{
// Calculate Feed in mm/min
	switch(FeedM)
	{
	case MMpMIN:
	case INCHpMIN:
		return Feed * FeedScale;
	case MMpREV:
	case INCHpREV:
		{
			int SpinNum = SPINDLE_TURN;
			if(Spindle[SPINDLE_TURN].Speed <= 0.)
				SpinNum = SPINDLE_MILL;
			return Feed * FeedScale * Spindle[SpinNum].Speed;
		}
	default:
		return 100000;
	}
}


void NCUnitState::SetCurCadr(int val)
{
	CurCadr = val;
	CadrID.Cadr = val;
}

void NCUnitState::SetCurCadr(cadrID ID)
{
	CurCadr = ID.Cadr;
	CadrID = ID;
}

void NCUnitState::SetCurCadrEx(int val)
{
	CurHCadrAbs = val;
	CadrIDEx.Cadr = val;
}

void NCUnitState::SetCurCadrEx(cadrID ID)
{
//	CurHCadr = ID.Cadr;
	CadrIDEx = ID;
}

void NCUnitState::ClearCurHCadr()
{
//	CurHCadr = 0;
	CadrIDEx.Cadr = 0;
}

void NCUnitState::IncrCurHCadr(int val)
{
//	CurHCadr += val;
	CadrIDEx.Cadr += val;
}

double NCUnitState::CalcAngle(const NCUnitStatePrev &PrevState)
{
	Angle = 0.;
	BPoint	v0,	v1; 
	double DeltaA = 0;
	if(	(CurCurveType == cwarc || CurCurveType == ccwarc) &&
		(PrevState.CurCurveType == cwarc || PrevState.CurCurveType == ccwarc))
	{
		
		v0.Set(PrevState.CurI,PrevState.CurJ,PrevState.CurK,0.);
		v1.Set(CurI,CurJ,CurK,0.);
		BPoint pe(PrevState.MashX,PrevState.MashY,PrevState.MashZ,1.);//end point of the prev cadr
		BPoint pb = pe - BPoint(PrevState.CurX,PrevState.CurY,PrevState.CurZ,0.);//first point of the prev cadr
		v0 = (pb + v0) - pe;
	}
	if(	(CurCurveType == cwarc || CurCurveType == ccwarc) &&
		(PrevState.CurCurveType == line || PrevState.CurCurveType == fast))
	{
		
		v0.Set(PrevState.CurX,PrevState.CurY,PrevState.CurZ,0.);
		v1.Set(CurI,CurJ,CurK,0.);
		DeltaA = 90.;
	}
	if(	(CurCurveType == line || CurCurveType == fast) &&
		(PrevState.CurCurveType == cwarc || PrevState.CurCurveType == ccwarc))
	{
		
		v0.Set(PrevState.CurI,PrevState.CurJ,PrevState.CurK,0.);
		v1.Set(CurX,CurY,CurZ,0.);
		BPoint pe(PrevState.MashX,PrevState.MashY,PrevState.MashZ,1.);//end point of the prev cadr
		BPoint pb = pe - BPoint(PrevState.CurX,PrevState.CurY,PrevState.CurZ,0.);//first point of the prev cadr
		v0 = (pb + v0) - pe;
		DeltaA = 90.;
	}
	if(	(CurCurveType == line || CurCurveType == fast) &&
		(PrevState.CurCurveType == line || PrevState.CurCurveType == fast))
	{
		v0.Set(PrevState.CurX,PrevState.CurY,PrevState.CurZ,0.);
		v1.Set(CurX,CurY,CurZ,0.);
	}
	double	l0 = v0.D2(),
			l1 = v1.D2();
	if(l0 < 1.e-9 || l1 < 1.e-9)
		return 0.;
	double sp = v0 * v1 / sqrt(l0*l1);
	sp = sp < -1. ? -1. : sp;
	sp = sp >  1. ?  1. : sp;
	Angle = fabs(acos(sp)*180./PI - DeltaA);
	return Angle;
}

void NCUnitState::CalcArcParam(NCadrGeom *pGeom)
{
	if(!pGeom->IsArc())
	{
		Ic = 0.;
		Jc = 0.;
		Kc = 0.;
		Rc = 0.;
		return;
	}
	double x,y,z;
	pGeom->GetI(&x,&y,&z);
	BPoint I(x,y,z,0.);
	Rc = I.D2();
	if(Rc > 0)
		Rc = sqrt(Rc);
	pGeom->GetB(&x,&y,&z);
	BPoint B(x,y,z,1.);
	B = B + I;
	Ic = B.GetX();
	Jc = B.GetY();
	Kc = B.GetZ();
}

void NCUnitState::CalcAxisAngles(NCadrGeom *pGeom)
{
//Calculate axis angles
	AngleX = AngleY = AngleZ = 0.;
	double x,y,z;
	BPoint r;
	if( pGeom->IsLine())
	{
		pGeom->GetB(&x,&y,&z);
		BPoint B(x,y,z,1.);
		pGeom->GetE(&x,&y,&z);
		BPoint E(x,y,z,1.);
		r = E-B;
	}
	if( pGeom->IsArc())
	{
		pGeom->GetB(&x,&y,&z);
		BPoint B(x,y,z,1.);
		pGeom->GetI(&x,&y,&z);
		BPoint I(x,y,z,0.);
		pGeom->GetN(&x,&y,&z);
		BPoint N(x,y,z,0.);
		double RotAng = ( pGeom->IsCWArc() ? 90. : -90.);
		r = I*(BMatr()).rotg(RotAng,B,N);
	}
	double d = r.D2();
	if( d <= 0.)
		return;
	d = 1./sqrt(d);
	const double s = 180./PI;
	AngleX = acos(r.GetX()*d)*s;
	AngleY = acos(r.GetY()*d)*s;
	AngleZ = acos(r.GetZ()*d)*s;
}

bool NCUnitState::SetCurDirComp(enum LR val)
{
	if(CurCycle != CYCLE_OFF && val != LR_OFF)
		if(!(CurCycle == TURNROUGH || CurCycle == TURNLOOP
			 || CurCycle == TURNLOOP || CurCycle == TURNGROOVZ
			 || CurCycle == TURNGROOVX || CurCycle == TURNCUT
			 || CurCycle == TURNFACECUT /*|| CurCycle == TURNCLEAR*/
			 || CurCycle == APPRLT || CurCycle == APPRCT
			 || CurCycle == APPRLN || CurCycle == APPRLCT
			 || CurCycle == DEPLT || CurCycle == DEPCT
			 || CurCycle == DEPLN || CurCycle == DEPLCT
			 ))
			return false;// Can't use a compensation while cycle is active
	CurDirComp = val;
	return true;
}

bool NCUnitState::SetCurCycle(enum CycleType val)
{
	if(CurDirComp != LR_OFF && val != CYCLE_OFF)
		if(!(val == APPRLT || val == APPRCT
			 || val == APPRLN || val == APPRLCT
			 || val == DEPLT || val == DEPCT
			 || val == DEPLN || val == DEPLCT
			 || val == TURNCLEAR
			 ))
		return false;// Can't use cycles while compensation is active
	CurCycle = val;
	return true;
}

bool NCUnitStatePrevGeom::DIntArcArc(const NCadrGeom & Geom0, const NCadrGeom & Geom1, double d, BPoint & p)
{
	// Найти точку пересечения дуг, отстоящих от заданных на расстояние d
	BPoint o0(Geom0.GetC());
	BPoint p1(Geom1.GetB());
	BPoint o1(Geom1.GetC());
	double R0 = Geom0.GetR(); //Arc radius
	if (R0 < RMU)
		return false;// radius is too small
	double R1 = Geom1.GetR(); //Arc radius
	if (R1 < RMU)
		return false;// radius is too small

	BPoint e(o1 - o0);
	BPoint N(Geom1.GetN());
	e = e - N * (N * e);// Project

	double le = e.D2();
	if (le < DMU * DMU * 0.1)
	{
		return false;
	}

	double s0 = (Geom0.IsCWArc()) ? 1 : -1;
	double s1 = (Geom1.IsCWArc()) ? 1 : -1;
	double R0d = R0 + s0*d;
	double R1d = R1 + s1*d;
	double u = 1. - 0.5*(1. + (R0d * R0d - R1d * R1d) / le);
	BPoint q1 = o1 - e * u;

	e = e * (1. / sqrt(le));
	BPoint er = e * BMatr().RotZ(0., 0., 0., 90.);// rotated e

	double h = R0d * R0d - (q1 - o0).D2();
	if (h < DMU * DMU * 0.1)
		return false;
	h = sqrt(h);

	// Построить результ точку
	p = (q1 + er * h - p1).D2() >(q1 - er * h - p1).D2() ?
		q1 - er * h : q1 + er * h;
	return true;
}

bool NCUnitStatePrevGeom::DIntOtrArc(const NCadrGeom & Geom0, const NCadrGeom & Geom1, double d, BOOL OtrFirst, BPoint &p)
{
	// Найти точку пересечения отрезка и дуги, отстоящих от заданных на расстояние d
	// Geom0 - отрезок, Geom1 - дуга
	// Отрезок может проходить через начальную/конечную точку дуги, т.е. не обязательно совпадать с началом/концом дуги своей конечной/начальной точкой

	// P = P1 + e0r*d + alf*e0
	// alf = I*e0 +- sqrt((Ie0)^2 + 2d(e0r*I + sR))
	// где е0 направляющий вектор; e0r - тот же вектор, 
	//	повернутый на 90 вокруг OZ; P1 - точка пересечения исходных элементов;
	// s - (+1 | -1) - направление дуги.
	if (!OtrFirst)// if first element is an arc
		d = -d;

	BPoint b = Geom0.GetB();
	BPoint e = Geom0.GetE();
	BPoint e0 = e - b;
	double le = e0.D2();
	if (le < DMU * DMU * 0.1)
		return false;
	e0 = e0 * (1. / sqrt(le));// direction vectors of first line

	double s = (Geom1.IsCWArc()) ? 1. : -1.;

	BPoint I = Geom1.GetI(); // Radius vector of arc
	if (!OtrFirst)
	{
		e0 = e0 * -1.;
		s = -s;
		I = (Geom1.GetB() + I) - Geom1.GetE();
	}
	BPoint e0r = e0 * BMatr().RotZ(0., 0., 0., 90.);// rotated e0

	double R = I.D2(); //Arc radius
	if (R < RMU * RMU)
		return false;// radius is too small
	R = sqrt(R);
	double t2 = e0*I;
	double alf = t2*t2 + 2.*d*(e0r*I + s*R);
	if (alf < DMU * DMU * 0.1)
		return false;// there are no solutions
	alf = sqrt(alf);
	alf = (fabs(t2 + alf) > fabs(t2 - alf)) ? alf = t2 - alf : alf = t2 + alf;
	if (fabs(t2) < 1.e-6)
		alf = -fabs(alf);

	p = OtrFirst ? Geom1.GetB() : Geom1.GetE();// result point
	p = p + e0r*d + e0*alf;
	return true;
}

bool NCUnitStatePrevGeom::DIntOtrOtr(const NCadrGeom & Geom0, const NCadrGeom & Geom1, double d, BPoint & p)
{
	// Найти точку пересечения отрезков, отстоящих от заданных на расстояние d
	// Второй отрезок может начинаться не в конце первого

	// P = P1 + (e0r+e1r)*d/(1+e0e1)
	// P = P1 + (e0r|e1| + e1r|e0|)*d / (|e0||e1| + e0e1)
	// где е0, e1 направляющие векторы; e0r,e1r - те же векторы, 
	//	повернутые на 90 вокруг OZ; P1 - точка пересечения исходных отрезков
	// построение невозможно если |e0|=0 или |e1|=0 или e0e1=-|e0||e1|

	//e0,e1 direction vectors of first and second lines
	BPoint e0 = Geom0.GetE() - Geom0.GetB();
	BPoint e1 = Geom1.GetE() - Geom1.GetB();

	BMatr T90;
	T90.RotZ(0., 0., 0., 90.);
	BPoint e0r = e0 * T90;// rotated e0 e1
	BPoint e1r = e1 * T90;

	double r0 = Geom0.Length();
	if (r0 < LNS)
		return false; // first line is too short
	double r1 = Geom1.Length();
	if (r1 < LNS)
		return false; // second line is too short
	double div = r0*r1 + e0*e1;
	if (div < DMU * DMU)
		return false; //  e0 = -e1 impossible to find intersection point

	p = Geom0.GetE() + (e0r*r1 + e1r*r0)*(d / div);
	return true;
}

bool NCUnitStatePrevGeom::IsAngleConcave(const NCadrGeom & NextGeom) const
{
	double Angle = NextGeom.Angle(Geom);
	if( Angle > -MIN_ANGD && CurDirComp == LEFT )
		return true;
	if( Angle < MIN_ANGD && CurDirComp == RIGHT )
		return true;
	return false;
}


double NCUnitState::CalcFixedCutTime(const NCadrGeom * pGeom) const
{
	// FeedM must be MMpRev or INCHpREV
	if(Feed < MIND)
		return 0.;
	double Rlim = 1.;
	if(Spindle[SPINDLE_TURN].MaxSpeed > 0.)
		Rlim = Spindle[SPINDLE_TURN].FixedCutSpeed / (2. * PI * Spindle[SPINDLE_TURN].MaxSpeed);
	double Rmax = fabs(pGeom->GetB().GetX());
	double Rmin = fabs(pGeom->GetE().GetX());
	if(Rmin > Rmax)
	{
		double b = Rmin; Rmin = Rmax; Rmax = b;
	}
	double Length = CadrLength4Time(*pGeom);
	//if(Rmax - Rmin < MIND)
	//{
	//	return Length * 60. / (Spindle[SPINDLE_TURN].Speed * Feed);
	//}
	double Sc = (Rmax - Rmin < MIND) ? (Rmax < Rlim ? 1. : 0.) : min( 1., max(0, Rlim - Rmin) / (Rmax - Rmin));// MaxSpeed part length
	double L0 = Length * Sc;
	double L1 = Length - L0; // Variable speed part length
	Rmin = max(Rmin, Rlim);

	double T = 0.;
	// Max speed time
	T += L0 * 60. / (Spindle[SPINDLE_TURN].MaxSpeed * Feed);
	// Variable speed time
	T += L1 * 60. * PI * (Rmax + Rmin) / (Spindle[SPINDLE_TURN].FixedCutSpeed * Feed);
	return T;
}

const NCBase NCUnitState::GetActiveUCS(void) const
{
	NCBase B = UCSs[ActiveUCS];
	B.MoveBPoint(UCSMatr);
	return B;
}

void NCUnitStatePrev::ChangePlane(enum Plane Src, enum Plane Dest)
{
	if(Src == Dest)
		return;
	NCUnitStatePrevGeom::ChangePlane(Src, Dest);

	BMatr Tr = ChangePlaneMatr(Src, Dest);
	
	TrCoord(&CurX, &CurY, &CurZ, 0., Tr);
	TrCoord(&CurI, &CurJ, &CurK, 0., Tr);

	CompGeom.Tr(Tr);
	AddCompGeom.Tr(Tr);

	CurToolDisplacement = CurToolDisplacement * Tr;
	PrevToolDisplacement = PrevToolDisplacement * Tr;

	CurPlane = Dest;


}

void NCUnitStatePrev::Reset()
{
	NCUnitStatePrevGeom::Reset();
	GetNextGeomActive = false;
	AutoFixed = false;
	PolarCoordActive = false;
	ElectrS = 0.;
	ElectrSL = 0.;
	ElectrSF = 0.;
	ElectrT1 = 0.;
	ElectrA = 10.;
	ElectrB = 10.;
}

void NCUnitStatePrevGeom::TrCoord(double * pX, double * pY, double * pZ, double H, const BMatr & Tr)
{
	BPoint P;
	P = BPoint(*pX, *pY, *pZ, H) * Tr;
	*pX = P.GetX();
	*pY = P.GetY();
	*pZ = P.GetZ();
}

const BMatr NCUnitStatePrevGeom::ChangePlaneMatr(enum Plane Src, enum Plane Dest)
{
	BMatr Tr;
	switch(Src)
	{
	case XY:
		switch(Dest)
		{
		case XZ:
			Tr.RotX(0.,0.,0.,-90.);
			break;
		case YZ:
			Tr.RotY(0.,0.,0.,90.);
			break;
		}
		break;
	case XZ:
		switch(Dest)
		{
		case XY:
			Tr.RotX(0.,0.,0.,90.);
			break;
		case YZ:
			Tr.RotZ(0.,0.,0.,90.);
			break;
		}
		break;
	case YZ:
		switch(Dest)
		{
		case XY:
			Tr.RotY(0.,0.,0.,-90.);
			break;
		case XZ:
			Tr.RotZ(0.,0.,0.,-90.);
			break;
		}
		break;
	}
	return Tr;
}

int NCUnitState::AddGeom2CycleGeom(BGeomArray & CycleGeom)
{
	int k = 0;
	if(!Geom.HaveGeom())
		return k;
	if(AddCompGeom.HaveGeom())
	{
		AddCompGeom.SetAttr(1);
		AddCompGeom.Move(CurToolDisplacement * (-1.));
		CycleGeom.AddElement(AddCompGeom);
		++k;
	}
	Geom.SetAttr(1);
	CompGeom.SetAttr(1);
	CompGeom.Move(CurToolDisplacement * (-1.));
	CycleGeom.AddElement(CompGeom);
	++k;
	return k;
}

void NCUnitState::GetSignsFromK(double Code, double * s0, double * s1)
{
	switch(int(Code + 0.5))
	{
	case 1:
		*s0 = 1.;
		*s1 = 1.;
		break;
	case 2:
		*s0 = 1.;
		*s1 =-1.;
		break;
	case 3:
		*s0 =-1.;
		*s1 =-1.;
		break;
	case 4:
		*s0 =-1.;
		*s1 = 1.;
		break;
	case 5:
		*s0 = 0.;
		*s1 = 1.;
		break;
	case 6:
		*s0 = 1.;
		*s1 = 0.;
		break;
	case 7:
		*s0 =-1.;
		*s1 = 0.;
		break;
	case 8:
		*s0 = 0.;
		*s1 =-1.;
		break;
	case 0:
	case 9:
	default:
		*s0 = 0.;
		*s1 = 0.;
		break;
	}
}

const BPoint NCUnitState::GetToolDisplHAAS(bool CompensationOn) const
{
	if(CompensationOn)
	{
		double R = CurDiamComp;
		double dX = 0., dY = 0., Code = 0.;
//		DComp.Lookup(CurLengthCompNum + 1000, Code);
		Code = CurCodeComp;
		DComp.Lookup(CurLengthCompNum + 2000, dX);
		DComp.Lookup(CurLengthCompNum + 3000, dY);
		dY = - dY;

		double sx, sy;
		GetSignsFromK(Code, &sx, &sy);
		sy = -sy;

		return BPoint( sx * R - dX, sy * R - dY, 0., 0.);
	}
	else
	{
		double dX = 0., dY = 0.;
		DComp.Lookup(CurLengthCompNum + 2000, dX);
		DComp.Lookup(CurLengthCompNum + 3000, dY);
		dY = - dY;
		return BPoint(-dX, -dY, 0., 0.);
	}
}

bool NCUnitState::SetRegister(int Ind, double Value)
{
	if(Ind < 0 || Ind >= MAX_REG)
		return false;

	Registers.SetAt(Ind, Value);

	if (Ind >= DComp2RegStart && Ind <= DComp2RegEnd)
	{// Set D-compensation value
		DComp[Ind - DComp2RegStart] = Value;
	}

	for(int i = 0; i <= NUCSs::MaxUcsInd; ++i)
	{
		int DeltaI = Ind - NCUnitState::UCSRegsSubst[i];
		if(DeltaI >= 0 && DeltaI < 6)
		{
			switch(DeltaI)
			{
			case 0:
				UCSs[i].x0 = Value;
				break;
			case 1:
				UCSs[i].y0 = Value;
				break;
			case 2:
				UCSs[i].z0 = Value;
				break;
			case 3:
				UCSs[i].a0 = Value;
				break;
			case 4:
				UCSs[i].b0 = Value;
				break;
			case 5:
				UCSs[i].c0 = Value;
				break;
			}
			CurBase = GetActiveUCS();
		}
	}
	return true;
}

double NCUnitState::GetRegister(int Ind)
{
	double Value = 0.;
	Registers.Lookup(Ind, Value);
	return Value;
}

bool NCUnitState::CopyUCS2Regs(void)
{
	for(int i = 0; i <= NUCSs::MaxUcsInd; ++i)
	{
		if (UCSs[i].IsEmpty())
		{
			//for (int k = 0; k < 6; ++k)
			//	Registers.RemoveKey(UCSRegsSubst[i] + k);
		}
		else
		{
			SetRegister(NCUnitState::UCSRegsSubst[i] + 0, UCSs[i].x0);
			SetRegister(NCUnitState::UCSRegsSubst[i] + 1, UCSs[i].y0);
			SetRegister(NCUnitState::UCSRegsSubst[i] + 2, UCSs[i].z0);
			SetRegister(NCUnitState::UCSRegsSubst[i] + 3, UCSs[i].a0);
			SetRegister(NCUnitState::UCSRegsSubst[i] + 4, UCSs[i].b0);
			SetRegister(NCUnitState::UCSRegsSubst[i] + 5, UCSs[i].c0);
		}
	}
	return true;
}
NMapIntToDouble & NCUnitState::GetRegisters(void)
{
	return Registers;
}

void NCUnitState::ResetRegisters(void)
{
	Registers.RemoveAll();
	VarSet.Clear();
	CopyUCS2Regs();
	SetRegister(8999, (Debug ? 1. : 0.));
}

void NCUnitState::SetStockPos2Geom(int SP)
{
	Geom.SetStockPos(SP);
	CompGeom.SetStockPos(SP);
	AddCompGeom.SetStockPos(SP);
}

const BInterp & NCUnitStatePrev::GetInterp(void) const
{
	if(PolarInterp.IsActive())
		return PolarInterp;
	return CylInterp;
}

void NCUnitStatePrev::SetDCompFromRegisters()
{
	for (int i = DComp2RegStart; i <= DComp2RegEnd; ++i)
	{
		double Val;
		if (Registers.Lookup(i, Val))
			DComp[i - DComp2RegStart] = Val;
	}
}

double NCUnitStatePrev::GetCurLengthComp() const
{
	return CurLengthComp;
}

void NCUnitStatePrev::SetCurLengthComp(double Val)
{
	CurLengthComp = Val;
}

NCUnitStatePrev::EvaluateCadrResult NCUnitState::ProcessProbe(int HistInd, const BProgram *pProg)
{
	if(!ProbeCadr || pProg == nullptr)
		return ECR_OK;
	INT_PTR i = 0;
	for (; i < pProg->GetProbeHistory().GetSize(); ++i)
	{
		const BProgram::ProbePos &Pos = pProg->GetProbeHistory().GetAt(i);
		if (Pos.Gethpos() == HistInd)
			break;
	}
	if(i == pProg->GetProbeHistory().GetSize())
		return ECR_PROG_END;
	const BProgram::ProbePos &Pos = pProg->GetProbeHistory().GetAt(i);
	if (!Pos.IsProcessed())
		return ECR_PROG_END;

	ProbeCadr = false;
	EndProgram = false;
	// Set Geom from program geom arrays
	NCadrGeom *pGeom = pProg->GetGeomArray().GetAt(HistInd);
	if (pGeom == nullptr)
		return ECR_ERROR;
	Geom = *pGeom;
	pGeom = pProg->GetCompGeomArray().GetAt(HistInd);
	if (pGeom == nullptr)
		return ECR_ERROR;
	CompGeom = *pGeom;

	SetCoordsFromGeom();

	if (ProbeExpTouch && Pos.GetProbeResult() != BProgram::ProbePos::PR_Collision
		|| ProbeExpNoTouch && Pos.GetProbeResult() != BProgram::ProbePos::PR_Empty)
	{
		return ECR_ERROR;
	}
	return ECR_OK;
}

void NCUnitState::SetCoordsFromGeom()
{
	// Set coords from Geom to state
	BPoint XYZ(0., 0., 0., 1), ABC(0., 0., 0., 1);
	Geom.CalcCurCoord(1., XYZ, ABC);
	MashX = XYZ.GetX();
	MashY = XYZ.GetY();
	MashZ = XYZ.GetZ();
	MashA = ABC.GetX();
	MashB = ABC.GetY();
	MashC = ABC.GetZ();
	CompGeom.CalcCurCoord(1., XYZ, ABC);
	MashcX = XYZ.GetX();
	MashcY = XYZ.GetY();
	MashcZ = XYZ.GetZ();
	Geom.CalcCurCoord(0., XYZ, ABC);
	CurX = MashX - XYZ.GetX();
	CurY = MashY - XYZ.GetY();
	CurZ = MashZ - XYZ.GetZ();
	CurA = MashA - ABC.GetX();
	CurB = MashB - ABC.GetY();
	CurC = MashC - ABC.GetZ();
	CalcUCSCoords();
}

void NCUnitState::CalcUCSCoords()
{
	MashcXUCS = MashcX - CurBase.x0;
	MashcYUCS = MashcY - CurBase.y0;
	MashcZUCS = MashcZ - CurBase.z0;
	MashXUCS = MashX - CurBase.x0;
	MashYUCS = MashY - CurBase.y0;
	MashZUCS = MashZ - CurBase.z0;
}

void NCUnitState::RecalcFromMachCS()
{
	BMatr invM = CSMatr.invr();
	BPoint B = BPoint(MashX, MashY, MashZ, 1.) * invM;
	double H;
	B.Get(MashX, MashY, MashZ, H);
	B.Get(MashcX, MashcY, MashcZ, H);
	BPoint C = BPoint(CurX, CurY, CurZ, 0.) * invM;
	C.Get(CurX, CurY, CurZ, H);
}

void NCUnitState::RecalcToMachCS()
{
	BMatr invM = CSMatr.invr();
	BPoint B = BPoint(MashX, MashY, MashZ, 1.) * CSMatr;
	double H;
	B.Get(MashX, MashY, MashZ, H);
	B = BPoint(MashcX, MashcY, MashcZ, 1.) * CSMatr;
	B.Get(MashcX, MashcY, MashcZ, H);
}

void NCUnitState::SetMatrNum2Geom(int MatrNum)
{
	Geom.SetMatrNum(MatrNum);
	CompGeom.SetMatrNum(MatrNum);
	AddCompGeom.SetMatrNum(MatrNum);
}

void NCUnitState::SetInterpNum2Geom(int InterpNum)
{
	Geom.SetInterpNum(InterpNum);
	CompGeom.SetInterpNum(InterpNum);
	AddCompGeom.SetInterpNum(InterpNum);
}

void NCUnitState::SetMTConfNum2Geom(const BMTLinkageDiff& Diff)
{
	Geom.SetMTLink(Diff);
	CompGeom.SetMTLink(Diff);
	AddCompGeom.SetMTLink(Diff);
}

void NCUnitState::SetPolarGeom(void)
{
	Geom.SetPolar();
	CompGeom.SetPolar();
	AddCompGeom.SetPolar();
}

void NCUnitState::SetCylindricGeom(void)
{
	Geom.SetCylindric();
	CompGeom.SetCylindric();
	AddCompGeom.SetCylindric();
}

bool NCUnitState::IsNotSpecCycle(void)
{
	return CurCycle != TURNROUGH 
		&& CurCycle != TURNLOOP
		&& CurCycle != TURNGROOVX
		&& CurCycle != TURNGROOVZ
		&& CurCycle != TURNTHREADCUT
		&& CurCycle != TURNTHREADAUTO
		&& CurCycle != TURNTHREAD
		&& CurCycle != APPRLT
		&& CurCycle != APPRCT
		&& CurCycle != APPRLN
		&& CurCycle != APPRLCT
		&& CurCycle != DEPLT
		&& CurCycle != DEPCT
		&& CurCycle != DEPLN
		&& CurCycle != DEPLCT;
}

bool NCUnitState::IsNotDrillAxis(const CString & CurWord)
{
	if(CurCycle == DRILL_OLD)
		return IsCoordAxis(CurWord, 'X') || IsCoordAxis(CurWord, 'Y');
	if(CurCycle == DRILL_X || CurCycle == RHTAPPINGX || CurCycle == DRILLPECKX)
		return IsCoordAxis(CurWord, 'Z') || IsCoordAxis(CurWord, 'Y');
	if(CurCycle == DRILL_Y )
		return IsCoordAxis(CurWord, 'Z') || IsCoordAxis(CurWord, 'X');
	if(CurCycle == RHTAPPINGZ || CurCycle == DRILLPECKZ)
		return IsCoordAxis(CurWord, 'X') || IsCoordAxis(CurWord, 'Y');

	switch(CurPlane)
	{
	case XY:
		return IsCoordAxis(CurWord, 'X') || IsCoordAxis(CurWord, 'Y');
	case XZ:
		return IsCoordAxis(CurWord, 'X') || IsCoordAxis(CurWord, 'Z');
	case YZ:
		return IsCoordAxis(CurWord, 'Z') || IsCoordAxis(CurWord, 'Y');
	}
	
	return false;
}

bool NCUnitState::IsCoordAxis(const CString& CurWord, char CoordName)
{
	CString Name;
	switch (CoordName)
	{
	case 'X':
		Name = _T("HorizontalAxis");
		break;
	case 'Y':
		Name = _T("VerticalAxis");
		break;
	case 'Z':
		Name = _T("SpindleAxis");
		break;
	default:
		break;
	}
	return CurWord == Name || CurWord == Name + _T("Inc") || CurWord == Name + _T("Abs");
}

void NCUnitState::ClearCurHCadrAbs()
{
	CurHCadrAbs = 0;
}

void NCUnitState::SetCurHCadrAbs(INT_PTR val)
{
	CurHCadrAbs = int(val);
}

void NCUnitState::CalcLength(NCadrGeom *pGeom)
{
	double l = pGeom->Length();
	Length += l;
	if(pGeom->IsFast())
		FastLength += l;
	else
		WorkLength += l;
	CadrLength = l;
}

SMachStateFix1 NCUnitStatePrev::GetMachStateFix(void) const
{
	BPoint XYZ;
	XYZ.Set(MashX, MashY, MashZ, 1.);
	XYZ = XYZ * CSMatr;
	BPoint ABC(MashA, MashB, MashC, 1.);
	const BInterp &Interp = GetInterp();
	if(PolarInterp.IsActive())
	{
		Interp.MapP2PA(XYZ, XYZ, ABC);
		ABC += Interp.GetABCStd() * (Interp.GetABCStd() * BPoint(CurBase.a0, CurBase.b0, CurBase.c0, 0.));
		Interp.AddAlfa0(ABC);
	}
	else if(CylInterp.IsActive())
	{
		Interp.MapP2PA(XYZ, XYZ, ABC);
	}
	return  SMachStateFix1(XYZ, ABC);
}

SMachStateFix1 NCUnitStatePrev::GetMachStateFixComp(void) const
{
	BPoint XYZ;
	XYZ.Set(MashcX, MashcY, MashcZ, 1.);
	XYZ = XYZ * CSMatr;
	BPoint ABC(MashA, MashB, MashC, 1.);
//	CompGeom.CalcCurCoordMCS(1, CSMatr, XYZ, ABC);

	const BInterp& Interp = GetInterp();
	if (PolarInterp.IsActive())
	{
		Interp.MapP2PA(XYZ, XYZ, ABC);
		ABC += Interp.GetABCStd() * (Interp.GetABCStd() * BPoint(CurBase.a0, CurBase.b0, CurBase.c0, 0.));
		Interp.AddAlfa0(ABC);
	}
	else if (CylInterp.IsActive())
	{
		Interp.MapP2PA(XYZ, XYZ, ABC);
	}
	return  SMachStateFix1(XYZ, ABC);
}

double NCUnitStatePrev::GetPenetrDist(void) const
{
	return (ElectrSF == 0.)  
		? ElectrS * ElectrA + ElectrSL * ElectrB
		: ElectrSF;
}

void NCUnitState::SetCurProg(const CString &ProgName)
{
	CurProg = ProgName;
}

void NCUnitState::SetCurProgEmpty(void)
{
	CurProg.Empty();
	CadrIDEx.Prog = -1;
}

BPoint NCUnitState::GetPolarCenter(void)
{
	return GetActiveUCS().GetBPoint() + PolarCenter;
}

void NCUnitState::SetPolarCenter(const BPoint &P)
{// P should have H = 1
	PolarCenter = P - GetActiveUCS().GetBPoint(); 
}

void NCUnitState::ApplyPolarCoord()
{
	double Angle = PolarAngle * PI / 180.;
	double X0 = PolarDist * cos(Angle);
	double X1 = PolarDist * sin(Angle);
	BPoint C = GetPolarCenter();
	switch(CurPlane)
	{
	case XY:
		MashX = C.GetX() + X0;
		MashY = C.GetY() + X1;
		ChangedCoord.x = TRUE;
		ChangedCoord.y = TRUE;
		break;
	case XZ:
		MashZ = C.GetZ() + X0;
		MashX = C.GetX() + X1;
		ChangedCoord.z = TRUE;
		ChangedCoord.x = TRUE;
		break;
	case YZ:
		MashY = C.GetY() + X0;
		MashZ = C.GetZ() + X1;
		ChangedCoord.y = TRUE;
		ChangedCoord.z = TRUE;
		break;
	}
}

void NCUnitState::CalcPolarCoord(double X, double Y, double Z)
{ // Calculate polar coordinates of the given point and store it in PolarDist and PolarAngle
	BPoint C = GetPolarCenter();
	double dX = X - C.GetX();
	double dY = Y - C.GetY();
	double dZ = Z - C.GetZ();
	switch(CurPlane)
	{
	case XY:
		PolarDist = sqrt(dX * dX + dY * dY);
		PolarAngle = BPoint(1., 0., 0., 0.).Angle(BPoint(dX, dY, 0., 0.));
		break;
	case XZ:
		PolarDist = sqrt(dX * dX + dZ * dZ);
		PolarAngle = BPoint(1., 0., 0., 0.).Angle(BPoint(dZ, dX, 0., 0.));
		break;
	case YZ:
		PolarDist = sqrt(dZ * dZ + dY * dY);
		PolarAngle = BPoint(1., 0., 0., 0.).Angle(BPoint(dY, dZ, 0., 0.));
		break;
	}
}

void NCUnitState::ForceMakeEqualTo(const NCUnitState &src)
{
	DComp.ToChange = true;
	HComp.ToChange = true;
	Registers.ToChange = true;

	*this = src;

	DComp.Changed = true;
	HComp.Changed = true;
	Registers.Changed = true;
}

bool NCUnitState::CalcCurToolDispl(bool PartialCompCancel)
{
	if(!(ToolDisplChanged.x || ToolDisplChanged.z))
		return false;
	if(!PartialCompCancel)
	{
		ToolDisplChanged.x = FALSE;
		ToolDisplChanged.z = FALSE;
		return true;
	}
	if(ToolDisplChanged.x)
		if(ChangedCoord.x)
			ToolDisplChanged.x = FALSE;
		else
			CurToolDisplacement.SetX(PrevToolDisplacement.GetX());
	if(ToolDisplChanged.z)
		if(ChangedCoord.z)
			ToolDisplChanged.z = FALSE;
		else
			CurToolDisplacement.SetZ(PrevToolDisplacement.GetZ());
	return true;
}

bool NCUnitState::Push133Registers(void)
{
	int StackSize = int(GetRegister(REG_STACK_BASE));
	if(StackSize > 9)
		return false;
	++StackSize;
	SetRegister(REG_STACK_BASE, double(StackSize));
	for(int i = 1; i <= 33; ++i)
		SetRegister(i + StackSize * 100 + REG_STACK_BASE, GetRegister(i));
	return true;
}

bool NCUnitState::Pop133Registers(void)
{
	int StackSize = int(GetRegister(REG_STACK_BASE));
	if(StackSize <= 0.)
		return false;
	for(int i = 1; i <= 33; ++i)
		SetRegister(i, GetRegister(i + StackSize * 100 + REG_STACK_BASE));
	--StackSize;
	SetRegister(REG_STACK_BASE, double(StackSize));
	return true;
}

void NCUnitState::Restore133Registers(void)
{
	for(int i = 1; i <= 33; ++i)
	{
		SetRegister(i, GetRegister(i + REG_COPY_BASE));
		SetRegister(i + REG_COPY_BASE, 0.);
	}
}

void NCUnitState::ReplaceByPrev(const NCUnitStatePrev &St)
{
	*static_cast<NCUnitStatePrev *>(this) = St;
}

void NCUnitState::RestartLifeTime(double ToolLifeTime)
{
	if (ID4LifeTime.IsEmpty())
		return;
	RestLifeTime = ToolLifeTime;
	WorkedLifeTime = 0.;
}

bool NCUnitState::AddReplacePair(const CString &RegExp, const CString &ReplExp, bool LocalVar)
{
	return VarSet.AddReplacePair(RegExp, ReplExp, LocalVar);
}

void NCUnitState::ProcessLifeTime()
{
	if (ID4LifeTime.IsEmpty())
		return;
	
	if (Geom.HaveGeom5())
	{
		if (!Geom.IsFast())
		{
			RestLifeTime -= CadrTime;
			WorkedLifeTime += CadrTime;
		}
	}
}

void NCUnitState::ProcLifeTime4ToolChange(const IntToolID &OldID, double ToolLifeTime)
{
	// Save values for previous tool
	if (!ID4LifeTime.IsEmpty())
	{
		RestLifeTimes[ID4LifeTime] = RestLifeTime;
		WorkedLifeTimes[ID4LifeTime] = WorkedLifeTime;
	}
	RestLifeTime = ToolLifeTime;
	WorkedLifeTime = 0.;
	ID4LifeTime = IntToolID();
	if (RestLifeTime != 0)
		ID4LifeTime = IntToolID(CurToolN.GetToolNum(), CurTurret);
	// Restore values for new tool if any
	if (!ID4LifeTime.IsEmpty())
	{
		if (RestLifeTimes.find(ID4LifeTime) != RestLifeTimes.end())
		{
			RestLifeTime = RestLifeTimes[ID4LifeTime];
			WorkedLifeTime = WorkedLifeTimes[ID4LifeTime];
		}
	}
}